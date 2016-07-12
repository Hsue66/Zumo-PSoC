/*******************************************************************************
* File Name: GyroTimer.h
* Version 2.70
*
*  Description:
*     Contains the function prototypes and constants available to the timer
*     user module.
*
*   Note:
*     None
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#if !defined(CY_Timer_v2_60_GyroTimer_H)
#define CY_Timer_v2_60_GyroTimer_H

#include "cytypes.h"
#include "cyfitter.h"
#include "CyLib.h" /* For CyEnterCriticalSection() and CyExitCriticalSection() functions */

extern uint8 GyroTimer_initVar;

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component Timer_v2_70 requires cy_boot v3.0 or later
#endif /* (CY_ PSOC5LP) */


/**************************************
*           Parameter Defaults
**************************************/

#define GyroTimer_Resolution                 16u
#define GyroTimer_UsingFixedFunction         1u
#define GyroTimer_UsingHWCaptureCounter      0u
#define GyroTimer_SoftwareCaptureMode        0u
#define GyroTimer_SoftwareTriggerMode        0u
#define GyroTimer_UsingHWEnable              0u
#define GyroTimer_EnableTriggerMode          0u
#define GyroTimer_InterruptOnCaptureCount    0u
#define GyroTimer_RunModeUsed                0u
#define GyroTimer_ControlRegRemoved          0u

#if defined(GyroTimer_TimerUDB_sCTRLReg_SyncCtl_ctrlreg__CONTROL_REG)
    #define GyroTimer_UDB_CONTROL_REG_REMOVED            (0u)
#elif  (GyroTimer_UsingFixedFunction)
    #define GyroTimer_UDB_CONTROL_REG_REMOVED            (0u)
#else 
    #define GyroTimer_UDB_CONTROL_REG_REMOVED            (1u)
#endif /* End GyroTimer_TimerUDB_sCTRLReg_SyncCtl_ctrlreg__CONTROL_REG */


/***************************************
*       Type defines
***************************************/


/**************************************************************************
 * Sleep Wakeup Backup structure for Timer Component
 *************************************************************************/
typedef struct
{
    uint8 TimerEnableState;
    #if(!GyroTimer_UsingFixedFunction)

        uint16 TimerUdb;
        uint8 InterruptMaskValue;
        #if (GyroTimer_UsingHWCaptureCounter)
            uint8 TimerCaptureCounter;
        #endif /* variable declarations for backing up non retention registers in CY_UDB_V1 */

        #if (!GyroTimer_UDB_CONTROL_REG_REMOVED)
            uint8 TimerControlRegister;
        #endif /* variable declaration for backing up enable state of the Timer */
    #endif /* define backup variables only for UDB implementation. Fixed function registers are all retention */

}GyroTimer_backupStruct;


/***************************************
*       Function Prototypes
***************************************/

void    GyroTimer_Start(void) ;
void    GyroTimer_Stop(void) ;

void    GyroTimer_SetInterruptMode(uint8 interruptMode) ;
uint8   GyroTimer_ReadStatusRegister(void) ;
/* Deprecated function. Do not use this in future. Retained for backward compatibility */
#define GyroTimer_GetInterruptSource() GyroTimer_ReadStatusRegister()

#if(!GyroTimer_UDB_CONTROL_REG_REMOVED)
    uint8   GyroTimer_ReadControlRegister(void) ;
    void    GyroTimer_WriteControlRegister(uint8 control) ;
#endif /* (!GyroTimer_UDB_CONTROL_REG_REMOVED) */

uint16  GyroTimer_ReadPeriod(void) ;
void    GyroTimer_WritePeriod(uint16 period) ;
uint16  GyroTimer_ReadCounter(void) ;
void    GyroTimer_WriteCounter(uint16 counter) ;
uint16  GyroTimer_ReadCapture(void) ;
void    GyroTimer_SoftwareCapture(void) ;

