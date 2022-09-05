/**
  Generated Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This is the main file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.7
        Device            :  PIC18LF14K22
        Driver Version    :  2.00
*/

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/

#include "mcc_generated_files/mcc.h"

// Thrusters
#define MOTOR_CMD_STOP 151
#define MOTOR_CMD_MIN 111
#define MOTOR_CMD_MAX 191
#define PWM_PERIOD 2000
volatile unsigned char countdown_thruster_cmd[2];
volatile unsigned char countdown_thruster[2];
volatile unsigned short countdown_pwm_period = PWM_PERIOD;

volatile unsigned char unable_motor[2]; 

// Watchdog
//#define EEPROM_WATCHDOG_RESTART_DEFAULT 0x00
volatile unsigned char watchdog_restart_default = 3;
volatile unsigned char watchdog_countdown_restart = 3;

// I2C
#define CODE_VERSION 0x01
volatile unsigned char i2c_nb_bytes = 0;
volatile unsigned char i2c_register = 0x00;

const char device_name[16] = "PIC_THRUSTER v1";

void i2c_handler_address()
{
    I2C_Read();
    i2c_nb_bytes = 0;    
}

void i2c_handler_read()
{
    unsigned char read_byte = I2C_Read();
    if(i2c_nb_bytes==0)
        i2c_register = read_byte;
    else
    {
        switch(i2c_register+i2c_nb_bytes-1)
        {
            case 0x00:
                if(read_byte > MOTOR_CMD_MIN && read_byte < MOTOR_CMD_MAX)
                    countdown_thruster_cmd[0] = read_byte;
                break;
            case 0x01:  
                if(read_byte > MOTOR_CMD_MIN && read_byte < MOTOR_CMD_MAX)
                    countdown_thruster_cmd[1] = read_byte;
                break;
            case 0x02:
                watchdog_restart_default = read_byte;
                break;
                
            case 0x10:
                unable_motor[0] = (read_byte==1);
                break;
            case 0x11:
                unable_motor[1] = (read_byte==1);
                break;
        default:
          break;
        }
    }
    i2c_nb_bytes++;
    watchdog_countdown_restart = watchdog_restart_default;
}

void i2c_handler_write()
{
    switch(i2c_register+i2c_nb_bytes)
    {
        case 0x00:
            I2C_Write(countdown_thruster_cmd[0]);
            break;
        case 0x01:
            I2C_Write(countdown_thruster_cmd[1]);
            break;
        case 0x03:
            I2C_Write(watchdog_restart_default);
            break;
        case 0x10:
            I2C_Write(unable_motor[0]);
            break;
        case 0x11:
            I2C_Write(unable_motor[1]);
            break;
        case 0xC0:
            I2C_Write(CODE_VERSION);
            break;
        case 0xF0:
        case 0xF1:
        case 0xF2:
        case 0xF3:
        case 0xF4:
        case 0xF5:
        case 0xF6:
        case 0xF7:
        case 0xF8:
        case 0xF9:
        case 0xFA:
        case 0xFB:
        case 0xFC:
        case 0xFD:
        case 0xFE:
        case 0xFF:
            I2C_Write(device_name[i2c_register + i2c_nb_bytes - 0xF0]);
            break;
        
        default:
            I2C_Write(0x00);
          break;
    }
    i2c_nb_bytes++;
}

void i2c_bus_col(){
    SSPCON1bits.SSPOV = 0;
    SSPCON1bits.WCOL = 0;
    I2C_Read();
}

void timer_watchdog()
{
    if(watchdog_countdown_restart>0)
      watchdog_countdown_restart--;  
    else{
      countdown_thruster_cmd[0] = MOTOR_CMD_STOP;
      countdown_thruster_cmd[1] = MOTOR_CMD_STOP;
      //watchdog_countdown_restart = watchdog_restart_default;
    }
}

/*
 Timer 1 is set to 10us minus execution time of timer_pwm()
 */
void timer_pwm()
{
    if(countdown_pwm_period==0){
        if(unable_motor[0]==1)
            ESC1_SetHigh();
        if(unable_motor[1]==1)
            ESC2_SetHigh();
        countdown_pwm_period = PWM_PERIOD;

        countdown_thruster[0] = countdown_thruster_cmd[0];
        countdown_thruster[1] = countdown_thruster_cmd[1];
    }
    else{
      countdown_pwm_period--;
      // ESC1 1
      if(countdown_thruster[0]==0)
          ESC1_SetLow();
      else
        countdown_thruster[0]--;
      // ESC 2
      if(countdown_thruster[1]==0)
          ESC2_SetLow();
      else
        countdown_thruster[1]--;
    }
}

/*
                         Main application
 */
void main(void)
{
    // Initialize the device
    SYSTEM_Initialize();
    
    countdown_thruster_cmd[0] = MOTOR_CMD_STOP;
    countdown_thruster_cmd[1] = MOTOR_CMD_STOP;
    
    unable_motor[0] = 1;
    unable_motor[1] = 1;
    
    I2C_Open();
    I2C_SlaveSetReadIntHandler(i2c_handler_read);
    I2C_SlaveSetWriteIntHandler(i2c_handler_write);
    I2C_SlaveSetAddrIntHandler(i2c_handler_address);
    
    I2C_SlaveSetBusColIntHandler(i2c_bus_col);
    I2C_SlaveSetWrColIntHandler(i2c_bus_col);
    
    TMR0_SetInterruptHandler(timer_watchdog);
    TMR1_SetInterruptHandler(timer_pwm);

    INTERRUPT_GlobalInterruptHighEnable();
    INTERRUPT_GlobalInterruptLowEnable();
    INTERRUPT_PeripheralInterruptEnable();

    while (1)
    {
        if(countdown_thruster_cmd[0] != MOTOR_CMD_STOP || countdown_thruster_cmd[1] != MOTOR_CMD_STOP)
            LED_SetHigh();
        else
            LED_SetLow();
    }
}
/**
 End of File
*/