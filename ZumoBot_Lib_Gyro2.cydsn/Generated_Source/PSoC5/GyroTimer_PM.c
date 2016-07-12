/*******************************************************************************
* File Name: GyroTimer_PM.c
* Version 2.70
*
*  Description:
*     This file provides the power management source code to API for the
*     Timer.
*
*   Note:
*     None
*
*******************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#include "GyroTimer.h"

static GyroTimer_backupStruct GyroTimer_backup;


/*******************************************************************************
* Function Name: GyroTimer_SaveConfig
********************************************************************************
*
* Summary:
*     Save the current user configuration
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  GyroTimer_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void GyroTimer_SaveConfig(void) 
{
    #if (!GyroTimer_UsingFixedFunction)
        GyroTimer_backup.TimerUdb = GyroTimer_ReadCounter();
        GyroTimer_backup.InterruptMaskValue = GyroTimer_STATUS_MASK;
        #if (GyroTimer_UsingHWCaptureCounter)
            GyroTimer_backup.TimerCaptureCounter = GyroTimer_ReadCaptureCount();
        #endif /* Back Up capture counter register  */

        #if(!GyroTimer_UDB_CONTROL_REG_REMOVED)
            GyroTimer_backup.TimerControlRegister = GyroTimer_ReadControlRegister();
        #endif /* Backup the enable state of the Timer component */
    #endif /* Backup non retention registers in UDB implementation. All fixed function registers are retention */
}


/*******************************************************************************
* Function Name: GyroTimer_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration.
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  GyroTimer_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void GyroTimer_RestoreConfig(void) 
{   
    #if (!GyroTimer_UsingFixedFunction)

        GyroTimer_WriteCounter(GyroTimer_backup.TimerUdb);
        GyroTimer_STATUS_MASK =GyroTimer_backup.InterruptMaskValue;
        #if (GyroTimer_UsingHWCaptureCounter)
            GyroTimer_SetCaptureCount(GyroTimer_backup.TimerCaptureCounter);
        #endif /* Restore Capture counter register*/

        #if(!GyroTimer_UDB_CONTROL_REG_REMOVED)
            GyroTimer_WriteControlRegister(GyroTimer_backup.TimerControlRegister);
        #endif /* Restore the enable state of the Timer component */
    #endif /* Restore non retention registers in the UDB implementation only */
}


/*******************************************************************************
* Function Name: GyroTimer_Sleep
********************************************************************************
*
* Summary:
*     Stop and Save the user configuration
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  GyroTimer_backup.TimerEnableState:  Is modified depending on the
*  enable state of the block before entering sleep mode.
*
*******************************************************************************/
void GyroTimer_Sleep(void) 
{
    #if(!GyroTimer_UDB_CONTROL_REG_REMOVED)
        /* Save Counter's enable state */
        if(GyroTimer_CTRL_ENABLE == (GyroTimer_CONTROL & GyroTimer_CTRL_ENABLE))
        {
            /* Timer is enabled */
            GyroTimer_backup.TimerEnableState = 1u;
        }
        else
        {
            /* Timer is disabled */
            GyroTimer_backup.TimerEnableState = 0u;
        }
    #endif /* Back up enable state from the Timer control register */
    GyroTimer_Stop();
    GyroTimer_SaveConfig();
}


/*******************************************************************************
* Function Name: GyroTimer_Wakeup
********************************************************************************
*
* Summary:
*  Restores and enables the user configuration
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  GyroTimer_backup.enableState:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void GyroTimer_Wakeup(void) 
{
    GyroTimer_RestoreConfig();
    #if(!GyroTimer_UDB_CONTROL_REG_REMOVED)
        if(GyroTimer_backup.TimerEnableState == 1u)
        {     /* Enable Timer's operation */
                GyroTimer_Enable();
        } /* Do nothing if Timer was disabled before */
    #endif /* Remove this code section if Control register is removed */
}


/* [] END OF FILE */
