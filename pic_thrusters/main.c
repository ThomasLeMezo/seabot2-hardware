/*  PIC18F14K22  mikroC PRO for PIC
External oscillator 16MHZ (HS)

* RC0 - ESC1
* RC1 - ESC2
* RB7 - LED

  Pulse Width Signal
  Signal Voltage  3.3-5 volts
  Update at 50 Hz
  Stopped         1500 microseconds
  Max forward     1900 microseconds
  Max reverse     1100 microseconds
  Signal Deadband    +/- 25 microseconds (centered around 1500 microseconds)

*/

#include "i2c.h"
#include "config.h"

/**
 * @brief main
 */
void main(){
  unsigned short k = 0;

  OSCCON = 0b01110000; // 0=4xPLL OFF, 111=IRCF<2:0>=16Mhz  OSTS=0  SCS<1:0>10 1x = Internal oscillator block
  OSCTUNE = 0x40; 

  asm CLRWDT;// Watchdog
  SWDTEN_bit = 1; //armement du watchdog

  init_io(); // Initialize I/O
  init_i2c(0x20); // Initialize I2C
  init_timer0(); // Initialize TIMER0 every 1 seconds
  init_timer1(); // Initialize TIMER3 every 

  INTCON3.INT1IP = 0; //INT1 External Interrupt Priority bit, INT0 always a high
  //priority interrupt source

  RCON.IPEN = 1;  //Enable priority levels on interrupts
  IPR1.SSPIP = 0; //Master Synchronous Serial Port Interrupt Priority bit (low priority = 0)
  INTCON.GIEH = 1; //enable all high-priority interrupts
  INTCON.GIEL = 1; //enable all low-priority interrupts

  INTCON.GIE = 1; // Global Interrupt Enable bit
  INTCON.PEIE = 1; // Peripheral Interrupt Enable bit

  TMR0IE_bit = 1;  //Enable TIMER0
  TMR0ON_bit = 1; // Start TIMER0
  
  TMR1IE_bit = 1;  //Enable TIMER1
  TMR1ON_bit = 1; // Start TIMER1

  is_init = 1;

  while(1){
    asm CLRWDT;

    if(cmd_motor[0] != MOTOR_CMD_STOP || cmd_motor[1] != MOTOR_CMD_STOP)
      LED = 1;
    else
      LED = 0;

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

  // Interruption TIMER1 toutes les 10us
  if (TMR1IF_bit){
    TMR1H = TIMER1_CPT_H;
    TMR1L = TIMER1_CPT_L;
    TMR1IF_bit = 0;

    if(cpt_global==0){
      MOT1 = 1;
      MOT2 = 1;
      cpt_global = PWM_PERIOD;

      cpt_motor_1 = cmd_motor[0];
      cpt_motor_2 = cmd_motor[1];
    }
    else{
      cpt_global--;

      // MOT 1
      if(cpt_motor_1==0)
        MOT1 = 0;
      else
        cpt_motor_1--;

      // MOT 2
      if(cpt_motor_2==0)
        MOT2 = 0;
      else
        cpt_motor_2--;
    }
  }

  if (TMR0IF_bit){
    // Watchdog
    if(watchdog_restart>0)
      watchdog_restart--;  
    else{
      cmd_motor[0] = MOTOR_CMD_STOP;
      cmd_motor[1] = MOTOR_CMD_STOP;
      watchdog_restart = WATCHDOG_RESTART_DEFAULT;
    }

    TMR0H = TIMER0_CPT_H;
    TMR0L = TIMER0_CPT_L;
    TMR0IF_bit = 0;
  }
}