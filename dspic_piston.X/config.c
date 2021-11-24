#include "config.h"
#include <xc.h>

volatile unsigned short is_init = 0;

// Watchdog
unsigned short watchdog_restart = 60; // in min
volatile unsigned short watchdog_restart_default = 60;
unsigned short watchdog_cpt_sec = 59;
unsigned short watchdog_cpt_default = 59;

signed  short qei_overflow = 0;

/**
 * @brief init_io
 * Initialize outputs and inputs
 */
void init_io(){
  // INPUT-OUTPUT
  TRISAbits.TRISA1 = 0; // ENABLE set to output
  TRISBbits.TRISB13 = 0; // LED set to output 

  // Interrupt for switch
  INTCON2bits.INT0EP = 0; // interrupt on positive edge
  IEC0bits.INT0IE = 1; // enable interrupt
  IFS0bits.INT0IF = 0; // set flag to 0
  
  // QEI (Quadrature Encoder Interface)
  // Quadrature Encoder Interface enabled (x4 mode) with position counter reset by match (MAXxCNT)
  QEI1CONbits.QEIM0 = 1;
  QEI1CONbits.QEIM1 = 1;
  QEI1CONbits.QEIM2 = 1;
  QEI1CONbits.POSRES = 0; // do not reset position with index
  QEI1CONbits.SWPAB = 1; // Swap Phase A and B

  DFLT1CONbits.CEID = 1 ; // count error disable bit (0=enable)
  DFLT1CONbits.QEOUT = 1 ; // Digital filter outputs are enabled

  MAXCNT = 0xFFFF; // max value (interrupt if overflow)
  POS1CNT = 0x00FF; // Counter inital position
  
  // Map the QEI to the pin of the coder (3 pins)
  RPINR14 = 10 + (11 << 8); // QEA (low byte of RP): 10, QEB : 11 (high byte of RP)
  RPINR15 = 12;

  _QEIIE = 1; // Enable interrupt from QEI

  // PWM
  P1TCONbits.PTEN = 1; // PWM Time Base Timer Enable bit
  PWM1CON1 = 0x0000; // Disable all pwm except PEN1H
  PWM1CON1bits.PEN1H = 1;

  // http://ww1.microchip.com/downloads/en/devicedoc/70187e.pdf (p.24)
  P1TPER = MOTOR_STOP - 1; // 16MHz => 20kHz
  P1DC1 = MOTOR_STOP;

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