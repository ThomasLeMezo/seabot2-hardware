/*  DSPIC33FJ32MC202
External oscillator 16MHz

RB5 : Top switch (RC0)
RB6 : Bottom switch (RC1)
RB7 : Interrupt for switch (Nand gate)

RB12 : Codeur Indx
RB11 : Codeur B
RB10 : Codeur A

RB13 : LED

RB14 : IN1 (PWM motor)
RA1 : Enable
RA0 : Motor Current Sensor

 */

#include <xc.h>
#include "config.h"

volatile unsigned char i2c_nb_bytes = 0;
volatile unsigned char i2c_register = 0x00;

void i2c_handler_address() {
    I2C_Read();
    i2c_nb_bytes = 0;
}

void i2c_handler_read() {
    unsigned char read_byte = I2C_Read();
    if (i2c_nb_bytes == 0)
        i2c_register = read_byte;
    else {
        switch (i2c_register + i2c_nb_bytes - 1) {
            case 0x00:
                if (read_byte)
                    LED_SetHigh();
                else
                    LED_SetLow();
                break;
            case 0x01:
                if (read_byte)
                    ENABLE_SetHigh();
                else
                    ENABLE_SetLow();
                break;
            case 0x02:
                P1DC1 = read_byte;
                break;
            default:
                break;
        }
        i2c_nb_bytes++;
    }
}

void i2c_handler_write() {
    switch (i2c_register + i2c_nb_bytes) {
        case 0x00:
            I2C_Write(POS1CNT);
            break;
        case 0x01:
            I2C_Write((POS1CNT >> 8)); // not recommended (two reads)
            break;
        case 0x02:
            I2C_Write(qei_overflow);
            break;
        case 0x03:
            I2C_Write((!SWITCH_TOP_GetValue())
                       | (!SWITCH_BOTTOM_GetValue() << 1));
        case 0x10:
            I2C_Write(QEI1CONbits.UPDN); // Direction status
            break;
        case 0xC0:
            I2C_Write(CODE_VERSION);
            break;
        case 0xC1:
            I2C_Write(is_init);
            break;
        default:
            I2C_Write(0x00);
            break;
    }
    watchdog_restart = watchdog_restart_default;
}

/**
 * Interrupt switch
 */
void __attribute__((__interrupt__, auto_psv)) _INT0Interrupt() {
    if (IFS0bits.INT0IF) {
        IFS0bits.INT0IF = 0;
        if ((!SWITCH_TOP_GetValue() && MOTOR_CMD > MOTOR_STOP)
                || (!SWITCH_BOTTOM_GetValue() && MOTOR_CMD < MOTOR_STOP)) {
            LED_SetHigh();
            MOTOR_CMD = MOTOR_STOP;
        }
    }
}

/**
 * Interruption for QEI when overflow
 */
void __attribute__((__interrupt__, auto_psv)) _QEIInterrupt() {
    _QEIIF = 0;
    if (QEI1CONbits.UPDN)
        qei_overflow += 1;
    else
        qei_overflow -= 1;

}

/**
 * @brief I2C
 */
void __attribute__((__interrupt__, auto_psv)) _SI2C1Interrupt(void) {
    if (IEC1bits.SI2C1IE == 1 && IFS1bits.SI2C1IF == 1) {
        MSSP_InterruptHandler();
    }
}

/*
 * @brief Timer 1
 */
void handle_timer1(){
    LED_Toggle();
}

/*
 * @brief Timer 2
 */
void handle_timer2(){
    
}

/**
 * @brief main
 */
int main() {
    SYSTEM_Initialize();  // 40 MIPS
    
    // Timers
    TMR1_SetInterruptHandler(handle_timer1);
    TMR2_SetInterruptHandler(handle_timer2);
    
     // Initialize I/O
    I2C_Open();
    I2C_SlaveSetReadIntHandler(i2c_handler_read);
    I2C_SlaveSetWriteIntHandler(i2c_handler_write);
    I2C_SlaveSetAddrIntHandler(i2c_handler_address);
    
    /*  init_timer0(); // Initialize TIMER0 every 1 seconds
      init_timer3(); // Initialize TIMER3 every 100ms*/

    //ADC1_Init();

    ENABLE_SetLow();
    LED_SetHigh();

    is_init = 1;

    // Resset the  POS1CNT = 0x00;
    POS1CNT = 0x0000;

    while (1) {
        //asm CLRWDT;

        //ENABLE = 1;
    }

    return 0;
}
