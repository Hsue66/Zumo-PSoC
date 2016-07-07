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
    
    /* //reflectance//
    ----------------------------------------------------
    sensor_isr_StartEx(sensor_isr_handler);
    
    Refelctance_Start();

    IR_led_Write(1);
    for(;;)
    {
        reflectance_period();      //print out each period of reflectance sensors
        reflectance_digital();      //print out 0 or 1 according to results of reflectance period
        
        CyDelay(500);
    }
    ----------------------------------------------------*/
    
     /* //ultra//
    ----------------------------------------------------
    ultra_isr_StartEx(ultra_isr_handler);   // Ultra Sonic Interrupt
    Ultra_Start();                          // Ultra Sonic Start function
   
    for(;;)
    {       
        CyDelay(100); 
        Trig_Write(1);           // Trigger High
        CyDelayUs(10);              // 10 micro seconds for trigger input signals
        Trig_Write(0);           // Trigger Low
    }
    ----------------------------------------------------*/
    
     /* //motor//
    ----------------------------------------------------
    motor_Start();              // motor start

    motor_forward(50,2000);     // moving forward
    motor_turn(20,50,2000);     // turn
    motor_turn(50,20,2000);     // turn
    motor_backward(50,2000);    // movinb backward
       
    motor_Stop();               // motor stop
    
    for(;;)
    {

    }
    ----------------------------------------------------*/
    
    
    /*  //nunchuk//
    ----------------------------------------------------
    nunchuk_start();
    nunchuk_init();
    
    for(;;)
    {    
        nunchuk_read();
    }
    ----------------------------------------------------*/
    
   
    
   
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
