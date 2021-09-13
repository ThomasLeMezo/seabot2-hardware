/*  PIC18F14K22  mikroC PRO for PIC v6.4
Oscillateur interne 16MHZ (attention l'edit project ne marche pas, paramétrer 
l'oscillateur en dur)

* RA3 - ILS
* RA5 - Relais de puissance
* RB4 - SDA
* RB6 - SCL
* AN11 - VIOUT (courant moteur)
* RB7 - LED
* AN8 - VIOUT1 (courant ESC2)
* AN9 - VIOUT2 (courant ESC1)
* AN4, 5, 6, 7 - Batt 4,2,3,1

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
  OSCCON = 0b11110010;   // 0=4xPLL OFF, 111=IRCF<2:0>=16Mhz  OSTS=0  SCS<1:0>10 1x = Internal oscillator block
  LED = 1; // sortie LED

  init_io(); // Initialisation des I/O
  init_i2c(0x42); // Initialisation de l'I2C en esclave

  ALIM = 0; // sortie MOSFET de puissance, commande de l'alimentation

  INTCON3.INT1IP = 0; //INT1 External Interrupt Priority bit, INT0 always a high
  //priority interrupt source

  RCON.IPEN = 1;  //Enable priority levels on interrupts
  IPR1.SSPIP = 0; //Master Synchronous Serial Port Interrupt Priority bit (low priority = 0)
  INTCON.GIEH = 1; //enable all high-priority interrupts
  INTCON.GIEL = 1; //enable all low-priority interrupts

  INTCON.GIE = 1; // Global Interrupt Enable bit
  INTCON.PEIE = 1; // Peripheral Interrupt Enable bit

  LED = 0;

  is_init = 1;

  while(1){
    if(nb_rx_octet>1 && SSPSTAT.P == 1){
        i2c_read_data_from_buffer();
        nb_rx_octet = 0;
    }
  }
}