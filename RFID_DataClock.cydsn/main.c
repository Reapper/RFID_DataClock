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

uint8 dataArray[1000];
uint16 bitCount = 0;
uint16 num = 0x30;

void displayData()
{
    UART_1_PutArray(dataArray,8);
}

CY_ISR(dataInterrupt)
{
    //UART_1_PutString("\nInterrupted");
    //UART_1_WriteTxData(num);
    //UART_1_PutString("\n");
    //num+=1;
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
    UART_1_PutString("Started\n");
    for(;;)
    {
        uint16 test = bitCount;
        /* Place your application code here. */
        if(bitCount >= 1)
        {
            displayData();
            bitCount = 0;
        }
        
    }
}

/* [] END OF FILE */