#if(!GyroTimer_UsingFixedFunction) /* UDB Prototypes */
    #if (GyroTimer_SoftwareCaptureMode)
        void    GyroTimer_SetCaptureMode(uint8 captureMode) ;
    #endif /* (!GyroTimer_UsingFixedFunction) */

    #if (GyroTimer_SoftwareTriggerMode)
        void    GyroTimer_SetTriggerMode(uint8 triggerMode) ;
    #endif /* (GyroTimer_SoftwareTriggerMode) */

    #if (GyroTimer_EnableTriggerMode)
        void    GyroTimer_EnableTrigger(void) ;
        void    GyroTimer_DisableTrigger(void) ;
    #endif /* (GyroTimer_EnableTriggerMode) */


    #if(GyroTimer_InterruptOnCaptureCount)
        void    GyroTimer_SetInterruptCount(uint8 interruptCount) ;
    #endif /* (GyroTimer_InterruptOnCaptureCount) */

    #if (GyroTimer_UsingHWCaptureCounter)
        void    GyroTimer_SetCaptureCount(uint8 captureCount) ;
        uint8   GyroTimer_ReadCaptureCount(void) ;
    #endif /* (GyroTimer_UsingHWCaptureCounter) */

    void GyroTimer_ClearFIFO(void) ;
#endif /* UDB Prototypes */

/* Sleep Retention APIs */
void GyroTimer_Init(void)          ;
void GyroTimer_Enable(void)        ;
void GyroTimer_SaveConfig(void)    ;
void GyroTimer_RestoreConfig(void) ;
void GyroTimer_Sleep(void)         ;
void GyroTimer_Wakeup(void)        ;


/***************************************
*   Enumerated Types and Parameters
***************************************/

/* Enumerated Type B_Timer__CaptureModes, Used in Capture Mode */
#define GyroTimer__B_TIMER__CM_NONE 0
#define GyroTimer__B_TIMER__CM_RISINGEDGE 1
#define GyroTimer__B_TIMER__CM_FALLINGEDGE 2
#define GyroTimer__B_TIMER__CM_EITHEREDGE 3
#define GyroTimer__B_TIMER__CM_SOFTWARE 4



/* Enumerated Type B_Timer__TriggerModes, Used in Trigger Mode */
#define GyroTimer__B_TIMER__TM_NONE 0x00u
#define GyroTimer__B_TIMER__TM_RISINGEDGE 0x04u
#define GyroTimer__B_TIMER__TM_FALLINGEDGE 0x08u
#define GyroTimer__B_TIMER__TM_EITHEREDGE 0x0Cu
#define GyroTimer__B_TIMER__TM_SOFTWARE 0x10u


/***************************************
*    Initialial Parameter Constants
***************************************/

#define GyroTimer_INIT_PERIOD             65535u
#define GyroTimer_INIT_CAPTURE_MODE       ((uint8)((uint8)1u << GyroTimer_CTRL_CAP_MODE_SHIFT))
#define GyroTimer_INIT_TRIGGER_MODE       ((uint8)((uint8)0u << GyroTimer_CTRL_TRIG_MODE_SHIFT))
#if (GyroTimer_UsingFixedFunction)
    #define GyroTimer_INIT_INTERRUPT_MODE (((uint8)((uint8)0u << GyroTimer_STATUS_TC_INT_MASK_SHIFT)) | \
                                                  ((uint8)((uint8)0 << GyroTimer_STATUS_CAPTURE_INT_MASK_SHIFT)))
#else
    #define GyroTimer_INIT_INTERRUPT_MODE (((uint8)((uint8)0u << GyroTimer_STATUS_TC_INT_MASK_SHIFT)) | \
                                                 ((uint8)((uint8)0 << GyroTimer_STATUS_CAPTURE_INT_MASK_SHIFT)) | \
                                                 ((uint8)((uint8)0 << GyroTimer_STATUS_FIFOFULL_INT_MASK_SHIFT)))
