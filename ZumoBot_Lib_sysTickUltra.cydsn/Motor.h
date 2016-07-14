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

void motor_Start();

void motor_Stop();

/* moving forward */
void motor_forward(uint8 speed,uint32 delay);


/* moving left when l_speed < r_speed or moving right when l_speed > r_speed */
void motor_turn(uint8 l_speed, uint8 r_speed, uint32 delay);


/* moving backward */
void motor_backward(uint8 speed,uint32 delay);

/* [] END OF FILE */
