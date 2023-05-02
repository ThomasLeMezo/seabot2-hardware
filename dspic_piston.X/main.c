/*  DSPIC33FJ32MC202
External oscillator 16MHz

RB5 : Top switch (RC0)
RB6 : Bottom switch (RC1)
RB7 : Interrupt for switch (Nand gate)

RB12 : Codeur Indx
RB11 : Codeur B
RB10 : Codeur A

RB13 : LED

RB14 : IN1 (PWM motor)
RA1 : Enable
RA0 : Motor Current Sensor

 */

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b)) 

#include <xc.h>
#include <stdlib.h>
#include "config.h"

#define FCY 16000000UL
#include <dsp.h>
#include <libpic30.h>
#include <libq.h>
#include <limits.h>
#include <float.h>
#include <ctype.h>

#include <math.h>

// I2C
volatile unsigned char i2c_nb_bytes = 0;
volatile unsigned char i2c_register = 0x00;

const char device_name[16] = "DSPIC_PISTON v6";

// State machine
enum state_piston {
    PISTON_SEARCH_SWITCH_BOTTOM, PISTON_RELEASE_SWITCH_BOTTOM, PISTON_BACK_SWITCH_BOTTOM, PISTON_REGULATION, PISTON_EXIT, PISTON_BATT_LOW
};
volatile unsigned char state = PISTON_SEARCH_SWITCH_BOTTOM;
unsigned char is_reset_once = false;

// Set point & position
volatile unsigned char i2c_set_point_new = 0;
volatile signed long int position = 0;
volatile signed long int position_reset = 0;
volatile signed long int position_set_point = 0;
volatile signed long int position_error = 0;
volatile unsigned char position_set_point_i2c[4];
#define NEW_WAYPOINT_I2C 0b1111

#define REGULATION_LOOP_FREQ 50

volatile uint16_t motor_set_point = MOTOR_STOP;
volatile uint16_t motor_delta_speed = (100/REGULATION_LOOP_FREQ)*MOTOR_V_TO_CMD; // Limit to 100V/s, delta_speed in PWM quantum/0.02s = 250
volatile unsigned char motor_enable_cpt_reset = 5;
volatile unsigned char motor_enable_cpt = 5;

volatile uint16_t adc_motor_current = 0;
volatile uint16_t adc_batt_tension = 0;
#define ADC_BATT_TENSION_THRESHOLD 2703.0
#define ADC_BATT_TENSION_FILTER_COEFF 0.9
volatile float adc_batt_tension_filtered = ADC_BATT_TENSION_THRESHOLD;

volatile float motor_current = 0;
volatile float motor_tension = 0;
volatile float batt_tension = 0;

unsigned short motor_cmd_i2c = MOTOR_STOP;

// Regulation
float motor_regulation_K = 0.3;
float motor_regulation_Ki = 0.0;
unsigned short motor_regulation_dead_zone = 50;

unsigned char i2c_new_data_motor_regulation_K = 0;
unsigned char i2c_new_motor_regulation_K[2]; /// div by 100 to get float motor_regulation

unsigned char i2c_new_data_motor_regulation_Ki = 0;
unsigned char i2c_new_motor_regulation_Ki[2]; /// div by 100 to get float motor_regulation

unsigned char i2c_new_data_motor_regulation_dead_zone = 0;
unsigned char i2c_new_motor_regulation_dead_zone[2];

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
            case 0x00 ... 0x03:
                i2c_set_point_new|=0b1<<(i2c_register + i2c_nb_bytes - 1);
                position_set_point_i2c[i2c_register + i2c_nb_bytes - 1] = read_byte;
                break;
            case 0x05:
                if(read_byte==PISTON_REGULATION){
                    if(is_reset_once)
                        state = PISTON_REGULATION;
                }
                else
                    state = read_byte;
                break;
            case 0x06:
                motor_enable_cpt_reset = read_byte;
                break;
            case 0x10:
                if (read_byte)
                    ENABLE_SetHigh();
                else
                    ENABLE_SetLow();
                break;
            case 0x20:
                if (read_byte)
                    LED_SetHigh();
                else
                    LED_SetLow();
                break;
                    /// Regulation data
            case 0x30:
                i2c_new_data_motor_regulation_dead_zone|=0b1<<(i2c_register + i2c_nb_bytes - 1 - 0x30);
                i2c_new_motor_regulation_dead_zone[i2c_register + i2c_nb_bytes - 1 - 0x30] = read_byte;
                break;
            case 0x32:
                i2c_new_data_motor_regulation_K|=0b1<<(i2c_register + i2c_nb_bytes - 1 - 0x32);
                i2c_new_motor_regulation_K[i2c_register + i2c_nb_bytes - 1 - 0x32] = read_byte;
                break;
            default:
                break;
        }
    }
    i2c_nb_bytes++;
    watchdog_countdown_restart = watchdog_restart_default;
}

