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
* Copyright (C) 2015, 2016 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_cg_rtc.c
* Version      : Applilet4 for RL78/I1C V1.00.01.04 [05 Aug 2016]
* Device(s)    : R5F10NMJ
* Tool-Chain   : CA78K0R
* Description  : This file implements device driver for RTC module.
* Creation Date: 11/8/2016
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
#include "wrp_mcu.h"
#include "startup.h"
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
extern volatile rtc_timecapture_value_t g_rtc_calendar_rtcic0;
extern volatile rtc_timecapture_value_t g_rtc_calendar_rtcic1;
extern volatile rtc_timecapture_value_t g_rtc_calendar_rtcic2;
extern volatile uint32_t g_rtc_binary_rtcic0;
extern volatile uint32_t g_rtc_binary_rtcic1;
extern volatile uint32_t g_rtc_binary_rtcic2;

/* Start user code for global. Do not edit comment generated here */
#define ABS(x)                  ((x) <  0  ? (-(x)) : (x))                          /* Get absolute value */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_RTC_Create
* Description  : This function initializes the real-time clock module.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_RTC_Create(void)
{
    volatile uint8_t tmp;

    VRTCEN = 1U;    /* enables input clock supply */

    RTCRMK = 1U;    /* disable INTRTCRPD interrupt */
    RTCRIF = 0U;    /* clear INTRTCRPD interrupt flag */
    RTCAMK = 1U;    /* disable INTRTCALM interrupt */
    RTCAIF = 0U;    /* clear INTRTCALM interrupt flag */
    /* Set INTRTCRPD low priority */
    RTCRPR1 = 1U;
    RTCRPR0 = 1U;

    if (RTCPORSR == 0)
    {
        /* Mark RTC initialized: make this before checking of condition
         * If VRTC not too low, still can write, then RTCPORSR will be 1 after set
         * Else, after set it still value 0, then can ignore the RTC register initialization
        */
        if ( (LVDVRTCF == 0) || (RTCPORSR == 1) )
        {
            RCR4 = _00_RTC_SELECT_FSUB;
            
            /* Set sub-clock oscillator : RTCEN bit 0 */
            BIT_SELECT(RCR3, 0) = 1U;
            while(BIT_SELECT(RCR3, 0) != 1U);
            
            /* Stop all counters : START is bit 0 */
            BIT_SELECT(RCR2, 0) = 0U;
            while(BIT_SELECT(RCR2, 0) != 0U);
            
            /* Select count mode : CNTMD is bit 7 */
            BIT_SELECT(RCR2, 7) = 0U;
            while (BIT_SELECT(RCR2, 7) != 0U);
            
            /* Execute RTC software reset : RESET is bit 1 */
            BIT_SELECT(RCR2,1) = 1U;
            while (BIT_SELECT(RCR2,1) != 0U);
            
            /* Set control registers */
            tmp = _D0_RTC_INTRTCPRD_CLOCK_7 | _00_RTC_1HZ_OUTPUT | _04_RTC_PERIODIC_ENABLE | _00_RTC_ALARM_DISABLE;
            RCR1 = tmp;
            while(RCR1 != tmp);
            
            tmp = _00_RTC_CALENDER_MODE | _40_RTC_24HOUR_MODE | _00_RTC_RTCOUT_DISABLE;
            RCR2 = tmp;
            while (RCR2 != tmp);
            
            /* Start the RTC once  */
            BIT_SELECT(RCR2, 0) = 1U;
            while (BIT_SELECT(RCR2, 0) != 1U);
            
            /* Unlock, set and lock the RCR5 */
            RCR5GD = 0x00U;
            RCR5GD = 0x72U;
            RCR5GD = 0x64U;
            RCR5 = 0x00U;
            RCR5GD = 0x00U;
        }
	RTCPORSR = 1U;
    }
    
   // RTCCR0 = _00_RTC_RTCICN0_DISABLE | _00_RTC_EVENT0_DETECTED;
   // RTCCR1 = _00_RTC_RTCICN1_DISABLE | _00_RTC_FILTER1_OFF | _04_RTC_EVENT1_NOTDETECTED | 
             _00_RTC_CAPTURE1_NOEVENT;
   // RTCCR2 = _00_RTC_RTCICN2_DISABLE | _00_RTC_FILTER2_OFF | _04_RTC_EVENT2_NOTDETECTED | 
             _00_RTC_CAPTURE2_NOEVENT;

    /* Set RTCIC1 pin */
   // PM15 |= 0x02U;
    /* Set RTCIC2 pin */
   // PM15 |= 0x04U;
}

