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

#include "Motor.h"

void motor_Start()
{
    PWM_Start();
}

void motor_Stop()
{
    PWM_Stop();
}

/* moving forward */
void motor_forward(uint8 speed,uint32 delay)
{
    PWM_WriteCompare1(speed); 
    PWM_WriteCompare2(speed); 
//    CyDelay(delay);
}

/* moving left when l_speed < r_speed or moving right when l_speed > r_speed */
void motor_turn(uint8 l_speed, uint8 r_speed, uint32 delay)
{
    PWM_WriteCompare1(l_speed); 
    PWM_WriteCompare2(r_speed); 
//    CyDelay(delay);
}

/* moving backward */
void motor_backward(uint8 speed,uint32 delay)
{
    MotorDirLeft_Write(1);
    MotorDirRight_Write(1);
    PWM_WriteCompare1(speed); 
    PWM_WriteCompare2(speed); 
    CyDelay(delay);
}
/* [] END OF FILE */
