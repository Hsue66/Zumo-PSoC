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
#include <stdio.h>
#include <accel_magnet.h>

void heading(double X_AXIS, double Y_AXIS)          // defined as the angle between X axis and Y axis measured in a clockwise direction when viewing from the top of the device = -Z
{
    double heading;
    heading = 180 * atan2 (X_AXIS, Y_AXIS) / M_PI;
    
    if(heading < 0)
        heading += 360;
    printf("heading: %7.3f \r\n", heading);
    CyDelay(250);
}


//uint16 value_convert_magnet(uint8 AXIS_H, uint8 AXIS_L)

