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
        Device            :  PIC18LF14K22
        Driver Version    :  2.00
 */

#include "mcc_generated_files/mcc.h"
#include <math.h>

#define ADC_DELAY_BETWEEN_SAMPLE 4

// I2C
#define CODE_VERSION 0x06
const char device_name[16] = "PIC_ALIM v6";
volatile unsigned char i2c_nb_bytes = 0;
volatile unsigned char i2c_register = 0x00;

// State machine
enum power_state {
    IDLE, MEASURE_VOLTAGE, POWER_ON, WAIT_TO_SLEEP, SLEEP
};
volatile unsigned char state = MEASURE_VOLTAGE;
volatile unsigned char step_state_machine = 0;

// Batteries
volatile uint8_t power_current[6] = {0, 0, 0, 0, 0, 0};

// Battery minimum voltage = 12V
// Bridge : R1 = 180, R2 = 820
// Adc resolution : 10bit
// Pic VCC : 3.3V
// 12 * (180/(820+180)) * (2^10 / 3.3) = 670
// Increase resolution for rounding : 1<<4 => 670<<4 = 10720
#define BATTERY_VOLTAGE_MIN_DEFAULT 10720
const unsigned short battery_voltage_adc_min = BATTERY_VOLTAGE_MIN_DEFAULT;
uint16_t battery_voltage_adc[2] = {BATTERY_VOLTAGE_MIN_DEFAULT, BATTERY_VOLTAGE_MIN_DEFAULT};
#define BATT_FILTER_VOLTAGE 0.1

// ILS
enum ils_state {
    ILS_STATE_NOT_DETECTED, ILS_STATE_DETECTED, ILS_STATE_WAIT_REMOVED
};
volatile unsigned short state_ils = ILS_STATE_NOT_DETECTED;
volatile unsigned short ils_duration_detection = 4;
volatile unsigned short ils_cpt = 4;

// Led
volatile unsigned short led_delay = 100;
volatile unsigned short cpt_led = 100;

// Sleep mode
volatile unsigned char time_to_start[3] = {0, 0, 5}; // hour, min, sec
volatile unsigned char default_time_to_start[3] = {0, 0, 5}; // hour, min, sec
volatile unsigned short time_to_stop = 350; // in sec (max 255 sec)
volatile unsigned short default_time_to_stop = 350;

// Watchdog
volatile unsigned char watchdog_cpt[2] = {60, 0}; // min, sec (only min can be set by the user)
volatile unsigned char watchdog_cpt_default = 60; // min

void i2c_handler_address() {
    I2C_Read();
    i2c_nb_bytes = 0;
}

void i2c_handler_read() {
    unsigned char read_byte = I2C_Read();
    if (i2c_nb_bytes == 0)
        i2c_register = read_byte;
    else {
        switch (i2c_register + i2c_nb_bytes - 1) {
            case 0x00:
                switch (read_byte) {
                    case 0x01:
                        state = POWER_ON;
                        break;
                    case 0x02:
                        time_to_stop = default_time_to_stop;
                        state = WAIT_TO_SLEEP;
                        break;
                }
                break;
            case 0x01:
                default_time_to_start[0] = read_byte;
                break;
            case 0x02:
                default_time_to_start[1] = read_byte;
                break;
            case 0x03:
                default_time_to_start[2] = read_byte;
                break;
            case 0x04:
                default_time_to_stop = (((unsigned short)read_byte)<<2);
                break;
            case 0x05:
                watchdog_cpt_default = read_byte;
                break;
            default:
                break;
        }
    }
    i2c_nb_bytes++;
    watchdog_cpt[0] = watchdog_cpt_default;
    watchdog_cpt[1] = 0;
}

void i2c_handler_write() {
    unsigned char mem_add = i2c_register + i2c_nb_bytes;
    switch (mem_add) {
        case 0x00:
            I2C_Write(battery_voltage_adc[0]&0xFF);
            __delay_us(20);
            break;
        case 0x01:  
            I2C_Write(battery_voltage_adc[0]>>8);
            __delay_us(20);
            break;
        case 0x02:   
            I2C_Write(battery_voltage_adc[1]&0xFF);
            __delay_us(20);
            break;
        case 0x03:   
            I2C_Write(battery_voltage_adc[1]>>8);
            __delay_us(20);
            break;
        case 0x08:
        case 0x09:
        case 0x0A:
        case 0x0B:
        case 0x0C:
        case 0x0D:
            I2C_Write(power_current[mem_add-0x08]);
            __delay_us(20);
            break;
        case 0x0E:
            I2C_Write(state);
            break;
            
        case 0xA0:
            I2C_Write(watchdog_cpt[0]);
            break;
        case 0xA1:
            I2C_Write(watchdog_cpt[1]);
            break;
        case 0xA2:
            I2C_Write(watchdog_cpt_default);
            break;

        case 0xB0:
            I2C_Write(ILS_GetValue());
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
            I2C_Write(device_name[mem_add - 0xF0]);
            break;
        
        default:
            I2C_Write(0x00);
            break;
    }
    i2c_nb_bytes++;
}

