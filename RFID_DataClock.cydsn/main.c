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
        //UART_1_PutString("0");
    } else {
        //UART_1_PutString("1");
        dataArray[bitCount] = 1;
    }
    
    bitCount+=1;
    Clock_ClearInterrupt();
}

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    
    
    UART_1_Start();
    PWM_1_Start();
    clockISR_StartEx(dataInterrupt);

    for(;;)
    {
        /* Place your application code here. */
        if(bitCount >= 96)
        {
            uint8 binaryNumber[4];
            char codeArray[7];
            uint8 number;
            char charNumber;
            // pour chaque chiffre de l'ID
            for(uint8 n=0; n<7; n+=1)
            {
                // crée un tableau de bits pour 1 chiffre
                for(uint8 i=0; i<5; i+=1)
                {
                    binaryNumber[i] = dataArray[59+i+(n*5)];
                }
                // transforme le binaire en décimal
                uint8 j=1;
                for(uint8 k=0; k<5; k+=1)
                {
                    number += binaryNumber[k]*j;
                    j*=2;
                }

                charNumber = number;
                    
                UART_1_WriteTxData(number);
                 //   codeArray[i] = number;
                 //   dataPointer = codeArray;
                
            }
            
            
            //sendData(codeArray,7);
            bitCount = 0;
        }
    }
}

/* [] END OF FILE */
