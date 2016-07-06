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

int rread(void);
void reflectance_period();
void reflectance_digital();
uint8 I2C_read(uint8 device_addr, uint8 Reg);
void I2C_write(uint8 device_addr, uint8 value);

//CY_ISR_PROTO(sensor_isr_handler);
CY_ISR_PROTO(ultra_isr_handler);

uint16_t i = 0;
uint16 distance = 0;


#define WHO_AM_I            0x0F   
#define GYRO_ADDR           0x6B
#define CTRL1_REG           0x20
#define OUT_X_AXIS_L        0x28
#define OUT_X_AXIS_H        0x29
#define OUT_Y_AXIS_L        0x2A
#define OUT_Y_AXIS_H        0x2B
#define OUT_Z_AXIS_L        0x2C
#define OUT_Z_AXIS_H        0x2D
#define ACCEL_ADDR      


int main()
{   
    
    I2C_Start();
    
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    UART_Start();
  
    //PWM_Start();
    //Timer_Start();

    
    printf("Start\r\n");
    
   
    int16 X_AXIS_L, X_AXIS_H;
    int16 Y_AXIS_L, Y_AXIS_H;
    int16 Z_AXIS_L, Z_AXIS_H;
    
    I2C_write(GYRO_ADDR, 0x1F);             //set gyroscope into active mode
    
    
    
    for(;;)
    {
        
        //deviceID = gyro_read(WHO_AM_I);
        //printf("deviceID : %d \n", deviceID);
        X_AXIS_L = I2C_read(GYRO_ADDR, OUT_X_AXIS_L);
        X_AXIS_H = I2C_read(GYRO_ADDR, OUT_X_AXIS_H);
        Y_AXIS_L = I2C_read(GYRO_ADDR, OUT_Y_AXIS_L);
        Y_AXIS_H = I2C_read(GYRO_ADDR, OUT_Y_AXIS_H);
        Z_AXIS_L = I2C_read(GYRO_ADDR, OUT_Z_AXIS_L);
        Z_AXIS_H = I2C_read(GYRO_ADDR, OUT_Z_AXIS_H);
        //printf("X_AXIS_L: %d, X_AXIS_H: %d, average: %d \r\n", X_AXIS_L, X_AXIS_H, (X_AXIS_H+X_AXIS_L)/2);
        //printf("Y_AXIS_L: %d, Y_AXIS_H: %d, average: %d \r\n", Y_AXIS_L, Y_AXIS_H, (Y_AXIS_H+Y_AXIS_L)/2);
        //printf("Z_AXIS_L: %d, Z_AXIS_H: %d, average: %d \r\n", Z_AXIS_L, Z_AXIS_H, (Z_AXIS_H+Z_AXIS_L)/2);
        printf("%d %d %d %d %d %d \r\n", X_AXIS_L, X_AXIS_H, Y_AXIS_L, Y_AXIS_H, Z_AXIS_L, Z_AXIS_H);
        CyDelay(500);
    }
    
}

void I2C_write(uint8 device_addr, uint8 value)             // Gyroscope: you need to change the value of PD of CTRL1 register(20h) to power on 
{
    uint8 write_buf[2] = {};
    write_buf[0] = CTRL1_REG;                //register address that you want to write
    write_buf[1] = value;                //value that you want to write to register
    
    I2C_MasterClearStatus();
    I2C_MasterWriteBuf(device_addr, write_buf, 2, I2C_MODE_COMPLETE_XFER);
    while(((I2C_MasterStatus()) & I2C_MSTAT_WR_CMPLT) == 0);
    
    return;
}

uint8 I2C_read(uint8 device_addr, uint8 Reg) 
{
   
    uint8 write_buf[1] = {};
    write_buf[0] = Reg;
    
    uint8 read_buf[1] = {};
    
    I2C_MasterClearStatus();
    CyDelay(100);
    I2C_MasterWriteBuf(device_addr, write_buf, 2, I2C_MODE_COMPLETE_XFER);
    while((I2C_MasterStatus() & I2C_MSTAT_WR_CMPLT) == 0);
    CyDelay(50);
    I2C_MasterClearStatus();
    I2C_MasterReadBuf(device_addr, read_buf, 1, I2C_MODE_COMPLETE_XFER);
    while((I2C_MasterStatus() & I2C_MSTAT_RD_CMPLT) == 0);
   
    return read_buf[0];
}

/*
CY_ISR(ultra_isr_handler)
{
    i = Timer_ReadCounter();
    distance = i / 58;
    printf("i = %d, distance = %d", i, distance);
}



void reflectance_period()
{
     printf("Period: %d %d %d %d \t", sensors.ll, sensors.lr, sensors.rl, sensors.rr); 
}

void reflectance_digital()
{
    //if the results of reflectance_period function is over 10000, set digital_sensor_value to 0, which means it's black
    if(sensors.ll > 10000)
        digital_sensor_value.ll  = 0;
    else
        digital_sensor_value.ll  = 1;
    if(sensors.lr > 10000)
        digital_sensor_value.lr  = 0;
    else
        digital_sensor_value.lr  = 1;
    if(sensors.rl > 10000)
        digital_sensor_value.rl  = 0;
    else
        digital_sensor_value.rl  = 1;
    if(sensors.rr > 10000)
        digital_sensor_value.rr  = 0;
    else
        digital_sensor_value.rr  = 1;
        
   //print out the results
   printf("LL LR RL RR : %d %d %d %d \t", digital_sensor_value.ll, digital_sensor_value.lr, digital_sensor_value.rl, digital_sensor_value.rr);  
}

*/

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
