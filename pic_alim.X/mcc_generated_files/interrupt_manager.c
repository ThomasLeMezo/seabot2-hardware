/**
  Generated Interrupt Manager Header File

  @Company:
    Microchip Technology Inc.

  @File Name:
    interrupt_manager.h

  @Summary:
    This is the Interrupt Manager file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  @Description:
    This header file provides implementations for global interrupt handling.
    For individual peripheral handlers please see the peripheral driver for
    all modules selected in the GUI.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.7
        Device            :  PIC18LF14K22
        Driver Version    :  2.12
    The generated drivers are tested against the following:
        Compiler          :  XC8 2.31 and above or later
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

#include "interrupt_manager.h"
#include "mcc.h"

void  INTERRUPT_Initialize (void)
{
    // Enable Interrupt Priority Vectors
    RCONbits.IPEN = 1;

    // Assign peripheral interrupt priority vectors

    // BCLI - high priority
    IPR2bits.BCLIP = 1;

    // SSPI - high priority
    IPR1bits.SSPIP = 1;


    // TMRI - low priority
    INTCON2bits.TMR0IP = 0;    

    // TMRI - low priority
    IPR1bits.TMR1IP = 0;    

    // TMRI - low priority
    IPR2bits.TMR3IP = 0;    

    // RABI - low priority
    INTCON2bits.RABIP = 0;    

}

void __interrupt() INTERRUPT_InterruptManagerHigh (void)
{
   // interrupt handler
    if(PIE2bits.BCLIE == 1 && PIR2bits.BCLIF == 1)
    {
        MSSP_InterruptHandler();
    }
    else if(PIE1bits.SSPIE == 1 && PIR1bits.SSPIF == 1)
    {
        MSSP_InterruptHandler();
    }
    else
    {
        //Unhandled Interrupt
    }
}

void __interrupt(low_priority) INTERRUPT_InterruptManagerLow (void)
{
    // interrupt handler
    if(INTCONbits.TMR0IE == 1 && INTCONbits.TMR0IF == 1)
    {
        TMR0_ISR();
    }
    else if(PIE1bits.TMR1IE == 1 && PIR1bits.TMR1IF == 1)
    {
        TMR1_ISR();
    }
    else if(PIE2bits.TMR3IE == 1 && PIR2bits.TMR3IF == 1)
    {
        TMR3_ISR();
    }
    else if(INTCONbits.RABIE == 1 && INTCONbits.RABIF == 1)
    {
        PIN_MANAGER_IOC();
    }
    else
    {
        //Unhandled Interrupt
    }
}
/**
 End of File
*/
