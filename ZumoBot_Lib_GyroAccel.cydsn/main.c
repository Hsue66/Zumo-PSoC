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
#include "Motor.h"
#include "Ultra.h"
#include "nunchuk.h"
#include "Reflectance.h"
#include "I2C_made.h"
#include "gyro.h"
#include "accel_magnet.h"
#include "IR.h"

#include <time.h>

int rread(void);


uint16 value_convert_gyro(uint16 raw)            // ****THING TO DO : converting to angle!!!****
{
    float rate_result;
    float angle_result;
    float G_GAIN = 0.07;
    
  
    rate_result = (float)raw * G_GAIN;          // degrees per second for Gyroscope
    angle_result += rate_result * 0.02;
    
    return angle_result;
    //return rate_result;
}

int main()
{
    CyGlobalIntEnable; 
    UART_Start();
    printf("Start\r\n");
    
    /* //gyroscope & accelerometer combine//
    //-----------------------------------------------------*/
    I2C_Start();
  
    uint8 X_L_G, X_H_G, Y_L_G, Y_H_G, Z_L_G, Z_H_G;
    uint16 X_G, Y_G, Z_G;
    
    uint8 X_L_A, X_H_A, Y_L_A, Y_H_A, Z_L_A, Z_H_A;
    uint16 X_A, Y_A, Z_A;
    
    double Gyro_X=0,Gyro_Y=0,Gyro_Z=0;
    double Accel_X=0,Accel_Y=0;
    double angleX=0, angleY=0;
    
    
    I2C_write(GYRO_ADDR, GYRO_CTRL1_REG, 0x0F);             // set gyroscope into active mode
    I2C_write(GYRO_ADDR, GYRO_CTRL4_REG, 0x30);             // set full scale selection to 2000dps    
    
    I2C_write(ACCEL_MAG_ADDR, ACCEL_CTRL1_REG, 0x37);           // set accelerometer & magnetometer into active mode
    I2C_write(ACCEL_MAG_ADDR, ACCEL_CTRL7_REG, 0x20);
    
    for(;;)
    {
        //print out gyroscope output
        
        X_L_G = I2C_read(GYRO_ADDR, OUT_X_AXIS_L);
        X_H_G = I2C_read(GYRO_ADDR, OUT_X_AXIS_H);
        Y_L_G = I2C_read(GYRO_ADDR, OUT_Y_AXIS_L);
        Y_H_G = I2C_read(GYRO_ADDR, OUT_Y_AXIS_H);
        Z_L_G = I2C_read(GYRO_ADDR, OUT_Z_AXIS_L);
        Z_H_G = I2C_read(GYRO_ADDR, OUT_Z_AXIS_H);
        
        X_G = convert_raw( X_L_G, X_H_G );
        Y_G = convert_raw( Y_L_G, Y_H_G );
        Z_G = convert_raw(Z_L_G, Z_H_G );
        
        X_L_A = I2C_read(ACCEL_MAG_ADDR, OUT_X_L_A);
        X_H_A = I2C_read(ACCEL_MAG_ADDR, OUT_X_H_A);
        Y_L_A = I2C_read(ACCEL_MAG_ADDR, OUT_Y_L_A);
        Y_H_A = I2C_read(ACCEL_MAG_ADDR, OUT_Y_H_A);
        Z_L_A = I2C_read(ACCEL_MAG_ADDR, OUT_Z_L_A);
        Z_H_A = I2C_read(ACCEL_MAG_ADDR, OUT_Z_H_A);
        
        X_A = convert_raw(X_L_A, X_H_A);
        Y_A = convert_raw(Y_L_A, Y_H_A);
        Z_A = convert_raw(Z_L_A, Z_H_A);
        
        //printf("H L : %d %d %d %d %d %d \r\n", X_AXIS_L, X_AXIS_H, Y_AXIS_L, Y_AXIS_H, Z_AXIS_L, Z_AXIS_H);
      //  printf("%d %d %d \r\n", X_AXIS, Y_AXIS, Z_AXIS);
    //    printf("%d %d %d \r\n", value_convert_gyro(X_G), value_convert_gyro(Y_G), value_convert_gyro(Z_G));    
        
        Gyro_X = value_convert_gyro(X_G);
        Gyro_Y = value_convert_gyro(Y_G);
        Gyro_Z = value_convert_gyro(Z_G);
        
        //value_convert_accel(X_A, Y_A, Z_A);
        Accel_X = value_convert_accelX(Y_A, Z_A);
        Accel_Y = value_convert_accelX(X_A, Z_A);
        
        angleX = 0.98*(Gyro_X) + (1-0.98)*Accel_X;
        angleY = 0.98*(Gyro_Y) + (1-0.98)*Accel_Y;
        
        printf("%f, %f\r\n",angleX,angleY);
        
       CyDelay(20);
    
    }
    //-----------------------------------------------------*/
    
      /* //gyroscope//
     //-----------------------------------------------------
    I2C_Start();
  
    uint8 X_AXIS_L, X_AXIS_H, Y_AXIS_L, Y_AXIS_H, Z_AXIS_L, Z_AXIS_H;
    int16 X_AXIS, Y_AXIS, Z_AXIS;
    
    I2C_write(GYRO_ADDR, GYRO_CTRL1_REG, 0x0F);             // set gyroscope into active mode
    I2C_write(GYRO_ADDR, GYRO_CTRL4_REG, 0x30);             // set full scale selection to 2000dps    
    
    for(;;)
    {
        //print out gyroscope output
        
        X_AXIS_L = I2C_read(GYRO_ADDR, OUT_X_AXIS_L);
        X_AXIS_H = I2C_read(GYRO_ADDR, OUT_X_AXIS_H);
        Y_AXIS_L = I2C_read(GYRO_ADDR, OUT_Y_AXIS_L);
        Y_AXIS_H = I2C_read(GYRO_ADDR, OUT_Y_AXIS_H);
        Z_AXIS_L = I2C_read(GYRO_ADDR, OUT_Z_AXIS_L);
        Z_AXIS_H = I2C_read(GYRO_ADDR, OUT_Z_AXIS_H);
        
        X_AXIS = convert_raw(X_AXIS_H, X_AXIS_L);
        Y_AXIS = convert_raw(Y_AXIS_H, Y_AXIS_L);
        Z_AXIS = convert_raw(Z_AXIS_H, Z_AXIS_L);
        
        
        //printf("H L : %d %d %d %d %d %d \r\n", X_AXIS_L, X_AXIS_H, Y_AXIS_L, Y_AXIS_H, Z_AXIS_L, Z_AXIS_H);
      //  printf("%d %d %d \r\n", X_AXIS, Y_AXIS, Z_AXIS);
        printf("%d %d %d \r\n", value_convert_gyro(X_AXIS), value_convert_gyro(Y_AXIS), value_convert_gyro(Z_AXIS));
        
       CyDelay(20);
    }
    ///-----------------------------------------------------------------*/ 
   
    /* //accelerometer//
    //--------------------------------------------------------------
    I2C_Start();
  
    uint8 X_L_A, X_H_A, Y_L_A, Y_H_A, Z_L_A, Z_H_A;
    int16 X_AXIS, Y_AXIS, Z_AXIS;
    
    I2C_write(ACCEL_MAG_ADDR, ACCEL_CTRL1_REG, 0x37);           // set accelerometer & magnetometer into active mode
    I2C_write(ACCEL_MAG_ADDR, ACCEL_CTRL7_REG, 0x20);
    
    
    for(;;)
    {
        //print out accelerometer output
        X_L_A = I2C_read(ACCEL_MAG_ADDR, OUT_X_L_A);
        X_H_A = I2C_read(ACCEL_MAG_ADDR, OUT_X_H_A);
        Y_L_A = I2C_read(ACCEL_MAG_ADDR, OUT_Y_L_A);
        Y_H_A = I2C_read(ACCEL_MAG_ADDR, OUT_Y_H_A);
        Z_L_A = I2C_read(ACCEL_MAG_ADDR, OUT_Z_L_A);
        Z_H_A = I2C_read(ACCEL_MAG_ADDR, OUT_Z_H_A);
        
        X_AXIS = convert_raw(X_L_A, X_H_A);
        Y_AXIS = convert_raw(Y_L_A, Y_H_A);
        Z_AXIS = convert_raw(Z_L_A, Z_H_A);
        
   //     printf("ACCEL: %d %d %d %d %d %d \r\n", X_L_A, X_H_A, Y_L_A, Y_H_A, Z_L_A, Z_H_A);
        value_convert_accel(X_AXIS, Y_AXIS, Z_AXIS);
        printf("\n");
        
        CyDelay(50);
    }
    ///---------------------------------------------------------- */
    
     /*  //IR receiver//
    ----------------------------------------------------
    unsigned int IR_val; 
    
    for(;;)
    {
       IR_val = get_IR();
       printf("%x\r\n\n",IR_val);
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