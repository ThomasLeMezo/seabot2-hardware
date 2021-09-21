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

/**
 * @brief main
 */
void main(){
  unsigned short k = 0;
  // External oscillator HS
  COSC_0_bit = 0;
  COSC_1_bit = 0;
  COSC_2_bit = 0;

  //asm CLRWDT;// Watchdog
  //SWDTEN_bit = 1; //armement du watchdog

  init_io(); // Initialize I/O
  init_i2c(0x30); // Initialize I2C
/*  init_timer0(); // Initialize TIMER0 every 1 seconds
  init_timer3(); // Initialize TIMER3 every 100ms*/

  ADC1_Init();

  ENABLE = 0;
  LED = 0;

  is_init = 1;

  while(1){
    //asm CLRWDT;


    if(nb_rx_octet>1 && I2C1STATbits.P == 1){
        i2c_read_data_from_buffer();
        nb_rx_octet = 0;
    }
  }
}

/**
 * @brief Interrupt (high)
 */
void interrupt(){
  /// ************************************************** //
  /// ********************** TIMERS  ******************* //

}