/***********************************************************************************************************************
* Function Name: R_RTC_Start
* Description  : This function enables the real-time clock.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_RTC_Start(void)
{
    volatile uint8_t tmp;
    
    RTCRIF = 0U;    /* clear INTRTCRPD interrupt flag */
    RTCRMK = 0U;    /* enable INTRTCRPD interrupt */
    
    #if 0
    /* Do not start the RTC, just enable the interrupt */
    tmp = RCR2;
    tmp |= _01_RTC_COUNTER_NORMAL;
    RCR2 = tmp;
    #endif
}

/***********************************************************************************************************************
* Function Name: R_RTC_Stop
* Description  : This function disables the real-time clock.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_RTC_Stop(void)
{
    #if 0
    volatile uint8_t tmp;
    volatile uint16_t w_count;
    
    /* Do not stop the RTC, just mask the interrupt */
    tmp = RCR2;
    tmp &= (uint8_t)~_01_RTC_COUNTER_NORMAL;
    RCR2 = tmp;

    /* Change the waiting time according to the system */
    for (w_count = 0U; w_count <= RTC_STARTWAITTIME; w_count++)
    {
        NOP();
    }

    tmp = RCR2;
    tmp |= _02_RTC_RESET_WRITER_INITIALIZED;
    RCR2 = tmp;
	
    /* Change the waiting time according to the system */
    for (w_count = 0U; w_count <= RTC_RESETWAITTIME; w_count++)
    {
        NOP();
    }
    #endif
    
    RTCRMK = 1U;    /* disable INTRTCRPD interrupt */
    RTCRIF = 0U;    /* clear INTRTCRPD interrupt flag */
}

/***********************************************************************************************************************
* Function Name: R_RTC_Set_CalendarCounterValue
* Description  : This function changes the calendar real-time clock value.
* Arguments    : counter_write_val -
*                    the expected real-time clock value(BCD code)
* Return Value : status -
*                    MD_OK or MD_BUSY1
***********************************************************************************************************************/
MD_STATUS R_RTC_Set_CalendarCounterValue(rtc_calendarcounter_value_t counter_write_val)
{
    MD_STATUS status = MD_OK;
    volatile uint32_t  w_count;
    volatile uint8_t tmp;
    
    /* Only set registers if VRTC enough */
    if (LVDVRTCF == 1)
    {
        return MD_ERROR;
    }
    
    /* Stop all counters : START is bit 0 */
    BIT_SELECT(RCR2,0) = 0U;
    while (BIT_SELECT(RCR2,0) != 0U);

    if (_01_RTC_COUNTER_NORMAL == (RCR2 & _01_RTC_COUNTER_NORMAL))
    {
        status = MD_BUSY1;
    }
    else
    {
        RSECCNT = counter_write_val.rseccnt;
        RMINCNT = counter_write_val.rmincnt;
        RHRCNT = counter_write_val.rhrcnt;
        RWKCNT = counter_write_val.rwkcnt;
        RDAYCNT = counter_write_val.rdaycnt;
        RMONCNT = counter_write_val.rmoncnt;
        RYRCNT = counter_write_val.ryrcnt;
    }
    
    /* Start the RTC after edit */
    BIT_SELECT(RCR2,0) = 1U;
    while (BIT_SELECT(RCR2,0) != 1U);
    
    return (status);
}


/***********************************************************************************************************************
* Function Name: R_RTC_Get_CalendarCounterValue
* Description  : This function reads the results of real-time clock and store them in the variables.
* Arguments    : counter_read_val -
*                    the expected real-time clock value(BCD code)
* Return Value : status -
*                    MD_OK or MD_ERROR
***********************************************************************************************************************/
MD_STATUS R_RTC_Get_CalendarCounterValue(rtc_calendarcounter_value_t * const counter_read_val)
{
    MD_STATUS status = MD_OK;
    volatile uint8_t tmp;
    
    tmp = RSR;
    tmp &= (uint8_t)~_02_RTC_SECOND_CARRY;
    RSR = tmp;
    counter_read_val->rseccnt = RSECCNT;
    counter_read_val->rmincnt = RMINCNT;
    counter_read_val->rhrcnt = RHRCNT & 0xBF;
    counter_read_val->rwkcnt = RWKCNT;
    counter_read_val->rdaycnt = RDAYCNT;
    counter_read_val->rmoncnt = RMONCNT;
    counter_read_val->ryrcnt = RYRCNT;
    if (_02_RTC_SECOND_CARRY == (RSR & _02_RTC_SECOND_CARRY))
    {
        status = MD_ERROR;
    }
    return (status);
}

