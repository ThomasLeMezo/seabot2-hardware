/**
  Generated main.c file from MPLAB Code Configurator

  @Company
    Microchip Technology Inc.

  @File Name
    main.c

  @Summary
    This is the generated main.c using PIC24 / dsPIC33 / PIC32MM MCUs.

  @Description
    This source file provides main entry point for system initialization and application code development.
    Generation Information :
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - 1.171.1
        Device            :  dsPIC33CK256MP202
    The generated drivers are tested against the following:
        Compiler          :  XC16 v1.70
        MPLAB 	          :  MPLAB X v5.50
*/

/*
    (c) 2020 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/

/**
  Section: Included Files
*/
#include "mcc_generated_files/system.h"
#include "mcc_generated_files/pin_manager.h"
#include "mcc_generated_files/i2c1.h"
#include "mcc_generated_files/dma.h"
#include "stdbool.h"
#include "mcc_generated_files/ext_int.h"
#include <math.h>
#include "mcc_generated_files/EEPROM2_example.h"

#define FCY 4000000UL
#include <xc.h>
#include <libpic30.h>

#define START_ADDRESS 0x117F
#define SAMPLE_NUMBER 100
// 0x117F + 2*8000 = 4FFF (max in X Data RAM (X) (8K)) => see page 49 of datasheet
uint16_t data_chirp[SAMPLE_NUMBER];

const char device_name[16] = "DSPIC_ACOUSTICv6";
const char code_version = 0x06;
volatile unsigned char i2c_nb_bytes = 0;
volatile unsigned char i2c_register = 0x00;

uint16_t freq_middle = 12500;
uint16_t freq_range = 2500; //2500.0;
const float sampling_duration = 4e-6;

uint16_t shoot_duration_between = 20; // in seconds
uint16_t shoot_offset_from_posix_zero = 0; // in seconds
uint32_t posix_time = 0; // in seconds

bool recompute_signal = true;
bool enable_chirp = false;

bool shoot_chirp_i2c = false;
bool is_shooting = false;

float dac_mean = 1200; //2047;
float dac_amplitude = 1000; // 1100

uint16_t dac_mean_16= 1200;
uint16_t dac_amplitude_16 = 1000; // 1100

uint8_t signal_selection = 0;

volatile uint8_t robot_code = 0;
volatile uint8_t robot_code_bit_index = 0;

volatile uint8_t data_spi[8];

void enable_emission(){
    SIGNAL_ENABLE_SetHigh(); // 3us
}

void enable_reception(){
    SIGNAL_ENABLE_SetLow();
}

void compute_chirp(){
    float t = 0.;
    for(int i = 0; i < SAMPLE_NUMBER; i++){
        data_chirp[i] = round(dac_amplitude*sin(2.0*M_PI*t*((float)freq_middle+(float)freq_range*(t/(2.0*(float)SAMPLE_NUMBER*sampling_duration)-0.5))) + dac_mean);
        t+=sampling_duration;
    }
}

void compute_cw(){
    float t = 0.;
    for(int i = 0; i < SAMPLE_NUMBER; i++){
        data_chirp[i] = round(dac_amplitude*sin(2.0*M_PI*t*(float)freq_middle) + dac_mean);
        t+=sampling_duration;
    }
}

void compute_signal(){
    switch (signal_selection){
        case 0x00:
            compute_chirp();
            break;
        case 0x01:
            compute_cw();
            break;
        default:
            break;
    }
}

void shoot_chirp(){
    LED_SetHigh();
    is_shooting = true;
    
    // Test if it is a 0 or 1
//    11 = DMASRCn is used in Peripheral Indirect Addressing and remains unchanged
//    10 = DMASRCn is decremented based on the SIZE bit after a transfer completion
//    01 = DMASRCn is incremented based on the SIZE bit after a transfer completion
//    00 = DMASRCn remains unchanged after a transfer completion
    
    if((robot_code>>robot_code_bit_index)&0b1){
        DMACH0bits.SAMODE = 0b01; // Increment
        DMASRC0 = START_ADDRESS;
    }
    else{
        DMACH0bits.SAMODE = 0b10; // Decrement
        DMASRC0 = START_ADDRESS+SAMPLE_NUMBER*2;
    }
    
    enable_emission();
    DMA_ChannelEnable(0);  
}

void DMA_Channel0_CallBack(){ 
    robot_code_bit_index++;
    if(robot_code_bit_index !=0)
        shoot_chirp();
    else{
        enable_reception();
        LED_SetLow();
        is_shooting = false;
    }
}

