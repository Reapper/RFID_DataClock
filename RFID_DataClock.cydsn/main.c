/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/

#include "project.h"

#define BINMODE 0
#define CONSOLECONTROLE 1
#define BITSMAX 96

#define NEWLINE 0x0D

uint8 dataArray[96];
uint8 *dataPointer = NULL;
uint16 bitCount = 0;

void sendData(uint8 array[], uint8 arrayLenght)
{
    uint8 i;
 
    for (i = 0 ; i <= arrayLenght ; i++)
    {
        UART_1_WriteTxData(array[i]);
        
    }
}

CY_ISR(dataInterrupt)
{
    if(Data_Read() == 1)
    {
        dataArray[bitCount] = 0;
        if(BINMODE == 1)UART_1_PutString("0");
    } else {
        if(BINMODE == 1)UART_1_PutString("1");
        dataArray[bitCount] = 1;
    }
    if(bitCount >= BITSMAX-1 && BINMODE == 1)UART_1_PutString("000000000000000\n");
    bitCount+=1;
    Clock_ClearInterrupt();
}

int main(void)
{
    CyGlobalIntEnable; // Enable global interrupts.

    //Place your initialization/startup code here (e.g. MyInst_Start())
    
    
    UART_1_Start();
    PWM_1_Start();
    clockISR_StartEx(dataInterrupt);

    for(;;)
    {
        //Place your application code here.
        if(bitCount >= BITSMAX)
        {
            uint8 binaryNumber[4];
            uint16 index = 56;
            uint8 codeArray[7];
            uint8 number=0;
            uint8 failed = 0;
            
            // pour chaque chiffre de l'ID
            for(uint8 n=0; n<6; n+=1)
            {
                // crée un tableau de bits pour 1 chiffre
                
                for(uint8 i=0; i<4; i+=1)
                {
                    binaryNumber[i] = dataArray[index];
                    index += 1;
                } // END for(uint8 i=0; i<4; i+=1)
                
                index = index + 1;
                
                // transforme le binaire en décimal
                uint8 j=1;
                for(uint8 k=0; k<4; k+=1)
                {
                    number += binaryNumber[k]*j;
                    j*=2;
                } // END for(uint8 k=0; k<4; k+=1)
                                
                if(number >= 0 && number <= 9) codeArray[n] = number+0x30;
                else failed = 1;
                
                number = 0;
                
            } // END for(uint8 n=0; n<7; n+=1)
            
            if(failed != 1 && BINMODE != 1)UART_1_PutArray(codeArray,7);
            //else UART_1_PutString("FAILED");
            CyDelay(1);
            
            
            
            bitCount = 0;
            failed = 0;
        } // END if(bitCount >= BITSMAX)
    }
}
/**/
/* [] END OF FILE */
