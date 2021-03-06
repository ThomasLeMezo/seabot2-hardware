/**
  @Generated Pin Manager Header File

  @Company:
    Microchip Technology Inc.

  @File Name:
    pin_manager.h

  @Summary:
    This is the Pin Manager file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  @Description
    This header file provides APIs for driver for .
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.7
        Device            :  PIC18LF14K22
        Driver Version    :  2.11
    The generated drivers are tested against the following:
        Compiler          :  XC8 2.31 and above
        MPLAB 	          :  MPLAB X 5.45	
*/

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/

#ifndef PIN_MANAGER_H
#define PIN_MANAGER_H

/**
  Section: Included Files
*/

#include <xc.h>

#define INPUT   1
#define OUTPUT  0

#define HIGH    1
#define LOW     0

#define ANALOG      1
#define DIGITAL     0

#define PULL_UP_ENABLED      1
#define PULL_UP_DISABLED     0

// get/set RA2 procedures
#define RA2_SetHigh()            do { LATAbits.LATA2 = 1; } while(0)
#define RA2_SetLow()             do { LATAbits.LATA2 = 0; } while(0)
#define RA2_Toggle()             do { LATAbits.LATA2 = ~LATAbits.LATA2; } while(0)
#define RA2_GetValue()              PORTAbits.RA2
#define RA2_SetDigitalInput()    do { TRISAbits.TRISA2 = 1; } while(0)
#define RA2_SetDigitalOutput()   do { TRISAbits.TRISA2 = 0; } while(0)
#define RA2_SetPullup()             do { WPUAbits.WPUA2 = 1; } while(0)
#define RA2_ResetPullup()           do { WPUAbits.WPUA2 = 0; } while(0)
#define RA2_SetAnalogMode()         do { ANSELbits.ANS2 = 1; } while(0)
#define RA2_SetDigitalMode()        do { ANSELbits.ANS2 = 0; } while(0)

// get/set RB4 procedures
#define RB4_SetHigh()            do { LATBbits.LATB4 = 1; } while(0)
#define RB4_SetLow()             do { LATBbits.LATB4 = 0; } while(0)
#define RB4_Toggle()             do { LATBbits.LATB4 = ~LATBbits.LATB4; } while(0)
#define RB4_GetValue()              PORTBbits.RB4
#define RB4_SetDigitalInput()    do { TRISBbits.TRISB4 = 1; } while(0)
#define RB4_SetDigitalOutput()   do { TRISBbits.TRISB4 = 0; } while(0)
#define RB4_SetPullup()             do { WPUBbits.WPUB4 = 1; } while(0)
#define RB4_ResetPullup()           do { WPUBbits.WPUB4 = 0; } while(0)
#define RB4_SetAnalogMode()         do { ANSELHbits.ANS10 = 1; } while(0)
#define RB4_SetDigitalMode()        do { ANSELHbits.ANS10 = 0; } while(0)

// get/set RB6 procedures
#define RB6_SetHigh()            do { LATBbits.LATB6 = 1; } while(0)
#define RB6_SetLow()             do { LATBbits.LATB6 = 0; } while(0)
#define RB6_Toggle()             do { LATBbits.LATB6 = ~LATBbits.LATB6; } while(0)
#define RB6_GetValue()              PORTBbits.RB6
#define RB6_SetDigitalInput()    do { TRISBbits.TRISB6 = 1; } while(0)
#define RB6_SetDigitalOutput()   do { TRISBbits.TRISB6 = 0; } while(0)
#define RB6_SetPullup()             do { WPUBbits.WPUB6 = 1; } while(0)
#define RB6_ResetPullup()           do { WPUBbits.WPUB6 = 0; } while(0)

// get/set LED aliases
#define LED_TRIS                 TRISBbits.TRISB7
#define LED_LAT                  LATBbits.LATB7
#define LED_PORT                 PORTBbits.RB7
#define LED_WPU                  WPUBbits.WPUB7
#define LED_SetHigh()            do { LATBbits.LATB7 = 1; } while(0)
#define LED_SetLow()             do { LATBbits.LATB7 = 0; } while(0)
#define LED_Toggle()             do { LATBbits.LATB7 = ~LATBbits.LATB7; } while(0)
#define LED_GetValue()           PORTBbits.RB7
#define LED_SetDigitalInput()    do { TRISBbits.TRISB7 = 1; } while(0)
#define LED_SetDigitalOutput()   do { TRISBbits.TRISB7 = 0; } while(0)
#define LED_SetPullup()          do { WPUBbits.WPUB7 = 1; } while(0)
#define LED_ResetPullup()        do { WPUBbits.WPUB7 = 0; } while(0)

// get/set ESC1 aliases
#define ESC1_TRIS                 TRISCbits.TRISC0
#define ESC1_LAT                  LATCbits.LATC0
#define ESC1_PORT                 PORTCbits.RC0
#define ESC1_ANS                  ANSELbits.ANS4
#define ESC1_SetHigh()            do { LATCbits.LATC0 = 1; } while(0)
#define ESC1_SetLow()             do { LATCbits.LATC0 = 0; } while(0)
#define ESC1_Toggle()             do { LATCbits.LATC0 = ~LATCbits.LATC0; } while(0)
#define ESC1_GetValue()           PORTCbits.RC0
#define ESC1_SetDigitalInput()    do { TRISCbits.TRISC0 = 1; } while(0)
#define ESC1_SetDigitalOutput()   do { TRISCbits.TRISC0 = 0; } while(0)
#define ESC1_SetAnalogMode()      do { ANSELbits.ANS4 = 1; } while(0)
#define ESC1_SetDigitalMode()     do { ANSELbits.ANS4 = 0; } while(0)

// get/set ESC2 aliases
#define ESC2_TRIS                 TRISCbits.TRISC1
#define ESC2_LAT                  LATCbits.LATC1
#define ESC2_PORT                 PORTCbits.RC1
#define ESC2_ANS                  ANSELbits.ANS5
#define ESC2_SetHigh()            do { LATCbits.LATC1 = 1; } while(0)
#define ESC2_SetLow()             do { LATCbits.LATC1 = 0; } while(0)
#define ESC2_Toggle()             do { LATCbits.LATC1 = ~LATCbits.LATC1; } while(0)
#define ESC2_GetValue()           PORTCbits.RC1
#define ESC2_SetDigitalInput()    do { TRISCbits.TRISC1 = 1; } while(0)
#define ESC2_SetDigitalOutput()   do { TRISCbits.TRISC1 = 0; } while(0)
#define ESC2_SetAnalogMode()      do { ANSELbits.ANS5 = 1; } while(0)
#define ESC2_SetDigitalMode()     do { ANSELbits.ANS5 = 0; } while(0)

/**
   @Param
    none
   @Returns
    none
   @Description
    GPIO and peripheral I/O initialization
   @Example
    PIN_MANAGER_Initialize();
 */
void PIN_MANAGER_Initialize (void);

/**
 * @Param
    none
 * @Returns
    none
 * @Description
    Interrupt on Change Handling routine
 * @Example
    PIN_MANAGER_IOC();
 */
void PIN_MANAGER_IOC(void);



#endif // PIN_MANAGER_H
/**
 End of File
*/