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

// get/set ILS aliases
#define ILS_TRIS                 TRISAbits.TRISA2
#define ILS_LAT                  LATAbits.LATA2
#define ILS_PORT                 PORTAbits.RA2
#define ILS_WPU                  WPUAbits.WPUA2
#define ILS_ANS                  ANSELbits.ANS2
#define ILS_SetHigh()            do { LATAbits.LATA2 = 1; } while(0)
#define ILS_SetLow()             do { LATAbits.LATA2 = 0; } while(0)
#define ILS_Toggle()             do { LATAbits.LATA2 = ~LATAbits.LATA2; } while(0)
#define ILS_GetValue()           PORTAbits.RA2
#define ILS_SetDigitalInput()    do { TRISAbits.TRISA2 = 1; } while(0)
#define ILS_SetDigitalOutput()   do { TRISAbits.TRISA2 = 0; } while(0)
#define ILS_SetPullup()          do { WPUAbits.WPUA2 = 1; } while(0)
#define ILS_ResetPullup()        do { WPUAbits.WPUA2 = 0; } while(0)
#define ILS_SetAnalogMode()      do { ANSELbits.ANS2 = 1; } while(0)
#define ILS_SetDigitalMode()     do { ANSELbits.ANS2 = 0; } while(0)

// get/set GLOBAL_POWER aliases
#define GLOBAL_POWER_TRIS                 TRISAbits.TRISA5
#define GLOBAL_POWER_LAT                  LATAbits.LATA5
#define GLOBAL_POWER_PORT                 PORTAbits.RA5
#define GLOBAL_POWER_WPU                  WPUAbits.WPUA5
#define GLOBAL_POWER_SetHigh()            do { LATAbits.LATA5 = 1; } while(0)
#define GLOBAL_POWER_SetLow()             do { LATAbits.LATA5 = 0; } while(0)
#define GLOBAL_POWER_Toggle()             do { LATAbits.LATA5 = ~LATAbits.LATA5; } while(0)
#define GLOBAL_POWER_GetValue()           PORTAbits.RA5
#define GLOBAL_POWER_SetDigitalInput()    do { TRISAbits.TRISA5 = 1; } while(0)
#define GLOBAL_POWER_SetDigitalOutput()   do { TRISAbits.TRISA5 = 0; } while(0)
#define GLOBAL_POWER_SetPullup()          do { WPUAbits.WPUA5 = 1; } while(0)
#define GLOBAL_POWER_ResetPullup()        do { WPUAbits.WPUA5 = 0; } while(0)

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

// get/set CURRENT_MOTOR aliases
#define CURRENT_MOTOR_TRIS                 TRISBbits.TRISB5
#define CURRENT_MOTOR_LAT                  LATBbits.LATB5
#define CURRENT_MOTOR_PORT                 PORTBbits.RB5
#define CURRENT_MOTOR_WPU                  WPUBbits.WPUB5
#define CURRENT_MOTOR_ANS                  ANSELHbits.ANS11
#define CURRENT_MOTOR_SetHigh()            do { LATBbits.LATB5 = 1; } while(0)
#define CURRENT_MOTOR_SetLow()             do { LATBbits.LATB5 = 0; } while(0)
#define CURRENT_MOTOR_Toggle()             do { LATBbits.LATB5 = ~LATBbits.LATB5; } while(0)
#define CURRENT_MOTOR_GetValue()           PORTBbits.RB5
#define CURRENT_MOTOR_SetDigitalInput()    do { TRISBbits.TRISB5 = 1; } while(0)
#define CURRENT_MOTOR_SetDigitalOutput()   do { TRISBbits.TRISB5 = 0; } while(0)
#define CURRENT_MOTOR_SetPullup()          do { WPUBbits.WPUB5 = 1; } while(0)
#define CURRENT_MOTOR_ResetPullup()        do { WPUBbits.WPUB5 = 0; } while(0)
#define CURRENT_MOTOR_SetAnalogMode()      do { ANSELHbits.ANS11 = 1; } while(0)
#define CURRENT_MOTOR_SetDigitalMode()     do { ANSELHbits.ANS11 = 0; } while(0)

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

// get/set BATT_4 aliases
#define BATT_4_TRIS                 TRISCbits.TRISC0
#define BATT_4_LAT                  LATCbits.LATC0
#define BATT_4_PORT                 PORTCbits.RC0
#define BATT_4_ANS                  ANSELbits.ANS4
#define BATT_4_SetHigh()            do { LATCbits.LATC0 = 1; } while(0)
#define BATT_4_SetLow()             do { LATCbits.LATC0 = 0; } while(0)
#define BATT_4_Toggle()             do { LATCbits.LATC0 = ~LATCbits.LATC0; } while(0)
#define BATT_4_GetValue()           PORTCbits.RC0
#define BATT_4_SetDigitalInput()    do { TRISCbits.TRISC0 = 1; } while(0)
#define BATT_4_SetDigitalOutput()   do { TRISCbits.TRISC0 = 0; } while(0)
#define BATT_4_SetAnalogMode()      do { ANSELbits.ANS4 = 1; } while(0)
#define BATT_4_SetDigitalMode()     do { ANSELbits.ANS4 = 0; } while(0)

