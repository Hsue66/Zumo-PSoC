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

void nunchuk_start();
void nunchuk_init();
void nunchuk_read();

/////////////////////////////

volatile struct joy_ {
    int x;
    int y;
} joy;

int getJoyX();
int getJoyY();
/////////////////////////////
/* [] END OF FILE */
