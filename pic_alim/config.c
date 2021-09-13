#include "config.h"

volatile unsigned short is_init = 0;


/** RA3 - ILS
* RA5 - Relais de puissance
* RB4 - SDA
* RB6 - SCL
* AN11 - VIOUT (courant moteur)
* RB7 - LED
* AN8 - VIOUT1 (courant ESC2)
* AN9 - VIOUT2 (courant ESC1)
* AN4, 5, 6, 7 - Batt 4,2,3,1*/


/**
 * @brief init_io
 * Initialisation des entrées sorties du PIC
 */
void init_io(){
  // ANSEL : enable if 0
  // ANS0-7
  ANSEL = 0b00001111;  // Set RC0,RC1,RC2,RC3 to analog (AN4,AN5,AN6,AN7)
  ANSELH = 0b11110100; // Set RC6, RC7, RC5 to analog (AN8, AN9, AN11)

  CM1CON0 = 0x00; // Not using the comparators
  CM2CON0 = 0x00;

  // NVCFG = 00; PVCFG = 00;

  TRISA = 0xFF;
  TRISA2_bit = 1; // INPUT (ILS)
  TRISA5_bit = 0; // OUTPUT (POWER)

  INTCON2.RABPU = 0; // PORTA and PORTB Pull-up Enable bit
  WPUA.WPUA2 = 1; // Pull-up enabled sur RA2

  TRISB = 0xFF;
  TRISB5_bit = 1; // RC5 input for AN11
  TRISB7_bit = 0; // OUTPUT (LED)

  TRISC = 0xFF;
  TRISC0_bit = 1; // RC0 input for AN4
  TRISC1_bit = 1; // RC1 input for AN5
  TRISC2_bit = 1; // RC2 input for AN6
  TRISC3_bit = 1; // RC3 input for AN7

  TRISC6_bit = 1; // RC6 input for AN8
  TRISC7_bit = 1; // RC7 input for AN9
}