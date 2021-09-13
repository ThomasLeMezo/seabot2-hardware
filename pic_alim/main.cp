#line 1 "C:/Users/Thomas/Documents/seabot2-hardware/pic_alim/main.c"
#line 1 "c:/users/thomas/documents/seabot2-hardware/pic_alim/i2c.h"
#line 24 "c:/users/thomas/documents/seabot2-hardware/pic_alim/i2c.h"
volatile extern unsigned short rxbuffer_tab[ 8 ];
volatile extern unsigned short tmp_rx;
volatile extern unsigned short nb_tx_octet;
volatile extern unsigned short nb_rx_octet;
volatile extern unsigned short i2c_status;
#line 33 "c:/users/thomas/documents/seabot2-hardware/pic_alim/i2c.h"
void i2c_read_data_from_buffer();
#line 39 "c:/users/thomas/documents/seabot2-hardware/pic_alim/i2c.h"
inline void i2c_write_data_to_buffer(const unsigned short nb_tx_octet);
#line 44 "c:/users/thomas/documents/seabot2-hardware/pic_alim/i2c.h"
void init_i2c(const unsigned short address_i2c);
#line 49 "c:/users/thomas/documents/seabot2-hardware/pic_alim/i2c.h"
void i2c_write_buffer(unsigned short val);
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
#line 23 "C:/Users/Thomas/Documents/seabot2-hardware/pic_alim/main.c"
void main(){
 unsigned short k = 0;
#line 34 "C:/Users/Thomas/Documents/seabot2-hardware/pic_alim/main.c"
 OSCCON = 0b11110010;

 asm CLRWDT;
 SWDTEN_bit = 1;

 init_io();
 init_i2c(0x42);
 init_timer0();
 init_timer3();

 ADC_Init();

 ALIM = 1;
 LED = 0;

 INTCON3.INT1IP = 0;


 RCON.IPEN = 1;
 IPR1.SSPIP = 0;
 INTCON.GIEH = 1;
 INTCON.GIEL = 1;

 INTCON.GIE = 1;
 INTCON.PEIE = 1;

 TMR0IE_bit = 1;
 TMR0ON_bit = 1;

 TMR3IE_bit = 1;
 TMR3ON_bit = 1;

 is_init = 1;

 while(1){
 asm CLRWDT;

 if(step_state_machine==1){
 step_state_machine=0;
 measure_power();

 switch (state){
 case IDLE:
 ALIM = 0;
 led_delay = 50;
 watchdog_restart = watchdog_restart_default;

 ils_analysis(POWER_ON);
 break;

 case POWER_ON:
 ALIM = 1;
#line 89 "C:/Users/Thomas/Documents/seabot2-hardware/pic_alim/main.c"
 led_delay = 20;

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
 led_delay = 200;
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
#line 132 "C:/Users/Thomas/Documents/seabot2-hardware/pic_alim/main.c"
void interrupt(){




 if (TMR0IF_bit){


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


 if(state == POWER_ON && watchdog_restart_default!=0){
 if(watchdog_cpt_sec>0)
 watchdog_cpt_sec--;
 else{
 watchdog_cpt_sec = watchdog_cpt_default;

 if(watchdog_restart>0)
 watchdog_restart--;
 else{

 default_time_to_start[0] = 0;
 default_time_to_start[1] = 0;
 default_time_to_start[2] = 2;
 time_to_stop = default_time_to_stop;

 state = WAIT_TO_SLEEP;
 watchdog_restart = watchdog_restart_default;
 }
 }
 }

 TMR0H =  0x0B ;
 TMR0L =  0xDC ;
 TMR0IF_bit = 0;
 }


 else if (TMR3IF_bit){

 if(set_led_on == 1)
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


 if(cpt_state_machine==0){
 cpt_state_machine =  5 ;
 step_state_machine = 1;
 }
 else
 cpt_state_machine--;

 TMR3H =  0x3C ;
 TMR3L =  0xB0 ;
 TMR3IF_bit = 0;
 }
}
