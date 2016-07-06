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

#define Gyro 0x6B
#define Gyro_R 0xD7
#define Gyro_W 0xD6
#define WHO_AM_I 0x0f
#define CTRL_REG1 0x20
#define OUT_X_L 0x28
#define OUT_X_H 0x29
#define OUT_Y_L 0x2A
#define OUT_Y_H 0x2B
#define OUT_Z_L 0x2C
#define OUT_Z_H 0x2D

#define Accel_R 0x3B
#define Accel_W 0x3A

int rread(void);
void reflectance_period();
void reflectance_digital();
void gyro_init();
uint8 gyro_ReadReg(uint8 Reg);
void gyro_WriteReg(uint8 Reg, uint8 value);
void gyro_read();
void accel_read();

uint8_t buffer[22];

CY_ISR_PROTO(sensor_isr_handler);

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

/* moving forward */
void motor_forward(uint8 speed,uint32 delay)
{
    PWM_WriteCompare1(speed); 
    PWM_WriteCompare2(speed); 
    CyDelay(delay);
}

/* moving left when l_speed < r_speed or moving right when l_speed > r_speed */
void motor_turn(uint8 l_speed, uint8 r_speed, uint32 delay)
{
    PWM_WriteCompare1(l_speed); 
    PWM_WriteCompare2(r_speed); 
    CyDelay(delay);
}

/* moving backward */
void motor_backward(uint8 speed,uint32 delay)
{
    MotorDirLeft_Write(1);
    MotorDirRight_Write(1);
    PWM_WriteCompare1(speed); 
    PWM_WriteCompare2(speed); 
    CyDelay(delay);
}


int main()
{
    sensor_isr_StartEx(sensor_isr_handler);
    
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    UART_Start();
    PWM_Start();
    
    I2C_Start();
    
    printf("\r\n\n");
    printf("Start\r\n");
    
    uint8_t g_buf[6];
    
    I2C_Start();
    
    IR_led_Write(1);
    
    
    
//    uint8 deviceID;
//    deviceID = gyro_ReadReg(WHO_AM_I);
//    printf("%d\r\n",deviceID);
    for(;;)
    {
        uint8 Ctrl_before,Ctrl_after;
    
    Ctrl_before = gyro_ReadReg(CTRL_REG1);
   
    gyro_WriteReg(CTRL_REG1,0x0f);
     CyDelay(1000);
 //   Ctrl_after = gyro_ReadReg(CTRL_REG1);
    
//   printf("%d %d\r\n",Ctrl_before,Ctrl_after);
    //    gyro_init();
   //    gyro_read();
    }
}

CY_ISR(sensor_isr_handler)
{
    uint32_t statusRL = Timer_RL_ReadStatusRegister();
    uint32_t statusRR = Timer_RR_ReadStatusRegister();
    uint32_t statusLL = Timer_LL_ReadStatusRegister();
    uint32_t statusLR = Timer_LR_ReadStatusRegister();

    if(statusRL & Timer_RL_STATUS_CAPTURE) {
        sensors.rl = Timer_RL_ReadPeriod() - Timer_RL_ReadCapture();
    }
    else {
        sensors.rl = Timer_RL_ReadPeriod();
    }
    
    if(statusRR & Timer_RR_STATUS_CAPTURE) {
        sensors.rr = Timer_RR_ReadPeriod() - Timer_RR_ReadCapture();
    }
    else {
        sensors.rr = Timer_RR_ReadPeriod();
    }
    
    if(statusLL & Timer_LL_STATUS_CAPTURE) {
        sensors.ll = Timer_LL_ReadPeriod() - Timer_LL_ReadCapture();
    }
    else {
        sensors.ll = Timer_LL_ReadPeriod();
    }
    
    if(statusLR & Timer_LR_STATUS_CAPTURE) {
        sensors.lr = Timer_LR_ReadPeriod() - Timer_LR_ReadCapture();
    }
    else {
        sensors.lr = Timer_LR_ReadPeriod();
    }
    
    RL_SetDriveMode(PIN_DM_STRONG);
    RL_Write(1);
    RR_SetDriveMode(PIN_DM_STRONG);
    RR_Write(1);
    LL_SetDriveMode(PIN_DM_STRONG);
    LL_Write(1);
    LR_SetDriveMode(PIN_DM_STRONG);
    LR_Write(1);
    CyDelayUs(10);
    RL_SetDriveMode(PIN_DM_DIG_HIZ);
    RR_SetDriveMode(PIN_DM_DIG_HIZ);
    LL_SetDriveMode(PIN_DM_DIG_HIZ);
    LR_SetDriveMode(PIN_DM_DIG_HIZ);
        
}

void reflectance_period()
{
     printf("%d %d %d %d \t", sensors.ll, sensors.lr, sensors.rl, sensors.rr); 
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
   printf("%d %d %d %d \t", digital_sensor_value.ll, digital_sensor_value.lr, digital_sensor_value.rl, digital_sensor_value.rr);  
}

void accel_read(void)
{
    uint8_t a_status;
    uint8_t a_buf[6];
    // send conversion command
    a_buf[0] = 0x00;
    
    I2C_MasterClearStatus();
    a_status = I2C_MasterWriteBuf(Accel_W, a_buf, 1, I2C_MODE_COMPLETE_XFER);
    while(((a_status = I2C_MasterStatus()) & I2C_MSTAT_WR_CMPLT) == 0);
    // wait for conversion to complete
    CyDelay(50); // todo: check if shorter wait would work
    // read values
    I2C_MasterClearStatus();
    a_status = I2C_MasterReadBuf(Accel_R, a_buf, 6, I2C_MODE_COMPLETE_XFER);
    while(((a_status = I2C_MasterStatus()) & I2C_MSTAT_RD_CMPLT) == 0);
    
    printf("%d %d %d %d %d %d\r\n", a_buf[0], a_buf[1], a_buf[2], a_buf[3], a_buf[4], a_buf[5] );
   
}    

