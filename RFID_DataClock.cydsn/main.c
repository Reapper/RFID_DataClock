/* ========================================
 *
 * RFID Reader/tester
 *
 * Copyright Pesage du Sud-Ouest, 2021
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF Pesage du Sud-Ouest.
 *
 * Developped by : Axel Marceau
 *        GitHub : https://github.com/Reapper
 *          Mail : axel@e-marceau.com
 *
 * ========================================
*/
#define TRUE 1
#define FALSE 0
#include "project.h"

/// *** OPTIONS *** ///

// Active la transmission des données en mode binaire ASCII
//#define BINMODE TRUE
#define BINMODE FALSE

// Saut de ligne pour l'affichage des données dans une console
//#define CONSOLE_CTRL TRUE
#define CONSOLE_CTRL FALSE

// Definis le nombre de bits de la trame
#define BITSMAX 96 // Pour 13 Carateres
//#define BITSMAX 80 // Pour 10 Caracters

// Nombre de carateres a afficher en partant de la fin
#define ID_NUMBERS 6

#define NEWLINE 0x0D

/// *** GLOBALS VARS *** ///
uint8 dataArray[96];    // Tableau de la trame
uint16 bitCount = 0;    // Compte le nombre bits

// Fonction faite lors d'une interruption
CY_ISR(dataInterrupt)
{
    // Lecture bit a bit de la trame, ajout dans un tableau
    if(Data_Read() == 1)
    {
        if(BINMODE)UART_1_PutString("0"); // Affichage binaire
        else dataArray[bitCount] = 0;
    } else {
        if(BINMODE)UART_1_PutString("1");
        else dataArray[bitCount] = 1;
    }
    // Ajout de 16 "0" non traité par l'interruption
    // car la Clock s'arrete lors du 1er bit de stop
    if(bitCount >= BITSMAX-1 && BINMODE)UART_1_PutString("000000000000000");
    bitCount+=1;
    // Rearme l'interruption
    Clock_ClearInterrupt();
}

int main(void)
{
    CyGlobalIntEnable; // Enable global interrupts.
    
    UART_1_Start();
    clockISR_StartEx(dataInterrupt);
    GreenLED_Write(FALSE);
    RedLED_Write(FALSE);

    for(;;)
    {
        if(bitCount >= BITSMAX)
        {
            uint8 binaryNumber[4];                      // Tableau de 4 bits
            uint16 index = (BITSMAX-10)-(ID_NUMBERS*5); // Index de la trame pour le premier chiffre
            uint8 codeArray[ID_NUMBERS];                // Tableau de char pour l'envoie sur le port série
            uint16 number = 0;                          // Variable pour le resultat du nombre binaire
            uint8 failed = FALSE;                       // Varaible de test en cas de mauvaise lecture
            
            
            
            // Pour chaque chiffre de l'ID
            for(uint8 n=0; n<ID_NUMBERS; n+=1)
            {
                // Créé un tableau de bits pour 1 chiffre
                for(uint8 i=0; i<4; i+=1)
                {
                    binaryNumber[i] = dataArray[index];
                    index += 1;
                } // END for(uint8 i=0; i<4; i+=1)
                
                // Incrementation pour passer le bit de parité
                index = index + 1;
                
                // Transforme le binaire en décimal
                uint8 j=1;
                for(uint8 k=0; k<4; k+=1)
                {
                    number += binaryNumber[k]*j;
                    j*=2;
                } // END for(uint8 k=0; k<4; k+=1)
                
                // Teste si le caratere enregistré en bien un chiffre
                if(number >= 0 && number <= 9) codeArray[n] = number+0x30; // n+0x30 pour l'obtenir en ASCII
                else failed = TRUE;
                
                // RAZ de la variable pour le prochain chiffre
                number = 0;
            } // END for(uint8 n=0; n<7; n+=1)
            
            // Envoie sur le port série si le test est OK
            if(!failed && !BINMODE)
            {
                UART_1_PutArray(codeArray,ID_NUMBERS);
                GreenLED_Write(TRUE);
                RedLED_Write(FALSE);
            } else { 
                UART_1_PutString("FAILED"); // Envoie "FAILED" dans le cas contraire
                
                GreenLED_Write(FALSE);
                RedLED_Write(TRUE);
            }
            
            // \n(0x0D) en fin de d'affichage des bits
            if(BINMODE && CONSOLE_CTRL)UART_1_PutChar(NEWLINE);
            
            //CyDelay(1);
            
            // RAZ des variable pour la prochaine lecture
            bitCount = 0;
            failed = FALSE;
        } // END if(bitCount >= BITSMAX)
    } // END for(;;)
} // END int main(void)

/* [] END OF FILE */