void i2c_bus_col() {
    SSPCON1bits.SSPOV = 0;
    SSPCON1bits.WCOL = 0;
    I2C_Read();
}

void ils_analysis(unsigned char new_state) {
    switch (state_ils) {
        // Start with ILS not detected
        case ILS_STATE_NOT_DETECTED:
            ils_cpt = ils_duration_detection; // Reset ILS detection duration
            if (ILS_GetValue() == 0) // Check if ILS detected
                state_ils = ILS_STATE_DETECTED;
            break;
            
        // Verify ILS is detected sufficient amount of time
        case ILS_STATE_DETECTED:
            if (ILS_GetValue() == 0){ // Check if ILS is still detected
                if (ils_cpt > 0)
                    ils_cpt--;
                else {
                    state_ils = ILS_STATE_WAIT_REMOVED;
                    state = new_state; // Change the global state machine status
                }
            }
            else{
                state_ils = ILS_STATE_NOT_DETECTED;
            }
            break;
            
        // Check if ILS was removed to avoid turn on followed by turn off
        case ILS_STATE_WAIT_REMOVED:
            if (ILS_GetValue() == 1)
                state_ils = ILS_STATE_NOT_DETECTED;
            break;
        default:
            break;
    }
}

const char ADC_BATT[2] = {BATT_1, BATT_2};
const char ADC_CURRENT[3] = {CURRENT_ESC1, CURRENT_ESC2, CURRENT_MOTOR};
void measure_power() {
    // 10-bit ADC converter (0-3.3V)
    
    for(int i=0; i<2; i++){
        uint16_t result = ADC1_GetConversion(ADC_BATT[i]);
        battery_voltage_adc[i] = (uint16_t)(battery_voltage_adc[i]*0.9 + result*0.1);
        __delay_us(ADC_DELAY_BETWEEN_SAMPLE);
    }
    
    for(int i=0; i<3; i++){
        uint16_t result = ADC1_GetConversion(ADC_CURRENT[i]);
        power_current[i*2] = result & 0xFF;
        power_current[i*2+1] = result >> 8;
        __delay_us(ADC_DELAY_BETWEEN_SAMPLE);
    }
}

/*
Every 100 ms
 */
void timer_led() {
    if (state_ils == ILS_STATE_DETECTED) { // For ILS
        LED_SetHigh();
        cpt_led = led_delay;
    } else { // Blink the Led only when cpt_led==0 during 100ms
        if (cpt_led > 0) {
            LED_SetLow();
            cpt_led--;
        } else {
            LED_SetHigh();
            cpt_led = led_delay;
        }
    }
    measure_power();
}

/*
 Every 500 ms
 */
void timer_state_machine() {
    step_state_machine = 1;
}

/*
 * Every 1s
 */
void timer_sleep() {
    // Counters
    if (state == SLEEP) { // Decreasing counter: time_to_start
        // Decrease time in time_to_start until 0 is reached
        if (time_to_start[2] > 0) { // Seconds
            time_to_start[2]--;
        } else {
            if (time_to_start[1] > 0) { // Minutes
                time_to_start[1]--;
                time_to_start[2] = 59;
            } else {
                if (time_to_start[0] > 0) { // Hours
                    time_to_start[0]--;
                    time_to_start[1] = 59;
                }
            }
        }
    } else if (state == WAIT_TO_SLEEP) { // Decreasing counter: time_to_stop
        if (time_to_stop > 0)
            time_to_stop--;
    }

    /// Watchdog
    /// Set watchdog_cpt_default to zero to disable the watchdog
    if (state == POWER_ON && watchdog_cpt_default != 0) {
        if (watchdog_cpt[1] > 0)
            watchdog_cpt[1]--;
        else {
            watchdog_cpt[1] = 59;

            if (watchdog_cpt[0] > 0)
                watchdog_cpt[0]--;
            else {
                // hour, min, sec
                default_time_to_start[0] = 0;
                default_time_to_start[1] = 0;
                default_time_to_start[2] = 2; // 2s
                time_to_stop = default_time_to_stop;

                state = WAIT_TO_SLEEP;
                watchdog_cpt[0] = watchdog_cpt_default;
            }
        }
    }
}

