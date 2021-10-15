#ifndef CONFIG_H
#define CONFIG_H

#define CODE_VERSION 0x01

extern volatile unsigned short is_init;

sbit LED at LATC.B5; // sortie LED

// Timer0
#define TIMER0_CPT_H 0x0B
#define TIMER0_CPT_L 0xDB

// Timer3
#define TIMER3_CPT_H 0x3C
#define TIMER3_CPT_L 0xAF

/**
 * @brief initialization of input/ouput
 */
void init_io();

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