void gyro_init(void)
{
    uint8_t buf[1] = { 0x20 };
    uint8_t status;
    I2C_MasterClearStatus();
    CyDelay(100);
    status = I2C_MasterWriteBuf(0x6B, buf, 1, I2C_MODE_COMPLETE_XFER);
    while(((status = I2C_MasterStatus()) & I2C_MSTAT_WR_CMPLT) == 0);
    CyDelay(100);
}    

uint8 gyro_ReadReg(uint8 Reg)
{
    uint8 Write_Buf[1] = {0};
    Write_Buf[0] = Reg;
    
    uint8 Read_Buf[1]={0};
    
    I2C_MasterWriteBuf(Gyro,(uint8 *)Write_Buf,1,I2C_MODE_NO_STOP);
    while((I2C_MasterStatus() & I2C_MSTAT_WR_CMPLT)==0){};

    I2C_MasterReadBuf(Gyro,(uint8 *)Read_Buf,1,I2C_MODE_REPEAT_START);
    while((I2C_MasterStatus() & I2C_MSTAT_RD_CMPLT)==0){};
    
    return Read_Buf[0];
}

void gyro_WriteReg(uint8 Reg, uint8 value)
{
    uint8 Write_Buf[2] = {0};
    Write_Buf[0] = Reg;
    Write_Buf[1] = value;
    
    I2C_MasterWriteBuf(Gyro,(uint8 *)Write_Buf,2,I2C_MODE_COMPLETE_XFER);
    while((I2C_MasterStatus() & I2C_MSTAT_WR_CMPLT)==0){};
    
    return;
}

void gyro_read()
{
    uint8_t reg = 0x28;
    uint8_t g_status;
    uint8_t g_buf[6];//={0};
    // send conversion command
    
    I2C_MasterClearStatus();
    
    g_status = I2C_MasterWriteBuf(0x6B, &reg, 1, I2C_MODE_COMPLETE_XFER);
    
    while(((g_status = I2C_MasterStatus()) & I2C_MSTAT_WR_CMPLT) == 0);
    // wait for conversion to complete
    CyDelay(50); // todo: check if shorter wait would work
    // read values
    I2C_MasterClearStatus();
    g_status = I2C_MasterReadBuf(0x6B, g_buf, 1, I2C_MODE_COMPLETE_XFER);
    while(((g_status = I2C_MasterStatus()) & I2C_MSTAT_RD_CMPLT) == 0);
    
    printf("%d %d %d %d %d %d\r\n", g_buf[0], g_buf[1], g_buf[2], g_buf[3], g_buf[4], g_buf[5] );
    /*   I2C_Wakeup();   
   
    uint8_t i=0;
    
    I2C_MasterClearStatus();
    printf("0\r\n");
    uint8_t g_status;
    g_status = I2C_MasterSendStart(0x6B, I2C_WRITE_XFER_MODE);
//    printf("%d, %d, %d, %d, %d, %d, %d\r\n",g_status,I2C_MSTR_NO_ERROR,I2C_MSTR_BUS_BUSY,I2C_MSTR_NOT_READY,I2C_MSTR_ERR_LB_NAK,I2C_MSTR_ERR_ARB_LOST,I2C_MSTR_ERR_ABORT_START_GEN);
//    printf("1\r\n");
    g_status=I2C_MasterWriteByte(1);
    I2C_MasterSendStop();
    printf("2\r\n");
    */
//    uint8_t g_buf[0];//={0};
//    
//    g_buf[0] = 0x0f;
//    g_status = I2C_MasterWriteBuf(Gyro_R, g_buf , 1, I2C_MODE_COMPLETE_XFER);
//    while(((g_status = I2C_MasterStatus()) & I2C_MSTAT_WR_CMPLT) == 0);
//    printf("2-2\r\n");
//    I2C_MasterSendRestart(devAddr, I2C_READ_XFER_MODE);
//    printf("3\r\n");
//   while (i++ < (length-1)) {
//		*value++ = I2C_MasterReadByte(I2C_ACK_DATA);
//        printf("%d",i);
//	}
//	*value = I2C_MasterReadByte(I2C_NAK_DATA);
//	I2C_MasterSendStop();	
//    
//    /*
//     uint8_t g_status;
//    uint8_t g_buf[6];//={0};
//    // send conversion command
//    
//    g_buf[0] = 0x00;
//    
//    I2C_MasterClearStatus();
//    g_status = I2C_MasterWriteBuf(Gyro_W, g_buf, 1, I2C_MODE_COMPLETE_XFER);
//    
//    while(((g_status = I2C_MasterStatus()) & I2C_MSTAT_WR_CMPLT) == 0);
//    // wait for conversion to complete
//    CyDelay(50); // todo: check if shorter wait would work
//    // read values
//    I2C_MasterClearStatus();
//    g_status = I2C_MasterReadBuf(Gyro_R, g_buf, 6, I2C_MODE_COMPLETE_XFER);
//    while(((g_status = I2C_MasterStatus()) & I2C_MSTAT_RD_CMPLT) == 0);
//    
//    printf("%d %d %d %d %d %d\r\n", g_buf[0], g_buf[1], g_buf[2], g_buf[3], g_buf[4], g_buf[5] );
//   */
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
