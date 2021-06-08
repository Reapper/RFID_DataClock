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

uint8 dataArray[256];
//uint8 testArray[1000];
uint16 bitCount = 0;

void displayData()
{
    UART_1_PutArray(dataArray,255);
}

CY_ISR(dataInterrupt)
{
    dataArray[bitCount] = Data_Read();
    
    uint8 test1 = dataArray[bitCount];
    uint16 test2 = bitCount;
    bitCount+=1;
    Clock_ClearInterrupt();
}

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    
    
    UART_1_Start();
    clockISR_StartEx(dataInterrupt);
    //UART_1_PutString("Started\n");
    for(;;)
    {
        
        /* Place your application code here. */
        if(bitCount >= 96)
        {   
            /*for(uint16 i=0; i>1001; i+=1)
            {
                testArray[i] = dataArray[i];
            }*/
            displayData();
            uint16 test = bitCount;
            bitCount = 0;
        }
        
    }
}

/* [] END OF FILE */
