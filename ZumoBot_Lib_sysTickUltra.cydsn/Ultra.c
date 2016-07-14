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
#include "Ultra.h"

uint16_t i = 0;
float distance = 0;
float cent = 0;
int cnt = 0;

CY_ISR(ultra_isr_handler)
{    
    printf("3\r\n");
    
    
    if(cnt %5000 == 0)
    {
        Timer_Stop();
        Timer_ReadStatusRegister();
        
        if(!Echo_Read()) {
            i = Timer_ReadCounter();            // Read counter value of Timer
            distance = (float)i / 58;           // us / 58 = centimeters
            cent = 5 * (distance - 11) / 3;     // calibration for this Timer(650 kHz)
            printf("i = %6d, distance = %4d                \r\n", i, (int)cent);
            Timer_WriteCounter(0xFFFF);         // Counter initialization
        }
        else {
             Timer_WriteCounter(0xFFFF);
        }
        Timer_Start();
        cnt=0;
    }
    
    cnt++;
    //printf("%d \r\n",cnt);
}



void Ultra_Start()
{
    Timer_Start();
}

/* [] END OF FILE */
