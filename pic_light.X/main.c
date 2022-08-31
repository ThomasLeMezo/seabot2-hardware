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
#define CODE_VERSION 0x01
volatile unsigned char i2c_nb_bytes = 0;
volatile unsigned char i2c_register = 0x00;
volatile unsigned short int led_pwm = 199;
volatile unsigned char led_toogle = 0;
volatile unsigned char led_enable = 0;
#define NB_PATTERN 6
volatile unsigned char led_pattern[NB_PATTERN] = {1, 20, 0, 0, 0, 0};
volatile unsigned char led_pattern_index = NB_PATTERN-1;
volatile unsigned char led_count_down = 0;

const char device_name[16] = "PIC_LIGHT";

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
        switch(i2c_register+i2c_nb_bytes-1)
        {
            case 0x00:
                led_enable = read_byte;
                break;
            case 0x01:  // Power
                led_pwm = read_byte; // 0 to 199
                break;
            case 0x02 ... 0x07: // Pattern
                led_pattern[i2c_register+i2c_nb_bytes-1-2] = read_byte;
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
        case 0x02: // Pattern
        case 0x03: // Pattern
        case 0x04: // Pattern
        case 0x05: // Pattern
        case 0x06: // Pattern
        case 0x07: // Pattern
            I2C_Write(led_pattern[i2c_register+i2c_nb_bytes-1-2]);
            break;
        case 0xC0:
            I2C_Write(CODE_VERSION);
            break;
        case 0xF0 ... 0xFF:
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

void timer_handler(){
    if(led_count_down==0)
    {
        led_pattern_index++;
        while(led_pattern[led_pattern_index]==0 && led_pattern_index<NB_PATTERN)
            led_pattern_index++;
        if(led_pattern_index==NB_PATTERN)
            led_pattern_index = 0;
        
        
        led_count_down = led_pattern[led_pattern_index]-1;
        led_toogle = !led_toogle;
        if(led_toogle && led_enable)
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