#line 1 "C:/Users/Thomas/Documents/seabot2-hardware/pic_alim/config.c"
#line 1 "c:/users/thomas/documents/seabot2-hardware/pic_alim/config.h"





extern volatile unsigned short is_init;

sbit ILS at PORTA.B2;

sbit LED at PORTB.B7;
sbit ALIM at PORTA.B5;




extern unsigned short ils_cpt;
extern unsigned short ils_removed;


enum power_state {IDLE,POWER_ON,WAIT_TO_SLEEP, SLEEP};
extern volatile unsigned short state;

extern unsigned short cpt_state_machine;
extern unsigned short step_state_machine;


extern volatile unsigned int battery_voltage[4];
extern volatile unsigned int power_current[3];


extern unsigned short led_delay;
extern unsigned short cpt_led;
extern unsigned short set_led_on;


extern unsigned char time_to_start[3];
extern unsigned char time_to_stop;

extern unsigned char default_time_to_start[3];
extern unsigned char default_time_to_stop;

extern unsigned short start_time_to_stop;
extern unsigned short start_time_to_power_on;
extern unsigned short start_time_to_start;


extern unsigned short watchdog_restart;
extern volatile unsigned short watchdog_restart_default;
extern unsigned short watchdog_cpt_sec;
extern unsigned short watchdog_cpt_default;
#line 64 "c:/users/thomas/documents/seabot2-hardware/pic_alim/config.h"
void init_io();
#line 69 "c:/users/thomas/documents/seabot2-hardware/pic_alim/config.h"
void measure_power();
#line 76 "c:/users/thomas/documents/seabot2-hardware/pic_alim/config.h"
void init_timer0();
#line 83 "c:/users/thomas/documents/seabot2-hardware/pic_alim/config.h"
void init_timer3();
#line 89 "c:/users/thomas/documents/seabot2-hardware/pic_alim/config.h"
void ils_analysis(unsigned short new_state);
#line 3 "C:/Users/Thomas/Documents/seabot2-hardware/pic_alim/config.c"
volatile unsigned short is_init = 0;


unsigned short led_delay = 100;
unsigned short cpt_led = 100;
unsigned short set_led_on = 0;


unsigned short ils_cpt = 4;
unsigned short ils_removed = 1;


volatile unsigned int battery_voltage[4] = {0, 0, 0, 0};
volatile unsigned int power_current[3] = {0, 0, 0};


volatile unsigned short state = POWER_ON;
unsigned short cpt_state_machine =  5 ;
unsigned short step_state_machine = 0;


unsigned char time_to_start[3] = {0, 0, 5};
unsigned char time_to_stop = 60;

unsigned char default_time_to_start[3] = {0, 0, 5};
unsigned char default_time_to_stop = 60;

unsigned short start_time_to_stop = 0;
unsigned short start_time_to_power_on = 0;
unsigned short start_time_to_start = 0;


unsigned short watchdog_restart = 60;
volatile unsigned short watchdog_restart_default = 60;
unsigned short watchdog_cpt_sec = 59;
unsigned short watchdog_cpt_default = 59;
#line 56 "C:/Users/Thomas/Documents/seabot2-hardware/pic_alim/config.c"
void init_io(){


 ANSEL = 0b00001111;
 ANSELH = 0b11110100;

 CM1CON0 = 0x00;
 CM2CON0 = 0x00;



 TRISA = 0xFF;
 TRISA2_bit = 1;
 TRISA5_bit = 0;

 INTCON2.RABPU = 0;
 WPUA.WPUA2 = 1;

 TRISB = 0xFF;
 TRISB5_bit = 1;
 TRISB7_bit = 0;

 TRISC = 0xFF;
 TRISC0_bit = 1;
 TRISC1_bit = 1;
 TRISC2_bit = 1;
 TRISC3_bit = 1;

 TRISC6_bit = 1;
 TRISC7_bit = 1;
}

void measure_power(){
 battery_voltage[0] = ADC_Get_Sample(4);
 battery_voltage[1] = ADC_Get_Sample(5);
 battery_voltage[2] = ADC_Get_Sample(6);
 battery_voltage[3] = ADC_Get_Sample(7);


 power_current[0] = ADC_Get_Sample(11);
 power_current[1] = ADC_Get_Sample(8);
 power_current[2] = ADC_Get_Sample(9);
}

void init_timer0(){
 T0CON = 0x85;
 TMR0H =  0x0B ;
 TMR0L =  0xDC ;
 TMR0IE_bit = 0;
}

void init_timer3(){
 T3CON = 0x30;
 TMR3IF_bit = 0;
 TMR3H =  0x3C ;
 TMR3L =  0xB0 ;
 TMR3IE_bit = 0;
}

void ils_analysis(unsigned short new_state){
 if(ILS==0){
 ils_cpt--;
 set_led_on = 1;
 }
 else{
 ils_cpt =  4 ;
 set_led_on = 0;
 ils_removed = 1;
 }

 if(ils_removed == 1 && ils_cpt == 0){
 ils_cpt =  4 ;
 state = new_state;
 ils_removed = 0;
 set_led_on = 0;
 }
}