void EX_INT1_CallBack(){
    EX_INT1_InterruptDisable();
    posix_time++;
    
    if((posix_time - shoot_offset_from_posix_zero) % shoot_duration_between == 0){
        if(!recompute_signal && enable_chirp && !is_shooting){
            shoot_chirp();
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
                    I2C1_ReadPointerSet(&freq_middle + (i2c_address - 0x03));
                    break;
                case 0x03 ... 0x04:
                    I2C1_ReadPointerSet(&freq_range + (i2c_address - 0x05));
                    break;
                case 0x05:
                    I2C1_ReadPointerSet(&recompute_signal);
                    break;
                case 0x08:
                    I2C1_ReadPointerSet(&enable_chirp);
                    break;
                case 0x09 ... 0x0A:
                    I2C1_ReadPointerSet(&dac_mean_16 + (i2c_address - 0x05));
                    break;
                case 0x0B ... 0x0C:
                    I2C1_ReadPointerSet(&dac_amplitude_16 + (i2c_address - 0x05));
                    break;
                case 0x0D:
                    I2C1_ReadPointerSet(&signal_selection);
                    break;
                    
                case 0xB0 ... 0xB4:
                    I2C1_ReadPointerSet(&posix_time + (i2c_address - 0xB0));
                    break;
                case 0xB5 ... 0xB6:
                    I2C1_ReadPointerSet(&shoot_duration_between + (i2c_address - 0xB5));
                    break;
                case 0xB7 ... 0xB8:
                    I2C1_ReadPointerSet(&shoot_offset_from_posix_zero + (i2c_address - 0xB5));
                    break;
                
                case 0xC0:
                    I2C1_ReadPointerSet(&code_version);
                    break;
                    
                case 0xD0:
                    I2C1_ReadPointerSet(&data_spi[i2c_address - 0xD0]);
                    break;
                
                    
                case 0xF0 ... 0xFF:
                    I2C1_ReadPointerSet(&(((char*)device_name)[i2c_address-0xF0]));
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
                        
                    case 0x03 ... 0x04:
                    {
                        uint8_t *bytePointer = (uint8_t *)&freq_middle;
                        bytePointer[i2c_address-0x03] = i2c_data;
                    }
                        break;
                        
                    case 0x05 ... 0x06:
                    {
                        uint8_t *bytePointer = (uint8_t *)&freq_range;
                        bytePointer[i2c_address-0x05] = i2c_data;
                    }
                        break;
                        
                    case 0x07:
                        if(i2c_data == 1)
                            recompute_signal = true;
                        break;
                    case 0x08:
                        if(i2c_data == 1)
                            enable_chirp = true;
                        else
                            enable_chirp = false;
                        break;
                        
                    case 0x09 ... 0x0A:
                    {
                        uint8_t *bytePointer = (uint8_t *)&dac_mean_16;
                        bytePointer[i2c_address-0x09] = i2c_data;
                    }
                        break;
                    case 0x0B ... 0x0C:
                    {
                        uint8_t *bytePointer = (uint8_t *)&dac_amplitude_16;
                        bytePointer[i2c_address-0x0B] = i2c_data;
                    }
                        break;
                        
                    case 0x0D:
                        signal_selection = i2c_data;
                        break;
                    
                    case 0xA0:
                        if(i2c_data == 0x01){
                            enable_emission();
                        }
                        else{
                            enable_reception();
                        }
                        break;
                    case 0xA1:
                        if(i2c_data == 1){
                            shoot_chirp_i2c = true;
                        }
                        break;

                    case 0xB0 ... 0xB4:
                    {
                        uint8_t *bytePointer = (uint8_t *)&posix_time;
                        bytePointer[i2c_address-0xB0] = i2c_data;
                    }
                        break;
                    case 0xB5 ... 0xB6:
                    {
                        uint8_t *bytePointer = (uint8_t *)&shoot_duration_between;
                        bytePointer[i2c_address-0xB5] = i2c_data;
                    }
                        break;
                    case 0xB7 ... 0xB8:
                    {
                        uint8_t *bytePointer = (uint8_t *)&shoot_offset_from_posix_zero;
                        bytePointer[i2c_address-0xB7] = i2c_data;
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
    //compute_signal();
    recompute_signal = false;
    LED_SetLow();
    //enable_chirp = true;
    
    //uint8_t data_send = 0x42;
    //EEPROM_WriteByte(0x00, data_send, 1);
    
    EEPROM2_example();
    
    enable_reception();
    while (1)
    {
//        ClrWdt();
//        // Add your application code
//        if(recompute_signal){
//            LED_SetLow();
//            compute_signal();
//            LED_SetLow();
//            recompute_signal = false;
//        }
//        
//        if(shoot_chirp_i2c && !is_shooting){
//            shoot_chirp();
//            shoot_chirp_i2c = false;
//        }
    }
    return 1; 
}
/**
 End of File
*/

