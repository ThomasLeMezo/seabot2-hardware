#ifndef CONFIG_H
#define CONFIG_H

#include "interrupt_manager.h"
#include "i2c_slave.h"
#include "tmr3.h"
#include "tmr2.h"
#include "pwm.h"
#include "adc1.h"

#define CODE_VERSION 0x01
#define VCC 3.3 // dsPIC VCC

extern volatile unsigned short is_init;

#define ENABLE_SetHigh()          (_LATA1 = 1)
#define ENABLE_SetLow()           (_LATA1 = 0)
#define ENABLE_Toggle()           (_LATA1 ^= 1)
#define ENABLE_GetValue()         _RA1
#define ENABLE_SetDigitalInput()  (_TRISA1 = 1)
#define ENABLE_SetDigitalOutput() (_TRISA1 = 0)

#define LED_SetHigh()          (_LATB13 = 1)
#define LED_SetLow()           (_LATB13 = 0)
#define LED_Toggle()           (_LATB13 ^= 1)
#define LED_GetValue()         _RB13
#define LED_SetDigitalInput()  (_TRISB13 = 1)
#define LED_SetDigitalOutput() (_TRISB13 = 0)

#define BATT_VOLTAGE_SetHigh()          (_LATB3 = 1)
#define BATT_VOLTAGE_SetLow()           (_LATB3 = 0)
#define BATT_VOLTAGE_Toggle()           (_LATB3 ^= 1)
#define BATT_VOLTAGE_GetValue()         _RB3
#define BATT_VOLTAGE_SetDigitalInput()  (_TRISB3 = 1)
#define BATT_VOLTAGE_SetDigitalOutput() (_TRISB3 = 0)

#define SWITCH_TOP_SetHigh()          (_LATB5 = 1)
#define SWITCH_TOP_SetLow()           (_LATB5 = 0)
#define SWITCH_TOP_Toggle()           (_LATB5 ^= 1)
#define SWITCH_TOP_GetValue()         _RB5
#define SWITCH_TOP_SetDigitalInput()  (_TRISB5 = 1)
#define SWITCH_TOP_SetDigitalOutput() (_TRISB5 = 0)

#define SWITCH_BOTTOM_SetHigh()          (_LATB6 = 1)
#define SWITCH_BOTTOM_SetLow()           (_LATB6 = 0)
#define SWITCH_BOTTOM_Toggle()           (_LATB6 ^= 1)
#define SWITCH_BOTTOM_GetValue()         _RB6
#define SWITCH_BOTTOM_SetDigitalInput()  (_TRISB6 = 1)
#define SWITCH_BOTTOM_SetDigitalOutput() (_TRISB6 = 0)

#define SWITCH_INT_SetHigh()          (_LATB7 = 1)
#define SWITCH_INT_SetLow()           (_LATB7 = 0)
#define SWITCH_INT_Toggle()           (_LATB7 ^= 1)
#define SWITCH_INT_GetValue()         _RB7
#define SWITCH_INT_SetDigitalInput()  (_TRISB7 = 1)
#define SWITCH_INT_SetDigitalOutput() (_TRISB7 = 0)

// Watchdog
extern unsigned short watchdog_restart_default; // in sec
extern volatile unsigned short watchdog_countdown_restart;
//extern unsigned short watchdog_cpt_sec;
//extern unsigned short watchdog_cpt_default;

// Count
extern signed short qei_overflow; // number of overflow for the encoder

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
void PIN_MANAGER_Initialize();

/*
 @brief initialization of qei
 */
void QEI_Initialize();

/*
 @brief Reset the counter
 */
inline void QEI_Reset_Count();





#endif