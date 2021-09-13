#line 1 "C:/Users/Thomas/Documents/seabot2-hardware/pic_alim/i2c.c"
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
#line 5 "C:/Users/Thomas/Documents/seabot2-hardware/pic_alim/i2c.c"
volatile unsigned short rxbuffer_tab[ 8 ];
volatile unsigned short tmp_rx = 0;
volatile unsigned short nb_tx_octet = 0;
volatile unsigned short nb_rx_octet = 0;

void i2c_read_data_from_buffer(){
 short i = 0;

 for(i=0; i<(nb_rx_octet-1); i++){
 switch(rxbuffer_tab[0]+i){
 case 0x00:
 switch(rxbuffer_tab[i+1]){
 case 0x01:
 state = POWER_ON;
 break;
 case 0x02:
 time_to_stop = default_time_to_stop;
 start_time_to_stop = 1;
 state = WAIT_TO_SLEEP;
 break;
 default:
 break;
 }
 i++;
 break;
 case 0x03:
 default_time_to_start[0] = rxbuffer_tab[i+1];
 break;
 case 0x04:
 default_time_to_start[1] = rxbuffer_tab[i+1];
 break;
 case 0x05:
 default_time_to_start[2] = rxbuffer_tab[i+1];
 break;
 case 0x06:
 default_time_to_stop = rxbuffer_tab[i+1];
 break;
 case 0x07:
 watchdog_restart_default = rxbuffer_tab[i+1];
 watchdog_restart = watchdog_restart_default;
 break;
 default:
 break;
 }
 }
}

void i2c_write_data_to_buffer(const unsigned short nb_tx_octet){
 switch(rxbuffer_tab[0]+nb_tx_octet){
 case 0x00:
 SSPBUF = battery_voltage[0];
 break;
 case 0x01:
 SSPBUF = battery_voltage[0] >> 8;
 break;
 case 0x02:
 SSPBUF = battery_voltage[1];
 break;
 case 0x03:
 SSPBUF = battery_voltage[1] >> 8;
 break;
 case 0x04:
 SSPBUF = battery_voltage[2];
 break;
 case 0x05:
 SSPBUF = battery_voltage[2] >> 8;
 break;
 case 0x06:
 SSPBUF = battery_voltage[3];
 break;
 case 0x07:
 SSPBUF = battery_voltage[3] >> 8;
 break;
 case 0x08:
 SSPBUF = power_current[0];
 break;
 case 0x09:
 SSPBUF = power_current[0] >> 8;
 break;
 case 0x0A:
 SSPBUF = power_current[1];
 break;
 case 0x0B:
 SSPBUF = power_current[1] >> 8;
 break;
 case 0x0C:
 SSPBUF = power_current[2];
 break;
 case 0x0D:
 SSPBUF = power_current[2] >> 8;
 break;

 case 0xB0:
 SSPBUF = state;
 break;
 case 0xB1:
 SSPBUF = watchdog_restart_default;
 break;
 case 0xB2:
 SSPBUF = ILS;
 break;

 case 0xC0:
 SSPBUF =  0x01 ;
 break;
 case 0xC1:
 SSPBUF = is_init;
 break;
 default:
 SSPBUF = 0x00;
 break;
 }
 watchdog_restart = watchdog_restart_default;
}

void i2c_write_buffer(unsigned short val){
 do
 {
 SSPCON1.WCOL = 0;
 SSPBUF = val;
 }
 while(SSPCON1.WCOL);
}

void init_i2c(const unsigned short address_i2c){


 TRISB4_bit = 1;
 TRISB6_bit = 1;


 PIE1.SSPIE = 1;
 PIR1.SSPIF = 0;

 PIR2.BCLIE = 1;
 PIR2.BCLIF = 0;


 SSPADD = (address_i2c << 1);
 SSPMSK = 0xFF;



 SSPSTAT.SMP = 1;


 SSPSTAT.CKE = 1;


 SSPCON2 = 0x00;
 SSPCON2.GCEN = 0;
 SSPCON2.SEN = 0;


 SSPCON1.WCOL = 0;
 SSPCON1.SSPOV = 0;
 SSPCON1.CKP = 1;
 SSPCON1.SSPM3 = 0b0;
 SSPCON1.SSPM2 = 0b1;
 SSPCON1.SSPM1 = 0b1;
 SSPCON1.SSPM0 = 0b0;


 SSPCON1.SSPEN = 1;
}

void interrupt_low(){
 unsigned char state = 0;

 if (PIR1.SSPIF){

 if(SSPSTAT.S){
 state = SSPSTAT &  0x25 ;

 switch (state){
 case  0x04 :
 case  0x05 :
 nb_tx_octet = 0;
 tmp_rx = SSPBUF;


 case  0x24 :
 i2c_write_data_to_buffer(nb_tx_octet);
 nb_tx_octet++;
 break;

 case  0x01 :
 nb_rx_octet = 0;
 tmp_rx = SSPBUF;
 break;

 case  0x21 :
 if(nb_rx_octet <  8 ){
 rxbuffer_tab[nb_rx_octet] = SSPBUF;
 nb_rx_octet++;
 }
 else
 tmp_rx = SSPBUF;
 break;
 }
 }

 SSPCON1.CKP = 1;
 PIR1.SSPIF = 0;
 }
}
