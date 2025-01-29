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
#include <stdint.h>
#include "mcc_generated_files/EEPROM2_app.h"
#include "mcc_generated_files/pin_manager.h"
#include "mcc_generated_files/drivers/spi_master.h"
#include "mcc_generated_files/delay.h"
#include "mcc_generated_files/cmp1.h"
#include "mcc_generated_files/ext_int.h"

#include <xc.h>
#include <libpic30.h>

uint16_t freq_middle_ = 20000;
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
uint16_t signal_main_duration_ms_ = 25;
uint8_t signal_fade_ = 1;

uint8_t signal_selection = 0;

bool shoot_signal_run_ = false;

//y = -9.08900731E-08x2 + 5.70147051E-04x + 9.35330181E+02
// offset 0.95 V
double dac_interp_p2 = -9.08900731e-08;
double dac_interp_p1 = 5.70147051e-04;
double dac_interp_p0 = 9.35330181e+02;
double dac_offset = 1179.15;

uint16_t signal_number = 0;

volatile uint8_t robot_code_ = 0;
volatile uint64_t robot_data_ = 0;
volatile uint8_t robot_data_size_ = 0;

// 0 => 0xFFFFF => 0x17FFFE => 0x1FFFFD
// [0xFFFFF, 0x7FFFF, 0x7FFFF]
// [1,048,575 | 524,287 | 524,287], max = 0x1FFFFD < 0x1FFFFF
const uint32_t signal_add_[3] = {0, 0x100000, 0x180000};
// At 1us per sample => 1.048575s max for larger symbol

static inline uint8_t max_uint8(uint8_t a, uint8_t b) {
    return (a > b) ? a : b;
}

float get_dac_amplitude_freq(const double frequency){
    return fmin(2048.0, 
                    dac_interp_p2*frequency*frequency
                   +dac_interp_p1*frequency
                   +dac_interp_p0);
}

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
    
    DMA_ChannelEnable(0); // SPI => Memory
    DELAY_milliseconds(sample_duration_ms); // Wait for the signal to end
    
    // Wait for signal to be send
    DMA_ChannelDisable(0);
    EEPROM2_nCS_SetHigh(); /* set EEPROM2_nCS output high */
    spiMaster[EEPROM2].spiClose();

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
    const float signal_fade = ((float)signal_fade_)*1e-3;
    // Add sample after the signal_duration
    const unsigned long sample_number = ceil((signal_duration/sample_duration_)/256.0 + 2)*256;
    const float invert = sens ? 1.0 : -1.0;
    const float freq_middle = (float)freq_middle_;
    const float freq_range = (float)freq_range_;
    
    const float dac_mean = dac_offset;
        
    float t = 0.;
    uint32_t add = add_start;
    uint16_t data_chirp[256];
    for(unsigned long i = 0; i < sample_number; i+=256){
        for(uint16_t j = 0; j<256; j++){
            // Window
            const float time_ratio = (t/signal_duration);
			const float frequency = freq_middle+invert*freq_range*(time_ratio-0.5);
            
            const float dac_amplitude = get_dac_amplitude_freq(frequency);
            
            const float amplitude = dac_amplitude * fmin(1.0, t/signal_fade)
                                             * fmin(1.0, fmax(0.0, 
                                            (signal_duration - t)/signal_fade));

            // Phase has to be taken into account which explain the time_ratio/2.0
            data_chirp[j] = round(amplitude*sin(2.0*M_PI*t*
                    (freq_middle+invert*freq_range*(time_ratio/2.0-0.5))
                    ) + dac_mean);
            t+=sample_duration_;
        }
        EEPROM2_WriteBlock(data_chirp,512,add);
        add+=512;
    }
}

void compute_cw(const uint32_t add_start, const uint16_t signal_duration_ms, const bool level){
    const float signal_duration = (float)signal_duration_ms*1e-3;
    const float signal_fade = ((float)signal_fade_)*1e-3;
    // Add "1" to avoid 0xFF data if rounding is not correct
    const unsigned long long sample_number = ceil((signal_duration/sample_duration_)/256.0 + 2)*256;
    const float freq_middle = (float)freq_middle_;
    const float freq_range = (float)freq_range_;
    
    const float frequency = level ? freq_middle + freq_range/2.0 : freq_middle - freq_range/2.0;
    
    const float dac_amplitude = get_dac_amplitude_freq(frequency);
    const float dac_mean = dac_offset;
    
    float t = 0.;
    uint32_t add = add_start;
    uint16_t data_chirp[256];
    for(unsigned long long i = 0; i < sample_number; i+=256){
        for(int j = 0; j<256; j++){
            // Window
            const float amplitude = dac_amplitude * fmin(1.0, t/signal_fade)
                                             * fmin(1.0, fmax(0.0, 
                    (signal_duration - t)/signal_fade));
            uint16_t val = round(amplitude*sin(2.0*M_PI*t*frequency) + dac_mean);
            data_chirp[j] = val ; //(val << 8) | (val >> 8);
            t+=sample_duration_;
        }
        EEPROM2_WriteBlock(data_chirp,512,add);
        add+=512;
    }
}

