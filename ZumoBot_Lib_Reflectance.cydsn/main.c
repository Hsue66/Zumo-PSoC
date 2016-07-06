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
#include "Reflectance.h"

int rread(void);



int main()
{
    sensor_isr_StartEx(sensor_isr_handler);
    
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    UART_Start();
    Refelctance_Start();
    PWM_Start();

    printf("Start\r\n");
    IR_led_Write(1);
    for(;;)
    {
        reflectance_period();      //print out each period of reflectance sensors
        reflectance_digital();      //print out 0 or 1 according to results of reflectance period
        
        CyDelay(500);
    }
}

void nunchuk_init(void)
{
    uint8_t buf[2] = { 0x40, 0x00 };
    uint8_t status;
    I2C_2_MasterClearStatus();
    CyDelay(100);
    status = I2C_2_MasterWriteBuf(0x52, buf, 2, I2C_2_MODE_COMPLETE_XFER);
    while(((status = I2C_2_MasterStatus()) & I2C_2_MSTAT_WR_CMPLT) == 0);
    CyDelay(100);
}    

void nunchuk_read(void)
{
    uint8_t status;
    uint8_t buf[6];
    // send conversion command
    buf[0] = 0x00;
    I2C_2_MasterClearStatus();
    status = I2C_2_MasterWriteBuf(0x52, buf, 1, I2C_2_MODE_COMPLETE_XFER);
    while(((status = I2C_2_MasterStatus()) & I2C_2_MSTAT_WR_CMPLT) == 0);
    // wait for conversion to complete
    CyDelay(50); // todo: check if shorter wait would work
    // read values
    I2C_2_MasterClearStatus();
    status = I2C_2_MasterReadBuf(0x52, buf, 6, I2C_2_MODE_COMPLETE_XFER);
    while(((status = I2C_2_MasterStatus()) & I2C_2_MSTAT_RD_CMPLT) == 0);
    
    int i;
    // decrypt data
    for(i = 0; i < 6; i++) buf[i] = (buf[i] ^ 0x17) + 0x17;
    // accelerometer values
    /*
    int x,y,z;
    x = (buf[2] << 2) | ((buf[5] >> 2) & 0x03);
    y = (buf[3] << 2) | ((buf[5] >> 4) & 0x03);
    z = (buf[4] << 2) | ((buf[5] >> 6) & 0x03);
    // buf[0] = JoyX, buf[1] = JoyY
    //printf("%3d %3d, %d, %d, %4d %4d %4d\r\n", buf[0], buf[1], (buf[5] >> 1) & 1, buf[5] & 1, x, y, z);
    */
}    

#if 0
int rread(void)
{
    SC0_SetDriveMode(PIN_DM_STRONG);
    SC0_Write(1);
    CyDelayUs(10);
    SC0_SetDriveMode(PIN_DM_DIG_HIZ);
    Timer_1_Start();
    uint16_t start = Timer_1_ReadCounter();
    uint16_t end = 0;
    while(!(Timer_1_ReadStatusRegister() & Timer_1_STATUS_TC)) {
        if(SC0_Read() == 0 && end == 0) {
            end = Timer_1_ReadCounter();
        }
    }
    Timer_1_Stop();
    
    return (start - end);
}
#endif

int _write(int file, char *ptr, int len)
{
    (void) file;
	int n;
	for(n = 0; n < len; n++) {
		UART_PutChar(*ptr++);
	}
	return len;
}

/* [] END OF FILE */
