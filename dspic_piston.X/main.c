/*  DSPIC33FJ32MC202
External oscillator 16MHz

RB5 : Top switch (RC0)
RB6 : Bottom switch (RC1)
RB7 : Interrupt for switch (Nand gate)

RB12 : Codeur Indx
RB11 : Codeur B
RB10 : Codeur A

RB13 : LED

RB14 : IN1 (PWM motor)
RA1 : Enable
RA0 : Motor Current Sensor

*/

#include "i2c.h"
#include "config.h"
#include <xc.h>

/**
 * @brief main
 */
int main(){
  _COSC = 0b011; // External oscillator (Primary oscillator (XT, HS, EC) with PLL)

  init_io(); // Initialize I/O
  init_i2c(0x30); // Initialize I2C
/*  init_timer0(); // Initialize TIMER0 every 1 seconds
  init_timer3(); // Initialize TIMER3 every 100ms*/

  //ADC1_Init();

  ENABLE_SetLow();
  LED_SetLow();

  is_init = 1;

  // Resset the  POS1CNT = 0x00;
  POS1CNT = 0x0000;

  while(1){
    //asm CLRWDT;

    //ENABLE = 1;
    if(nb_rx_octet>1 && I2C1STATbits.P == 1){
        i2c_read_data_from_buffer();
        nb_rx_octet = 0;
    }
  }
  
  return 0;
}

void __attribute__((__interrupt__, auto_psv)) _INT0Interrupt(){
  if (IFS0bits.INT0IF){
      IFS0bits.INT0IF = 0;
    if((!SWITCH_TOP_GetValue() && MOTOR_CMD>MOTOR_STOP) 
        || (!SWITCH_BOTTOM_GetValue() && MOTOR_CMD<MOTOR_STOP)){
      LED_SetHigh();
      MOTOR_CMD = MOTOR_STOP;
    }    
  }
}

/*
 Interruption for QEI when overflow
 */
void __attribute__((__interrupt__, auto_psv)) _QEIInterrupt(){
  _QEIIF = 0;
  if(QEI1CONbits.UPDN)
    qei_overflow += 1;
  else
    qei_overflow -= 1;
  
}