/**
  Generated Main Source File

  Company:
    

  File Name:
    main.c

  Summary:
    This is the main file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.7
        Device            :  PIC18F14K22
        Driver Version    :  2.00
*/

#include "mcc_generated_files/mcc.h"
#define CODE_VERSION 0x02
volatile unsigned char i2c_nb_bytes = 0;
volatile unsigned char i2c_register = 0x00;
volatile unsigned char led_pwm = 199;
volatile unsigned char led_enable = 0;
#define NB_PATTERN 10
volatile unsigned char led_pattern[NB_PATTERN] = {1, 20, 0, 0, 0, 0, 0, 0, 0, 0};
volatile unsigned char led_pattern_index = NB_PATTERN-1;
volatile unsigned char led_count_down = 0;

const char device_name[16] = "PIC_LIGHT v2";

// i2cSlaveState

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
        unsigned char reg_add = i2c_register+i2c_nb_bytes-1;
        switch(reg_add)
        {
            case 0x00:
                led_enable = read_byte;
                led_pattern_index = NB_PATTERN-1;
                led_count_down = 0;
                break;
            case 0x01:  // Power
                led_pwm = read_byte; // 0 to 199
                break;
            case 0x02: // Pattern
            case 0x03:
            case 0x04:
            case 0x05:
            case 0x06:
            case 0x07:
            case 0x08:
            case 0x09:
            case 0x0A:
            case 0x0B:
                led_pattern[reg_add-(unsigned char)0x02] = read_byte;
                break;
        default:
          break;
        }
    }
    i2c_nb_bytes++;
}

void i2c_handler_write()
{
    switch(i2c_register+i2c_nb_bytes)
    {
        case 0x00:
            I2C_Write(led_enable);
            break;
        case 0x01:  // Power
            I2C_Write(led_pwm);
            break;
        case 0x02:
        case 0x03:
        case 0x04:
        case 0x05:
        case 0x06:
        case 0x07:
        case 0x08:
        case 0x09:
        case 0x0A:
        case 0x0B:
            I2C_Write(led_pattern[(i2c_register+i2c_nb_bytes) - (unsigned char)0x02]);
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
            I2C_Write(device_name[(i2c_register + i2c_nb_bytes) - (unsigned char)0xF0]);
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

void timer_handler(){
    if(led_count_down==0)
    {
        led_pattern_index++;
        while(led_pattern[led_pattern_index]==0 && led_pattern_index<NB_PATTERN)
            led_pattern_index++;
        if(led_pattern_index==NB_PATTERN)
            led_pattern_index = 0;
        
        led_count_down = led_pattern[led_pattern_index]-1; // Remove 1 bc it is the current state
        
        if((led_pattern_index%2==0) && led_enable)
            EPWM1_LoadDutyValue(led_pwm);
        else
            EPWM1_LoadDutyValue(0);
    }
    else
    {
        led_count_down--;
    }
}

/*
                         Main application
 */
void main(void)
{
    // Initialize the device
    SYSTEM_Initialize();
    
    I2C_Open();
    I2C_SlaveSetReadIntHandler(i2c_handler_read);
    I2C_SlaveSetWriteIntHandler(i2c_handler_write);
    I2C_SlaveSetAddrIntHandler(i2c_handler_address);
    
    I2C_SlaveSetBusColIntHandler(i2c_bus_col);
    I2C_SlaveSetWrColIntHandler(i2c_bus_col);
    
    TMR0_SetInterruptHandler(timer_handler);

    // If using interrupts in PIC18 High/Low Priority Mode you need to enable the Global High and Low Interrupts
    // If using interrupts in PIC Mid-Range Compatibility Mode you need to enable the Global and Peripheral Interrupts
    // Use the following macros to:

    // Enable the Global Interrupts
    INTERRUPT_GlobalInterruptEnable();

    // Disable the Global Interrupts
    //INTERRUPT_GlobalInterruptDisable();

    // Enable the Peripheral Interrupts
    INTERRUPT_PeripheralInterruptEnable();

    // Disable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptDisable();
    
    EPWM1_LoadDutyValue(0); // 0 to 199 [start at 49]

    while (1)
    {
        // Add your application code
    }
}
/**
 End of File
*/