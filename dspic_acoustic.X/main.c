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

const char device_name[16] = "DSPIC_ACOUSTIC";
const char code_version = 0x02;
volatile unsigned char i2c_nb_bytes = 0;
volatile unsigned char i2c_register = 0x00;


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
//                    case 0x00 ... 0x01:
//                        half_pwm = ((float)i2c_data)*((motor_cmd_max-motor_cmd_min)/255.0)+motor_cmd_min;
//                        countdown_pwm_cmd[i2c_address] = round(half_pwm); // (PWM_MAX-PWM_MIN)/255+PWM_MIN => 1102 for rounding
//                        watchdog_countdown_restart = watchdog_restart_default;
//                        break;
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

//void get_battery( uint16_t adcVal ){
//    battery_volt[0] = adcVal;
//    battery_volt[1] = adcVal>>8;
//}

/*
                         Main application
 */
int main(void)
{ 
    // initialize the device
    SYSTEM_Initialize();

    // Timers
//    TMR1_SetInterruptHandler(&timer_pwm);
//    ADC1_SetV_BATTInterruptHandler(&get_battery);
    
    for(int i=0; i<20; i++){
        LED_Toggle();
        __delay_ms(100);        
    }
    
    while (1)
    {
        ClrWdt();
        // Add your application code
                
    }
    return 1; 
}
/**
 End of File
*/

