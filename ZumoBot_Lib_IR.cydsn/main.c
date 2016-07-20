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

int rread(void);


#define START_BIT_TIME  100000
#define ONE_TIME        59000    

void wait_going_up(void)
{
    while(!IR_receiver_Read()==1);
}    
void wait_going_down(void)
{
    while(IR_receiver_Read()==1);
}

uint count_down_time(void)
{
    Timer_ir_WriteCounter(90000);
    Timer_ir_Start();
    wait_going_up();        
    Timer_ir_Stop();
    return(Timer_ir_ReadCounter());
}

uint count_up_time(void)
{
    Timer_ir_WriteCounter(90000);
    Timer_ir_Start();
    wait_going_down();      
    Timer_ir_Stop();
    return(Timer_ir_ReadCounter());
}


uint32 get_IR(void)
{
    int bit[32]={0};
    uint32 val;
    uint t1, t2, i, j;
    t1 = 0;
    t2 = 0; 
    
    wait_going_down();
  
    t1 = count_down_time();
    t2 = count_up_time();
    
    printf("%d %d %d\r\n",t1,t2,t1+t2);
 /*   if((t1 + t2) > START_BIT_TIME)
    {
        printf("START \r\n");
        
        for(i = 0; i < 32; i++)
        {
             wait_going_up();
            t2 = count_up_time();
            if(t2 > ONE_TIME)
            {
                bit[i]=1;
            }
            else
                bit[i]=0; 
        } 
        
        int cnt;
        for(cnt=0; cnt<32; cnt++)
            printf("%d ",bit[cnt]);
        printf("\r\n\n");
    } // end if START bit identification
  */  
    
    return val;
}



int main()
{
    CyGlobalIntEnable; 
    UART_Start();
    printf("Start\r\n");
    
    uint32 IR_val;
    
    for(;;)
    {
        IR_val = get_IR();
     
/*
        Timer_ir_Start();
         printf("B %d\r\n",(int)(Timer_ir_ReadPeriod()));
        CyDelay(13);
        Timer_ir_Stop();
        printf("A %d\r\n",(unsigned int)(Timer_ir_ReadCounter()));
       // printf("%d\r\n",(before-after));
        Timer_ir_WriteCounter(0xFFFF);
        
        //*/
        
        //  printf("%d %d\r\n",IR_receiver_Read(),Timer_ir_ReadCapture());
        /*
        if(IR_receiver_Read()==1)
        {
            Timer_ir_Enable();
        }
        else
        {
            Timer_ir_Stop();
            int time = Timer_ir_ReadPeriod()-Timer_ir_ReadCapture();
            printf("%d\r\n",time);
        }
      */
        
        //  printf("%d\r\n",time);
        /*
        int temp =IR_receiver_Read();
        if(before ==1 && temp==0)
        {
            printf("wow\r\n");
            Timer_ir_Start();
            Timer_ir_Enable();
        }
        before = temp;
        printf("%d \r\n", IR_receiver_Read());
        */
    }
    
       /*  //nunchuk//
    ----------------------------------------------------
    nunchuk_start();
    nunchuk_init();
    
    for(;;)
    {    
        nunchuk_read();
    }
    //----------------------------------------------------*/
    
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
