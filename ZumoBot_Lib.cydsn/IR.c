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
#include "IR.h"

void wait_going_up()
{
    while(!IR_receiver_Read()==1);
}    

void wait_going_down()
{
    while(IR_receiver_Read()==1);
}
 
int count_downtime()
{
    int cnt =0;
    while(!IR_receiver_Read()==1)
        cnt++;
    
    return cnt;
}

int count_uptime()
{
    int cnt =0;
    while(IR_receiver_Read()==1)
        cnt++;
    
    return cnt;
}



int get_IR()
{
    uint8 bit[33]={0};
    unsigned int val=0;
    uint t1, t2, i, j;
    
    wait_going_down();
    printf("START \r\n");
    
    for(i = 0; i < 33; i++)
    {
        int t1, t2;
        t1 = 0;
        t2 = 0;
        
        t1 = count_downtime();
        t2 = count_uptime();
        
        if(t2 > t1)
            bit[i]=1;
        else
            bit[i]=0; 
    } 
   
    /*
    int cnt;
    for(cnt=0; cnt<33; cnt++)
    {
        printf("%d ",bit[cnt]);
    }
    printf("\r\n\n");
    */
   
    for( i=0; i<32; i++)
        val += bit[i+1]*pow(2,i);
    
    return val;
}

/* [] END OF FILE */
