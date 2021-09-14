#include "config.h"

volatile unsigned short is_init = 0;

// Led
unsigned short led_delay = 100;
unsigned short cpt_led = 100;
unsigned short set_led_on = 0;

// ILS
unsigned short ils_cpt = 4;
unsigned short ils_removed = 1;

// Batteries
volatile unsigned int battery_voltage[4] = {0, 0, 0, 0};
volatile unsigned int power_current[3] = {0, 0, 0};

// State Machine
volatile unsigned short state = POWER_ON;
unsigned short cpt_state_machine = CPT_STATE_MACHINE_DEFAULT;
unsigned short step_state_machine = 0;

// Sleep mode
unsigned char time_to_start[3] = {0, 0, 5}; // hour, min, sec
unsigned char time_to_stop = 60; // in sec (max 255 sec)

unsigned char default_time_to_start[3] = {0, 0, 5};
unsigned char default_time_to_stop = 60;

unsigned short start_time_to_stop = 0;
unsigned short start_time_to_power_on = 0;
unsigned short start_time_to_start = 0;

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
  ANSEL = 0b11110000;  // Set RC0,RC1,RC2,RC3 to analog (AN4,AN5,AN6,AN7)
  ANSELH = 0b11110100; // Set RC6, RC7, RC5 to analog (AN8, AN9, AN11)

  CM1CON0 = 0x00; // Not using the comparators
  CM2CON0 = 0x00;

  // NVCFG = 00; PVCFG = 00;

  TRISA = 0xFF;
  TRISA2_bit = 1; // INPUT (ILS)
  TRISA5_bit = 0; // OUTPUT (POWER)

  INTCON2.RABPU = 0; // PORTA and PORTB Pull-up Enable bit
  WPUA.WPUA2 = 1; // Pull-up enabled sur RA2

  TRISB = 0xFF;
  TRISB5_bit = 1; // RC5 input for AN11
  TRISB7_bit = 0; // OUTPUT (LED)

  TRISC = 0xFF;
  TRISC0_bit = 1; // RC0 input for AN4
  TRISC1_bit = 1; // RC1 input for AN5
  TRISC2_bit = 1; // RC2 input for AN6
  TRISC3_bit = 1; // RC3 input for AN7

  TRISC6_bit = 1; // RC6 input for AN8
  TRISC7_bit = 1; // RC7 input for AN9
}

void measure_power(){
  battery_voltage[0] = ADC_Get_Sample(4);   // Get 10-bit results of AD conversion AN4 batterie 1
  battery_voltage[1] = ADC_Get_Sample(5);   // Get 10-bit results of AD conversion AN5 batterie 2
  battery_voltage[2] = ADC_Get_Sample(6);   // Get 10-bit results of AD conversion AN6 batterie 3
  battery_voltage[3] = ADC_Get_Sample(7);   // Get 10-bit results of AD conversion AN7 batterie 4


  power_current[0] = ADC_Get_Sample(11);
  power_current[1] = ADC_Get_Sample(8);
  power_current[2] = ADC_Get_Sample(9);
}

void init_timer0(){
  T0CON = 0x05; // TIMER0 ON (1 s)
  TMR0H = TIMER0_CPT_H;
  TMR0L = TIMER0_CPT_L;
  TMR0IE_bit = 0;
}

void init_timer3(){
  T3CON = 0x30;
  TMR3IF_bit = 0;
  TMR3H = TIMER3_CPT_H;
  TMR3L = TIMER3_CPT_L;
  TMR3IE_bit = 0;
}

void ils_analysis(unsigned short new_state){
  if(ILS==0){ // Magnet detected
    ils_cpt--;
    set_led_on = 1;
  }
  else{
    ils_cpt = ILS_CPT_TIME;
    set_led_on = 0;
    ils_removed = 1;
  }

  if(ils_removed == 1 && ils_cpt == 0){
    ils_cpt = ILS_CPT_TIME;
    state = new_state;
    ils_removed = 0;
    set_led_on = 0;
  }
}