/***********************************************************************************************************************
* Function Name: R_RTC_Set_CalendarAlarmValue
* Description  : This function write calendar alarm value.
* Arguments    : time_value -
*                    calendar alarm value
* Return Value : None
***********************************************************************************************************************/
void R_RTC_Set_CalendarAlarmValue(rtc_calendarcounter_value_t time_value, rtc_alarm_enable_t control)
{  
    volatile uint8_t tmp;
    volatile uint16_t w_count;

    tmp = RCR1;
    tmp &= (uint8_t)~_01_RTC_ALARM_ENABLE;
    RCR1 = tmp;
    RSECAR  = time_value.rseccnt   | (uint8_t)(control.sec_enb << 7);
    RMINAR  = time_value.rmincnt   | (uint8_t)(control.min_enb << 7);
    RHRAR   = time_value.rhrcnt    | (uint8_t)(control.hr_enb  << 7);
    RDAYAR  = time_value.rdaycnt   | (uint8_t)(control.day_enb << 7);
    RWKAR   = time_value.rwkcnt    | (uint8_t)(control.wk_enb  << 7);
    RMONAR  = time_value.rmoncnt   | (uint8_t)(control.mon_enb << 7);
    RYRAR   = time_value.ryrcnt;
    RYRAREN = (uint8_t)(control.yr_enb << 7);

    /* Change the waiting time according to the system */
    for (w_count = 0U; w_count < RTC_WAITTIME_ARSET; w_count++)
    {
        NOP();
    }

    tmp = RSR;
    tmp &= (uint8_t)~_01_RTC_COUNTER_MATCH;
    RSR = tmp;
    
    /* Workaround: Constant interrupt and Alarm interrupt conflict. Check flag only, do not enable interrupt */
//    tmp = RCR1;
//    tmp |= _01_RTC_ALARM_ENABLE;
//    RCR1 = tmp;
}


/***********************************************************************************************************************
* Function Name: R_RTC_Get_CalendarAlarmValue
* Description  : This function resd alarm value.
* Arguments    : rtc_calendarcounter_value_t -
*                    calendar alarm value
* Return Value : None
***********************************************************************************************************************/
void R_RTC_Get_CalendarAlarmValue(rtc_calendarcounter_value_t * const time_value, rtc_alarm_enable_t * const control)
{
    volatile uint8_t tmp;

    tmp = RCR1; 
    tmp &= (uint8_t)~_01_RTC_ALARM_ENABLE;
    RCR1 = tmp;
    
    time_value->rseccnt = RSECAR    &0x7F;
    time_value->rmincnt = RMINAR    &0x7F;
    time_value->rhrcnt  = RHRAR     &0x7F;
    time_value->rdaycnt = RDAYAR    &0x7F;
    time_value->rwkcnt  = RWKAR     &0x7F;
    time_value->rmoncnt = RMONAR    &0x7F;
    time_value->ryrcnt  = (uint8_t)RYRAR;
    
    control->sec_enb = RSECAR  >> 7;
    control->min_enb = RMINAR  >> 7;
    control->hr_enb  = RHRAR   >> 7;
    control->day_enb = RDAYAR  >> 7;
    control->wk_enb  = RWKAR   >> 7;
    control->mon_enb = RMONAR  >> 7;
    control->yr_enb  = RYRAREN >> 7;
    
    /* Workaround: Constant interrupt and Alarm interrupt conflict. Check flag only, do not enable interrupt */
//    tmp = RCR1; 
//    tmp |= _01_RTC_ALARM_ENABLE;
//    RCR1 = tmp;
}


/***********************************************************************************************************************
* Function Name: R_RTC_Set_ConstPeriodInterruptOn
* Description  : This function enables constant-period interrupt.
* Arguments    : period -
*                    the constant period of INTRTCRPD
* Return Value : status -
*                    MD_OK or MD_ARGERROR
***********************************************************************************************************************/
MD_STATUS R_RTC_Set_ConstPeriodInterruptOn(rtc_int_period_t period)
{
    MD_STATUS status = MD_OK;
    uint8_t tmp;
    
    if ((period < PES_1_256_SEC) || (period > PES_2_SEC))
    {
        status = MD_ARGERROR;
    }
    else
    {
        /* Disable PRD interrupt */
        RTCRMK = 1U;    /* disable INTRTCRPD interrupt */
    
        /* Only set register if VRTC enough */
        if (LVDVRTCF == 1)
        {
            status = MD_ERROR;
        }
        else
        {
            tmp = (uint8_t)(period | (RCR1 & 0x0FU) | _04_RTC_PERIODIC_ENABLE);
            RCR1 = tmp;
            while (RCR1 != tmp);
        }
        RTCRIF = 0U;    /* clear INTRTCRPD interrupt flag */
        RTCRMK = 0U;    /* enable INTRTCRPD interrupt */
    }

    return (status);
}