void i2c_handler_write() {
    switch (i2c_register + i2c_nb_bytes) {
        case 0x00 ... 0x03: /// Position of the piston
            I2C_Write(position>> (8*(i2c_register + i2c_nb_bytes - 0x00)));
        break;
        case 0x04: /// Switchs of the piston
            I2C_Write((!SWITCH_TOP_GetValue())
                       | (!SWITCH_BOTTOM_GetValue() << 1)
                        | (ENABLE_GetValue() << 2)
                        | (QEI1CONbits.UPDN << 3)); /// Direction status
            break;
        case 0x05: /// State of the state-machine
            I2C_Write(state);
            break;
        case 0x06 ... 0x09: /// Set point
            I2C_Write(position_set_point >> (8*(i2c_register + i2c_nb_bytes - 0x06)));
            break;
        case 0x0A ... 0x0B:
            I2C_Write(adc_batt_tension>> (8*(i2c_register + i2c_nb_bytes - 0x0A)));
            break;
        case 0x0C ... 0x0D:
            I2C_Write(adc_motor_current>> (8*(i2c_register + i2c_nb_bytes - 0x0C)));
            break;
        case 0x0E ... 0x0F:
            I2C_Write(motor_set_point>> (8*(i2c_register + i2c_nb_bytes - 0x0E)));
            break;
        case 0x10:
            I2C_Write(MOTOR_CMD);
            break;
        case 0x11:
            I2C_Write(MOTOR_CMD>>8);
            break;
        case 0x12 ... 0x15:
            I2C_Write(position_reset>> (8*(i2c_register + i2c_nb_bytes - 0x00)));
            
            /// End of status data
            
        /// Regulation data
        case 0x30 ... 0x31:
            I2C_Write(motor_regulation_dead_zone >> (8*(i2c_register + i2c_nb_bytes - 0x30)));
            break;
        case 0x32 ... 0x33:
            I2C_Write((unsigned short)(motor_regulation_K*100) >> (8*(i2c_register + i2c_nb_bytes - 0x32)));
            break;
            
        case 0x40 ... 0x43:
            I2C_Write(position_error>> (8*(i2c_register + i2c_nb_bytes - 0x40)));
            break;
            
        case 0xC0:
            I2C_Write(CODE_VERSION);
            break;
        case 0xC1:
            I2C_Write(is_init);
            break;
            
        case 0xF0 ... 0xFF:
            I2C_Write(device_name[i2c_register + i2c_nb_bytes - 0xF0]);
            break;
            
        default:
            I2C_Write(0x00);
            break;
    }
    watchdog_countdown_restart = watchdog_restart_default;
    i2c_nb_bytes++;
}

/**
 * Interrupt switch
 */
void __attribute__((__interrupt__, auto_psv)) _INT0Interrupt() {
    if (IFS0bits.INT0IF) {
        IFS0bits.INT0IF = 0;
        if ((!SWITCH_TOP_GetValue() && MOTOR_CMD > MOTOR_STOP)
                || (!SWITCH_BOTTOM_GetValue() && MOTOR_CMD < MOTOR_STOP)) {
            MOTOR_CMD = MOTOR_STOP;
        }
    }
}

/**
 * Interruption for QEI when overflow
 */
