/**
  Generated Pin Manager File

  Company:
    Microchip Technology Inc.

  File Name:
    pin_manager.c

  Summary:
    This is the Pin Manager file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  Description:
    This header file provides implementations for pin APIs for all pins selected in the GUI.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.7
        Device            :  PIC18LF14K22
        Driver Version    :  2.11
    The generated drivers are tested against the following:
        Compiler          :  XC8 2.31 and above
        MPLAB             :  MPLAB X 5.45

    Copyright (c) 2013 - 2015 released Microchip Technology Inc.  All rights reserved.
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

#include "pin_manager.h"




void (*IOCA2_InterruptHandler)(void);


void PIN_MANAGER_Initialize(void)
{
    /**
    LATx registers
    */
    LATA = 0x00;
    LATB = 0x00;
    LATC = 0x00;

    /**
    TRISx registers
    */
    TRISA = 0x17;
    TRISB = 0x70;
    TRISC = 0xFF;

    /**
    ANSELx registers
    */
    ANSEL = 0xFB;
    ANSELH = 0x0B;

    /**
    WPUx registers
    */
    WPUB = 0x00;
    WPUA = 0x04;
    INTCON2bits.nRBPU = 0;



    /**
    IOCx registers 
    */
    //interrupt on change for group IOCA - flag
    IOCAbits.IOCA2 = 1;



    // register default IOC callback functions at runtime; use these methods to register a custom function
    IOCA2_SetInterruptHandler(IOCA2_DefaultInterruptHandler);
   
    // Enable RABI interrupt 
    INTCONbits.RABIE = 1; 
    
}
  
void PIN_MANAGER_IOC(void)
{   
	// interrupt on change for pin IOCA2
    if(IOCAbits.IOCA2 == 1)
    {
        IOCA2_ISR();  
    }	
	// Clear global Interrupt-On-Change flag
    INTCONbits.RABIF = 0;
}

/**
   IOCA2 Interrupt Service Routine
*/
void IOCA2_ISR(void) {

    // Add custom IOCA2 code

    // Call the interrupt handler for the callback registered at runtime
    if(IOCA2_InterruptHandler)
    {
        IOCA2_InterruptHandler();
    }
}

/**
  Allows selecting an interrupt handler for IOCA2 at application runtime
*/
void IOCA2_SetInterruptHandler(void (* InterruptHandler)(void)){
    IOCA2_InterruptHandler = InterruptHandler;
}

/**
  Default interrupt handler for IOCA2
*/
void IOCA2_DefaultInterruptHandler(void){
    // add your IOCA2 interrupt custom code
    // or set custom function using IOCA2_SetInterruptHandler()
}

/**
 End of File
*/