void compute_signal(){
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
}

void EX_INT1_CallBack(){
    EX_INT1_InterruptDisable();
    posix_time++;
    
    if((posix_time - shoot_offset_from_posix_zero) % shoot_duration_between == 0){
        if(!recompute_signal && enable_chirp){
            shoot_signal_run_ = true;
            signal_number++;
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
                    I2C1_ReadPointerSet(&robot_code_);
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
                case 0x06:
                    I2C1_ReadPointerSet(&signal_fade_);
                    break;
                case 0x08:
                    I2C1_ReadPointerSet(&enable_chirp);
                    break;
                case 0x0A ... 0x0B:
                    I2C1_ReadPointerSet((uint16_t)(&signal_number) + (i2c_address - 0x0A));
                    break;
                case 0x0D:
                    I2C1_ReadPointerSet(&signal_selection);
                    break;
                case 0x0E ... 0x0F:
                    I2C1_ReadPointerSet((uint16_t)(&signal_main_duration_ms_) + (i2c_address - 0x0E));
                    break;
                case 0xA0 ... 0xA3:
                  	I2C1_ReadPointerSet((uint16_t)(&dac_interp_p0) + (i2c_address - 0xA0)); // ToDo : check type conversion
                    break;
                case 0xA4 ... 0xA7:
                    I2C1_ReadPointerSet((uint16_t)(&dac_interp_p1) + (i2c_address - 0xA4));
                    break;
                case 0xA8 ... 0xAB:
                    I2C1_ReadPointerSet((uint16_t)(&dac_interp_p2) + (i2c_address - 0xA8));
                    break;
                case 0xAC ... 0xAF:
                    I2C1_ReadPointerSet((uint16_t)(&dac_offset) + (i2c_address - 0xAC));
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
                    
                case 0xD0:
                    I2C1_ReadPointerSet(&robot_data_size_);
                    break;
                case 0xD1 ... 0xD9:
                    I2C1_ReadPointerSet((uint16_t)(&robot_data_) + (i2c_address - 0xD1));
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
                        robot_code_ = i2c_data;
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
                    case 0x06:
                    {
                        uint8_t *bytePointer = (uint8_t *)(&signal_fade_) + (i2c_address - 0x06);
                        *bytePointer = i2c_data;
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
                    case 0x09:
                    {
                        if(i2c_data == 1){
                            shoot_signal_run_ = true;
                        }
                    }
                        break;
                        
                    case 0x0A ... 0x0B:
                    {
                        uint8_t *bytePointer = (uint8_t *)(&signal_number) + (i2c_address - 0x0A);
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

                    case 0xA0 ... 0xA3:
                    {
                        uint8_t *bytePointer = (uint8_t *)(&dac_interp_p0) + (i2c_address - 0xA0);
                        *bytePointer = i2c_data;
                    }
                        break;
                    case 0xA4 ... 0xA7:
                    {
                        uint8_t *bytePointer = (uint8_t *)(&dac_interp_p1) + (i2c_address - 0xA4);
                        *bytePointer = i2c_data;
                    }
                        break;
                    case 0xA8 ... 0xAB:
                    {
                        uint8_t *bytePointer = (uint8_t *)(&dac_interp_p2) + (i2c_address - 0xA8);
                        *bytePointer = i2c_data;
                    }
                        break;
                    case 0xAC ... 0xAF:
                    {
                        uint8_t *bytePointer = (uint8_t *)(&dac_offset) + (i2c_address - 0xAC);
                        *bytePointer = i2c_data;
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
                    
                    case 0xD0:
                        robot_data_size_ = max_uint8(8, i2c_data);
                        break;
                    case 0xD1 ... 0xD9:
                    {
                        uint8_t *bytePointer = (uint8_t *)(&robot_data_) + (i2c_address - 0xD1);
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
    DAC1DATH = (uint16_t)round(dac_offset);
    compute_signal();
    
    while (1)
    {
        ClrWdt();

        if(recompute_signal){
            compute_signal();
        }
        
        if(shoot_signal_run_){
//            LED_SetHigh();
            SIGNAL_ENABLE_SetHigh();
            shoot_signal(0, signal_main_duration_ms_);
            for(uint8_t i=0; i<8; i++){
                shoot_signal((0b1 & (robot_code_>>i)) + 1, signal_main_duration_ms_/2);
            }
            if(robot_data_size_>0){
                for(uint8_t i=0; i<robot_data_size_; i++){
                    shoot_signal((0b1 & (robot_data_>>i)) + 1, signal_main_duration_ms_/2);
                }
            }
            shoot_signal_run_ = false;
            SIGNAL_ENABLE_SetLow();
//            LED_SetLow();
        }
    }
    return 1; 
}   
/**
 End of File
*/