void __attribute__((__interrupt__, auto_psv)) _QEIInterrupt() {
    _QEIIF = 0;
    /// ToDo : reset if overflow ?
}

/*
 * @brief Timer 3
 */
void handle_timer_watchdog(){
    if(watchdog_countdown_restart>0)
      watchdog_countdown_restart--;  
    else{
        if(state == PISTON_REGULATION)
            state = PISTON_EXIT;
    }
}

/*
 * @brief Timer 2
 */
void handle_timer_regulation(){
    int read_pos1cnt = POS1CNT - POS1CNT_MIDDLE;
    POS1CNT -= read_pos1cnt;
    position += read_pos1cnt;
    
    if(i2c_set_point_new==NEW_WAYPOINT_I2C){
        i2c_set_point_new = 0;
        position_set_point = (signed long int)((unsigned long int)position_set_point_i2c[0]
                + (((unsigned long int)position_set_point_i2c[1])<<8) 
                + (((unsigned long int)position_set_point_i2c[2])<<16)
                + (((unsigned long int)position_set_point_i2c[3])<<24));
    }
    
    if(i2c_new_data_motor_regulation_dead_zone==0b11){
        motor_regulation_dead_zone = i2c_new_motor_regulation_dead_zone[0] + (i2c_new_motor_regulation_dead_zone[1]<<8);
        i2c_new_data_motor_regulation_dead_zone = 0;
    }
    
    if(i2c_new_data_motor_regulation_K==0b11){
        motor_regulation_K = (float)(i2c_new_motor_regulation_K[0] + (i2c_new_motor_regulation_K[1]<<8))/100;
        i2c_new_data_motor_regulation_K = 0;
    }
    
    // ***************************
    // State measurements (adc 12bit : 4096)
    // sensor: ACS722LLCTR-05AB-T

    adc_motor_current = ADC1_ConversionResultGet(CURRENT_MOTOR); // => 0.5*Vcc for 0A (+-5A), 264mV/A, 12bit
    //motor_current = ((int16_t)adc_motor_current-2048)*(1.65/2048*0.264); // Vcc=3.3
            
    // => V_batt = (adc_result*3.3/4096)/(180/(820+180)) = adc_result * 0.00447591
    // ex : 3574 => 15.99V
    
    __delay_us(3);
    adc_batt_tension = ADC1_ConversionResultGet(BATT_VOLTAGE);
    //batt_tension = (adc_batt_tension*3.3/4096.0) / (180.0/(180.0+820.0)); // 180kOhm & 820kOhm
    adc_batt_tension_filtered = adc_batt_tension_filtered*ADC_BATT_TENSION_FILTER_COEFF
            + adc_batt_tension*(1.0-ADC_BATT_TENSION_FILTER_COEFF);
    
    //motor_tension = batt_tension * ((MOTOR_CMD-MOTOR_STOP)/(MOTOR_PWM_MAX/2.0));
    
    // ***************************
    // Regulation (dumy version)
    if(state==PISTON_REGULATION){
        position_error = position_set_point-position;
        if(abs(position_error)>motor_regulation_dead_zone){
            
            signed long int val = floor(((float)position_error)*motor_regulation_K);
            
            if(val>=0)
                motor_set_point = min(max(val, MOTOR_DEAD_ZONE)+MOTOR_STOP, MOTOR_UP);
            else
                motor_set_point = max(min(val, -MOTOR_DEAD_ZONE)+MOTOR_STOP, MOTOR_DOWN);
        }
        else{
            motor_set_point = MOTOR_STOP;
        }
    }
    
    // ***************************
    // Velocity Ramp + Switch protection
    if((motor_set_point<MOTOR_STOP && !SWITCH_BOTTOM_GetValue()) 
            || (motor_set_point>MOTOR_STOP && !SWITCH_TOP_GetValue())){
        MOTOR_CMD = MOTOR_STOP;
    }
    else{
        if(abs(motor_set_point-MOTOR_CMD)>motor_delta_speed){
            MOTOR_CMD += (MOTOR_CMD<motor_set_point)?motor_delta_speed:-motor_delta_speed;
        }
        else{
            MOTOR_CMD = motor_set_point;
        }
    }
    
    if(MOTOR_CMD == MOTOR_STOP){
        if(motor_enable_cpt>0)
            motor_enable_cpt--;
        else
            ENABLE_SetLow();
    }
    else{
        ENABLE_SetHigh();
        motor_enable_cpt = motor_enable_cpt_reset;
    }
    // Add a cpt to set enable Low when position set_point is reached in the case Regulation
}

