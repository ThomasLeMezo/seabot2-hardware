/*
    (c) 2016 Microchip Technology Inc. and its subsidiaries. You may use this
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

#ifndef EEPROM2_APP_H
#define	EEPROM2_APP_H

#define EEPROM2_READ         0x03                // read data from memory
#define EEPROM2_FREAD        0x0B
#define EEPROM2_WREN         0x06                // set the write enable latch
#define EEPROM2_WRITE        0x02                // write data to memory array
#define EEPROM2_RDSR         0x05                // read STATUS register
#define EEPROM2_ADDRBYTES    3

#include <stdint.h>

static void EEPROM2_AddressAssign(uint8_t *addressBuffer, uint32_t byteAddr)
{
    uint8_t i = 0;
    uint8_t j = EEPROM2_ADDRBYTES - 1; // = 2
    uint32_t address = byteAddr;
    
    while(address > 0)
    {
        addressBuffer[j-i] = address & 0xFF; // 0
        i++;
        address >>= 8;
    }
}

void EEPROM2_WriteByte (uint8_t byteData, uint32_t byteAddr);
uint8_t EEPROM2_ReadByte (uint32_t address);
void EEPROM2_WriteBlock(uint8_t *writeBuffer, uint16_t buflen, uint32_t startAddr);
void EEPROM2_ReadBlock(uint8_t *readBuffer, uint16_t buflen, uint32_t startAddr);
uint8_t EEPROM2_WritePoll(void);

#endif	/* EEPROM2_APP_H */