#endif /* (GyroTimer_UsingFixedFunction) */
#define GyroTimer_INIT_CAPTURE_COUNT      (2u)
#define GyroTimer_INIT_INT_CAPTURE_COUNT  ((uint8)((uint8)(1u - 1u) << GyroTimer_CTRL_INTCNT_SHIFT))


/***************************************
*           Registers
***************************************/

#if (GyroTimer_UsingFixedFunction) /* Implementation Specific Registers and Register Constants */


    /***************************************
    *    Fixed Function Registers
    ***************************************/

    #define GyroTimer_STATUS         (*(reg8 *) GyroTimer_TimerHW__SR0 )
    /* In Fixed Function Block Status and Mask are the same register */
    #define GyroTimer_STATUS_MASK    (*(reg8 *) GyroTimer_TimerHW__SR0 )
    #define GyroTimer_CONTROL        (*(reg8 *) GyroTimer_TimerHW__CFG0)
    #define GyroTimer_CONTROL2       (*(reg8 *) GyroTimer_TimerHW__CFG1)
    #define GyroTimer_CONTROL2_PTR   ( (reg8 *) GyroTimer_TimerHW__CFG1)
    #define GyroTimer_RT1            (*(reg8 *) GyroTimer_TimerHW__RT1)
    #define GyroTimer_RT1_PTR        ( (reg8 *) GyroTimer_TimerHW__RT1)

    #if (CY_PSOC3 || CY_PSOC5LP)
        #define GyroTimer_CONTROL3       (*(reg8 *) GyroTimer_TimerHW__CFG2)
        #define GyroTimer_CONTROL3_PTR   ( (reg8 *) GyroTimer_TimerHW__CFG2)
    #endif /* (CY_PSOC3 || CY_PSOC5LP) */
    #define GyroTimer_GLOBAL_ENABLE  (*(reg8 *) GyroTimer_TimerHW__PM_ACT_CFG)
    #define GyroTimer_GLOBAL_STBY_ENABLE  (*(reg8 *) GyroTimer_TimerHW__PM_STBY_CFG)

    #define GyroTimer_CAPTURE_LSB         (* (reg16 *) GyroTimer_TimerHW__CAP0 )
    #define GyroTimer_CAPTURE_LSB_PTR       ((reg16 *) GyroTimer_TimerHW__CAP0 )
    #define GyroTimer_PERIOD_LSB          (* (reg16 *) GyroTimer_TimerHW__PER0 )
    #define GyroTimer_PERIOD_LSB_PTR        ((reg16 *) GyroTimer_TimerHW__PER0 )
    #define GyroTimer_COUNTER_LSB         (* (reg16 *) GyroTimer_TimerHW__CNT_CMP0 )
    #define GyroTimer_COUNTER_LSB_PTR       ((reg16 *) GyroTimer_TimerHW__CNT_CMP0 )


    /***************************************
    *    Register Constants
    ***************************************/

    /* Fixed Function Block Chosen */
    #define GyroTimer_BLOCK_EN_MASK                     GyroTimer_TimerHW__PM_ACT_MSK
    #define GyroTimer_BLOCK_STBY_EN_MASK                GyroTimer_TimerHW__PM_STBY_MSK

    /* Control Register Bit Locations */
    /* Interrupt Count - Not valid for Fixed Function Block */
    #define GyroTimer_CTRL_INTCNT_SHIFT                  0x00u
    /* Trigger Polarity - Not valid for Fixed Function Block */
    #define GyroTimer_CTRL_TRIG_MODE_SHIFT               0x00u
    /* Trigger Enable - Not valid for Fixed Function Block */
    #define GyroTimer_CTRL_TRIG_EN_SHIFT                 0x00u
    /* Capture Polarity - Not valid for Fixed Function Block */
    #define GyroTimer_CTRL_CAP_MODE_SHIFT                0x00u
    /* Timer Enable - As defined in Register Map, part of TMRX_CFG0 register */
    #define GyroTimer_CTRL_ENABLE_SHIFT                  0x00u

    /* Control Register Bit Masks */
    #define GyroTimer_CTRL_ENABLE                        ((uint8)((uint8)0x01u << GyroTimer_CTRL_ENABLE_SHIFT))

    /* Control2 Register Bit Masks */
    /* As defined in Register Map, Part of the TMRX_CFG1 register */
    #define GyroTimer_CTRL2_IRQ_SEL_SHIFT                 0x00u
    #define GyroTimer_CTRL2_IRQ_SEL                      ((uint8)((uint8)0x01u << GyroTimer_CTRL2_IRQ_SEL_SHIFT))

    #if (CY_PSOC5A)
        /* Use CFG1 Mode bits to set run mode */
        /* As defined by Verilog Implementation */
        #define GyroTimer_CTRL_MODE_SHIFT                 0x01u
        #define GyroTimer_CTRL_MODE_MASK                 ((uint8)((uint8)0x07u << GyroTimer_CTRL_MODE_SHIFT))
    #endif /* (CY_PSOC5A) */
    #if (CY_PSOC3 || CY_PSOC5LP)
        /* Control3 Register Bit Locations */
        #define GyroTimer_CTRL_RCOD_SHIFT        0x02u
        #define GyroTimer_CTRL_ENBL_SHIFT        0x00u
        #define GyroTimer_CTRL_MODE_SHIFT        0x00u

        /* Control3 Register Bit Masks */
        #define GyroTimer_CTRL_RCOD_MASK  ((uint8)((uint8)0x03u << GyroTimer_CTRL_RCOD_SHIFT)) /* ROD and COD bit masks */
        #define GyroTimer_CTRL_ENBL_MASK  ((uint8)((uint8)0x80u << GyroTimer_CTRL_ENBL_SHIFT)) /* HW_EN bit mask */
        #define GyroTimer_CTRL_MODE_MASK  ((uint8)((uint8)0x03u << GyroTimer_CTRL_MODE_SHIFT)) /* Run mode bit mask */

        #define GyroTimer_CTRL_RCOD       ((uint8)((uint8)0x03u << GyroTimer_CTRL_RCOD_SHIFT))
        #define GyroTimer_CTRL_ENBL       ((uint8)((uint8)0x80u << GyroTimer_CTRL_ENBL_SHIFT))
    #endif /* (CY_PSOC3 || CY_PSOC5LP) */

    /*RT1 Synch Constants: Applicable for PSoC3 and PSoC5LP */
    #define GyroTimer_RT1_SHIFT                       0x04u
    /* Sync TC and CMP bit masks */
    #define GyroTimer_RT1_MASK                        ((uint8)((uint8)0x03u << GyroTimer_RT1_SHIFT))
    #define GyroTimer_SYNC                            ((uint8)((uint8)0x03u << GyroTimer_RT1_SHIFT))
    #define GyroTimer_SYNCDSI_SHIFT                   0x00u
    /* Sync all DSI inputs with Mask  */
    #define GyroTimer_SYNCDSI_MASK                    ((uint8)((uint8)0x0Fu << GyroTimer_SYNCDSI_SHIFT))
    /* Sync all DSI inputs */
    #define GyroTimer_SYNCDSI_EN                      ((uint8)((uint8)0x0Fu << GyroTimer_SYNCDSI_SHIFT))

    #define GyroTimer_CTRL_MODE_PULSEWIDTH            ((uint8)((uint8)0x01u << GyroTimer_CTRL_MODE_SHIFT))
    #define GyroTimer_CTRL_MODE_PERIOD                ((uint8)((uint8)0x02u << GyroTimer_CTRL_MODE_SHIFT))
    #define GyroTimer_CTRL_MODE_CONTINUOUS            ((uint8)((uint8)0x00u << GyroTimer_CTRL_MODE_SHIFT))

    /* Status Register Bit Locations */
    /* As defined in Register Map, part of TMRX_SR0 register */
    #define GyroTimer_STATUS_TC_SHIFT                 0x07u
    /* As defined in Register Map, part of TMRX_SR0 register, Shared with Compare Status */
    #define GyroTimer_STATUS_CAPTURE_SHIFT            0x06u
    /* As defined in Register Map, part of TMRX_SR0 register */
    #define GyroTimer_STATUS_TC_INT_MASK_SHIFT        (GyroTimer_STATUS_TC_SHIFT - 0x04u)
    /* As defined in Register Map, part of TMRX_SR0 register, Shared with Compare Status */
    #define GyroTimer_STATUS_CAPTURE_INT_MASK_SHIFT   (GyroTimer_STATUS_CAPTURE_SHIFT - 0x04u)

    /* Status Register Bit Masks */
    #define GyroTimer_STATUS_TC                       ((uint8)((uint8)0x01u << GyroTimer_STATUS_TC_SHIFT))
    #define GyroTimer_STATUS_CAPTURE                  ((uint8)((uint8)0x01u << GyroTimer_STATUS_CAPTURE_SHIFT))
    /* Interrupt Enable Bit-Mask for interrupt on TC */
    #define GyroTimer_STATUS_TC_INT_MASK              ((uint8)((uint8)0x01u << GyroTimer_STATUS_TC_INT_MASK_SHIFT))
    /* Interrupt Enable Bit-Mask for interrupt on Capture */
    #define GyroTimer_STATUS_CAPTURE_INT_MASK         ((uint8)((uint8)0x01u << GyroTimer_STATUS_CAPTURE_INT_MASK_SHIFT))

