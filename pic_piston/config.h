#ifndef CONFIG_H
#define CONFIG_H

#define CODE_VERSION 0x01

extern volatile unsigned short is_init;

sbit ENABLE at PORTA.B1; // output ENABLE
sbit LED at PORTB.B13; // output ENABLE

sbit SWITCH_TOP at PORTB.B5; // top switch
sbit SWITCH_BOTTOM at PORTB.B6; // bottom switch

// Watchdog
extern unsigned short watchdog_restart; // in min
extern volatile unsigned short watchdog_restart_default;
extern unsigned short watchdog_cpt_sec;
extern unsigned short watchdog_cpt_default;

/**
 * @brief initialization of input/ouput
 */
void init_io();

/**
 * @brief read current
 */
void measure_power();

/**
 * @brief init_timer0
 * Initialization of TIMER0
 * Prescaler 1:128; TMR0 Preload = 3036; Actual Interrupt Time : 1 s
 */
void init_timer0();

/**
 * @brief init_timer3
 * Initialization of TIMER3
 * Prescaler 1:8; TMR1 Preload = 15536; Actual Interrupt Time : 100 ms
 */
void init_timer3();

#endif