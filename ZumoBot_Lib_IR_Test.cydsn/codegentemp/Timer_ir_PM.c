/*******************************************************************************
* File Name: Timer_ir_PM.c
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

#include "Timer_ir.h"

static Timer_ir_backupStruct Timer_ir_backup;


/*******************************************************************************
* Function Name: Timer_ir_SaveConfig
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
*  Timer_ir_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void Timer_ir_SaveConfig(void) 
{
    #if (!Timer_ir_UsingFixedFunction)
        Timer_ir_backup.TimerUdb = Timer_ir_ReadCounter();
        Timer_ir_backup.InterruptMaskValue = Timer_ir_STATUS_MASK;
        #if (Timer_ir_UsingHWCaptureCounter)
            Timer_ir_backup.TimerCaptureCounter = Timer_ir_ReadCaptureCount();
        #endif /* Back Up capture counter register  */

        #if(!Timer_ir_UDB_CONTROL_REG_REMOVED)
            Timer_ir_backup.TimerControlRegister = Timer_ir_ReadControlRegister();
        #endif /* Backup the enable state of the Timer component */
    #endif /* Backup non retention registers in UDB implementation. All fixed function registers are retention */
}


/*******************************************************************************
* Function Name: Timer_ir_RestoreConfig
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
*  Timer_ir_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void Timer_ir_RestoreConfig(void) 
{   
    #if (!Timer_ir_UsingFixedFunction)

        Timer_ir_WriteCounter(Timer_ir_backup.TimerUdb);
        Timer_ir_STATUS_MASK =Timer_ir_backup.InterruptMaskValue;
        #if (Timer_ir_UsingHWCaptureCounter)
            Timer_ir_SetCaptureCount(Timer_ir_backup.TimerCaptureCounter);
        #endif /* Restore Capture counter register*/

        #if(!Timer_ir_UDB_CONTROL_REG_REMOVED)
            Timer_ir_WriteControlRegister(Timer_ir_backup.TimerControlRegister);
        #endif /* Restore the enable state of the Timer component */
    #endif /* Restore non retention registers in the UDB implementation only */
}


/*******************************************************************************
* Function Name: Timer_ir_Sleep
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
*  Timer_ir_backup.TimerEnableState:  Is modified depending on the
*  enable state of the block before entering sleep mode.
*
*******************************************************************************/
void Timer_ir_Sleep(void) 
{
    #if(!Timer_ir_UDB_CONTROL_REG_REMOVED)
        /* Save Counter's enable state */
        if(Timer_ir_CTRL_ENABLE == (Timer_ir_CONTROL & Timer_ir_CTRL_ENABLE))
        {
            /* Timer is enabled */
            Timer_ir_backup.TimerEnableState = 1u;
        }
        else
        {
            /* Timer is disabled */
            Timer_ir_backup.TimerEnableState = 0u;
        }
    #endif /* Back up enable state from the Timer control register */
    Timer_ir_Stop();
    Timer_ir_SaveConfig();
}


/*******************************************************************************
* Function Name: Timer_ir_Wakeup
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
*  Timer_ir_backup.enableState:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void Timer_ir_Wakeup(void) 
{
    Timer_ir_RestoreConfig();
    #if(!Timer_ir_UDB_CONTROL_REG_REMOVED)
        if(Timer_ir_backup.TimerEnableState == 1u)
        {     /* Enable Timer's operation */
                Timer_ir_Enable();
        } /* Do nothing if Timer was disabled before */
    #endif /* Remove this code section if Control register is removed */
}


/* [] END OF FILE */