void ils_interrupt(){
    char read_portA = PORTA; /// See datasheet to avoid new interruptions
    char read_portB = PORTB;
}

/*
   Main application
 */
void main(void) {
    // Initialize the device
    SYSTEM_Initialize();

    // I2C
    I2C_Open();
    I2C_SlaveSetReadIntHandler(i2c_handler_read);
    I2C_SlaveSetWriteIntHandler(i2c_handler_write);
    I2C_SlaveSetAddrIntHandler(i2c_handler_address);

    I2C_SlaveSetBusColIntHandler(i2c_bus_col);
    I2C_SlaveSetWrColIntHandler(i2c_bus_col);

    // Timers
    TMR0_SetInterruptHandler(timer_sleep);
    TMR1_SetInterruptHandler(timer_led);
    TMR3_SetInterruptHandler(timer_state_machine);
    
    IOCA2_SetInterruptHandler(ils_interrupt);
    
    // Interruptions

    INTERRUPT_GlobalInterruptHighEnable();
    INTERRUPT_GlobalInterruptLowEnable();
    INTERRUPT_PeripheralInterruptEnable();

    while (1) {
        // State machine running at 500ms
        if (step_state_machine == 1) {
            step_state_machine = 0;
            
            switch (state) {
                /* Idle state
                 * Wait for ILS to be activated
                 */
                case IDLE: 
                    GLOBAL_POWER_SetLow();
                    led_delay = 100;
                    watchdog_cpt[0] = watchdog_cpt_default;
                    watchdog_cpt[1] = 0;

                    ils_analysis(MEASURE_VOLTAGE); // Before and after to reset when ils not detected
                    if (ILS_GetValue() == 1){ /// ILS not detected
                        // Ensure light is off before going to sleep
                        cpt_led = led_delay;
                        LED_SetLow();
                        // Go to sleep
                        SLEEP();
                        
                        // Go out of sleep if interruption of ILS
                        // Set the ILS state to detected as it comes from interruption
                        ils_cpt = ils_duration_detection; // Reset coundown
                        state_ils = ILS_STATE_DETECTED; // Set ILS state
                    }
                    break;
                    
                /*  Measure the voltage of the battery          
                 *  Ensure voltage is more than a minimum voltage
                 */
                case MEASURE_VOLTAGE:
                    GLOBAL_POWER_SetLow();
                    if(battery_voltage_adc[0] > battery_voltage_adc_min ||
                                battery_voltage_adc[1] > battery_voltage_adc_min)
                        state = POWER_ON;
                    else
                        state = IDLE;
                    break;

                case POWER_ON:
                    GLOBAL_POWER_SetHigh();
                    led_delay = 20; // 2sec

                    ils_analysis(IDLE);
                    
                    if(battery_voltage_adc[0] < battery_voltage_adc_min &&
                                battery_voltage_adc[1] < battery_voltage_adc_min)
                        state = WAIT_TO_SLEEP;
                    break;

                case WAIT_TO_SLEEP:
                    GLOBAL_POWER_SetHigh();
                    led_delay = 1;
                    if (time_to_stop == 0) {
                        for (char k = 0; k < 3; k++)
                            time_to_start[k] = default_time_to_start[k];
                        if(battery_voltage_adc[0] < battery_voltage_adc_min &&
                                battery_voltage_adc[1] < battery_voltage_adc_min)
                            state = IDLE;
                        else
                            state = SLEEP;
                        time_to_stop = default_time_to_stop;
                    }
                    ils_analysis(POWER_ON);
                    break;

                case SLEEP:
                    GLOBAL_POWER_SetLow();
                    led_delay = 200; // 20 sec
                    if (time_to_start[0] == 0 && time_to_start[1] == 0 && time_to_start[2] == 0) {
                        state = POWER_ON;
                    }
                    ils_analysis(MEASURE_VOLTAGE);
                    if(state==MEASURE_VOLTAGE){
                        Reset();
                    }
                    break;

                default:
                    state = POWER_ON;
                    break;
            }
        }
    }
}
/**
 End of File
 */