#include "i2c.h"
#include "config.h"

volatile unsigned short rxbuffer_tab[SIZE_RX_BUFFER];
volatile unsigned short tmp_rx = 0;
volatile unsigned short nb_tx_octet = 0;
volatile unsigned short nb_rx_octet = 0;

void i2c_read_data_from_buffer(){
  short i = 0;

  for(i=0; i<(nb_rx_octet-1); i++){
    switch(rxbuffer_tab[0]+i){
      case 0x00:
        LED = rxbuffer_tab[i+1];
        break;
      case 0x01:
        ENABLE = rxbuffer_tab[i+1];
        break;
      case 0x02:
        P1DC1 = rxbuffer_tab[i+1];
        break;
      default:
        break;
    }
  }
}

void i2c_write_data_to_buffer(const unsigned short nb_tx_octet){
  switch(rxbuffer_tab[0]+nb_tx_octet){
    case 0x00:
      I2C1TRN = POS1CNT;
      break;
    case 0x01:
      I2C1TRN = (POS1CNT >> 8); // not recommended (two reads)
      break;
    case 0x02:
      I2C1TRN = qei_overflow;
      break;
    case 0x03:
      I2C1TRN = !SWITCH_TOP
                | (!SWITCH_BOTTOM<<1);
    case 0x10:
      I2C1TRN = QEI1CONbits.UPDN;
      break;      
    case 0xC0:
      I2C1TRN = CODE_VERSION;
      break;
    case 0xC1:
      I2C1TRN = is_init;
      break;
    default:
      I2C1TRN = 0x00;
      break;
  }
  watchdog_restart = watchdog_restart_default;
}

void init_i2c(const unsigned short address_i2c){

// https://ww1.microchip.com/downloads/en/DeviceDoc/70046E.pdf

  // **** IO I2C **** //
  TRISB8_bit = 1; // RB4 input
  TRISB9_bit = 1; // RB6 input

  I2C1CONbits.I2CEN = 1; // Enable I2C
  I2C1CONbits.A10M = 0; // 7-bit address
  I2C1CONbits.SMEN = 1; // SMBUS enable
  I2C1CONbits.DISSLW = 0; // Slew rate enable
  I2C1CONbits.STREN = 1; // Clock stretch enable

  // **** ADDRESS **** //
  I2C1ADD = (address_i2c); // Address Register, Get address (7-1 bit). Lsb is read/write flag

  // **** SSPCON1 **** //
  I2C1STATbits.IWCOL = 0;
  I2C1STATbits.I2COV = 0;

  IEC1bits.SI2C1IE = 1;
  IFS1bits.SI2C1IF = 0;

}

void interrupt_i2c() org 0x000034{
  unsigned char state = 0;
  
  if (IFS1bits.SI2C1IF){  // I2C Interrupt

    if(I2C1STATbits.S){
      state  = I2C1STAT & MASTER_MASK;

      switch (state){
        case MASTER_READ_ADD:
          nb_tx_octet = 0;
          tmp_rx = I2C1RCV;
          // do not add break here
          // In both D_A case (transmit data after receive add)
        case MASTER_READ_DATA:
          i2c_write_data_to_buffer(nb_tx_octet);
          nb_tx_octet++;
          break;

        case MASTER_WRITE_ADD:
          nb_rx_octet = 0;
          tmp_rx = I2C1RCV;
        break;

        case MASTER_WRITE_DATA:
          if(nb_rx_octet < SIZE_RX_BUFFER){
              rxbuffer_tab[nb_rx_octet] = I2C1RCV;
              nb_rx_octet++;
            }
            else // case overload rxbuffer_tab size
              tmp_rx = I2C1RCV;
          break;
      }
    }

    I2C1CONbits.SCLREL = 1;
    IFS1bits.SI2C1IF = 0; // reset SSP interrupt flag
  }

  if(I2C1STATbits.IWCOL || I2C1STATbits.I2COV  || I2C1STATbits.BCL){
    I2C1STATbits.BCL = 0;
    I2C1STATbits.IWCOL = 0;
    I2C1STATbits.I2COV = 0;
    tmp_rx = I2C1RCV;
  }
}