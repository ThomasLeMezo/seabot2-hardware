/*  PIC18F14K22  mikroC PRO for PIC
Internal oscillator 16MHz

* RC5 - Flash
* RB4 - SDA
* RB6 - SCL

*/

#include "i2c.h"
#include "config.h"

/**
 * @brief main
 */
void main(){
  /** Edit config (Project > Edit Project)
  *   -> Oscillator Selection : Internal oscillator block 16 MHz
  *   -> 4xPLL : Diseabled
  *   -> Watchdog Timer : WDT is controlled by SWDTEN bit of the WDTCON register
  *   -> Watchdog Time Postscale : 1:256 (32768/31000 = environ 1Hz)
  *   -> MCLR : disabled (external reset)
  */

  // Oscillateur interne de 16Mhz
  OSCCON = 0b01110010;   // 0=Idlen, 111=IRCF<2:0>=16Mhz  OSTS=0  HFIOFS=0 SCS<1:0>10 1x = Internal oscillator block
  OSCTUNE = 0x40; // INTSRC disabled; PLLEN enabled; TUN 0; 

  asm CLRWDT;// Watchdog
  SWDTEN_bit = 1; //armement du watchdog

  init_io(); // Initialize I/O
  init_i2c(0x40); // Initialize I2C
  init_timer0(); // Initialize TIMER0 every 1 seconds
  init_timer3(); // Initialize TIMER3 every 100ms*/

  LED = 0;

  INTCON3.INT1IP = 0; //INT1 External Interrupt Priority bit, INT0 always a high
  //priority interrupt source

  RCON.IPEN = 1;  //Enable priority levels on interrupts
  IPR1.SSPIP = 0; //Master Synchronous Serial Port Interrupt Priority bit (low priority = 0)
  INTCON.GIEH = 1; //enable all high-priority interrupts
  INTCON.GIEL = 1; //enable all low-priority interrupts

  INTCON.GIE = 1; // Global Interrupt Enable bit
  INTCON.PEIE = 1; // Peripheral Interrupt Enable bit

  is_init = 1;

  while(1){
    asm CLRWDT;

    if(nb_rx_octet>1 && SSPSTAT.P == 1){
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

  // Interruption du TIMER0 (1 s) (cpt to start/stop + Watchdog)
  if (TMR0IF_bit){
    TMR0H = TIMER0_CPT_H;
    TMR0L = TIMER0_CPT_L;
    TMR0IF_bit = 0;
  }

  // Interruption du TIMER3 (State Machine, 100ms)
  if (TMR3IF_bit){

    TMR3H = TIMER3_CPT_H;
    TMR3L = TIMER3_CPT_L;
    TMR3IF_bit = 0;
    
  }
}