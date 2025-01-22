/**
  Section: Included Files
 * 
 * 
 * Note : Vitesse du SPI à régler dans EEPROM et non SPI foundation
 * 
*/
#include "mcc_generated_files/system.h"
#include "mcc_generated_files/pin_manager.h"
#include "mcc_generated_files/i2c1.h"
#include "mcc_generated_files/dma.h"
#include "stdbool.h"
#include <math.h>
#include "mcc_generated_files/EEPROM2_app.h"
#include "mcc_generated_files/pin_manager.h"
#include "mcc_generated_files/drivers/spi_master.h"
#include "mcc_generated_files/delay.h"
#include "mcc_generated_files/cmp1.h"
#include "mcc_generated_files/ext_int.h"

#include <xc.h>
#include <libpic30.h>

uint16_t freq_middle_ = 40000;
uint16_t freq_range_ = 5000; //2500.0;
const float sample_duration_ = 1e-6;

const char device_name_[16] = "DSPIC_ACOUSTICvB";
const char code_version_ = 0x0B;

volatile unsigned char i2c_nb_bytes = 0;
volatile unsigned char i2c_register = 0x00;

uint16_t shoot_duration_between = 30; // in seconds
uint32_t shoot_offset_from_posix_zero = 0; // in seconds
uint32_t posix_time = 0; // in seconds

bool recompute_signal = true;
bool enable_chirp = false;
uint16_t signal_main_duration_ms_ = 250;

bool shoot_signal_run_ = false;

uint16_t dac_mean_16_ = 1200;
uint16_t dac_amplitude_16_ = 1100; // 1100

uint8_t signal_selection = 0;

volatile uint8_t robot_code = 0;

// 0 => 0xFFFFF => 0x17FFFE => 0x1FFFFD
// [0xFFFFF, 0x7FFFF, 0x7FFFF]
// [1,048,575 | 524,287 | 524,287], max = 0x1FFFFD < 0x1FFFFF
const uint32_t signal_add_[3] = {0, 0x100000, 0x180000};
// At 1us per sample => 1.048575s max for larger symbol

void shoot_signal(const uint8_t signal_id, const uint16_t sample_duration_ms){
    uint8_t addressBuffer[EEPROM2_ADDRBYTES] = {0, 0, 0};
    EEPROM2_AddressAssign(addressBuffer, signal_add_[signal_id]);
    EEPROM2_WritePoll(); //Wait for write cycle to complete
    spiMaster[EEPROM2].spiOpen(); 
    EEPROM2_nCS_SetLow(); // set EEPROM2_nCS output low
    spiMaster[EEPROM2].exchangeByte(EEPROM2_FREAD); //Send Read Command
    spiMaster[EEPROM2].exchangeBlock(addressBuffer,EEPROM2_ADDRBYTES); //Send Address bytes
    
    // Do not send dummy byte ?
    spiMaster[EEPROM2].exchangeByte(0x00); // Send dummy byte ?
    spiMaster[EEPROM2].exchangeByte(0x00); // Send dummy byte ?
    
    LED_SetLow();
    SIGNAL_ENABLE_SetHigh();
    DMA_ChannelEnable(0); // SPI => Memory
    DELAY_milliseconds(sample_duration_ms); // Wait for the signal to end
    
    // Wait for signal to be send
    DMA_ChannelDisable(0);
    EEPROM2_nCS_SetHigh(); /* set EEPROM2_nCS output high */
    spiMaster[EEPROM2].spiClose();
    SIGNAL_ENABLE_SetLow();
    LED_SetHigh();
    
    // Set signal to mean
    DAC1DATH = dac_mean_16_;
}

void erase_eeprom(){
    EEPROM2_WriteEnable();
    EEPROM2_CheckStatusRegister();
    
    spiMaster[EEPROM2].spiOpen(); 
    EEPROM2_nCS_SetLow(); // set EEPROM2_nCS output low
    // CHER
    spiMaster[EEPROM2].exchangeByte(EEPROM2_CHER); //Send Read Command
    EEPROM2_nCS_SetHigh(); /* set EEPROM2_nCS output high */
    spiMaster[EEPROM2].spiClose();
    DELAY_milliseconds(30);
}

