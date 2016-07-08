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
#include <project.h>
#include <stdio.h>
#include "Motor.h"
#include "Ultra.h"
#include "nunchuk.h"
#include "Reflectance.h"

int rread(void);

int main()
{
    CyGlobalIntEnable; 
    UART_Start();
    printf("Start\r\n");
    
    sensor_isr_StartEx(sensor_isr_handler);
    Refelctance_Start();
    
    nunchuk_start();
    nunchuk_init();
    
    for(;;)
    {
        nunchuk_read();
        printf("%d %d\r\n",getJoyX(),getJoyY());
        
        int MidX = 128;
        int MidY = 123;
        
        if(getJoyX() < 130 && getJoyX() > 125 && getJoyY() < 125 && getJoyY() > 120)
        {
            motor_Stop();
        }
        else if( getJoyX() < 140 && getJoyX() > 129 && getJoyY() > 123)
        {
            motor_Start();
            motor_forward((getJoyY()-MidY)/2,2000);
        }
        else if(getJoyX()<MidX && getJoyY() > MidY)
        {
            motor_Start();
            motor_turn((getJoyY()-MidY)/2,(MidX-getJoyX()),2000);
        }
        else if(getJoyX()>MidX && getJoyY() > MidY)
        {
            motor_Start();
            motor_turn((getJoyX()-MidX),(getJoyY()-MidY)/2,2000);
        }
       
        /* line tracing
        reflectance_period();
        ref_calibration(10000);
        CyDelay(500);
        //reflectance_digital();      //print out 0 or 1 according to results of reflectance period
        printf("%d %d %d %d\r\n",getDLL(),getDLR(),getDRL(),getDRR());   
      
        
        if(getDLR()==0 && getDRL()==0)
        {
            motor_Start();
            motor_forward(50,2000);   
        }
        else if(getDLR()==1 && getDRL()==0)
        {
            motor_Start();
            motor_turn(50,20,2000);   
        }
        else if(getDLR()==0 && getDRL()==1)
        {
            motor_Start();
            motor_turn(20,50,2000);   
        }
        else
            motor_Stop();
         
        */
    }
    
}


#if 0
int rread(void)
{
    SC0_SetDriveMode(PIN_DM_STRONG);
    SC0_Write(1);
    CyDelayUs(10);
    SC0_SetDriveMode(PIN_DM_DIG_HIZ);
    Timer_1_Start();
    uint16_t start = Timer_1_ReadCounter();
    uint16_t end = 0;
    while(!(Timer_1_ReadStatusRegister() & Timer_1_STATUS_TC)) {
        if(SC0_Read() == 0 && end == 0) {
            end = Timer_1_ReadCounter();
        }
    }
    Timer_1_Stop();
    
    return (start - end);
}
#endif

int _write(int file, char *ptr, int len)
{
    (void) file;
	int n;
	for(n = 0; n < len; n++) {
		UART_PutChar(*ptr++);
	}
	return len;
}

/* [] END OF FILE */
