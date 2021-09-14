#ifndef CONFIG_H
#define CONFIG_H

#define CODE_VERSION 0x01

extern volatile unsigned short is_init;

sbit MOT1 at LATC.B0; // output thruster 1
sbit MOT2 at LATC.B1; // output thruster 2
sbit LED at LATB.B7; // LED

// Motor
#define MOTOR_CMD_STOP 150
#define PWM_PERIOD 2000
extern unsigned short cmd_motor[2];

extern unsigned char cpt_motor_1;
extern unsigned char cpt_motor_2;
extern unsigned int cpt_global = PWM_PERIOD;

// Watchdog
#define WATCHDOG_RESTART_DEFAULT 3 // 3 s
extern volatile unsigned short watchdog_restart;

// Timer0
#define TIMER0_CPT_H 0x0B
#define TIMER0_CPT_L 0xDC

// Timer1
#define TIMER1_CPT_H 0xFF
#define TIMER1_CPT_L 110


/**
 * @brief initialization of input/ouput
 */
void init_io();

/**
 * @brief init_timer0 (1s)
  */
void init_timer0();

/**
 * @brief init_timer1 (10 us)
 */
void init_timer1();

#endif