void compute_chirp(const uint32_t add_start, const uint16_t signal_duration_ms, const bool sens){
    const float signal_duration = ((float)signal_duration_ms)*1e-3;
    const float signal_fade = signal_duration/10.0;
    const unsigned long long sample_number = ceil((signal_duration/sample_duration_)/256.0 + 2)*256;
    const float invert = sens ? 1.0 : -1.0;
    const float freq_middle = (float)freq_middle_;
    const float freq_range = (float)freq_range_;
    const float dac_amplitude = (float)dac_amplitude_16_;
    const float dac_mean = (float)dac_mean_16_;
        
    float t = 0.;
    uint32_t add = add_start;
    uint16_t data_chirp[256];
    for(unsigned long long i = 0; i < sample_number; i+=256){
        for(uint16_t j = 0; j<256; j++){
            // Window
            const double amplitude = dac_amplitude * fmin(1.0, t/signal_fade)
                                             * fmin(1.0, (signal_duration_ms - t)/signal_fade);
            double frequency = freq_middle+invert*freq_range*(t/signal_duration-0.5);
            data_chirp[j] = round(amplitude*sin(2.0*M_PI*t*frequency) + dac_mean);
            t+=sample_duration_;
        }
        EEPROM2_WriteBlock(data_chirp,512,add);
        add+=512;
    }
}

void compute_cw(const uint32_t add_start, const uint16_t signal_duration_ms, const bool level){
    const float signal_duration = (float)signal_duration_ms*1e-3;
    const float signal_fade = signal_duration/10.0;
    // Add "1" to avoid 0xFF data if rounding is not correct
    const unsigned long long sample_number = ceil((signal_duration/sample_duration_)/256.0 + 2)*256;
    const float freq_middle = (float)freq_middle_;
    const float freq_range = (float)freq_range_;
    const float dac_amplitude = (float)dac_amplitude_16_;
    const float dac_mean = (float)dac_mean_16_;
    
    const float frequency = level ? freq_middle + freq_range/2.0 : freq_middle - freq_range/2.0;
    
    float t = 0.;
    uint32_t add = add_start;
    uint16_t data_chirp[256];
    for(unsigned long long i = 0; i < sample_number; i+=256){
        for(int j = 0; j<256; j++){
            // Window
            const double amplitude = dac_amplitude * fmin(1.0, t/signal_fade)
                                             * fmin(1.0, (signal_duration_ms - t)/signal_fade);
            uint16_t val = round(amplitude*sin(2.0*M_PI*t*frequency) + dac_mean);
            data_chirp[j] = val ; //(val << 8) | (val >> 8);
            t+=sample_duration_;
        }
        EEPROM2_WriteBlock(data_chirp,512,add);
        add+=512;
    }
}

void compute_signal(){
    LED_SetHigh();
    recompute_signal = true;
    erase_eeprom();
    switch (signal_selection){
        case 0x00:
            compute_chirp(signal_add_[0], signal_main_duration_ms_, true);
            compute_chirp(signal_add_[1], signal_main_duration_ms_/2, true);
            compute_chirp(signal_add_[2], signal_main_duration_ms_/2, false);
            break;
        case 0x01:
            compute_cw(signal_add_[0], signal_main_duration_ms_, true);
            compute_cw(signal_add_[1], signal_main_duration_ms_/2, true);
            compute_cw(signal_add_[2], signal_main_duration_ms_/2, false);
            break;
        default:
            break;
    }
    recompute_signal = false;
    LED_SetLow();
}

void EX_INT1_CallBack(){
    EX_INT1_InterruptDisable();
    posix_time++;
    
    if((posix_time - shoot_offset_from_posix_zero) % shoot_duration_between == 0){
        if(!recompute_signal && enable_chirp){
            shoot_signal_run_ = true;
        }
    }
    EX_INT1_InterruptEnable();
}

