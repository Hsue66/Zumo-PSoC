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
#include "Ambient.h"

double getLux(double CH0, double CH1)
{
	double result = 0;
    double ChannelRatio = (double)CH1/CH0;
    
    if((0<ChannelRatio) && (ChannelRatio <= 0.5))
        result = (0.0304 * CH0) - (0.062 * CH0 * pow(ChannelRatio, 1.4));
    else if((0.5<ChannelRatio)&&(ChannelRatio <= 0.61))
        result = (0.0224 * CH0) - (0.031 * CH1);
    else if((0.61<ChannelRatio)&&(ChannelRatio <= 0.80))
        result = (0.0128 * CH0) - (0.0153 * CH1);
    else if((0.80<ChannelRatio)&&(ChannelRatio <= 1.30))
        result = (0.00146 * CH0) - (0.00112 * CH1);
    else 
        result = 0;

    return result;
}

/* [] END OF FILE */
