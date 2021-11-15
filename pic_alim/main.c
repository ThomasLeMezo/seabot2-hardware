/*  PIC18F14K22  mikroC PRO for PIC
Internal oscillator 16MHz

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
  unsigned short k = 0;
  /** Edit config (Project > Edit Project)
  *   -> Oscillator Selection : Internal oscillator block 16 MHz
  *   -> 4xPLL : Diseabled
  *   -> Watchdog Timer : WDT is controlled by SWDTEN bit of the WDTCON register
  *   -> Watchdog Time Postscale : 1:256 (32768/31000 = environ 1Hz)
  *   -> MCLR : disabled (external reset)
  */

  // Oscillateur interne de 16Mhz
  OSCCON = 0b01110010;   // 0=4xPLL OFF, 111=IRCF<2:0>=16Mhz  OSTS=0  SCS<1:0>10 1x = Internal oscillator block
  OSCTUNE = 0x40;
  
  asm CLRWDT;// Watchdog
  SWDTEN_bit = 1; //armement du watchdog

  init_io(); // Initialize I/O
  init_i2c(0x39); // Initialize I2C
  init_timer0(); // Initialize TIMER0 every 1 seconds
  init_timer3(); // Initialize TIMER3 every 100ms

  ADC_Init();

  ALIM = 1; // sortie MOSFET de puissance, commande de l'alimentation
  LED = 0;

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
  
  TMR3IE_bit = 1;  //Enable TIMER3
  TMR3ON_bit = 1; // Start TIMER3

  is_init = 1;

  while(1){
    asm CLRWDT;

    if(step_state_machine==1){ // Every 500ms
      step_state_machine=0;
      measure_power();
      
      switch (state){
      case IDLE: // Idle state
        ALIM = 0;
        led_delay = 50;
        watchdog_restart = watchdog_restart_default;  

        ils_analysis(POWER_ON);
        break;

      case POWER_ON:
        ALIM = 1;
        /*if(battery_global_default == 1)
          led_delay = 5; // 0.5 sec
        else*/
        led_delay = 20; // 2sec
        
        ils_analysis(IDLE);
        break;

      case WAIT_TO_SLEEP:

        ALIM = 1;
        led_delay = 1;
        if(time_to_stop==0){
          for(k=0; k<3; k++)
            time_to_start[k] = default_time_to_start[k];
          state = SLEEP;
          time_to_stop = default_time_to_stop;
        }
        ils_analysis(POWER_ON);
        break;

      case SLEEP:
        ALIM = 0;
        led_delay = 200; // 20 sec
        if(time_to_start[0] == 0 && time_to_start[1] == 0 && time_to_start[2] == 0){
          state = POWER_ON;
        }
        ils_analysis(POWER_ON);
        break;

      default:
        state = POWER_ON;
        break;
      }
    }

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

    // To Do
    if(state == SLEEP){
      if(time_to_start[2]>0){
        time_to_start[2]--;
      }
      else{
        if(time_to_start[1]>0){
          time_to_start[1]--;
          time_to_start[2]=59;
        }
        else{
          if(time_to_start[0]>0){
            time_to_start[0]--;
            time_to_start[1]=59;
          }
        }
      }
    }

    if(state == WAIT_TO_SLEEP){
      if(time_to_stop>0)
        time_to_stop--;
    }

    // Watchdog
    if(state == POWER_ON && watchdog_restart_default!=0){
      if(watchdog_cpt_sec>0)
        watchdog_cpt_sec--;
      else{
        watchdog_cpt_sec = watchdog_cpt_default;

        if(watchdog_restart>0)
          watchdog_restart--;
        else{
         // hour, min, sec
          default_time_to_start[0] = 0;
          default_time_to_start[1] = 0;
          default_time_to_start[2] = 2; // 2s
          time_to_stop = default_time_to_stop;
          
          state = WAIT_TO_SLEEP;
          watchdog_restart = watchdog_restart_default;
        }
      }
    }    
  }

  // Interruption du TIMER3 (State Machine, 100ms)
  if (TMR3IF_bit){
    TMR3H = TIMER3_CPT_H;
    TMR3L = TIMER3_CPT_L;
    TMR3IF_bit = 0;

    // LED (ILS)
    if(set_led_on == 1) // For ILS
      LED = 1;
    else{
      if (cpt_led > 0){
        LED = 0;
        cpt_led--;
      }
      else{
        LED = 1;
        cpt_led=led_delay;
      }
    }
    
    // State Machine (500 ms)
    if(cpt_state_machine==0){
      cpt_state_machine = CPT_STATE_MACHINE_DEFAULT;
      step_state_machine = 1;
    }
    else
      cpt_state_machine--;

    
  }
}