/***********************************************************************************************************************
* Function Name: R_RTC_Set_ConstPeriodInterruptOff
* Description  : This function disables constant-period interrupt.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_RTC_Set_ConstPeriodInterruptOff(void)
{
    /* Disable PRD interrupt */
    RTCRMK = 1U;
    RTCRIF = 0U;
    
    /* Only set register if VRTC enough */
    if (LVDVRTCF == 1)
    {
        return;
    }
    
    BIT_SELECT(RCR1,2) = 0U;
    while (BIT_SELECT(RCR1,2) != 0U);
}

/***********************************************************************************************************************
* Function Name: R_RTC_Set_ClockOutputOn
* Description  : This function set RTCOUT on.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_RTC_Set_ClockOutputOn(rtc_clock_out_t clk_out)
{
    /* Set RTCOS */
    if (clk_out == RTCOS_FREQ_1HZ)
    {
        BIT_SELECT(RCR1,3) = 0U;
    }
    else
    {
        BIT_SELECT(RCR1,3) = 1U;
    }
    
    /* RTCOE enable */
    BIT_SELECT(RCR2,3) = 1U;
}
/***********************************************************************************************************************
* Function Name: R_RTC_Set_ClockOutputOff
* Description  : This function set RTCOUT off.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_RTC_Set_ClockOutputOff(void)
{
    /* RTCOE disable */
    BIT_SELECT(RCR2,3) = 0U;
}

/* Start user code for adding. Do not edit comment generated here */
/******************************************************************************
* Function Name: R_RTC_Compensate
* Description  : This function compensate the error on sub-clock by a provided ppm
* Arguments    : float ppm: ppm error of sub-clock crystal
* Return Value : None
*******************************************************************************/
void R_RTC_Compensate(float ppm)
{
    volatile uint8_t tmp;
    volatile uint16_t w_count;

    float   abs_ppm = ABS(ppm);
    uint8_t radj_reg = 0;
    
    /* Only set register if VRTC enough */
    if (LVDVRTCF == 1)
    {
        return;
    }
    
    /* This feature only run on 32.768KHz clock source : RCKSEL is bit 0 */
    if (BIT_SELECT(RCR4,0) == 0U)
    {
        /* Clear adjustment first : PMADJ bit [7:6] */
        while ((RADJ & 0xC0) != 0)
        {
            RADJ &= 0x3F;
        }
        
        /* Enable automatic adj : AADJE bit 4 */
        BIT_SELECT(RCR2,4) = 1U;
        
        /* Enable sub or add */
        if (ppm > 0.0f)
        {
            radj_reg |= _80_RTC_TIMER_ERROR_ADJUST_MINUS;       /* sub */
        }
        else
        {
            radj_reg |= _40_RTC_TIMER_ERROR_ADJUST_PLUS;        /* add */
        }
        
        if (abs_ppm <= 32.043f)         /* 1 minutes adjustment */
        {
            /* 60 seconds adjustment (AADJP = 0) */
            BIT_SELECT(RCR2,5) = 0U;                /* AADJP is bit 5 */
            radj_reg |= ( (uint8_t)(abs_ppm * (32768.0f * 60.0f / 1e6) + 0.5f) ) & 0x3F;
        }
        else if (abs_ppm <= 192.26f)    /* 10-second adjustment */
        {
            /* 10 seconds adjustment (AADJP = 1) */
            BIT_SELECT(RCR2,5) = 1U;                /* AADJP is bit 5 */
            radj_reg |= ( (uint8_t)(abs_ppm * (32768.0f * 10.0f / 1e6) + 0.5f) ) & 0x3F;
        }
        else
        {
            /* Out of control !! no adj at all */
            BIT_SELECT(RCR2,4) = 0U;
            BIT_SELECT(RCR2,5) = 0U;
            radj_reg = 0;
        }
        
        while (RADJ != radj_reg)
        {
            RADJ = radj_reg;
        }
    }
}

/******************************************************************************
* Function Name: R_RTC_StopCompensation
* Description  : Stop the rtc compensation 
* Arguments    : None
* Return Value : None
*******************************************************************************/
void R_RTC_StopCompensation(void)
{
    volatile uint8_t tmp;
    
    /* Only set register if VRTC enough */
    if (LVDVRTCF == 1)
    {
        return;
    }
    
    /* AADJE bit 4, AADJP is bit 5 */
    tmp = RCR2;
    tmp &= ~(_10_RTC_ADJUSTMENT_ENABLE | _20_RTC_CALENDER_10SECONDS);
    RCR2 = tmp;
    while(RCR2 != tmp);   
}

/* End user code. Do not edit comment generated here */
