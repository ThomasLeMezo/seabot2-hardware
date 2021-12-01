#include <xc.h>
#include <stdbool.h>

#include "config.h"

volatile unsigned short is_init = 0;

// Watchdog
unsigned short watchdog_restart = 60; // in min
volatile unsigned short watchdog_restart_default = 60;
unsigned short watchdog_cpt_sec = 59;
unsigned short watchdog_cpt_default = 59;

signed  short qei_overflow = 0;

void CLOCK_Initialize(void){
    // FRCDIV FRC/2; PLLPRE 2; DOZE 1:8; PLLPOST 1:4; DOZEN disabled; ROI disabled; 
    // Input 16MHz Crystal
    
    // DOZE = 1, FRCDIV = 1
    CLKDIVbits.DOZE = 0b000; // FOSC = FCY/1
    CLKDIVbits.FRCDIV = 0b000; // FRC div by 1
    
    // PLLPRE = 1/2
    CLKDIVbits.PLLPRE = 0b0; // 16MHz/2 = 8MHz in [0.8, 8]MHz
    
    // PLLDIV = 40 (start at 2))
    PLLFBDbits.PLLDIV = 18; // 8MHz * 20 = 160 in [100,200]MHz
    
    // PLLPOST /2
    CLKDIVbits.PLLPOST = 0b00; // 160MHz / 2 = 80MHz in [12.5, 80]MHz
    
    // TUN Center frequency; 
    OSCTUN = 0x00;
    
    OSCCONbits.COSC = 0b011; // External oscillator (Primary oscillator (XT, HS, EC) with PLL)
    
//    // CF no clock failure; NOSC PRIPLL; CLKLOCK unlocked; OSWEN Switch is Complete; 
    __builtin_write_OSCCONH((uint8_t) (0x03));
    __builtin_write_OSCCONL((uint8_t) (0x01));
    // Wait for Clock switch to occur
    while (OSCCONbits.OSWEN != 0);
    while (OSCCONbits.LOCK != 1);
}


/**
 * @brief init_io
 * Initialize outputs and inputs
 */
void init_io(){
    
  // I2C
    TRISBbits.TRISB8 = 1; // RB4 input
    TRISBbits.TRISB9 = 1; // RB6 input
    
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

void init_timer1(){
    //TMR1 0; 
    TMR1 = 0x00;
    //Period = 0.2 s; Frequency = 40000000 Hz; PR1 31249; 
    PR1 = 0x7A11;
    //TCKPS 1:256; TON enabled; TSIDL disabled; TCS FOSC/2; TSYNC disabled; TGATE disabled; 
    T1CON = 0x8030;
    IFS0bits.T1IF = false;
    IEC0bits.T1IE = true;
}

void SYSTEM_Initialize(){
    CLOCK_Initialize();
    INTERRUPT_Initialize();
    init_io();
    TMR1_Initialize();
    I2C_Open();
}
