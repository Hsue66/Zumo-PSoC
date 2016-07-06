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
#include <Timer.h>

int rread(void);
void reflectance_period();
void reflectance_digital();

//CY_ISR_PROTO(sensor_isr_handler);
CY_ISR_PROTO(ultra_isr_handler);

uint16_t i = 0;
float distance = 0;
float cent = 0;

#define MAX_VALUE   65535

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

int main()
{
    
     ultra_isr_StartEx(ultra_isr_handler);
    CyGlobalIntEnable; /* Enable global interrupts. */
    
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    UART_Start();
    Timer_Start();
    //PWM_Start();

    printf("Start\r\n");
        
    for(;;)
    {
        /* Place your application code here. */
   
        CyDelay(100); 
        Trigger_Write(1);           // Trigger High
        CyDelayUs(10);              // 10 micro seconds for trigger input signals
        Trigger_Write(0);           // Trigger Low
   
    }
}

CY_ISR(ultra_isr_handler)
{
    Timer_Stop();
    
    Timer_ReadStatusRegister();
    
    if(!Echo_Read()) {
        i = Timer_ReadCounter();            // Read counter value of Timer
        distance = (float)i / 58;           // us / 58 = centimeters
        cent = 5 * (distance - 11) / 3;     // calibration for this Timer(650 kHz)
        printf("i = %6d, distance = %4d                \r\n", i, (int)cent);
        Timer_WriteCounter(0xFFFF);         // Counter initialization
    }
    else {
        
         Timer_WriteCounter(0xFFFF);
    }
    Timer_Start();
}

/*
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