bool I2C1_StatusCallback(I2C1_SLAVE_DRIVER_STATUS status){
    static uint8_t i2c_data, i2c_address;
    static uint8_t i2c_address_rest = true;
    static uint8_t i2c_default_data = 0x00;

    switch (status)
    {
        case I2C1_SLAVE_TRANSMIT_REQUEST_DETECTED:
            // set up the slave driver buffer transmit pointer
            i2c_address_rest = false;
            
            switch(i2c_address){
                case 0x00:
                    I2C1_ReadPointerSet(&robot_code);
                    break;
                case 0x01 ... 0x02:
                    I2C1_ReadPointerSet((uint16_t)(&freq_middle_) + (i2c_address - 0x01));
                    break;
                case 0x03 ... 0x04:
                    I2C1_ReadPointerSet((uint16_t)(&freq_range_) + (i2c_address - 0x03));
                    break;
                case 0x05:
                    I2C1_ReadPointerSet(&recompute_signal);
                    break;
                case 0x08:
                    I2C1_ReadPointerSet(&enable_chirp);
                    break;
                case 0x09 ... 0x0A:
                    I2C1_ReadPointerSet((uint16_t)(&dac_mean_16_) + (i2c_address - 0x09));
                    break;
                case 0x0B ... 0x0C:
                    I2C1_ReadPointerSet((uint16_t)(&dac_amplitude_16_) + (i2c_address - 0x0B));
                    break;
                case 0x0D:
                    I2C1_ReadPointerSet(&signal_selection);
                    break;
                case 0x0E ... 0x0F:
                    I2C1_ReadPointerSet((uint16_t)(&signal_main_duration_ms_) + (i2c_address - 0x0E));
                    break;
                    
                case 0xB0 ... 0xB3:
                    I2C1_ReadPointerSet((uint16_t)(&posix_time) + (i2c_address - 0xB0));
                    break;
                case 0xB4 ... 0xB5:
                    I2C1_ReadPointerSet((uint16_t)(&shoot_duration_between) + (i2c_address - 0xB5));
                    break;
                case 0xB6 ... 0xB7:
                    I2C1_ReadPointerSet((uint16_t)(&shoot_offset_from_posix_zero) + (i2c_address - 0xB7));
                    break;
                
                case 0xC0:
                    I2C1_ReadPointerSet(&code_version_);
                    break;                
                    
                case 0xF0 ... 0xFF:
                    I2C1_ReadPointerSet(&(((char*)device_name_)[i2c_address-0xF0]));
                    break;
                default:
                    I2C1_ReadPointerSet(&i2c_default_data);
            }
            i2c_address++;
            break;

        case I2C1_SLAVE_RECEIVE_REQUEST_DETECTED:
            // set up the slave driver buffer receive pointer
            I2C1_WritePointerSet(&i2c_data);
            i2c_address_rest = true;
            i2c_address = i2c_data;
            break;

        case I2C1_SLAVE_RECEIVED_DATA_DETECTED:
            if(i2c_address_rest){
                i2c_address = i2c_data;
                i2c_address_rest = false;
            }
            else{
                switch(i2c_address){
                    case 0x00:
                        robot_code = i2c_data;
                        break;
                        
                    case 0x01 ... 0x02:
                    {
                        uint8_t *bytePointer = (uint8_t *)(&freq_middle_) + (i2c_address - 0x01);
                        *bytePointer = i2c_data;
                    }
                        break;
                        
                    case 0x03 ... 0x04:
                    {
                        uint8_t *bytePointer = (uint8_t *)(&freq_range_) + (i2c_address - 0x03);
                        *bytePointer = i2c_data;
                    }
                        break;
                        
                    case 0x05:
                    {
                        if(i2c_data == 1)
                            recompute_signal = true;
                    }
                        break;
                    case 0x08:
                    {
                        if(i2c_data == 1)
                            enable_chirp = true;
                        else
                            enable_chirp = false;
                    }
                        break;
                        
                    case 0x09 ... 0x0A:
                    {
                        uint8_t *bytePointer = (uint8_t *)(&dac_mean_16_) + (i2c_address - 0x09);
                        *bytePointer = i2c_data;
                    }
                        break;
                    case 0x0B ... 0x0C:
                    {
                        uint8_t *bytePointer = (uint8_t *)(&dac_amplitude_16_) + (i2c_address - 0x0B);
                        *bytePointer = i2c_data;
                    }
                        break;
                        
                    case 0x0D:
                        signal_selection = i2c_data;
                        break;

                    case 0x0E ... 0x0F:
                    {
                        uint8_t *bytePointer = (uint8_t *)(&signal_main_duration_ms_) + (i2c_address - 0x0E);
                        *bytePointer = i2c_data;
                    }
                        break;
                    
                    case 0xA1:
                    {
                        if(i2c_data == 1){
                            shoot_signal_run_ = true;
                        }
                    }
                        break;

                    case 0xB0 ... 0xB3:
                    {
                        uint8_t *bytePointer = (uint8_t *)(&posix_time) + (i2c_address - 0xB0);
                        *bytePointer = i2c_data;
                    }
                        break;
                    case 0xB4 ... 0xB5:
                    {
                        uint8_t *bytePointer = (uint8_t *)(&shoot_duration_between) + (i2c_address - 0xB4);
                        *bytePointer = i2c_data;
                    }
                        break;
                    case 0xB6 ... 0xB7:
                    {
                        uint8_t *bytePointer = (uint8_t *)(&shoot_offset_from_posix_zero) + (i2c_address - 0xB6);
                        *bytePointer = i2c_data;
                    }
                        break;
                        
                    default:
                        break;
                }
                i2c_address++;
            }

            break;

        default:
            break;
    }
    return true;
}

/*
    Main application
 */
int main(void)
{
    LED_SetHigh();
    SYSTEM_Initialize();
    DAC1DATH = dac_mean_16_;
    compute_signal();
    
    while (1)
    {
        ClrWdt();

        if(recompute_signal){
            compute_signal();
        }
        
        if(shoot_signal_run_){
            shoot_signal(0, signal_main_duration_ms_);
            DELAY_microseconds(800);
            for(uint8_t i=0; i<8; i++){
                shoot_signal((0b1 & (robot_code>>i)) + 1, signal_main_duration_ms_/2);
                DELAY_microseconds(800);
            }
            shoot_signal_run_ = false;
        }
    }
    return 1; 
}   
/**
 End of File
*/

