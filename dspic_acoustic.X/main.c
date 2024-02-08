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
#include "stdbool.h"
#include "mcc_generated_files/i2c1.h"
#include <math.h>

#define FCY 100000000UL
#include <xc.h>
#include <libpic30.h>

#define SAMPLE_NUMBER 8000
uint16_t data_chirp[SAMPLE_NUMBER] __attribute__((address(0x1001)));

const char device_name[16] = "DSPIC_ACOUSTIC";
const char code_version = 0x02;
volatile unsigned char i2c_nb_bytes = 0;
volatile unsigned char i2c_register = 0x00;

uint16_t freq_middle = 12500.0;
uint16_t freq_range = 2500.0; //2500.0;
const float sampling_duration = 4e-6;
uint8_t pps_sync = 0;
uint8_t pps_sync_max = 15;
uint8_t pps_sync_chirp = 0;
bool recompute_chirp = false;
bool enable_chirp = false;

void enable_emission(){
    RELAIS_1_SetLow();
    RELAIS_2_SetLow();
    SIGNAL_ENABLE_SetHigh();
}

void enable_reception(){
    RELAIS_1_SetHigh();
    RELAIS_2_SetHigh();
    SIGNAL_ENABLE_SetLow();
}

void compute_chirp(){
    float t = 0.;
    for(int i = 0; i < SAMPLE_NUMBER; i++){
        data_chirp[i] = round(1842.5*sin(2.0*M_PI*t*(freq_middle+freq_range*(t/(2.0*SAMPLE_NUMBER*sampling_duration)-0.5))) + 2047.0);
        t+=sampling_duration;
    }
}

void EX_INT0_CallBack(){
    pps_sync++;
    if(pps_sync>pps_sync_max)
        pps_sync = 0;
    if(pps_sync == pps_sync_chirp){
        if(!recompute_chirp && enable_chirp){
            LED_SetLow();
            enable_emission();
            DMA_ChannelEnable(0);
            enable_reception();
            LED_SetHigh();
        }
    }
}

bool I2C1_StatusCallback(I2C1_SLAVE_DRIVER_STATUS status)
{
    static uint8_t i2c_data, i2c_address;
    static uint8_t i2c_address_rest = true;
    static uint8_t i2c_default_data = 0x00;

    switch (status)
    {
        case I2C1_SLAVE_TRANSMIT_REQUEST_DETECTED:
            // set up the slave driver buffer transmit pointer
            i2c_address_rest = false;
            
            switch(i2c_address){
//                case 0x00 ... 0x03: // PWM values
//                    I2C1_ReadPointerSet(&(((char*)countdown_pwm_cmd)[i2c_address]));
//                    break;
//                case 0x10 ... 0x34: // Channels
//                    I2C1_ReadPointerSet(&(((char*)channels)[i2c_address-0x10]));
//                    break;
                case 0x00:
                    I2C1_ReadPointerSet(&pps_sync);
                    break;
                case 0x01:
                    I2C1_ReadPointerSet(&pps_sync_max);
                    break;
                case 0x02:
                    I2C1_ReadPointerSet(&pps_sync_chirp);
                    break;
                case 0x03 ... 0x04:
                    I2C1_ReadPointerSet(&freq_middle + (i2c_address - 0x03));
                    break;
                case 0x05 ... 0x06:
                    I2C1_ReadPointerSet(&freq_range + (i2c_address - 0x05));
                    break;
                case 0x07:
                    I2C1_ReadPointerSet(&recompute_chirp);
                    break;
                case 0x08:
                    I2C1_ReadPointerSet(&enable_chirp);
                    break;                
                
                case 0xC0:
                    I2C1_ReadPointerSet(&code_version);
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
                        pps_sync = i2c_data;
                        break;
                    case 0x01:
                        pps_sync_max = i2c_data;
                        break;
                    case 0x02:
                        pps_sync_chirp = i2c_data;
                        break;
                    case 0x03:
                        freq_middle = (freq_middle & 0xFF00) + i2c_data;
                        break;
                    case 0x04:
                        freq_middle = (freq_middle & 0xFF) + (i2c_data<<8);
                        break;
                    case 0x05:
                        freq_range = (freq_range & 0xFF00) + i2c_data;
                        break;
                    case 0x06:
                        freq_range = (freq_range & 0xFF) + (i2c_data<<8);
                        break;
                    case 0x07:
                        if(i2c_data == 1)
                            recompute_chirp = true;
                        break;
                    case 0x08:
                        if(i2c_data == 1)
                            enable_chirp = true;
                        else
                            enable_chirp = false;
                        break;
                    
                    case 0xA0:
                        if(i2c_data == 1){
                            enable_emission();
                        }
                        else{
                            enable_reception();
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
    LED_SetLow();
    compute_chirp();
    LED_SetHigh();
    
    // initialize the device
    SYSTEM_Initialize();
    
    while (1)
    {
        ClrWdt();
        // Add your application code
        if(recompute_chirp){
            
            compute_chirp();
            recompute_chirp = false;
        }
                
    }
    return 1; 
}
/**
 End of File
*/

