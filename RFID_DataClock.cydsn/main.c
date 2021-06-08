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

uint16 dataArray[256];
//uint8 testArray[1000];
uint16 bitCount = 0;

void displayData()
{
    //UART_1_PutArray(dataArray,255);
    for(uint8 i=0;i<9;i+=1)
    {
        UART_1_WriteTxData(dataArray[i]);
    }
    //UART_1_PutString("https://www.youtube.com/watch?v=dQw4w9WgXcQ");
}

CY_ISR(dataInterrupt)
{
    if(Data_Read() == 1) UART_1_PutString("0");
    else UART_1_PutString("1");
    /*
    dataArray[bitCount] = Data_Read();
    bitCount+=1;*/
    Clock_ClearInterrupt();
}

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    
    
    UART_1_Start();
    PWM_1_Start();
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
            //displayData();
            bitCount = 0;
        }
        
    }
}

/* [] END OF FILE */
