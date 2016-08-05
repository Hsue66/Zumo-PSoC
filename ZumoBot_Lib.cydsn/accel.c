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
#include <math.h>
#include <accel_magnet.h>
#include <stdio.h>


void value_convert_accel(uint16 X_AXIS, uint16 Y_AXIS, uint16 Z_AXIS)
{
    double AccXangle, AccYangle;
   
    AccXangle = (float) (atan2(Y_AXIS, Z_AXIS)+M_PI) *180 / M_PI;
    AccYangle = (float) (atan2(Z_AXIS, X_AXIS)+M_PI) *180 / M_PI;
    
    printf("%7.3f %7.3f \r\n", AccXangle, AccYangle);
}


