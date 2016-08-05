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
#include <stdio.h>

int rread(void);

uint8 I2C_read(uint8 device_addr, uint8 Reg);
void I2C_write(uint8 device_addr, uint8 Reg, uint8 value);