/**
 * @brief main
 */
int main() {
    RCONbits.SWDTEN = 0; // Disable the watchdog
    
    SYSTEM_Initialize();  // 40 MIPS
    
    // Timers
    TMR3_SetInterruptHandler(handle_timer_watchdog); // 1 s
    TMR2_SetInterruptHandler(handle_timer_regulation); // 0.02 s
    
     // Initialize I/O
    I2C_Open();
    I2C_SlaveSetReadIntHandler(i2c_handler_read);
    I2C_SlaveSetWriteIntHandler(i2c_handler_write);
    I2C_SlaveSetAddrIntHandler(i2c_handler_address);

    ENABLE_SetLow();
    LED_SetLow();

    is_init = 1;

    // Resset the  POS1CNT = 0x00;
    QEI_Reset_Count();
    
    while (1) {
        ClrWdt(); // Clear wdt
        
        if (!SWITCH_TOP_GetValue() || !SWITCH_BOTTOM_GetValue())
            LED_SetHigh();
        else
            LED_SetLow();
        
        switch(state){
            /// Rapid move to bottom to find the bottom switch
            case PISTON_SEARCH_SWITCH_BOTTOM:
                if(!SWITCH_BOTTOM_GetValue()){
                    MOTOR_CMD = MOTOR_STOP;
                    motor_set_point = MOTOR_STOP;
                    __delay_ms(250);
                    if(!SWITCH_BOTTOM_GetValue()){
                        state = PISTON_RELEASE_SWITCH_BOTTOM;
                    }
                }
                else{
                    motor_set_point = MOTOR_DOWN;
                }
                break;
            
            /// Slow move upward to release the switch
            case PISTON_RELEASE_SWITCH_BOTTOM:
                if(SWITCH_BOTTOM_GetValue()){
                    MOTOR_CMD = MOTOR_STOP;
                    motor_set_point = MOTOR_STOP;
                    if(SWITCH_BOTTOM_GetValue()){
                        state = PISTON_BACK_SWITCH_BOTTOM;
                    }
                }
                else{
                    motor_set_point = MOTOR_UP_RESET;
                }
                break;
                
            /// Slow move backward to enable the switch and make the zero
            case PISTON_BACK_SWITCH_BOTTOM:
                if(!SWITCH_BOTTOM_GetValue()){
                    MOTOR_CMD = MOTOR_STOP;
                    motor_set_point = MOTOR_STOP;
                    if(SWITCH_BOTTOM_GetValue()){
                        state = PISTON_REGULATION;
                        QEI_Reset_Count();
                        position_reset = position;
                        position = 0;
                        is_reset_once = true;
                    }
                }
                else{
                    motor_set_point = MOTOR_DOWN_RESET;
                }
                break;
                
            case PISTON_REGULATION:
                if(adc_batt_tension_filtered<ADC_BATT_TENSION_THRESHOLD)
                    state = PISTON_BATT_LOW;
                break;
                
            case PISTON_EXIT:
                if(!SWITCH_BOTTOM_GetValue()){
                    MOTOR_CMD = MOTOR_STOP;
                    motor_set_point = MOTOR_STOP;
                    position_set_point = position;
                }
                else{
                    motor_set_point = MOTOR_DOWN;
                }
                break;
                
            case PISTON_BATT_LOW:
                if(!SWITCH_BOTTOM_GetValue()){
                    MOTOR_CMD = MOTOR_STOP;
                    motor_set_point = MOTOR_STOP;
                    position_set_point = position;
                }
                else{
                    motor_set_point = MOTOR_DOWN;
                }
                break;
                
            default:
                break;
        }
        //ENABLE = 1;
    }

    return 0;
}
