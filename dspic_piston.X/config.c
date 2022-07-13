
// DSPIC33FJ32MC202 Configuration Bit Settings

// 'C' source line config statements

// FBS

#pragma config BWRP = WRPROTECT_OFF     // Boot Segment Write Protect (Boot Segment may be written)
#pragma config BSS = NO_FLASH           // Boot Segment Program Flash Code Protection (No Boot program Flash segment)

// FGS
#pragma config GWRP = OFF               // General Code Segment Write Protect (User program memory is not write-protected)
#pragma config GSS = OFF                // General Segment Code Protection (User program memory is not code-protected)

// FOSCSEL
#pragma config FNOSC = PRIPLL           // Oscillator Mode (Primary Oscillator (XT, HS, EC) w/ PLL)
#pragma config IESO = ON               // Internal External Switch Over Mode (Start-up device with user-selected oscillator source)

// FOSC
#pragma config POSCMD = HS              // Primary Oscillator Source (HS Oscillator Mode)
#pragma config OSCIOFNC = OFF           // OSC2 Pin Function (OSC2 pin has clock out function)
#pragma config IOL1WAY = ON             // Peripheral Pin Select Configuration (Allow Only One Re-configuration)
#pragma config FCKSM = CSECME           // Clock Switching and Monitor (Both Clock Switching and Fail-Safe Clock Monitor are enabled)

// FWDT
#pragma config WDTPOST = PS32768        // Watchdog Timer Postscaler (1:32,768)
#pragma config WDTPRE = PR128           // WDT Prescaler (1:128)
#pragma config WINDIS = OFF             // Watchdog Timer Window (Watchdog Timer in Non-Window mode)
#pragma config FWDTEN = OFF              // Watchdog Timer Enable (Watchdog timer always enabled)

// FPOR
#pragma config FPWRT = PWR128           // POR Timer Value (128ms)
#pragma config ALTI2C = OFF             // Alternate I2C  pins (I2C mapped to SDA1/SCL1 pins)
#pragma config LPOL = ON                // Motor Control PWM Low Side Polarity bit (PWM module low side output pins have active-high output polarity)
#pragma config HPOL = ON                // Motor Control PWM High Side Polarity bit (PWM module high side output pins have active-high output polarity)
#pragma config PWMPIN = ON              // Motor Control PWM Module Pin Mode bit (PWM module pins controlled by PORT register at device Reset)

// FICD
#pragma config ICS = PGD1               // Comm Channel Select (Communicate on PGC1/EMUC1 and PGD1/EMUD1)
#pragma config JTAGEN = OFF             // JTAG Port Enable (JTAG is Disabled)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include <stdbool.h>

#include "config.h"

volatile unsigned short is_init = 0;

// Watchdog
unsigned short watchdog_restart_default = 30; // in min
volatile unsigned short watchdog_countdown_restart = 30;

//unsigned short watchdog_cpt_sec = 59;
//unsigned short watchdog_cpt_default = 59;

signed short qei_overflow = 0;

void CLOCK_Initialize(void){
    // FRCDIV FRC/2; PLLPRE 2; DOZE 1:8; PLLPOST 1:4; DOZEN disabled; ROI disabled; 
    // Input 16MHz Crystal
    
    // DOZE = 1, FRCDIV = 1
    CLKDIVbits.DOZE = 0b000; // FOSC = FCY/1
    CLKDIVbits.FRCDIV = 0b000; // FRC div by 1
    
    // PLLPRE = 1/2
    CLKDIVbits.PLLPRE = 0b0; // 16MHz/2 = 8MHz in [0.8, 8]MHz
    
    // PLLDIV = 40 (start at 2))
    PLLFBDbits.PLLDIV = 18; // 8MHz * 20 = 160MHz in [100,200]MHz
    
    // PLLPOST /2
    CLKDIVbits.PLLPOST = 0b00; // 160MHz / 2 = 80MHz in [12.5, 80]MHz
    
    // TUN Center frequency; 
    OSCTUN = 0x00;
}

/**
 * @brief PIN_MANAGER_Initialize
 * Initialize outputs and inputs
 */
void PIN_MANAGER_Initialize(){
    
    // I2C
    TRISBbits.TRISB8 = 1; // RB4 input
    TRISBbits.TRISB9 = 1; // RB6 input
    
    // INPUT-OUTPUT
    ENABLE_SetDigitalOutput();
    SWITCH_BOTTOM_SetDigitalInput();
    SWITCH_TOP_SetDigitalInput();
    SWITCH_INT_SetDigitalInput();
    LED_SetDigitalOutput();

    // Interrupt for switch
    INTCON2bits.INT0EP = 0; // interrupt on positive edge
    IEC0bits.INT0IE = 1; // enable interrupt
    IFS0bits.INT0IF = 0; // set flag to 0
    
    // Analog
    TRISAbits.TRISA0 = 1;
    TRISBbits.TRISB3 = 1;
    AD1PCFGLbits.PCFG0 = 0;
    AD1PCFGLbits.PCFG5 = 0;
    
}

void QEI_Initialize(){
    // QEI (Quadrature Encoder Interface)
  // Quadrature Encoder Interface enabled (x4 mode) with position counter reset by match (MAXxCNT)
  QEI1CONbits.QEIM0 = 1;
  QEI1CONbits.QEIM1 = 1;
  QEI1CONbits.QEIM2 = 1;
  QEI1CONbits.POSRES = 0; // do not reset position with index
  QEI1CONbits.SWPAB = 1; // Swap Phase A and B

  DFLT1CONbits.CEID = 0 ; // count error disable bit
  DFLT1CONbits.QEOUT = 1 ; // Digital filter outputs are enabled

  MAXCNT = 0xFFFF; // max value (interrupt if overflow)
  POS1CNT = 0x00FF; // Counter inital position
  
  // Map the QEI to the pin of the coder (3 pins)
  RPINR14 = 10 + (11 << 8); // QEA (low byte of RP): 10, QEB : 11 (high byte of RP)
  RPINR15 = 12;

  _QEIIE = 1; // Enable interrupt from QEI
}

void SYSTEM_Initialize(){
    CLOCK_Initialize();
    INTERRUPT_Initialize();
    PIN_MANAGER_Initialize();
    QEI_Initialize();
    PWM_Initialize();
    TMR3_Initialize();
    TMR2_Initialize();
    I2C_Initialize();
    ADC1_Initialize();
}

void QEI_Reset_Count(){
    qei_overflow = 0;
    POS1CNT = 0;
}