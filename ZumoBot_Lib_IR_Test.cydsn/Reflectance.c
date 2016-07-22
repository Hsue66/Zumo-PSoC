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

#include "Reflectance.h"

/*
CY_ISR(sensor_isr_handler)
{
    uint32_t statusRL = Timer_RL_ReadStatusRegister();
    uint32_t statusRR = Timer_RR_ReadStatusRegister();
    uint32_t statusLL = Timer_LL_ReadStatusRegister();
    uint32_t statusLR = Timer_LR_ReadStatusRegister();

    if(statusRL & Timer_RL_STATUS_CAPTURE) {
        sensors.rl = Timer_RL_ReadPeriod() - Timer_RL_ReadCapture();
    }
    else {
        sensors.rl = Timer_RL_ReadPeriod();
    }
    
    if(statusRR & Timer_RR_STATUS_CAPTURE) {
        sensors.rr = Timer_RR_ReadPeriod() - Timer_RR_ReadCapture();
    }
    else {
        sensors.rr = Timer_RR_ReadPeriod();
    }
    
    if(statusLL & Timer_LL_STATUS_CAPTURE) {
        sensors.ll = Timer_LL_ReadPeriod() - Timer_LL_ReadCapture();
    }
    else {
        sensors.ll = Timer_LL_ReadPeriod();
    }
    
    if(statusLR & Timer_LR_STATUS_CAPTURE) {
        sensors.lr = Timer_LR_ReadPeriod() - Timer_LR_ReadCapture();
    }
    else {
        sensors.lr = Timer_LR_ReadPeriod();
    }
    
    RL_SetDriveMode(PIN_DM_STRONG);
    RL_Write(1);
    RR_SetDriveMode(PIN_DM_STRONG);
    RR_Write(1);
    LL_SetDriveMode(PIN_DM_STRONG);
    LL_Write(1);
    LR_SetDriveMode(PIN_DM_STRONG);
    LR_Write(1);
    CyDelayUs(10);
    RL_SetDriveMode(PIN_DM_DIG_HIZ);
    RR_SetDriveMode(PIN_DM_DIG_HIZ);
    LL_SetDriveMode(PIN_DM_DIG_HIZ);
    LR_SetDriveMode(PIN_DM_DIG_HIZ);
        
}
void Refelctance_Start()
{
    Timer_RL_Start();
    Timer_RR_Start();
    Timer_LL_Start();
    Timer_LR_Start();
}

void reflectance_period()
{
     printf("%d %d %d %d \r\n", sensors.ll, sensors.lr, sensors.rl, sensors.rr); 
}

void reflectance_digital()
{
    //if the results of reflectance_period function is over 10000, set digital_sensor_value to 0, which means it's black
    if(sensors.ll > 10000)
        digital_sensor_value.ll  = 0;
    else
        digital_sensor_value.ll  = 1;
    if(sensors.lr > 10000)
        digital_sensor_value.lr  = 0;
    else
        digital_sensor_value.lr  = 1;
    if(sensors.rl > 10000)
        digital_sensor_value.rl  = 0;
    else
        digital_sensor_value.rl  = 1;
    if(sensors.rr > 10000)
        digital_sensor_value.rr  = 0;
    else
        digital_sensor_value.rr  = 1;
        
   //print out the results
   printf("%d %d %d %d \r\n", digital_sensor_value.ll, digital_sensor_value.lr, digital_sensor_value.rl, digital_sensor_value.rr);  
}

*/
/* [] END OF FILE */
