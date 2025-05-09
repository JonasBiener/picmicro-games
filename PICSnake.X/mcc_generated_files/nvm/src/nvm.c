/**
 * @file nvm.c
 *
 * @ingroup nvm_driver
 *
 * @brief Contains the implementation file for the NVM driver.
 *
 * @version NVM Driver Version 1.0.2 
 */

/*
© [2025] Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms, you may use Microchip 
    software and any derivatives exclusively with Microchip products. 
    You are responsible for complying with 3rd party license terms  
    applicable to your use of 3rd party software (including open source  
    software) that may accompany Microchip software. SOFTWARE IS ?AS IS.? 
    NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS 
    SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT,  
    MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT 
    WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY 
    KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF 
    MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE 
    FORESEEABLE. TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP?S 
    TOTAL LIABILITY ON ALL CLAIMS RELATED TO THE SOFTWARE WILL NOT 
    EXCEED AMOUNT OF FEES, IF ANY, YOU PAID DIRECTLY TO MICROCHIP FOR 
    THIS SOFTWARE.
*/

#include "../nvm.h"

/*
 * AoU: These variables are placed in the Access RAM using the "__near" qualifier.
 *      Additionally, "Address Qualifiers" must be configured to the "Require" under
 *      "Project Properties -> XC8 Compiler -> Optimizations".
 */
__near volatile static uint8_t unlockKeyLow;
__near volatile static uint8_t unlockKeyHigh;

void NVM_Initialize(void)
{
    NVM_StatusClear();
}

bool NVM_IsBusy(void)
{
    return (EECON1bits.WR || EECON1bits.RD);
}

nvm_status_t NVM_StatusGet(void)
{
    nvm_status_t nvmStatus;
    if (EECON1bits.WRERR == 1)
    {
        nvmStatus = NVM_ERROR;
    }
    else
    {
        nvmStatus = NVM_OK;
    }
    return nvmStatus;
}

void NVM_StatusClear(void)
{
    EECON1bits.WRERR = 0;
}

void NVM_UnlockKeySet(uint16_t unlockKey)
{
    unlockKeyHigh = (uint8_t) (unlockKey >> 8);
    unlockKeyLow = (uint8_t) unlockKey;
}

void NVM_UnlockKeyClear(void)
{
    unlockKeyHigh = 0x00;
    unlockKeyLow = 0x00;
}

eeprom_data_t EEPROM_Read(eeprom_address_t address)
{
    //Access EEPROM
    EECON1bits.CFGS = 0;
    EECON1bits.EEPGD = 0;  
    
    //Load EEADR with the EEPROM address
    EEADR = (uint8_t) address;
    
    //Initiate Read
    EECON1bits.RD = 1;
    NOP();  // NOPs may be required for latency at high frequencies
    NOP();

    return EEDATA;
}

void EEPROM_Write(eeprom_address_t address, eeprom_data_t data)
{
    //Save global interrupt enable bit value
    uint8_t globalInterruptBitValue = INTCONbits.GIE;

    //Access EEPROM
    EECON1bits.CFGS = 0;
    EECON1bits.EEPGD = 0;
    
    //Enable write operation
    EECON1bits.WREN = 1;

    //Load EEADR with the EEPROM address
    EEADR = (uint8_t) address;

    //Load NVMDAT with the desired value
    EEDATA = data;

    //Disable global interrupt
    INTCONbits.GIE = 0;

    //Perform the unlock sequence
    EECON2 = unlockKeyLow;
    EECON2 = unlockKeyHigh;
    EECON1bits.WR = 1;
    NOP();
    NOP();

    //Restore global interrupt enable bit value
    INTCONbits.GIE = globalInterruptBitValue; 

    //Disable write operation
    EECON1bits.WREN = 0;
}
