/**
  ADC1 Generated Driver File

  @Company
    Microchip Technology Inc.

  @File Name
    adc1.c

  @Summary
    This is the generated driver implementation file for the ADC1 driver using PIC24 / dsPIC33 / PIC32MM MCUs

  @Description
    This source file provides APIs for ADC1.
    Generation Information :
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - 1.170.0
        Device            :  dsPIC33EV256GM106      
    The generated drivers are tested against the following:
        Compiler          :  XC16 v1.61
        MPLAB 	          :  MPLAB X v5.45
*/

/*
    (c) 2020 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/

/**
  Section: Included Files
*/

#include "adc1.h"

/**
  Section: File Specific Functions
*/

// ADC1 Default Interrupt Handler
static void (*ADC1_DefaultInterruptHandler)(void) = NULL;

/**
  Section: Driver Interface
*/

void ADC1_Initialize (void)
{
    // ASAM enabled; ADDMABM disabled; ADSIDL disabled; DONE disabled; SIMSAM Sequential; FORM Absolute decimal result, unsigned, right-justified; SAMP disabled; SSRC Clearing sample bit ends sampling and starts conversion; AD12B 12-bit; ADON enabled; SSRCG disabled; 
    AD1CON1bits.ADON = 0; // ADC is OFF
    AD1CON1bits.ADSIDL = 0; // Continue in idle mode
    AD1CON1bits.AD12B = 1; // 12bit mode
    AD1CON1bits.FORM = 0b00; // Integer result
    
    AD1CON1bits.SSRC = 0b111; // automatic sampling and conversion (Internal counter ends sampling and starts conversion (auto-convert))
    AD1CON1bits.SIMSAM = 0; // sequential sampling
    AD1CON1bits.ASAM = 1; // Auto-restart sampling
    AD1CON1bits.SAMP = 0; // holding
    AD1CON1bits.DONE = 0; // clear status bit
    
    // CSCNA enabled; VCFG0 AVDD; VCFG1 AVSS; ALTS disabled; BUFM disabled; SMPI Generates interrupt after completion of every 2nd sample/conversion operation; CHPS 1 Channel; 
    AD1CON2bits.VCFG = 0b000;
    AD1CON2bits.CSCNA = 0b1; // Scan CH0+ Inputs
    AD1CON2bits.CHPS = 0b0; // Channel Select bits (Converts CH0)
    AD1CON2bits.SMPI = 0b0001;
    AD1CON2bits.BUFM = 0b0;
    AD1CON2bits.ALTS = 0b0;
    
    // SAMC 1; ADRC FOSC/2; ADCS 4; 
    AD1CON3bits.ADRC = 0b0;
    AD1CON3bits.SAMC = 1; // 1 TAD (Auto Sample Time bits)
    AD1CON3bits.ADCS = 4; // TAD  4 * TCY

    // CH0SA OA2/AN0; CH0SB OA2/AN0; CH0NB VREFL; CH0NA VREFL; 
    AD1CHS0bits.CH0NA = 0; // Select VREF- for CH0- input
    
    // CSS9 disabled; CSS8 disabled; CSS7 disabled; CSS6 disabled; CSS5 enabled; CSS4 disabled; CSS3 disabled; CSS2 disabled; CSS15 disabled; CSS1 disabled; CSS14 disabled; CSS0 enabled; CSS13 disabled; CSS12 disabled; CSS11 disabled; CSS10 disabled; 
    AD1CSSLbits.CSS0 = 1;
    AD1CSSLbits.CSS5 = 1;
    // CH123SA2 disabled; CH123SB2 CH1=OA2/AN0,CH2=AN1,CH3=AN2; CH123NA disabled; CH123NB CH1=VREF-,CH2=VREF-,CH3=VREF-; 
    //AD1CHS123 = 0x00;
    
    AD1PCFGLbits.PCFG0 = 0;
    AD1PCFGLbits.PCFG5 = 0;
    
    //Assign Default Callbacks
    ADC1_SetInterruptHandler(&ADC1_CallBack);
   
    AD1CON1bits.ADON = 1; // Start ADC
}

void __attribute__ ((weak)) ADC1_CallBack(void)
{

}

void ADC1_SetInterruptHandler(void* handler)
{
    ADC1_DefaultInterruptHandler = handler;
}

void __attribute__ ((weak)) ADC1_Tasks ( void )
{
    if(IFS0bits.AD1IF)
    {
        if(ADC1_DefaultInterruptHandler) 
        { 
            ADC1_DefaultInterruptHandler(); 
        }

        // clear the ADC interrupt flag
        IFS0bits.AD1IF = false;
    }
}

/**
  End of File
*/

