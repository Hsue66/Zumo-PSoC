/* ======== ================================
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
#include "Motor.h"
#include "Ultra.h"
#include "nunchuk.h"
#include "Reflectance.h"
#include "I2C_made.h"
#include "gyro.h"
#include "accel_magnet.h"
#include <math.h>

int rread(void);

#define CH0_L           0x8C
#define CH0_H           0x8D
#define CH1_L           0x8E
#define CH1_H           0x8F

float getLux(float CH0, float CH1)
{
	float result = 0;
    float ChannelRatio = (float)CH1/CH0;
    
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


int main()
{
    CyGlobalIntEnable; 
    UART_Start();
    printf("Start\r\n");
   
    I2C_Start();
    
    uint16 value =0;
    
    I2C_write(0x29,0x80,0x00);
    
    value = I2C_read(0x29,0x80);
    printf("%x ",value);
    
    I2C_write(0x29,0x80,0x03);
    value = I2C_read(0x29,0x80);
    printf("%x\r\n",value);
        
    value = I2C_read(0x29,0x81);
    printf("%x\r\n",value);
    for(;;)
    {
        
        uint8 Data0Low,Data0High,Data1Low,Data1High;
        Data0Low = I2C_read(0x29,CH0_L);
        Data0High = I2C_read(0x29,CH0_H);
        Data1Low = I2C_read(0x29,CH1_L);
        Data1High = I2C_read(0x29,CH1_H);
        
        uint8 CH0, CH1;
        CH0 = convert_raw(Data0Low,Data0High);
        CH1 = convert_raw(Data1Low,Data1High);

   //     printf("%d %d %d %d\r\n",Data0Low,Data0High, Data1Low,Data1High);
   //     printf("%d %d\r\n",CH0,CH1);
   //        printf("%f\r\n",(float)CH1/CH0);
        
   
        
  //*
        float Ch0 = CH0;
        float Ch1 = CH1;
        
        float data = 0;
        data = getLux(Ch0,Ch1);
        printf("%f\r\n",data);
       //*/
        
        
    }
 
    
    /* //accelerometer//
    //--------------------------------------------------------------
    I2C_Start();
  
    uint8 X_L_A, X_H_A, Y_L_A, Y_H_A, Z_L_A, Z_H_A;
    int16 X_AXIS, Y_AXIS, Z_AXIS;
    
    I2C_write(ACCEL_MAG_ADDR, ACCEL_CTRL1_REG, 0x37);           // set accelerometer & magnetometer into active mode
    I2C_write(ACCEL_MAG_ADDR, ACCEL_CTRL7_REG, 0x22);
    
    
    for(;;)
    {
        //print out accelerometer output
        X_L_A = I2C_read(ACCEL_MAG_ADDR, OUT_X_L_A);
        X_H_A = I2C_read(ACCEL_MAG_ADDR, OUT_X_H_A);
        X_AXIS = convert_raw(X_L_A, X_H_A);
        
        Y_L_A = I2C_read(ACCEL_MAG_ADDR, OUT_Y_L_A);
        Y_H_A = I2C_read(ACCEL_MAG_ADDR, OUT_Y_H_A);
        Y_AXIS = convert_raw(Y_L_A, Y_H_A);
        
        Z_L_A = I2C_read(ACCEL_MAG_ADDR, OUT_Z_L_A);
        Z_H_A = I2C_read(ACCEL_MAG_ADDR, OUT_Z_H_A);
        Z_AXIS = convert_raw(Z_L_A, Z_H_A);
        
        printf("ACCEL: %d %d %d %d %d %d \r\n", X_L_A, X_H_A, Y_L_A, Y_H_A, Z_L_A, Z_H_A);
        value_convert_accel(X_AXIS, Y_AXIS, Z_AXIS);
        printf("\n");
        
        CyDelay(50);
    }
    ///---------------------------------------------------------- */
    
    /*  //nunchuk//
    ----------------------------------------------------
    nunchuk_start();
    nunchuk_init();
    
    for(;;)
    {    
        nunchuk_read();
    }
    //----------------------------------------------------*/
    
    
     /* //ultra//
    ----------------------------------------------------
    ultra_isr_StartEx(ultra_isr_handler);   // Ultra Sonic Interrupt
    Ultra_Start();                          // Ultra Sonic Start function
   
    for(;;)
    {       
        CyDelay(100); 
        Trig_Write(1);           // Trigger High
        CyDelayUs(10);              // 10 micro seconds for trigger input signals
        Trig_Write(0);           // Trigger Low
    }
    //----------------------------------------------------*/
    
    
    
    /* //reflectance//
    ----------------------------------------------------
    sensor_isr_StartEx(sensor_isr_handler);
    
    Refelctance_Start();

    IR_led_Write(1);
    for(;;)
    {
        reflectance_period();      //print out each period of reflectance sensors
        reflectance_digital();      //print out 0 or 1 according to results of reflectance period
        
        CyDelay(500);
    }
    ///----------------------------------------------------*/
   
    
     /* //motor//
    ----------------------------------------------------
    motor_Start();              // motor start

    motor_forward(50,2000);     // moving forward
    motor_turn(10,50,2000);     // turn
    motor_turn(50,10,2000);     // turn
    motor_backward(50,2000);    // movinb backward
       
    motor_Stop();               // motor stop
    
    for(;;)
    {

    }
    ///----------------------------------------------------*/
    
    
   /* //gyroscope//
     //-----------------------------------------------------
    I2C_Start();
  
    uint8 X_AXIS_L, X_AXIS_H, Y_AXIS_L, Y_AXIS_H, Z_AXIS_L, Z_AXIS_H;
    //uint8 X_L_M, X_H_M, Y_L_M, Y_H_M, Z_L_M, Z_H_M;
    //uint8 X_L_A, X_H_A, Y_L_A, Y_H_A, Z_L_A, Z_H_A;
    int16 X_AXIS, Y_AXIS, Z_AXIS;
    
    I2C_write(GYRO_ADDR, GYRO_CTRL1_REG, 0x0F);             // set gyroscope into active mode
    I2C_write(GYRO_ADDR, GYRO_CTRL4_REG, 0x30);             // set full scale selection to 2000dps    
    
    for(;;)
    {
        //print out gyroscope output
        X_AXIS_L = I2C_read(GYRO_ADDR, OUT_X_AXIS_L);
        X_AXIS_H = I2C_read(GYRO_ADDR, OUT_X_AXIS_H);
        X_AXIS = convert_raw(X_AXIS_H, X_AXIS_L);
        
        
        Y_AXIS_L = I2C_read(GYRO_ADDR, OUT_Y_AXIS_L);
        Y_AXIS_H = I2C_read(GYRO_ADDR, OUT_Y_AXIS_H);
        Y_AXIS = convert_raw(Y_AXIS_H, Y_AXIS_L);
        
        
        Z_AXIS_L = I2C_read(GYRO_ADDR, OUT_Z_AXIS_L);
        Z_AXIS_H = I2C_read(GYRO_ADDR, OUT_Z_AXIS_H);
        Z_AXIS = convert_raw(Z_AXIS_H, Z_AXIS_L);
        
        
        //printf("X_AXIS_L: %d, X_AXIS_H: %d, average: %d \r\n", X_AXIS_L, X_AXIS_H, (X_AXIS_H+X_AXIS_L)/2);
        //printf("Y_AXIS_L: %d, Y_AXIS_H: %d, average: %d \r\n", Y_AXIS_L, Y_AXIS_H, (Y_AXIS_H+Y_AXIS_L)/2);
        //printf("Z_AXIS_L: %d, Z_AXIS_H: %d, average: %d \r\n", Z_AXIS_L, Z_AXIS_H, (Z_AXIS_H+Z_AXIS_L)/2);
        
        printf("H L : %d %d %d %d %d %d \r\n", X_AXIS_L, X_AXIS_H, Y_AXIS_L, Y_AXIS_H, Z_AXIS_L, Z_AXIS_H);
        printf("%d %d %d \r\n", X_AXIS, Y_AXIS, Z_AXIS);
        //printf("%d %d %d \r\n", value_convert_gyro(X_AXIS), value_convert_gyro(Y_AXIS), value_convert_gyro(Z_AXIS));
        
       CyDelay(50);
    }
    ///-----------------------------------------------------------------*/ 
   
  
    
    /* //magnetometer//
     //--------------------------------------------------------------
    I2C_Start();
   
    uint8 X_L_M, X_H_M, Y_L_M, Y_H_M, Z_L_M, Z_H_M;
    int16 X_AXIS, Y_AXIS, Z_AXIS;
    
    I2C_write(GYRO_ADDR, GYRO_CTRL1_REG, 0x0F);             // set gyroscope into active mode
    I2C_write(GYRO_ADDR, GYRO_CTRL4_REG, 0x30);             // set full scale selection to 2000dps
    I2C_write(ACCEL_MAG_ADDR, ACCEL_CTRL1_REG, 0x37);           // set accelerometer & magnetometer into active mode
    I2C_write(ACCEL_MAG_ADDR, ACCEL_CTRL7_REG, 0x22);
    
    
    for(;;)
    {
        X_L_M = I2C_read(ACCEL_MAG_ADDR, OUT_X_L_M);
        X_H_M = I2C_read(ACCEL_MAG_ADDR, OUT_X_H_M);
        X_AXIS = convert_raw(X_L_M, X_H_M);
        
        Y_L_M = I2C_read(ACCEL_MAG_ADDR, OUT_Y_L_M);
        Y_H_M = I2C_read(ACCEL_MAG_ADDR, OUT_Y_H_M);
        Y_AXIS = convert_raw(Y_L_M, Y_H_M);
        
        Z_L_M = I2C_read(ACCEL_MAG_ADDR, OUT_Z_L_M);
        Z_H_M = I2C_read(ACCEL_MAG_ADDR, OUT_Z_H_M);
        Z_AXIS = convert_raw(Z_L_M, Z_H_M);
        
        heading(X_AXIS, Y_AXIS);
        printf("MAGNET: %d %d %d %d %d %d \r\n", X_L_M, X_H_M, Y_L_M, Y_H_M, Z_L_M, Z_H_M);
        printf("%d %d %d \r\n", X_AXIS,Y_AXIS, Z_AXIS);
        CyDelay(50);
          
    }
    ///----------------------------------------------------------*/
}

uint16 convert_raw(uint8 L, uint8 H)            // concatenation
{
    uint16 raw;
    raw = (int16)(L | H << 8);
    
    return raw;
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
