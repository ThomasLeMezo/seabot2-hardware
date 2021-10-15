#include "config.h"

volatile unsigned short is_init = 0;

/**
 * @brief init_io
 * Initialisation des entrées sorties du PIC
 */
void init_io(){
  TRISC5_bit = 0; // RC3 output
}

void init_timer0(){
  T0CON = 0x05; // TIMER0 ON (1 s)
  TMR0H = TIMER0_CPT_H;
  TMR0L = TIMER0_CPT_L;
  
  TMR0IE_bit = 1;  //Enable TIMER0
  TMR0ON_bit = 1; // Start TIMER0
}

void init_timer3(){
  T3CON = 0x30;
  TMR3IF_bit = 0;
  TMR3H = TIMER3_CPT_H;
  TMR3L = TIMER3_CPT_L;
  
  TMR3IE_bit = 1;  //Enable TIMER3
  TMR3ON_bit = 1; // Start TIMER3
}