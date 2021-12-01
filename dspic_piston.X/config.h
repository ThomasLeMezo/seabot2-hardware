#ifndef CONFIG_H
#define CONFIG_H

#include "interrupt_manager.h"
#include "i2c_slave.h"
#include "tmr1.h"
#include "tmr2.h"

#define CODE_VERSION 0x01

extern volatile unsigned short is_init;

// get/set ENABLE aliases
#define ENABLE_TRIS                 TRISBbits.TRISB1
#define ENABLE_LAT                  LATBbits.LATB1
#define ENABLE_PORT                 PORTBbits.RB1
#define ENABLE_WPU                  WPUBbits.WPUB1
#define ENABLE_ANS                  ANSELbits.ANS1
#define ENABLE_SetHigh()            do { LATBbits.LATB1 = 1; } while(0)
#define ENABLE_SetLow()             do { LATBbits.LATB1 = 0; } while(0)
#define ENABLE_Toggle()             do { LATBbits.LATB1 = ~LATBbits.LATB1; } while(0)
#define ENABLE_GetValue()           PORTAbits.RB1
#define ENABLE_SetDigitalInput()    do { TRISBbits.TRISB1 = 1; } while(0)
#define ENABLE_SetDigitalOutput()   do { TRISBbits.TRISB1 = 0; } while(0)
#define ENABLE_SetPullup()          do { WPUBbits.WPUB1 = 1; } while(0)
#define ENABLE_ResetPullup()        do { WPUBbits.WPUB1 = 0; } while(0)
#define ENABLE_SetAnalogMode()      do { ANSELbits.ANS1 = 1; } while(0)
#define ENABLE_SetDigitalMode()     do { ANSELbits.ANS1 = 0; } while(0)

// get/set LED aliases
#define LED_TRIS                 TRISBbits.TRISB13
#define LED_LAT                  LATBbits.LATB13
#define LED_PORT                 PORTBbits.RB13
#define LED_WPU                  WPUBbits.WPUB13
#define LED_SetHigh()            do { LATBbits.LATB13 = 1; } while(0)
#define LED_SetLow()             do { LATBbits.LATB13 = 0; } while(0)
#define LED_Toggle()             do { LATBbits.LATB13 = ~LATBbits.LATB13; } while(0)
#define LED_GetValue()           PORTBbits.RB13
#define LED_SetDigitalInput()    do { TRISBbits.TRISB13 = 1; } while(0)
#define LED_SetDigitalOutput()   do { TRISBbits.TRISB13 = 0; } while(0)
#define LED_SetPullup()          do { WPUBbits.WPUB13 = 1; } while(0)
#define LED_ResetPullup()        do { WPUBbits.WPUB13 = 0; } while(0)

// get/set LED aliases
#define SWITCH_TOP_TRIS                 TRISBbits.TRISB5
#define SWITCH_TOP_LAT                  LATBbits.LATB5
#define SWITCH_TOP_PORT                 PORTBbits.RB5
#define SWITCH_TOP_WPU                  WPUBbits.WPUB5
#define SWITCH_TOP_SetHigh()            do { LATBbits.LATB5 = 1; } while(0)
#define SWITCH_TOP_SetLow()             do { LATBbits.LATB5 = 0; } while(0)
#define SWITCH_TOP_Toggle()             do { LATBbits.LATB5 = ~LATBbits.LATB5; } while(0)
#define SWITCH_TOP_GetValue()           PORTBbits.RB5
#define SWITCH_TOP_SetDigitalInput()    do { TRISBbits.TRISB5 = 1; } while(0)
#define SWITCH_TOP_SetDigitalOutput()   do { TRISBbits.TRISB5 = 0; } while(0)
#define SWITCH_TOP_SetPullup()          do { WPUBbits.WPUB5 = 1; } while(0)
#define SWITCH_TOP_ResetPullup()        do { WPUBbits.WPUB5 = 0; } while(0)

// get/set LED aliases
#define SWITCH_BOTTOM_TRIS                 TRISBbits.TRISB6
#define SWITCH_BOTTOM_LAT                  LATBbits.LATB6
#define SWITCH_BOTTOM_PORT                 PORTBbits.RB6
#define SWITCH_BOTTOM_WPU                  WPUBbits.WPUB6
#define SWITCH_BOTTOM_SetHigh()            do { LATBbits.LATB6 = 1; } while(0)
#define SWITCH_BOTTOM_SetLow()             do { LATBbits.LATB6 = 0; } while(0)
#define SWITCH_BOTTOM_Toggle()             do { LATBbits.LATB6 = ~LATBbits.LATB6; } while(0)
#define SWITCH_BOTTOM_GetValue()           PORTBbits.RB6
#define SWITCH_BOTTOM_SetDigitalInput()    do { TRISBbits.TRISB6 = 1; } while(0)
#define SWITCH_BOTTOM_SetDigitalOutput()   do { TRISBbits.TRISB6 = 0; } while(0)
#define SWITCH_BOTTOM_SetPullup()          do { WPUBbits.WPUB6 = 1; } while(0)
#define SWITCH_BOTTOM_ResetPullup()        do { WPUBbits.WPUB6 = 0; } while(0)

// Motor
#define MOTOR_CMD P1DC1

// Watchdog
extern unsigned short watchdog_restart; // in min
extern volatile unsigned short watchdog_restart_default;
extern unsigned short watchdog_cpt_sec;
extern unsigned short watchdog_cpt_default;

// Count
extern signed short qei_overflow; // number of overflow for the encoder

#define MOTOR_STOP 80

/**
 * @brief System initialization
 */
void SYSTEM_Initialize();

/**
 * @brief Clock initialization
 */
void CLOCK_Initialize();

/**
 * @brief initialization of input/ouput
 */
void init_io();

/**
 * @brief read current
 */
//void measure_power();

/**
 * @brief init_timer0
 * Initialization of TIMER0
 * Prescaler 1:128; TMR0 Preload = 3036; Actual Interrupt Time : 1 s
 */
//void init_timer0();

/**
 * @brief init_timer3
 * Initialization of TIMER3
 * Prescaler 1:8; TMR1 Preload = 15536; Actual Interrupt Time : 100 ms
 */
//void init_timer3();

#endif