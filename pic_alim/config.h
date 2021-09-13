#ifndef CONFIG_H
#define CONFIG_H

#define CODE_VERSION 0x01

extern volatile unsigned short is_init;

sbit ILS at PORTA.B2; //  entrée de l'ILS

sbit LED at PORTB.B7; // sortie LED
sbit ALIM at PORTA.B5; // sortie MOSFET de puissance, commande de l'alimentation


// ILS
#define ILS_CPT_TIME 4
extern unsigned short ils_cpt;
extern unsigned short ils_removed;

// State Machine
enum power_state {IDLE,POWER_ON,WAIT_TO_SLEEP, SLEEP};
extern volatile unsigned short state;
#define CPT_STATE_MACHINE_DEFAULT 5
extern unsigned short cpt_state_machine;
extern unsigned short step_state_machine;

// Batteries
extern volatile unsigned int battery_voltage[4];
extern volatile unsigned int power_current[3];

// Led
extern unsigned short led_delay;
extern unsigned short cpt_led;
extern unsigned short set_led_on;

// Sleep mode
extern unsigned char time_to_start[3]; // hour, min, sec
extern unsigned char time_to_stop; // in sec (max 255 sec)

extern unsigned char default_time_to_start[3]; // hour, min, sec
extern unsigned char default_time_to_stop;

extern unsigned short start_time_to_stop;
extern unsigned short start_time_to_power_on;
extern unsigned short start_time_to_start;

// Watchdog
extern unsigned short watchdog_restart; // in min
extern volatile unsigned short watchdog_restart_default;
extern unsigned short watchdog_cpt_sec;
extern unsigned short watchdog_cpt_default;

// Timer0
#define TIMER0_CPT_H 0x0B
#define TIMER0_CPT_L 0xDC

// Timer3
#define TIMER3_CPT_H 0x3C
#define TIMER3_CPT_L 0xB0


/**
 * @brief initialization of input/ouput
 */
void init_io();

/**
 * @brief read batteries voltage + current
 */
void measure_power();

/**
 * @brief init_timer0
 * Initialization of TIMER0
 * Prescaler 1:128; TMR0 Preload = 3036; Actual Interrupt Time : 1 s
 */
void init_timer0();

/**
 * @brief init_timer3
 * Initialization of TIMER3
 * Prescaler 1:8; TMR1 Preload = 15536; Actual Interrupt Time : 100 ms
 */
void init_timer3();

/**
 * @brief ils_analysis
 * Function to change the state wrt ILS
 */
void ils_analysis(unsigned short new_state);

#endif