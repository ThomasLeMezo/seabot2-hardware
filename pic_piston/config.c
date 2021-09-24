#include "config.h"

volatile unsigned short is_init = 0;

// Watchdog
unsigned short watchdog_restart = 60; // in min
volatile unsigned short watchdog_restart_default = 60;
unsigned short watchdog_cpt_sec = 59;
unsigned short watchdog_cpt_default = 59;


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
  /*ANSEL = 0b11110000;  // Set RC0,RC1,RC2,RC3 to analog (AN4,AN5,AN6,AN7)
  ANSELH = 0b11110100; // Set RC6, RC7, RC5 to analog (AN8, AN9, AN11) */

  // INPUT-OUTPUT
  TRISA1_bit = 0; // ENABLE output
  TRISA0_bit = 1; // current measure input

  TRISB10_bit = 1; // Codeur A
  TRISB11_bit = 1; // Codeur B
  TRISB12_bit = 1; // Codeur Idx

  TRISB5_bit = 1; // Top switch
  TRISB6_bit = 1; // Bottom switch
  TRISB7_bit = 1; // Interrupt for switch

  TRISB13_bit = 0;

  // Interrupt for switch
  INTCON2bits.INT0EP = 0; // interrupt on positive edge
  IEC0bits.INT0IE = 1; // enable interrupt
  IFS0bits.INT0IF = 0; // set flag to 0
  
  // QEI (Quadrature Encoder Interface)
  // Quadrature Encoder Interface enabled (x4 mode) with position counter reset by match (MAXxCNT)
  QEI1CONbits.QEIM0 = 1;
  QEI1CONbits.QEIM1 = 1;
  QEI1CONbits.QEIM2 = 1;
  QEI1CONbits.POSRES = 0; // do not resset position with index

  // PWM
  P1TCONbits.PTEN = 1; // PWM Time Base Timer Enable bit
  PWM1CON1 = 0x0000; // Disable all pwm except PEN1H
  PWM1CON1bits.PEN1H = 1;

  // http://ww1.microchip.com/downloads/en/devicedoc/70187e.pdf (p.24)
  P1TPER = 99; // 16MHz => 20kHz
  P1DC1 = 100; 

}

void measure_power(){
/*  power_current[0] = ADC_Get_Sample(11);
  power_current[1] = ADC_Get_Sample(8);
  power_current[2] = ADC_Get_Sample(9);*/
}

void init_timer0(){
/*  T0CON = 0x05; // TIMER0 ON (1 s)
  TMR0H = TIMER0_CPT_H;
  TMR0L = TIMER0_CPT_L;
  TMR0IE_bit = 0;*/
}

void init_timer3(){
/*  T3CON = 0x30;
  TMR3IF_bit = 0;
  TMR3H = TIMER3_CPT_H;
  TMR3L = TIMER3_CPT_L;
  TMR3IE_bit = 0;*/
}