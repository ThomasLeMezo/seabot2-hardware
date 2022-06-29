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

#define min(a, b) (a) < (b) ? (a) : (b) 
#define max(a, b) (a) > (b) ? (a) : (b) 

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

const char device_name[16] = "DSPIC_PISTON";

// State machine
enum state_piston {
    PISTON_SEARCH_SWITCH_BOTTOM, PISTON_RESET_SWITCH_BOTTOM, PISTON_REGULATION
};
volatile unsigned char state = PISTON_SEARCH_SWITCH_BOTTOM;

// Set point & position
volatile unsigned char i2c_set_point_new = 0;
volatile signed long int position = 0;
volatile signed long int position_set_point = 0;
volatile signed long int position_error = 0;
volatile unsigned char position_set_point_i2c[4];
#define NEW_WAYPOINT_I2C 0b1111

volatile uint16_t motor_set_point = MOTOR_STOP;
volatile uint16_t motor_delta_speed = 30; // 0.4 V/0.02s 

volatile uint16_t motor_current = 0;
volatile uint16_t motor_tension = 0;

unsigned short motor_cmd_i2c = MOTOR_STOP;

// Regulation
float motor_regulation_K = 0.05;
unsigned short motor_regulation_dead_zone = 100;

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
            case 0x21:
                motor_cmd_i2c = (unsigned short)read_byte << 4;
                if(motor_cmd_i2c<=MOTOR_UP && motor_cmd_i2c>=MOTOR_DOWN){
                    motor_set_point = motor_cmd_i2c;
                }
                break;
            default:
                break;
        }
    }
    i2c_nb_bytes++;
}

void i2c_handler_write() {
    switch (i2c_register + i2c_nb_bytes) {
        case 0x00:
            I2C_Write(POS1CNT);
            break;
        case 0x01:
            I2C_Write((POS1CNT >> 8)); // not recommended (two reads)
            break;
        case 0x02:
            I2C_Write(qei_overflow);
            break;
        case 0x03:
            I2C_Write((!SWITCH_TOP_GetValue())
                       | (!SWITCH_BOTTOM_GetValue() << 1));
            break;
        case 0x04:
            I2C_Write(state);
            break;
        case 0x10:
            I2C_Write(QEI1CONbits.UPDN); // Direction status
            break;
        case 0x11:
            I2C_Write(P1DC1);
            break;
        case 0x12:
            I2C_Write(P1DC1>>8);
            break;
        
        case 0x20 ... 0x23:
        I2C_Write(position>> (8*(i2c_register + i2c_nb_bytes - 0x20)));
        break;
            
        case 0x30 ... 0x33:
            I2C_Write(position_set_point >> (8*(i2c_register + i2c_nb_bytes - 0x30)));
            break;
            
        case 0x40 ... 0x43:
            I2C_Write(position_error>> (8*(i2c_register + i2c_nb_bytes - 0x40)));
            break;
            
        case 0xB0:
            I2C_Write(motor_tension);
            break;
        case 0xB1:
            I2C_Write(motor_tension>>8);
            break;
        case 0xB2:
            I2C_Write(motor_current);
            break;
        case 0xB3:
            I2C_Write(motor_current>>8);
            break;
        case 0xB4:
            I2C_Write(motor_set_point);
            break;
        case 0xB5:
            I2C_Write(motor_set_point>>8);
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
    watchdog_restart = watchdog_restart_default;
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
    if (QEI1CONbits.UPDN)
        qei_overflow += 1;
    else
        qei_overflow -= 1;

}

/*
 * @brief Timer 1
 */
void handle_timer_light(){
    //LED_Toggle();
}

/*
 * @brief Timer 2
 */
void handle_timer_regulation(){    
    position = ((signed long int)(qei_overflow)<<16) + POS1CNT;
    
    if(i2c_set_point_new==NEW_WAYPOINT_I2C){
        i2c_set_point_new = 0;
        position_set_point = (signed long int)((unsigned long int)position_set_point_i2c[0]
                + (((unsigned long int)position_set_point_i2c[1])<<8) 
                + (((unsigned long int)position_set_point_i2c[2])<<16)
                + (((unsigned long int)position_set_point_i2c[3])<<24));
    }
    
    // ***************************
    // State measurements
    motor_current = ADC1_ConversionResultGet(CURRENT_MOTOR); // => 0.5*Vcc for 0A, 264mV/A, 12bit
    
    // => V_batt = (adc_result*3.3/4096)/0.18 = adc_result * 0.00447591
    // ex : 3574 => 15.99V
    motor_tension = ADC1_ConversionResultGet(BATT_VOLTAGE);
    
    // ***************************
    // Regulation (dumy version)
    if(state==PISTON_REGULATION){
        position_error = position_set_point-position;
        if(abs(position_error)>motor_regulation_dead_zone){
            
            signed long int val = position_error*motor_regulation_K;
            
            if(val>0)
                motor_set_point = min(max(val+MOTOR_STOP, MOTOR_STOP+MOTOR_DEAD_ZONE), MOTOR_UP);
            else
                motor_set_point = max(min(val+MOTOR_STOP, MOTOR_STOP-MOTOR_DEAD_ZONE), MOTOR_DOWN);
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
        if(abs(MOTOR_CMD-motor_set_point)>motor_delta_speed){
            MOTOR_CMD += (MOTOR_CMD<motor_set_point)?motor_delta_speed:-motor_delta_speed;
        }
        else{
            MOTOR_CMD = motor_set_point;
        }
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
    TMR3_SetInterruptHandler(handle_timer_light); // 1 s
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
    POS1CNT = 0x0000;
    
    while (1) {
        ClrWdt(); // Clear wdt
        
        if (!SWITCH_TOP_GetValue() || !SWITCH_BOTTOM_GetValue())
            LED_SetHigh();
        else
            LED_SetLow();
        
        switch(state){
            case PISTON_SEARCH_SWITCH_BOTTOM:
                if(!SWITCH_BOTTOM_GetValue()){
                    MOTOR_CMD = MOTOR_STOP;
                    motor_set_point = MOTOR_STOP;
                    state = PISTON_RESET_SWITCH_BOTTOM;
                    __delay_ms(250);
                }
                else{
                    ENABLE_SetHigh();
                    motor_set_point = MOTOR_DOWN;
                }
                    
                break;
            
            case PISTON_RESET_SWITCH_BOTTOM:
                if(SWITCH_BOTTOM_GetValue()){
                    MOTOR_CMD = MOTOR_STOP;
                    motor_set_point = MOTOR_STOP;
                    state = PISTON_REGULATION;
                    __delay_ms(250);
                    QEI_Reset_Count();
                }
                else{
                    ENABLE_SetHigh();
                    motor_set_point = MOTOR_UP_RESET;
                }
                break;
                
            case PISTON_REGULATION:
                ENABLE_SetHigh();
                break;
            default:
                break;
        }
        //ENABLE = 1;
    }

    return 0;
}
