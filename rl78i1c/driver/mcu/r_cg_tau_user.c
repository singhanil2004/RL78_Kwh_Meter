/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIESREGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY
* LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT,
* INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR
* ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability 
* of this software. By using this software, you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2013, 2015 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_cg_tau_user.c
* Version      : 
* Device(s)    : RL78/I1C
* Tool-Chain   : CCRL
* Description  : This file implements device driver for TAU module.
* Creation Date: 
***********************************************************************************************************************/

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_tau.h"
/* Start user code for include. Do not edit comment generated here */
#include "wrp_em_sw_config.h"
#include "wrp_em_timer.h"
//#include "r_cg_rtc.h"
#include "r_calib.h"
#include "em_measurement.h"
#include "inst_read.h"
#include "wrp_em_rtc.h"

/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
extern uint16_t g_tau0_ch6_cascade_count_init;
extern uint16_t g_tau0_ch6_cascade_count;
extern uint16_t g_tau0_ch6_tdr_update_value;
extern const uint16_t g_tau0_ch6_max_tdr_support;
void r_callback_periodic(void);
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: r_tau0_channel0_interrupt
* Description  : This function INTTM00 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
#ifdef __CCRL__
#pragma interrupt r_tau0_channel0_interrupt(vect=INTTM00)
#endif /* __CCRL__ */
#ifdef __ICCRL78__
#pragma vector = INTTM00_vect
#endif /* __ICCRL78__ */
__INTERRUPT static void r_tau0_channel0_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */
    r_callback_periodic();
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_rtc_callback_periodic
* Description  : This function is real-time clock interrupt service handler.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
 void r_callback_periodic(void)
{
    
    /* Start user code. Do not edit comment generated here */
    EM_RTC_ConstInterruptCallback();
     

//    if (R_CALIB_IsInCalibMode() == 1)
//    {
//        EM_CALIB_RTCConstantInterruptCallback();
//    }
    
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_tau0_channel1_interrupt
* Description  : This function INTTM01 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
#ifdef __CCRL__
#pragma interrupt r_tau0_channel1_interrupt(vect=INTTM01)
#endif /* __CCRL__ */
#ifdef __ICCRL78__
#pragma vector = INTTM01_vect
#endif /* __ICCRL78__ */
__INTERRUPT static void r_tau0_channel1_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_tau0_channel2_interrupt
* Description  : This function INTTM02 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
#ifdef __CCRL__
#pragma interrupt r_tau0_channel2_interrupt(vect=INTTM02)
#endif /* __CCRL__ */
#ifdef __ICCRL78__
#pragma vector = INTTM02_vect
#endif /* __ICCRL78__ */
__INTERRUPT static void r_tau0_channel2_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */
    EI();
    
    if (R_CALIB_IsInCalibMode() == 0)
    {
        EM_TIMER_InterruptCallback();
        
    }
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_tau0_channel3_interrupt
* Description  : This function INTTM03 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
#ifdef __CCRL__
#pragma interrupt r_tau0_channel3_interrupt(vect=INTTM03)
#endif /* __CCRL__ */
#ifdef __ICCRL78__
#pragma vector = INTTM03_vect
#endif /* __ICCRL78__ */
__INTERRUPT static void r_tau0_channel3_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_tau0_channel4_interrupt
* Description  : This function INTTM04 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
#ifdef __CCRL__
#pragma interrupt r_tau0_channel4_interrupt(vect=INTTM04)
#endif /* __CCRL__ */
#ifdef __ICCRL78__
#pragma vector = INTTM04_vect
#endif /* __ICCRL78__ */
__INTERRUPT static void r_tau0_channel4_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */
    EM_PULSE3_ON_STATEMENT;
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_tau0_channel5_interrupt
* Description  : This function INTTM05 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
#ifdef __CCRL__
#pragma interrupt r_tau0_channel5_interrupt(vect=INTTM05)
#endif /* __CCRL__ */
#ifdef __ICCRL78__
#pragma vector = INTTM05_vect
#endif /* __ICCRL78__ */
__INTERRUPT static void r_tau0_channel5_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */
    EM_PULSE3_OFF_STATEMENT;
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_tau0_channel6_interrupt
* Description  : This function INTTM06 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
#ifdef __CCRL__
#pragma interrupt r_tau0_channel6_interrupt(vect=INTTM06)
#endif /* __CCRL__ */
#ifdef __ICCRL78__
#pragma vector = INTTM06_vect
#endif /* __ICCRL78__ */
__INTERRUPT static void r_tau0_channel6_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */
    if (g_tau0_ch6_cascade_count == 0)
    {
        /* Optimized for the call to R_TAU0_Channel4_Set_SoftwareTriggerOn() */
        TS0 = _0010_TAU_CH4_START_TRG_ON;
        
        g_tau0_ch6_cascade_count = g_tau0_ch6_cascade_count_init;
        TDR06 = g_tau0_ch6_tdr_update_value;
    }
    else
    {
        g_tau0_ch6_cascade_count--;
        TDR06 = g_tau0_ch6_max_tdr_support;
    }
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
