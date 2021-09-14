#ifndef I2C_H
#define I2C_H

// I2C

// pickit serial communication states
#define I2C_NO_TRANSACTION                  0
#define I2C_SLAVE_ADDRESS_RECEIVED          1
#define I2C_WORD_ADDRESS_RECEIVED           2
#define I2C_READ_ADDRESS                    3
#define I2C_READ_DATA                       4
#define I2C_MASTER_NACK                     5

// define states
#define MASTER_MASK 0x25
#define MASTER_WRITE_ADD 0x01         //Master Writes Address
#define MASTER_WRITE_DATA 0x21        //Master Writes Data
#define MASTER_READ_ADD 0x04         //Master Reads Address
#define MASTER_READ_ADD2 0x05         //Master Reads Address
#define MASTER_READ_DATA 0x24        //Master Reads Data
#define MASTER_UNKNOWN 0x20

#define SIZE_RX_BUFFER 8
volatile extern unsigned short rxbuffer_tab[SIZE_RX_BUFFER];
volatile extern unsigned short tmp_rx;
volatile extern unsigned short nb_tx_octet;
volatile extern unsigned short nb_rx_octet;
volatile extern unsigned short i2c_status;

/**
 * @brief i2c_read_data_from_buffer
 */
void i2c_read_data_from_buffer();

/**
 * @brief i2c_write_data_to_buffer
 * @param nb_tx_octet
 */
inline void i2c_write_data_to_buffer(const unsigned short nb_tx_octet);

/**
 * @brief initialisation de l'I2C en mode esclave
 */
void init_i2c(const unsigned short address_i2c);

/**
 * @brief write data to buffer
 */
void i2c_write_buffer(unsigned short val);


#endif