// get/set BATT_3 aliases
#define BATT_3_TRIS                 TRISCbits.TRISC1
#define BATT_3_LAT                  LATCbits.LATC1
#define BATT_3_PORT                 PORTCbits.RC1
#define BATT_3_ANS                  ANSELbits.ANS5
#define BATT_3_SetHigh()            do { LATCbits.LATC1 = 1; } while(0)
#define BATT_3_SetLow()             do { LATCbits.LATC1 = 0; } while(0)
#define BATT_3_Toggle()             do { LATCbits.LATC1 = ~LATCbits.LATC1; } while(0)
#define BATT_3_GetValue()           PORTCbits.RC1
#define BATT_3_SetDigitalInput()    do { TRISCbits.TRISC1 = 1; } while(0)
#define BATT_3_SetDigitalOutput()   do { TRISCbits.TRISC1 = 0; } while(0)
#define BATT_3_SetAnalogMode()      do { ANSELbits.ANS5 = 1; } while(0)
#define BATT_3_SetDigitalMode()     do { ANSELbits.ANS5 = 0; } while(0)

// get/set BATT_2 aliases
#define BATT_2_TRIS                 TRISCbits.TRISC2
#define BATT_2_LAT                  LATCbits.LATC2
#define BATT_2_PORT                 PORTCbits.RC2
#define BATT_2_ANS                  ANSELbits.ANS6
#define BATT_2_SetHigh()            do { LATCbits.LATC2 = 1; } while(0)
#define BATT_2_SetLow()             do { LATCbits.LATC2 = 0; } while(0)
#define BATT_2_Toggle()             do { LATCbits.LATC2 = ~LATCbits.LATC2; } while(0)
#define BATT_2_GetValue()           PORTCbits.RC2
#define BATT_2_SetDigitalInput()    do { TRISCbits.TRISC2 = 1; } while(0)
#define BATT_2_SetDigitalOutput()   do { TRISCbits.TRISC2 = 0; } while(0)
#define BATT_2_SetAnalogMode()      do { ANSELbits.ANS6 = 1; } while(0)
#define BATT_2_SetDigitalMode()     do { ANSELbits.ANS6 = 0; } while(0)

// get/set BATT_1 aliases
#define BATT_1_TRIS                 TRISCbits.TRISC3
#define BATT_1_LAT                  LATCbits.LATC3
#define BATT_1_PORT                 PORTCbits.RC3
#define BATT_1_ANS                  ANSELbits.ANS7
#define BATT_1_SetHigh()            do { LATCbits.LATC3 = 1; } while(0)
#define BATT_1_SetLow()             do { LATCbits.LATC3 = 0; } while(0)
#define BATT_1_Toggle()             do { LATCbits.LATC3 = ~LATCbits.LATC3; } while(0)
#define BATT_1_GetValue()           PORTCbits.RC3
#define BATT_1_SetDigitalInput()    do { TRISCbits.TRISC3 = 1; } while(0)
#define BATT_1_SetDigitalOutput()   do { TRISCbits.TRISC3 = 0; } while(0)
#define BATT_1_SetAnalogMode()      do { ANSELbits.ANS7 = 1; } while(0)
#define BATT_1_SetDigitalMode()     do { ANSELbits.ANS7 = 0; } while(0)

// get/set CURRENT_ESC1 aliases
#define CURRENT_ESC1_TRIS                 TRISCbits.TRISC6
#define CURRENT_ESC1_LAT                  LATCbits.LATC6
#define CURRENT_ESC1_PORT                 PORTCbits.RC6
#define CURRENT_ESC1_ANS                  ANSELHbits.ANS8
#define CURRENT_ESC1_SetHigh()            do { LATCbits.LATC6 = 1; } while(0)
#define CURRENT_ESC1_SetLow()             do { LATCbits.LATC6 = 0; } while(0)
#define CURRENT_ESC1_Toggle()             do { LATCbits.LATC6 = ~LATCbits.LATC6; } while(0)
#define CURRENT_ESC1_GetValue()           PORTCbits.RC6
#define CURRENT_ESC1_SetDigitalInput()    do { TRISCbits.TRISC6 = 1; } while(0)
#define CURRENT_ESC1_SetDigitalOutput()   do { TRISCbits.TRISC6 = 0; } while(0)
#define CURRENT_ESC1_SetAnalogMode()      do { ANSELHbits.ANS8 = 1; } while(0)
#define CURRENT_ESC1_SetDigitalMode()     do { ANSELHbits.ANS8 = 0; } while(0)

// get/set CURRENT_ESC2 aliases
#define CURRENT_ESC2_TRIS                 TRISCbits.TRISC7
#define CURRENT_ESC2_LAT                  LATCbits.LATC7
#define CURRENT_ESC2_PORT                 PORTCbits.RC7
#define CURRENT_ESC2_ANS                  ANSELHbits.ANS9
#define CURRENT_ESC2_SetHigh()            do { LATCbits.LATC7 = 1; } while(0)
#define CURRENT_ESC2_SetLow()             do { LATCbits.LATC7 = 0; } while(0)
#define CURRENT_ESC2_Toggle()             do { LATCbits.LATC7 = ~LATCbits.LATC7; } while(0)
#define CURRENT_ESC2_GetValue()           PORTCbits.RC7
#define CURRENT_ESC2_SetDigitalInput()    do { TRISCbits.TRISC7 = 1; } while(0)
#define CURRENT_ESC2_SetDigitalOutput()   do { TRISCbits.TRISC7 = 0; } while(0)
#define CURRENT_ESC2_SetAnalogMode()      do { ANSELHbits.ANS9 = 1; } while(0)
#define CURRENT_ESC2_SetDigitalMode()     do { ANSELHbits.ANS9 = 0; } while(0)

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