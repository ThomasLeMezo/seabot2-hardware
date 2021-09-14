#include "i2c.h"
#include "config.h"


volatile unsigned short rxbuffer_tab[SIZE_RX_BUFFER];
volatile unsigned short tmp_rx = 0;
volatile unsigned short nb_tx_octet = 0;
volatile unsigned short nb_rx_octet = 0;

void i2c_read_data_from_buffer(){
  unsigned short k=0;
  unsigned short nb_motor=0;

  for(k=1; k<nb_rx_octet; k++){
    nb_motor = rxbuffer_tab[0] + k - 1;

    if(nb_motor<2){
      if(rxbuffer_tab[k]>=110 && rxbuffer_tab[k]<=190)
        cmd_motor[nb_motor] = rxbuffer_tab[k];
      else
        cmd_motor[nb_motor] = MOTOR_CMD_STOP;
    }
  }
  watchdog_restart = WATCHDOG_RESTART_DEFAULT;
}

void i2c_write_data_to_buffer(const unsigned short nb_tx_octet){
  switch(rxbuffer_tab[0]+nb_tx_octet){
    case 0xC0:
      SSPBUF = CODE_VERSION;
      break;
    case 0xC1:
      SSPBUF = is_init;
      break;
    default:
      SSPBUF = 0x00;
      break;
  }
}

void init_i2c(const unsigned short address_i2c){

  // **** IO I2C **** //
  TRISB4_bit = 1; // RB4 en entrée
  TRISB6_bit = 1; // RB6 en entrée

  // **** Interruptions **** //
  PIE1.SSPIE = 1; // Synchronous Serial Port Interrupt Enable bit
  PIR1.SSPIF = 0; // Synchronous Serial Port (SSP) Interrupt Flag, I2C Slave

  PIR2.BCLIE = 1;
  PIR2.BCLIF = 0;

  // **** ADDRESS **** //
  SSPADD = (address_i2c << 1); // Address Register, Get address (7-1 bit). Lsb is read/write flag
  SSPMSK = 0xFF; // A zero (‘0’) bit in the SSPMSK register has the effect of making
                 // the corresponding bit in the SSPSR register a “don’t care”

  // **** SSPSTAT **** //
  SSPSTAT.SMP = 1; // Slew Rate Control bit
  // 1 = Slew rate control disabled for standard Speed mode (100 kHz and 1 MHz)
  // 0 = Slew rate control enabled for High-Speed mode (400 kHz)
  SSPSTAT.CKE = 1; // // SMBusTM Select bit (1 = Enable SMBus specific inputs)

  // **** SSPCON2 **** //
  SSPCON2 = 0x00;
  SSPCON2.GCEN = 0; // General Call Enable bit (0 = disabled)
  SSPCON2.SEN = 0; // Start Condition Enable/Stretch Enable bit (1 = enabled)

  // **** SSPCON1 **** //
  SSPCON1.WCOL = 0; // Write Collision Detect bit
  SSPCON1.SSPOV = 0; // Receive Overflow Indicator bit
  SSPCON1.CKP = 1; // SCK Release Control bit (1=Release clock)
  SSPCON1.SSPM3 = 0b0; // I2C Slave mode, 7-bit address with Start and Stop bit interrupts enabled (1-> with S/P, 0 -> without)
  SSPCON1.SSPM2 = 0b1; // I2C Slave mode, 7-bit address with Start and Stop bit interrupts enabled
  SSPCON1.SSPM1 = 0b1; // I2C Slave mode, 7-bit address with Start and Stop bit interrupts enabled
  SSPCON1.SSPM0 = 0b0; // I2C Slave mode, 7-bit address with Start and Stop bit interrupts enabled

  // (START the I2C Module)
  SSPCON1.SSPEN = 1; // Synchronous Serial Port Enable bit
}

void interrupt_low(){
  unsigned char state = 0;

  if (PIR1.SSPIF){  // I2C Interrupt

    if(SSPSTAT.S){
      state  = SSPSTAT & MASTER_MASK;

      switch (state){
        case MASTER_READ_ADD:
        case MASTER_READ_ADD2:
          nb_tx_octet = 0;
          tmp_rx = SSPBUF;
          // do not add break here
          // In both D_A case (transmit data after receive add)
        case MASTER_READ_DATA:
          i2c_write_data_to_buffer(nb_tx_octet);
          nb_tx_octet++;
          break;

        case MASTER_WRITE_ADD:
          nb_rx_octet = 0;
          tmp_rx = SSPBUF;
        break;

        case MASTER_WRITE_DATA:
          if(nb_rx_octet < SIZE_RX_BUFFER){
              rxbuffer_tab[nb_rx_octet] = SSPBUF;
              nb_rx_octet++;
            }
            else // case overload rxbuffer_tab size
              tmp_rx = SSPBUF;
          break;
      }
    }

    SSPCON1.CKP = 1;
    PIR1.SSPIF = 0; // reset SSP interrupt flag
  }
  
  if(SSPCON1.SSPOV || SSPCON1.WCOL){
    SSPCON1.SSPOV = 0;
    SSPCON1.WCOL = 0;
    tmp_rx = SSPBUF;
  }
}