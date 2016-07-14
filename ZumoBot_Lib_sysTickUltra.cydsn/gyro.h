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

/* [] END OF FILE */

#include <project.h>

uint16 value_convert_gyro(uint16 raw);
uint16 convert_raw(uint8 L, uint8 H);

#define WHO_AM_I_GYRO       0x0F   
#define GYRO_ADDR           0x6B
#define GYRO_CTRL1_REG      0x20  
#define GYRO_CTRL4_REG      0x23
#define OUT_X_AXIS_L        0x28            // Gyroscope output
#define OUT_X_AXIS_H        0x29
#define OUT_Y_AXIS_L        0x2A
#define OUT_Y_AXIS_H        0x2B
#define OUT_Z_AXIS_L        0x2C
#define OUT_Z_AXIS_H        0x2D
