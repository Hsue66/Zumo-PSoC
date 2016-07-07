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

/* [] END OF FILE */

#include <I2C_made.h>

void I2C_write(uint8 device_addr, uint8 Reg, uint8 value)             // Gyroscope: you need to change the value of PD of CTRL1 register(20h) to power on 
{
    uint8 write_buf[2] = {};
    write_buf[0] = Reg;                //register address that you want to write
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
    I2C_MasterWriteBuf(device_addr, write_buf, 1, I2C_MODE_COMPLETE_XFER);
    while((I2C_MasterStatus() & I2C_MSTAT_WR_CMPLT) == 0);
    CyDelay(50);
    I2C_MasterClearStatus();
    I2C_MasterReadBuf(device_addr, read_buf, 1, I2C_MODE_COMPLETE_XFER);
    while((I2C_MasterStatus() & I2C_MSTAT_RD_CMPLT) == 0);
   
    return read_buf[0];
}


