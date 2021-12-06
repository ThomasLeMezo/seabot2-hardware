/*
 * File:   pwm.c
 * Author: Thomas
 *
 * Created on December 3, 2021, 1:43 PM
 */


#include <xc.h>
#include "pwm.h"

void PWM_Initialize(){
  // PWM
  P1TCONbits.PTEN = 1; // PWM Time Base Timer Enable bit
  PWM1CON1 = 0x0000; // Disable all pwm except PEN1H
  PWM1CON1bits.PEN1H = 1;

  // http://ww1.microchip.com/downloads/en/devicedoc/70187e.pdf (p.24)
  // MOTOR_STOP = 40MIPS/20kHz = 2000
  P1TPER = MOTOR_STOP - 1;
  P1DC1 = MOTOR_STOP;
}