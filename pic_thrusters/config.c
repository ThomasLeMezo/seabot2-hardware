#include "config.h"

volatile unsigned short is_init = 0;


// Motor
unsigned short cmd_motor[2] = {MOTOR_CMD_STOP, MOTOR_CMD_STOP};
unsigned char cpt_motor_1 = 0;
unsigned char cpt_motor_2 = 0;
unsigned int cpt_global = PWM_PERIOD;

// Watchdog
volatile unsigned short watchdog_restart = WATCHDOG_RESTART_DEFAULT;

/**
 * @brief init_io
 * Initialisation des entrées sorties du PIC
 */
void init_io(){
  TRISB7_bit = 0; // OUTPUT (LED)

  TRISC = 0x00;
  TRISC0_bit = 0; // RC0 output for ESC0
  TRISC1_bit = 0; // RC1 output for ESC1
}

void init_timer0(){
  T0CON = 0x87; // TIMER0 ON (1 s)
  TMR0H = TIMER0_CPT_H;
  TMR0L = TIMER0_CPT_L;
  TMR0IE_bit = 0;
}

void init_timer1(){
  T1CON = 0x01; // 10 us
  TMR1H = TIMER1_CPT_H;
  TMR1L = TIMER1_CPT_L;
  TMR1IE_bit = 0; // overflow interrupt reset
}