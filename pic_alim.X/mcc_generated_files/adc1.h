/**
  ADC1 Generated Driver API Header File

  @Company
    Microchip Technology Inc.

  @File Name
    adc1.h

  @Summary
    This is the generated header file for the ADC1 driver using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  @Description
    This header file provides APIs for driver for ADC1.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.7
        Device            :  PIC18LF14K22
        Driver Version    :  2.02
    The generated drivers are tested against the following:
        Compiler          :  XC8 2.31 and above
        MPLAB             :  MPLAB X 5.45
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

#ifndef ADC1_H
#define ADC1_H

/**
  Section: Included Files
*/

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif

/**
  Section: Data Types Definitions
*/

/**
 *  result size of an A/D conversion
 */

typedef uint16_t adc_result_t;

/**
 *  result type of a Double ADC conversion
 */
typedef struct
{
    adc_result_t adcResult1;
    adc_result_t adcResult2;
} adc_sync_double_result_t;

/** ADC Channel Definition

 @Summary
   Defines the channels available for conversion.

 @Description
   This routine defines the channels that are available for the module to use.

 Remarks:
   None
 */

typedef enum
{
    BATT_4 =  0x4,
    BATT_3 =  0x5,
    CURRENT_ESC1 =  0x8,
    CURRENT_ESC2 =  0x9,
    CURRENT_MOTOR =  0xB,
    channel_DAC =  0xE,
    channel_FVR =  0xF
} adc_channel_t;

/**
  Section: ADC Module APIs
*/

/**
  @Summary
    Initializes the ADC1

  @Description
    This routine initializes the Initializes the ADC1.
    This routine must be called before any other ADC1 routine is called.
    This routine should only be called once during system initialization.

  @Preconditions
    None

  @Param
    None

  @Returns
    None

  @Comment
    

  @Example
    <code>
    uint16_t convertedValue;

    ADC1_Initialize();
    convertedValue = ADC1_GetConversionResult();
    </code>
*/
void ADC1_Initialize(void);

/**
  @Summary
    Allows selection of a channel for conversion

  @Description
    This routine is used to select desired channel for conversion.
    available

  @Preconditions
    ADC1_Initialize() function should have been called before calling this function.

  @Returns
    None

  @Param
    Pass in required channel number
    "For available channel refer to enum under adc.h file"

  @Example
    <code>
    uint16_t convertedValue;

    ADC1_Initialize();
    ADC1_SelectChannel(AN1_Channel);
    ADC1_StartConversion();
    convertedValue = ADC1_GetConversionResult();
    </code>
*/
void ADC1_SelectChannel(adc_channel_t channel);

/**
  @Summary
    Starts conversion

  @Description
    This routine is used to start conversion of desired channel.
    
  @Preconditions
    ADC1_Initialize() function should have been called before calling this function.

  @Returns
    None

  @Param
    None

  @Example
    <code>
    uint16_t convertedValue;

    ADC1_Initialize();    
    ADC1_StartConversion();
    convertedValue = ADC1_GetConversionResult();
    </code>
*/
void ADC1_StartConversion(void);

/**
  @Summary
    Returns true when the conversion is completed otherwise false.

  @Description
    This routine is used to determine if conversion is completed.
    When conversion is complete routine returns true. It returns false otherwise.

  @Preconditions
    ADC1_Initialize() and ADC1_StartConversion(void)
    function should have been called before calling this function.

  @Returns
    true  - If conversion is complete
    false - If conversion is not completed

  @Param
    None

  @Example
    <code>
    uint16_t convertedValue;

    ADC1_Initialize();
    ADC1_StartConversion();

    while(!ADC1_IsConversionDone());
    convertedValue = ADC1_GetConversionResult();
    </code>
 */
bool ADC1_IsConversionDone(void);

/**
  @Summary
    Returns the ADC1 conversion value.

  @Description
    This routine is used to get the analog to digital converted value. This
    routine gets converted values from the channel specified.

  @Preconditions
    This routine returns the conversion value only after the conversion is complete.
    Completion status can be checked using
    ADC1_IsConversionDone() routine.

  @Returns
    Returns the converted value.

  @Param
    None

  @Example
    <code>
    uint16_t convertedValue;

    ADC1_Initialize();
    ADC1_StartConversion();

    while(ADC1_IsConversionDone());

    convertedValue = ADC1_GetConversionResult();
    </code>
 */
adc_result_t ADC1_GetConversionResult(void);

/**
  @Summary
    Returns the ADC1 conversion value
    also allows selection of a channel for conversion.

  @Description
    This routine is used to select desired channel for conversion
    and to get the analog to digital converted value.

  @Preconditions
    ADC1_Initialize() function should have been called before calling this function.

  @Returns
    Returns the converted value.

  @Param
    Pass in required channel number.
    "For available channel refer to enum under adc.h file"

  @Example
    <code>
    uint16_t convertedValue;

    ADC1_Initialize();

    conversion = ADC1_GetConversion(AN1_Channel);
    </code>
*/
adc_result_t ADC1_GetConversion(adc_channel_t channel);

/**
  @Summary
    Acquisition Delay for temperature sensor

  @Description
    This routine should be called when temperature sensor is used.
    
  @Preconditions
    ADC1_Initialize() function should have been called before calling this function.

  @Returns
    None

  @Param
    None

  @Example
    <code>
    uint16_t convertedValue;

    ADC1_Initialize();    
    ADC1_StartConversion();
    ADC1_temperatureAcquisitionDelay();
    convertedValue = ADC1_GetConversionResult();
    </code>
*/
void ADC1_TemperatureAcquisitionDelay(void);

#ifdef __cplusplus  // Provide C++ Compatibility

    }

#endif

#endif	//ADC1_H
/**
 End of File
*/