#else   /* UDB Registers and Register Constants */


    /***************************************
    *           UDB Registers
    ***************************************/

    #define GyroTimer_STATUS              (* (reg8 *) GyroTimer_TimerUDB_rstSts_stsreg__STATUS_REG )
    #define GyroTimer_STATUS_MASK         (* (reg8 *) GyroTimer_TimerUDB_rstSts_stsreg__MASK_REG)
    #define GyroTimer_STATUS_AUX_CTRL     (* (reg8 *) GyroTimer_TimerUDB_rstSts_stsreg__STATUS_AUX_CTL_REG)
    #define GyroTimer_CONTROL             (* (reg8 *) GyroTimer_TimerUDB_sCTRLReg_SyncCtl_ctrlreg__CONTROL_REG )
    
    #if(GyroTimer_Resolution <= 8u) /* 8-bit Timer */
        #define GyroTimer_CAPTURE_LSB         (* (reg8 *) GyroTimer_TimerUDB_sT16_timerdp_u0__F0_REG )
        #define GyroTimer_CAPTURE_LSB_PTR       ((reg8 *) GyroTimer_TimerUDB_sT16_timerdp_u0__F0_REG )
        #define GyroTimer_PERIOD_LSB          (* (reg8 *) GyroTimer_TimerUDB_sT16_timerdp_u0__D0_REG )
        #define GyroTimer_PERIOD_LSB_PTR        ((reg8 *) GyroTimer_TimerUDB_sT16_timerdp_u0__D0_REG )
        #define GyroTimer_COUNTER_LSB         (* (reg8 *) GyroTimer_TimerUDB_sT16_timerdp_u0__A0_REG )
        #define GyroTimer_COUNTER_LSB_PTR       ((reg8 *) GyroTimer_TimerUDB_sT16_timerdp_u0__A0_REG )
    #elif(GyroTimer_Resolution <= 16u) /* 8-bit Timer */
        #if(CY_PSOC3) /* 8-bit addres space */
            #define GyroTimer_CAPTURE_LSB         (* (reg16 *) GyroTimer_TimerUDB_sT16_timerdp_u0__F0_REG )
            #define GyroTimer_CAPTURE_LSB_PTR       ((reg16 *) GyroTimer_TimerUDB_sT16_timerdp_u0__F0_REG )
            #define GyroTimer_PERIOD_LSB          (* (reg16 *) GyroTimer_TimerUDB_sT16_timerdp_u0__D0_REG )
            #define GyroTimer_PERIOD_LSB_PTR        ((reg16 *) GyroTimer_TimerUDB_sT16_timerdp_u0__D0_REG )
            #define GyroTimer_COUNTER_LSB         (* (reg16 *) GyroTimer_TimerUDB_sT16_timerdp_u0__A0_REG )
            #define GyroTimer_COUNTER_LSB_PTR       ((reg16 *) GyroTimer_TimerUDB_sT16_timerdp_u0__A0_REG )
        #else /* 16-bit address space */
            #define GyroTimer_CAPTURE_LSB         (* (reg16 *) GyroTimer_TimerUDB_sT16_timerdp_u0__16BIT_F0_REG )
            #define GyroTimer_CAPTURE_LSB_PTR       ((reg16 *) GyroTimer_TimerUDB_sT16_timerdp_u0__16BIT_F0_REG )
            #define GyroTimer_PERIOD_LSB          (* (reg16 *) GyroTimer_TimerUDB_sT16_timerdp_u0__16BIT_D0_REG )
            #define GyroTimer_PERIOD_LSB_PTR        ((reg16 *) GyroTimer_TimerUDB_sT16_timerdp_u0__16BIT_D0_REG )
            #define GyroTimer_COUNTER_LSB         (* (reg16 *) GyroTimer_TimerUDB_sT16_timerdp_u0__16BIT_A0_REG )
            #define GyroTimer_COUNTER_LSB_PTR       ((reg16 *) GyroTimer_TimerUDB_sT16_timerdp_u0__16BIT_A0_REG )
        #endif /* CY_PSOC3 */
    #elif(GyroTimer_Resolution <= 24u)/* 24-bit Timer */
        #define GyroTimer_CAPTURE_LSB         (* (reg32 *) GyroTimer_TimerUDB_sT16_timerdp_u0__F0_REG )
        #define GyroTimer_CAPTURE_LSB_PTR       ((reg32 *) GyroTimer_TimerUDB_sT16_timerdp_u0__F0_REG )
        #define GyroTimer_PERIOD_LSB          (* (reg32 *) GyroTimer_TimerUDB_sT16_timerdp_u0__D0_REG )
        #define GyroTimer_PERIOD_LSB_PTR        ((reg32 *) GyroTimer_TimerUDB_sT16_timerdp_u0__D0_REG )
        #define GyroTimer_COUNTER_LSB         (* (reg32 *) GyroTimer_TimerUDB_sT16_timerdp_u0__A0_REG )
        #define GyroTimer_COUNTER_LSB_PTR       ((reg32 *) GyroTimer_TimerUDB_sT16_timerdp_u0__A0_REG )
    #else /* 32-bit Timer */
        #if(CY_PSOC3 || CY_PSOC5) /* 8-bit address space */
            #define GyroTimer_CAPTURE_LSB         (* (reg32 *) GyroTimer_TimerUDB_sT16_timerdp_u0__F0_REG )
            #define GyroTimer_CAPTURE_LSB_PTR       ((reg32 *) GyroTimer_TimerUDB_sT16_timerdp_u0__F0_REG )
            #define GyroTimer_PERIOD_LSB          (* (reg32 *) GyroTimer_TimerUDB_sT16_timerdp_u0__D0_REG )
            #define GyroTimer_PERIOD_LSB_PTR        ((reg32 *) GyroTimer_TimerUDB_sT16_timerdp_u0__D0_REG )
            #define GyroTimer_COUNTER_LSB         (* (reg32 *) GyroTimer_TimerUDB_sT16_timerdp_u0__A0_REG )
            #define GyroTimer_COUNTER_LSB_PTR       ((reg32 *) GyroTimer_TimerUDB_sT16_timerdp_u0__A0_REG )
        #else /* 32-bit address space */
            #define GyroTimer_CAPTURE_LSB         (* (reg32 *) GyroTimer_TimerUDB_sT16_timerdp_u0__32BIT_F0_REG )
            #define GyroTimer_CAPTURE_LSB_PTR       ((reg32 *) GyroTimer_TimerUDB_sT16_timerdp_u0__32BIT_F0_REG )
            #define GyroTimer_PERIOD_LSB          (* (reg32 *) GyroTimer_TimerUDB_sT16_timerdp_u0__32BIT_D0_REG )
            #define GyroTimer_PERIOD_LSB_PTR        ((reg32 *) GyroTimer_TimerUDB_sT16_timerdp_u0__32BIT_D0_REG )
            #define GyroTimer_COUNTER_LSB         (* (reg32 *) GyroTimer_TimerUDB_sT16_timerdp_u0__32BIT_A0_REG )
            #define GyroTimer_COUNTER_LSB_PTR       ((reg32 *) GyroTimer_TimerUDB_sT16_timerdp_u0__32BIT_A0_REG )
        #endif /* CY_PSOC3 || CY_PSOC5 */ 
    #endif

    #define GyroTimer_COUNTER_LSB_PTR_8BIT       ((reg8 *) GyroTimer_TimerUDB_sT16_timerdp_u0__A0_REG )
    
    #if (GyroTimer_UsingHWCaptureCounter)
        #define GyroTimer_CAP_COUNT              (*(reg8 *) GyroTimer_TimerUDB_sCapCount_counter__PERIOD_REG )
        #define GyroTimer_CAP_COUNT_PTR          ( (reg8 *) GyroTimer_TimerUDB_sCapCount_counter__PERIOD_REG )
        #define GyroTimer_CAPTURE_COUNT_CTRL     (*(reg8 *) GyroTimer_TimerUDB_sCapCount_counter__CONTROL_AUX_CTL_REG )
        #define GyroTimer_CAPTURE_COUNT_CTRL_PTR ( (reg8 *) GyroTimer_TimerUDB_sCapCount_counter__CONTROL_AUX_CTL_REG )
    #endif /* (GyroTimer_UsingHWCaptureCounter) */


    /***************************************
    *       Register Constants
    ***************************************/

    /* Control Register Bit Locations */
    #define GyroTimer_CTRL_INTCNT_SHIFT              0x00u       /* As defined by Verilog Implementation */
    #define GyroTimer_CTRL_TRIG_MODE_SHIFT           0x02u       /* As defined by Verilog Implementation */
    #define GyroTimer_CTRL_TRIG_EN_SHIFT             0x04u       /* As defined by Verilog Implementation */
    #define GyroTimer_CTRL_CAP_MODE_SHIFT            0x05u       /* As defined by Verilog Implementation */
    #define GyroTimer_CTRL_ENABLE_SHIFT              0x07u       /* As defined by Verilog Implementation */

    /* Control Register Bit Masks */
    #define GyroTimer_CTRL_INTCNT_MASK               ((uint8)((uint8)0x03u << GyroTimer_CTRL_INTCNT_SHIFT))
    #define GyroTimer_CTRL_TRIG_MODE_MASK            ((uint8)((uint8)0x03u << GyroTimer_CTRL_TRIG_MODE_SHIFT))
    #define GyroTimer_CTRL_TRIG_EN                   ((uint8)((uint8)0x01u << GyroTimer_CTRL_TRIG_EN_SHIFT))
    #define GyroTimer_CTRL_CAP_MODE_MASK             ((uint8)((uint8)0x03u << GyroTimer_CTRL_CAP_MODE_SHIFT))
    #define GyroTimer_CTRL_ENABLE                    ((uint8)((uint8)0x01u << GyroTimer_CTRL_ENABLE_SHIFT))

    /* Bit Counter (7-bit) Control Register Bit Definitions */
    /* As defined by the Register map for the AUX Control Register */
    #define GyroTimer_CNTR_ENABLE                    0x20u

    /* Status Register Bit Locations */
    #define GyroTimer_STATUS_TC_SHIFT                0x00u  /* As defined by Verilog Implementation */
    #define GyroTimer_STATUS_CAPTURE_SHIFT           0x01u  /* As defined by Verilog Implementation */
    #define GyroTimer_STATUS_TC_INT_MASK_SHIFT       GyroTimer_STATUS_TC_SHIFT
    #define GyroTimer_STATUS_CAPTURE_INT_MASK_SHIFT  GyroTimer_STATUS_CAPTURE_SHIFT
    #define GyroTimer_STATUS_FIFOFULL_SHIFT          0x02u  /* As defined by Verilog Implementation */
    #define GyroTimer_STATUS_FIFONEMP_SHIFT          0x03u  /* As defined by Verilog Implementation */
    #define GyroTimer_STATUS_FIFOFULL_INT_MASK_SHIFT GyroTimer_STATUS_FIFOFULL_SHIFT

    /* Status Register Bit Masks */
    /* Sticky TC Event Bit-Mask */
    #define GyroTimer_STATUS_TC                      ((uint8)((uint8)0x01u << GyroTimer_STATUS_TC_SHIFT))
    /* Sticky Capture Event Bit-Mask */
    #define GyroTimer_STATUS_CAPTURE                 ((uint8)((uint8)0x01u << GyroTimer_STATUS_CAPTURE_SHIFT))
    /* Interrupt Enable Bit-Mask */
    #define GyroTimer_STATUS_TC_INT_MASK             ((uint8)((uint8)0x01u << GyroTimer_STATUS_TC_SHIFT))
    /* Interrupt Enable Bit-Mask */
    #define GyroTimer_STATUS_CAPTURE_INT_MASK        ((uint8)((uint8)0x01u << GyroTimer_STATUS_CAPTURE_SHIFT))
    /* NOT-Sticky FIFO Full Bit-Mask */
    #define GyroTimer_STATUS_FIFOFULL                ((uint8)((uint8)0x01u << GyroTimer_STATUS_FIFOFULL_SHIFT))
    /* NOT-Sticky FIFO Not Empty Bit-Mask */
    #define GyroTimer_STATUS_FIFONEMP                ((uint8)((uint8)0x01u << GyroTimer_STATUS_FIFONEMP_SHIFT))
    /* Interrupt Enable Bit-Mask */
    #define GyroTimer_STATUS_FIFOFULL_INT_MASK       ((uint8)((uint8)0x01u << GyroTimer_STATUS_FIFOFULL_SHIFT))

    #define GyroTimer_STATUS_ACTL_INT_EN             0x10u   /* As defined for the ACTL Register */

    /* Datapath Auxillary Control Register definitions */
    #define GyroTimer_AUX_CTRL_FIFO0_CLR             0x01u   /* As defined by Register map */
    #define GyroTimer_AUX_CTRL_FIFO1_CLR             0x02u   /* As defined by Register map */
    #define GyroTimer_AUX_CTRL_FIFO0_LVL             0x04u   /* As defined by Register map */
    #define GyroTimer_AUX_CTRL_FIFO1_LVL             0x08u   /* As defined by Register map */
    #define GyroTimer_STATUS_ACTL_INT_EN_MASK        0x10u   /* As defined for the ACTL Register */

#endif /* Implementation Specific Registers and Register Constants */

#endif  /* CY_Timer_v2_30_GyroTimer_H */


/* [] END OF FILE */
