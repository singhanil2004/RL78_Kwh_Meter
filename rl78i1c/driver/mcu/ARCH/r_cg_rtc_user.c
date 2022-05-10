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
* File Name    : r_cg_rtc_user.c
* Version      : 
* Device(s)    : RL78/I1C
* Tool-Chain   : CCRL
* Description  : This file implements device driver for RTC module.
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
#include "r_cg_rtc.h"
/* Start user code for include. Do not edit comment generated here */
#include "wrp_em_rtc.h"
#include "config_storage.h"
#include "r_calib.h"
#include "em_measurement.h"
#include "inst_read.h"
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
unsigned char g_Second_f,Tcount;
float32_t Frequency1, Frequency2, Frequency3, Frequency ;
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: r_rtc_prd_interrupt
* Description  : None
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
#ifdef __CCRL__
#pragma interrupt r_rtc_rpd_interrupt(vect=INTRTCRPD)
#endif /* __CCRL__ */
#ifdef __ICCRL78__
#pragma vector = INTRTCRPD_vect
#endif /* __ICCRL78__ */
__INTERRUPT static void r_rtc_rpd_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */
    volatile uint8_t tmp;
    EI();
    
    VRTCEN = 1U;
    
    if (_01_RTC_COUNTER_MATCH == (RSR & _01_RTC_COUNTER_MATCH))
    {
        tmp = RSR;
        tmp &= (uint8_t)~_01_RTC_COUNTER_MATCH;     /* clear AF */
        RSR = tmp;
        r_rtc_callback_alarm();
    }
    
    if (_04_RTC_PERIODIC_INTERRUPT == (RSR & _04_RTC_PERIODIC_INTERRUPT))
    {
        tmp = RSR;
        tmp &= (uint8_t)~_04_RTC_PERIODIC_INTERRUPT;    /* clear PF */
        RSR = tmp;
        r_rtc_callback_periodic();
    }
    
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_rtc_callback_alarm
* Description  : This function is alarm interrupt service handler.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_rtc_callback_alarm(void)
{
    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_rtc_callback_periodic
* Description  : This function is real-time clock interrupt service handler.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
 void r_rtc_callback_periodic(void)
{
    /* Start user code. Do not edit comment generated here */
    EM_RTC_ConstInterruptCallback();

//    if (R_CALIB_IsInCalibMode() == 1)
//    {
//        EM_CALIB_RTCConstantInterruptCallback();
//    }
    
//    CONFIG_RtcCallback();
//    INST_READ_RTC_InterruptCallBack();
    Frequency = EM_GetLineFrequency(LINE_TOTAL);
    Frequency1 = EM_GetLineFrequency(LINE_PHASE_R);
    Frequency2 = EM_GetLineFrequency(LINE_PHASE_Y);
    Frequency3 = EM_GetLineFrequency(LINE_PHASE_B);
    
    
    /* End user code. Do not edit comment generated here */
}
/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
