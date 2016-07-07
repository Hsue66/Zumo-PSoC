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
#include <stdio.h>
#include <project.h>


volatile struct sensors_ {
    uint16_t ll;
    //uint16_t lc; // not used at the moment
    uint16_t lr;
    uint16_t rl;
    //uint16_t rc; // not used at the moment
    uint16_t rr;
} sensors;

volatile struct digital_sensor_value_ {         //digital value for recognizing black or white
    uint16_t ll;
    //uint16_t lc; // not used at the moment
    uint16_t lr;
    uint16_t rl;
    //uint16_t rc; // not used at the moment
    uint16_t rr;
} digital_sensor_value;

void Refelctance_Start();

void reflectance_period();

void reflectance_digital();

CY_ISR_PROTO(sensor_isr_handler);

///////////////////////////////
void ref_calibration(int dark);
int getDLL();
int getDLR();
int getDRL();
int getDRR();
/* [] END OF FILE */
