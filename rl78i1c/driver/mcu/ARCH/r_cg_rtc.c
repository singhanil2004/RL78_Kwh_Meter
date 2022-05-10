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
* File Name    : r_cg_rtc.c
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
#include "wrp_mcu.h"
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* CAUTIONS:
 * Those bits can be write directly without checking for synchronization:
 *  . RCR1:
 *      RTCOS[3]
 *  . RCR2:
 *      ADJ30[2], RTCOE[3]
 *  . RCR5: All
 *  . RADJ: 
 * Value written to belows will reflected after 4 read operations:
 *  . All calendar counters
 *  . All alarm compare and enable registers
 *  . RCR2: AADJE[4], AADJP[5], HR24[6]
 *  . RCR3, RCR4
*/
//#define ABS(x)                  ((x) <  0  ? (-(x)) : (x))                          /* Get absolute value */
/* End user code. Do not edit comment generated here */

#define ABS(x)                  ((x) <  0  ? (-(x)) : (x))                          /* Get absolute value */

#define BIT7   0x80
#define BIT6   0x40
#define BIT5   0x20
#define BIT4   0x10
#define BIT3   0x08
#define BIT2   0x04
#define BIT1   0x02
#define BIT0   0x01
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_RTC_Create
* Description  : This function initializes the RTC module.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
unsigned char CheckForSync(volatile unsigned char *ptr, unsigned char val, unsigned char cntr)
{
    unsigned char volatile i;
	unsigned int temp;
	for(i=0;i<cntr;i++)
	{
		if(*ptr != val)
		{
			/* Wait for setting value sync to Register */
			for(temp=0;temp<1000;temp++);
		}
		else
		{
			break;
		}
	}
	if(i>=cntr)
	{
		NOP();
		return 0;
	}
	else
	{
		NOP();
		return 1;
	}
}
void DelayMs(unsigned int d)
{
	unsigned int p=0,j=0;
	for(p=0;p<700;p++)
	{
		j=d;
		while(j--);
	}
}
/***********************************************************************************************************************
* Function Name: R_RTC_Create
* Description  : This function initializes the RTC module.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_RTC_Create(void)
{
	
    volatile uint8_t tmp;
    unsigned char stat;
    /* Supply clock to access RTC domain register */
	/* Enables input clock supply */
	VRTCEN = 1U;    

	/* A 20 ms delay is required for proper initialization of Clock supply */
	DelayMs(20);
	
	/* Check for Low Voltage, if VRTC < 2.16 V then not to init RTC */

		RTCRMK = 1U;    /* disable INTRTCRPD interrupt */
		RTCRIF = 0U;    /* clear INTRTCRPD interrupt flag */
		RTCAMK = 1U;    /* disable INTRTCALM interrupt */
		RTCAIF = 0U;    /* clear INTRTCALM interrupt flag */

		/* Set INTRTCRPD low priority */
		RTCRPR1 = 1U;
		RTCRPR0 = 1U;

		    	
			
			/* Selecting Sub System Clock 32.768 Khz as a Count Clock */
			RCR4 = 0x00;//_00_RTC_SELECT_FSUB;
			
			/* Enable RTC Event Capture function */ 
//			tmp = BIT0;
//			RCR3 = tmp;
			
//			/* 1 will be return if no error and 0 is return if 
//			register value not updated in 50 ms */
//			stat = CheckForSync(&(RCR3),tmp,50);
//			if(stat==0)
//			  return 2;
			
			/* Stop all counters : START is bit 0 */
			tmp = RCR2;  
			tmp &= (~BIT0);
			RCR2 = tmp;
			
			/* 1 will be return if no error and 0 is return if 
			register value not updated in 20 ms */
			stat = CheckForSync(&(RCR2),tmp,20);
			if(stat==0)
			  return;
			
			/* Select RTC in Calender Count Mode */
			tmp = RCR2;  
			tmp &= (~BIT7);
			RCR2 = tmp;
			
			/* 1 will be return if no error and 0 is return if 
			register value not updated in 20 ms */
			stat = CheckForSync(&(RCR2),tmp,20);
			if(stat==0)
			  return;
			
			/* Execute RTC software reset : RESET is bit 1 */
			tmp = RCR2;  
			tmp |= BIT1;
			RCR2 = tmp;
			DelayMs(40);
			tmp = RCR2;
			/* Check Reset bit for 0, because reading this bit 1 defines reset is pending */
			tmp &= (~BIT1);  
			
			/* 1 will be return if no error and 0 is return if 
			register value not updated in 20 ms */
			stat = CheckForSync(&(RCR2),tmp,20);
			if(stat==0)
			  return;
			
			/* Set control registers */
			tmp = _D0_RTC_INTRTCPRD_CLOCK_7 | _00_RTC_1HZ_OUTPUT | _04_RTC_PERIODIC_ENABLE | _00_RTC_ALARM_DISABLE;
			RCR1 = tmp;
			
			/* 1 will be return if no error and 0 is return if 
			register value not updated in 20 ms */
			stat = CheckForSync(&(RCR1),tmp,20);
			if(stat==0)
			  return;
			
			tmp = 0x40|0x20|0x10;// | _40_RTC_24HOUR_MODE | _20_RTC_CALENDER_10SECONDS | _10_RTC_ADJUSTMENT_ENABLE | _00_RTC_RTCOUT_DISABLE;
			RCR2 = tmp;
			
			/* 1 will be return if no error and 0 is return if 
			register value not updated in 20 ms */
			stat = CheckForSync(&(RCR2),tmp,20);
			if(stat==0)
			  return;
			
			/* Unlock, set and lock the RCR5 */
			RCR5GD = 0x00U;
			RCR5GD = 0x72U;
			RCR5GD = 0x64U;
			RCR5 = 0x00U;
			RCR5GD = 0x00U;
			 /* Start the RTC once  */
			tmp = RCR2;  
			tmp |= BIT0;
			RCR2 = tmp;
			
			/* 1 will be return if no error and 0 is return if 
			register value not updated in 20 ms */
			stat = CheckForSync(&(RCR2),tmp,20);
			if(stat==0)
			  return;

			/* Set RTC initialization done */
			RTCPORSR = 1U;				

    /* Set RTCIC1 pin */
 //   PM15 |= 0x02U;
    /* Set RTCIC2 pin */
 //   PM15 |= 0x04U;

//    volatile uint8_t tmp;
    
//    /* Supply clock to access RTC domain register */
//    VRTCEN = 1U;
    
//    /* Disable interrupts */
//    RTCAMK = 1U;
//    RTCAIF = 0U;
//    RTCRMK = 1U;
//    RTCRIF = 0U;
    
//    /* Set INTRTCRPD low priority */
//    RTCRPR1 = 1U;
//    RTCRPR0 = 1U;
    
//    /* Set ALM interrupt priority low level */
//    RTCAPR1 = 1U;
//    RTCAPR0 = 1U;
//    /* Set RPD interrupt priority low level */
//    RTCRPR1 = 1U;
//    RTCRPR0 = 1U;
    
//    /* Initialization when POR */
//    if (RTCPORSR == 0)
//    {
//        /* Only set register if VRTC enough */
//        if ( LVDVRTCF == 0 )
//        {
//            /* If this is power on reset, init the time */
//            /* Clock setting to use 32KHz source */
//            RCR4 = _00_RTC_SELECT_FSUB;
            
//            /* Set global RTCIC enable: RTCICEN bit 0 */
//            tmp = RCR3;
//            tmp |= _01_RTC_INPUT_ENABLE;
//            RCR3 = tmp;
            
//            /* Stop all counters : START is bit 0 */
//            tmp = RCR2;
//            tmp &= (uint8_t)~_01_RTC_COUNTER_NORMAL;
//            RCR2 = tmp;
//            while (RCR2 != tmp);
            
//            /* Select count mode : CNTMD is bit 7 */
//            tmp = RCR2;
//            tmp &= (uint8_t)~_80_RTC_BINARY_MODE;
//            RCR2 = tmp;
//            while (RCR2 != tmp);
            
//            /* Execute RTC software reset : RESET is bit 1 */
//            tmp = RCR2;
//            /* Clear bits that will be cleared by RTC Software Reset */
//            tmp &= (uint8_t)~(_20_RTC_CALENDER_10SECONDS | _10_RTC_ADJUSTMENT_ENABLE | _04_RTC_WRITER_EXECUTED);
//            /* Start reset */
//            tmp |= _02_RTC_RESET_WRITER_INITIALIZED;
//            RCR2 = tmp;
//            /* Reset bit will be clear to 0 after finish reset, clear this bit to check */
//            tmp &= (uint8_t)~_02_RTC_RESET_WRITER_INITIALIZED;
//            while (RCR2 != tmp);
            
//            /* Set control registers 1 */
//            tmp = _04_RTC_PERIODIC_ENABLE | _D0_RTC_INTRTCPRD_CLOCK_7 | _00_RTC_1HZ_OUTPUT | _00_RTC_ALARM_DISABLE;
//            RCR1 = tmp;
//            while (RCR1 != tmp);
            
//            /* Set control registers 2 */
//            tmp = _00_RTC_CALENDER_MODE | _40_RTC_24HOUR_MODE | _00_RTC_RTCOUT_DISABLE;
//            RCR2 = tmp;
//            while (RCR2 != tmp);
            
//            /* Set noise filter  */
////            RTCICNFEN = _00_RTC_RTCIC2_FILTER_OFF | _00_RTC_RTCIC1_FILTER_OFF;
            
//            /* Set time capture control register 0: exclude capture event */
////            tmp = RTCCR0_OPERATION_STATE | RTCCR0_NOISE_FILTER_CONFIG;
////            RTCCR0 = tmp;
////            while (RTCCR0 != tmp);

//            /* Set time capture control register 1: exclude capture event */
//            tmp = RTCCR1_OPERATION_STATE | RTCCR1_NOISE_FILTER_CONFIG;
////            RTCCR1 = tmp;
////            while (RTCCR1 != tmp);

//            /* Set time capture control register 2: exclude capture event */
////            tmp = RTCCR2_OPERATION_STATE | RTCCR2_NOISE_FILTER_CONFIG; 
////            RTCCR2 = tmp;
////            while (RTCCR2 != tmp);
            
//            /* Set time capture 0 event */
////            tmp = RTCCR0 | RTCCR0_CAPTURE_EVENT_EDGE;
////            RTCCR0 = tmp;
////            while ((RTCCR0 & (uint8_t)~_04_RTC_EVENT0_DETECTED) != tmp);

//            /* Set time capture 1 event */
////            tmp = RTCCR1 | RTCCR1_CAPTURE_EVENT_EDGE;
////            RTCCR1 = tmp;
////            while ((RTCCR1 & (uint8_t)~_04_RTC_EVENT1_DETECTED) != tmp);

////            /* Set time capture 2 event */
////            tmp = RTCCR2 | RTCCR2_CAPTURE_EVENT_EDGE; 
////            RTCCR2 = tmp;
////            while ((RTCCR2 & (uint8_t)~_04_RTC_EVENT2_DETECTED) != tmp);
            
//            /* Start the RTC once */
//            tmp = RCR2;
//            tmp |= _01_RTC_COUNTER_NORMAL;
//            RCR2 = tmp;
//            while (RCR2 != tmp);
            
//            /* Unlock, set and lock the RCR5 */
//            RCR5GD = 0x00U;
//            RCR5GD = 0x72U;
//            RCR5GD = 0x64U;
//            RCR5 = 0x00U;
//            RCR5GD = 0x00U;
            
//            /* Mark RTC initialized */
//            RTCPORSR = 1U;
//        }
//      }
    
    /* Set RTCIC1 pin */
//    PM15 |= 0x02U;
    /* Set RTCIC2 pin */
//    PM15 |= 0x04U;
}

/***********************************************************************************************************************
* Function Name: R_RTC_Start
* Description  : This function starts RTC counter.
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
* Description  : This function stops RTC counter.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_RTC_Stop(void)
{
    volatile uint8_t tmp;
    
    /* Disable PRD interrupt */
    RTCRMK = 1U;
    RTCRIF = 0U;
       
    #if 0
    /* Do not stop the RTC, just mask the interrupt */
    tmp = RCR2;
    tmp &= (uint8_t)~_01_RTC_COUNTER_NORMAL;
    RCR2 = tmp;
    #endif /* 0 or 1 */
}

/***********************************************************************************************************************
* Function Name: R_RTC_Restart
* Description  : This function restarts RTC counter.
* Arguments    : counter_write_val -
*                    counter write value
* Return Value : None
***********************************************************************************************************************/
void R_RTC_Restart(rtc_calendarcounter_value_t counter_write_val)
{
    volatile uint8_t tmp;
    volatile uint8_t bcount;
    
    /* Supply clock to access RTC domain register */
    VRTCEN = 1U;
    
    /* Disable interrupts */
    RTCAMK = 1U;
    RTCAIF = 0U;
    RTCRMK = 1U;
    RTCRIF = 0U;

    /* Stop all counters : START is bit 0 */
    tmp = RCR2;
    tmp &= (uint8_t)~_01_RTC_COUNTER_NORMAL;
    RCR2 = tmp;
    while (RCR2 != tmp);
    
    /* Set control registers */
    tmp = _40_RTC_24HOUR_MODE | _00_RTC_RTCOUT_DISABLE;
    RCR2 = tmp;
    while (RCR2 != tmp);
    
    /* Clear TCST */
//    tmp = RTCCR0 & (uint8_t)~_04_RTC_EVENT0_DETECTED;
//    RTCCR0 = tmp;
//    while (RTCCR0 != tmp);
//    tmp = RTCCR1 & (uint8_t)~_04_RTC_EVENT1_DETECTED;
//    RTCCR1 = tmp;
//    while (RTCCR1 != tmp);
//    tmp = RTCCR2 & (uint8_t)~_04_RTC_EVENT2_DETECTED;
//    RTCCR2 = tmp;
//    while (RTCCR2 != tmp);
    
    /* Set time capture control register 0: exclude capture event */
//    tmp = RTCCR0_OPERATION_STATE | RTCCR0_NOISE_FILTER_CONFIG;
//    RTCCR0 = tmp;
//    while (RTCCR0 != tmp);

//    /* Set time capture control register 1: exclude capture event */
//    tmp = RTCCR1_OPERATION_STATE | RTCCR1_NOISE_FILTER_CONFIG;
//    RTCCR1 = tmp;
//    while (RTCCR1 != tmp);

//    /* Set time capture control register 2: exclude capture event */
//    tmp = RTCCR2_OPERATION_STATE | RTCCR2_NOISE_FILTER_CONFIG; 
//    RTCCR2 = tmp;
//    while (RTCCR2 != tmp);
    
//    /* Set time capture 0 event */
//    tmp = RTCCR0 | RTCCR0_CAPTURE_EVENT_EDGE;
//    RTCCR0 = tmp;
//    while ((RTCCR0 & (uint8_t)~_04_RTC_EVENT0_DETECTED) != tmp);

//    /* Set time capture 1 event */
//    tmp = RTCCR1 | RTCCR1_CAPTURE_EVENT_EDGE;
//    RTCCR1 = tmp;
//    while ((RTCCR1 & (uint8_t)~_04_RTC_EVENT1_DETECTED) != tmp);

//    /* Set time capture 2 event */
//    tmp = RTCCR2 | RTCCR2_CAPTURE_EVENT_EDGE; 
//    RTCCR2 = tmp;
//    while ((RTCCR2 & (uint8_t)~_04_RTC_EVENT2_DETECTED) != tmp);
    
    /* Set counter values */
    RSECCNT = counter_write_val.rseccnt;
    RMINCNT = counter_write_val.rmincnt;
    RHRCNT = counter_write_val.rhrcnt;
    RWKCNT = counter_write_val.rwkcnt;
    RDAYCNT = counter_write_val.rdaycnt;
    RMONCNT = counter_write_val.rmoncnt;
    RYRCNT = counter_write_val.ryrcnt;
    
    /* Read back to make sure value is set */
    for (bcount = 0; bcount < 4; bcount++)
    {
        tmp = RSECCNT;
        tmp = RMINCNT;
        tmp = RHRCNT;
        tmp = RWKCNT;
        tmp = RDAYCNT;
        tmp = RMONCNT;
        tmp = RYRCNT;
    }
    
    /* Enable PRD interrupt */
    RTCRIF = 0U;    
    RTCRMK = 0U;

    /* Start RTC counting */
    tmp = RCR2;
    tmp |= _01_RTC_COUNTER_NORMAL;
    RCR2 = tmp;
    while (RCR2 != tmp);
}

/***********************************************************************************************************************
* Function Name: R_RTC_Get_CalendarCounterValue
* Description  : This function get RTC calendar counter value.
* Arguments    : counter_read_val -
*                    counter read pointer
* Return Value : None
***********************************************************************************************************************/
void R_RTC_Get_CalendarCounterValue(rtc_calendarcounter_value_t * const counter_read_val)
{
    /* Supply clock to access RTC domain register */
    VRTCEN = 1U;
    
    counter_read_val->rseccnt = RSECCNT;
    counter_read_val->rmincnt = RMINCNT;
    counter_read_val->rhrcnt  = RHRCNT & 0xBF;
    counter_read_val->rwkcnt  = RWKCNT;
    counter_read_val->rdaycnt = RDAYCNT;
    counter_read_val->rmoncnt = RMONCNT;
    counter_read_val->ryrcnt  = RYRCNT;
}

/***********************************************************************************************************************
* Function Name: R_RTC_Set_CalendarCounterValue
* Description  : This function set RTC calendar counter value.
* Arguments    : counter_write_val -
*                    counter write value
* Return Value : None
***********************************************************************************************************************/
void R_RTC_Set_CalendarCounterValue(rtc_calendarcounter_value_t counter_write_val)
{
    volatile uint8_t tmp;
    volatile uint8_t bcount;
    
    /* Only set register if VRTC enough */
    if (LVDVRTCF == 1)
    {
        return;
    }
    
    /* Supply clock to access RTC domain register */
    VRTCEN = 1U;
    
    /* Stop all counters : START is bit 0 */
    tmp = RCR2;
    tmp &= (uint8_t)~_01_RTC_COUNTER_NORMAL;
    RCR2 = tmp;
    while (RCR2 != tmp);
    
    /* Set counter values */
    RSECCNT = counter_write_val.rseccnt;
    RMINCNT = counter_write_val.rmincnt;
    RHRCNT = counter_write_val.rhrcnt;
    RWKCNT = counter_write_val.rwkcnt;
    RDAYCNT = counter_write_val.rdaycnt;
    RMONCNT = counter_write_val.rmoncnt;
    RYRCNT = counter_write_val.ryrcnt;
    
    /* Read back to make sure value is set */
    for (bcount = 0; bcount < 4; bcount++)
    {
        tmp = RSECCNT;
        tmp = RMINCNT;
        tmp = RHRCNT;
        tmp = RWKCNT;
        tmp = RDAYCNT;
        tmp = RMONCNT;
        tmp = RYRCNT;
    }
    
    /* Restart counting : START is bit 0 */
    tmp = RCR2;
    tmp |= _01_RTC_COUNTER_NORMAL;
    RCR2 = tmp;
    while (RCR2 != tmp);
}

/******************************************************************************
* Function Name: R_RTC_Get_TimeCapture0Value
* Description  : Get RTC time capture value of input channel 0
* Arguments    : None
* Return Value : None
*******************************************************************************/
void R_RTC_Get_TimeCapture0Value(rtc_timecapture_value_t * const counter_read_val)
{
//    volatile uint8_t tmp;
    
//    /* Supply clock to access RTC domain register */
//    VRTCEN = 1U;
    
//    /* Only set register if VRTC enough */
//    if (LVDVRTCF == 1)
//    {
//        return;
//    }
    
//    /* NOTE: before calling this function, if using time capture function
//     * Make sure the TCST is set after RTCIC interrupt
//     * The RTCIC interrupt processing may finished before capture is complete
//    */
    
//    /* Clear TCCT: no detection, to read counter and clear the TCST */
//    tmp = RTCCR0;
//    tmp &= (uint8_t)~_03_RTC_CAPTURE0_BOTH;
//    RTCCR0 = tmp;
//    while (RTCCR0 != tmp);
    
//    /* Read counter */
//    counter_read_val->rseccnt = RSECCP0;
//    counter_read_val->rmincnt = RMINCP0;
//    counter_read_val->rhrcnt  = RHRCP0 & 0xBF;
//    counter_read_val->rdaycnt = RDAYCP0;
//    counter_read_val->rmoncnt = RMONCP0;
    
//    /* Clear TCST for next capture */
//    tmp = RTCCR0;
//    tmp &= (uint8_t)~_04_RTC_EVENT0_DETECTED;
//    RTCCR0 = tmp;  
//    while (RTCCR0 != tmp);
    
//    /* Reset the TCCT */
//    RTCCR0 |= RTCCR0_CAPTURE_EVENT_EDGE;
}

/******************************************************************************
* Function Name: R_RTC_Get_TimeCapture1Value
* Description  : Get RTC time capture value of input channel 1
* Arguments    : None
* Return Value : None
*******************************************************************************/
void R_RTC_Get_TimeCapture1Value(rtc_timecapture_value_t * const counter_read_val)
{
//    volatile uint8_t tmp;
    
//    /* Supply clock to access RTC domain register */
//    VRTCEN = 1U;
    
//    /* Only set register if VRTC enough */
//    if (LVDVRTCF == 1)
//    {
//        return;
//    }
    
//    /* NOTE: before calling this function, if using time capture function
//     * Make sure the TCST is set after RTCIC interrupt
//     * The RTCIC interrupt processing may finished before capture is complete
//    */
    
//    /* Clear TCCT: no detection, to read counter and clear the TCST */
//    tmp = RTCCR1;
//    tmp &= (uint8_t)~_03_RTC_CAPTURE1_BOTH;
//    RTCCR1 = tmp;
//    while (RTCCR1 != tmp);
    
//    /* Read counter */
//    counter_read_val->rseccnt = RSECCP1;
//    counter_read_val->rmincnt = RMINCP1;
//    counter_read_val->rhrcnt  = RHRCP1 & 0xBF;
//    counter_read_val->rdaycnt = RDAYCP1;
//    counter_read_val->rmoncnt = RMONCP1;
    
//    /* Clear TCST for next capture */
//    tmp = RTCCR1 & (uint8_t)~_04_RTC_EVENT1_DETECTED;
//    RTCCR1 = tmp;  
//    while (RTCCR1 != tmp);
    
//    /* Reset the TCCT */
//    tmp = RTCCR1 | RTCCR1_CAPTURE_EVENT_EDGE;
//    RTCCR1 = tmp;
//    while ((RTCCR1 & (uint8_t)~_04_RTC_EVENT1_DETECTED)!= tmp);
}

/******************************************************************************
* Function Name: R_RTC_Get_TimeCapture2Value
* Description  : Get RTC time capture value of input channel 2
* Arguments    : None
* Return Value : None
*******************************************************************************/
void R_RTC_Get_TimeCapture2Value(rtc_timecapture_value_t * const counter_read_val)
{
//    volatile uint8_t tmp;
    
//    /* Supply clock to access RTC domain register */
//    VRTCEN = 1U;
    
//    /* Only set register if VRTC enough */
//    if (LVDVRTCF == 1)
//    {
//        return;
//    }
    
//    /* NOTE: before calling this function, if using time capture function
//     * Make sure the TCST is set after RTCIC interrupt
//     * The RTCIC interrupt processing may finished before capture is complete
//    */
    
//    /* Clear TCCT: no detection, to read counter and clear the TCST */
//    tmp = RTCCR2;
//    tmp &= (uint8_t)~_03_RTC_CAPTURE2_BOTH;
//    RTCCR2 = tmp;
//    while (RTCCR2 != tmp);
    
//    /* Read counter */
//    counter_read_val->rseccnt = RSECCP2;
//    counter_read_val->rmincnt = RMINCP2;
//    counter_read_val->rhrcnt  = RHRCP2 & 0xBF;
//    counter_read_val->rdaycnt = RDAYCP2;
//    counter_read_val->rmoncnt = RMONCP2;
    
//    /* Clear TCST for next capture */
//    tmp = RTCCR2 & (uint8_t)~_04_RTC_EVENT2_DETECTED;
//    RTCCR2 = tmp;  
//    while (RTCCR2 != tmp);
    
//    /* Reset the TCCT */
//    tmp = RTCCR2 | RTCCR2_CAPTURE_EVENT_EDGE;
//    RTCCR2 = tmp;
//    while ((RTCCR2 & (uint8_t)~_04_RTC_EVENT2_DETECTED)!= tmp);
}

/***********************************************************************************************************************
* Function Name: R_RTC_Set_ConstPeriodInterruptOn
* Description  : This function set RTC constant period interrupt and enable the interrupt(PRD).
* Arguments    : period -
*                    new period of constant period interrupt
* Return Value : None
***********************************************************************************************************************/
void R_RTC_Set_ConstPeriodInterruptOn(rtc_int_period_t period)
{
    volatile uint8_t tmp;
    
    /* Only set register if VRTC enough */
    if (LVDVRTCF == 1)
    {
        return;
    }
    
    /* Supply clock to access RTC domain register */
    VRTCEN = 1U;
    
    /* Disable PRD interrupt */
    RTCRMK = 1U;
    RTCRIF = 0U;
    
    tmp = (uint8_t)(period | (RCR1 & 0x0FU) | _04_RTC_PERIODIC_ENABLE);
    RCR1 = tmp;
  //  while (RCR1 != tmp);
    DelayMs(50);
    /* Enable PRD interrupt */
    RTCRMK = 0U;
}

/***********************************************************************************************************************
* Function Name: R_RTC_Set_ConstPeriodInterruptOff
* Description  : This function disable RTC constant period interrupt(PRD).
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_RTC_Set_ConstPeriodInterruptOff(void)
{
    volatile uint8_t tmp;
    
    /* Only set register if VRTC enough */
    if (LVDVRTCF == 1)
    {
        return;
    }
    
    /* Supply clock to access RTC domain register */
    VRTCEN = 1U;
    
    /* Disable PRD interrupt */
    RTCRMK = 1U;
    RTCRIF = 0U;
    
    tmp = RCR1;
    tmp &= (uint8_t)~_04_RTC_PERIODIC_ENABLE;
    RCR1 = tmp;
    while (RCR1 != tmp);
}

/***********************************************************************************************************************
* Function Name: R_RTC_Set_ClockOutputOn
* Description  : This function enable and set the frequency of clock output
* Arguments    : clk_out -
*                    new clock frequency output
* Return Value : None
***********************************************************************************************************************/
void R_RTC_Set_ClockOutputOn(rtc_clock_out_t clk_out)
{
    volatile uint8_t tmp;
    
    /* Only set register if VRTC enough */
    if (LVDVRTCF == 1)
    {
        return;
    }
    
    /* Supply clock to access RTC domain register */
    VRTCEN = 1U;
    
    /* Stop all counters : START is bit 0 */
    tmp = RCR2;
    tmp &= (uint8_t)~_01_RTC_COUNTER_NORMAL;
    RCR2 = tmp;
    while (RCR2 != tmp);
    
    /* Disable clock output before setting RTCOS */
    tmp = RCR2;
    tmp &= (uint8_t)~_08_RTC_RTCOUT_ENABLE;
    RCR2 = tmp;
    
    tmp = RCR1;
    /* Set RTCOS */
    if (clk_out == RTCOS_FREQ_1HZ)
    {
        tmp &= (uint8_t)~_08_RTC_64HZ_OUTPUT;
    }
    else
    {
        tmp |= _08_RTC_64HZ_OUTPUT;
    }
    RCR1 = tmp;
    
    /* RTCOE enable */
    tmp = RCR2;
    tmp |= _08_RTC_RTCOUT_ENABLE;
    RCR2 = tmp;
    
    /* Restart counting : START is bit 0 */
    tmp = RCR2;
    tmp |= _01_RTC_COUNTER_NORMAL;
    RCR2 = tmp;
    while (RCR2 != tmp);
}

/***********************************************************************************************************************
* Function Name: R_RTC_Set_ClockOutputOff
* Description  : This function disable RTC clock output
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_RTC_Set_ClockOutputOff(void)
{
    volatile uint8_t tmp;
    
    /* Only set register if VRTC enough */
    if (LVDVRTCF == 1)
    {
        return;
    }
    
    /* Supply clock to access RTC domain register */
    VRTCEN = 1U;
    
    /* Stop all counters : START is bit 0 */
    tmp = RCR2;
    tmp &= (uint8_t)~_01_RTC_COUNTER_NORMAL;
    RCR2 = tmp;
    while (RCR2 != tmp);
    
    /* RTCOE disable */
    tmp = RCR2;
    tmp &= (uint8_t)~_08_RTC_RTCOUT_ENABLE;
    RCR2 = tmp;
    
    /* Restart counting : START is bit 0 */
    tmp = RCR2;
    tmp |= _01_RTC_COUNTER_NORMAL;
    RCR2 = tmp;
    while (RCR2 != tmp);
}

/******************************************************************************
* Function Name: R_RTC_Compensate
* Description  : This function compensate the error on sub-clock by a provided ppm
* Arguments    : float ppm: ppm error of sub-clock crystal
* Return Value : None
*******************************************************************************/
void R_RTC_Compensate(float32_t ppm)
{
    volatile uint8_t tmp;
    float32_t abs_ppm = ABS(ppm);
    uint8_t radj_reg = 0;
    
    /* Only set register if VRTC enough */
    if (LVDVRTCF == 1)
    {
        return;
    }
    
    /* Supply clock to access RTC domain register */
    VRTCEN = 1U;

    /* This feature only run on 32.768KHz clock source : RCKSEL is bit 0 */
    if ((RCR4 & _00_RTC_SELECT_FSUB) == 0)
    {
        /* Clear adjustment first : PMADJ bit [7:6] */
        while ((RADJ & _C0_RTC_TIMER_ERROR_ADJUST_ALL) != 0)
        {
            RADJ &= (uint8_t)~_C0_RTC_TIMER_ERROR_ADJUST_ALL;
        }
        
        /* Enable automatic adj : AADJE bit 4 */
        tmp = RCR2;
        tmp |= _10_RTC_ADJUSTMENT_ENABLE;
        RCR2 = tmp;
        while(RCR2 != tmp);
        
        /* Enable sub or add */
        if (ppm > 0.0f)
        {
            radj_reg |= _80_RTC_TIMER_ERROR_ADJUST_MINUS;       /* sub */
        }
        else
        {
            radj_reg |= _40_RTC_TIMER_ERROR_ADJUST_PLUS;        /* add */
        }
        
        tmp = RCR2;
        if (abs_ppm <= 32.043f)         /* 1 minutes adjustment */
        {
            tmp &= (uint8_t)~_20_RTC_CALENDER_10SECONDS;    
            radj_reg |= ( (uint8_t)(abs_ppm * (32768.0f * 60.0f / 1e6) + 0.5f) ) & 0x3F;
        }
        else if (abs_ppm <= 192.26f)    /* 10-second adjustment */
        {
            tmp |= _20_RTC_CALENDER_10SECONDS;    
            radj_reg |= ( (uint8_t)(abs_ppm * (32768.0f * 10.0f / 1e6) + 0.5f) ) & 0x3F;
        }
        else
        {
            /* Out of control !! no adj at all */
            tmp &= (uint8_t)~(_20_RTC_CALENDER_10SECONDS | _10_RTC_ADJUSTMENT_ENABLE);
            radj_reg = 0;
        }
        
        RCR2 = tmp;
        while (RCR2 != tmp);
        
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
    
    /* Only do the compensation if VRTC enough */
    if (LVDVRTCF == 1)
    {
        return;
    }
    
    /* Supply clock to access RTC domain register */
    VRTCEN = 1U;
    
    /* AADJE bit 4, AADJP is bit 5 */
    tmp = RCR2;
    tmp &= (uint8_t)~(_20_RTC_CALENDER_10SECONDS | _10_RTC_ADJUSTMENT_ENABLE);
    RCR2 = tmp;
    while(RCR2 != tmp);
    
    while (RADJ != 0)
    {
        RADJ = 0;
    }
}

/***********************************************************************************************************************
* Function Name: R_RTC_Set_AlarmValue
* Description  : This function sets alarm value.
* Arguments    : alarm_val -
*                    the expected alarm value(BCD code)
* Return Value : None
***********************************************************************************************************************/
void R_RTC_Set_AlarmValue(rtc_calendarcounter_value_t time_value, rtc_alarm_enable_t control)
{
    volatile uint8_t tmp;
    volatile uint8_t bcount;
    
    /* Supply clock to access RTC domain register */
    VRTCEN = 1U;
    
    /* Disable RTC alarm operation */
    tmp = RCR1;
    tmp &= (uint8_t)~_01_RTC_ALARM_ENABLE;
    RCR1 = tmp;
    while (RCR1 != tmp);

    RSECAR  = time_value.rseccnt   | (uint8_t)(control.sec_enb << 7);
    RMINAR  = time_value.rmincnt   | (uint8_t)(control.min_enb << 7);
    RHRAR   = time_value.rhrcnt    | (uint8_t)(control.hr_enb  << 7);
    RDAYAR  = time_value.rdaycnt   | (uint8_t)(control.day_enb << 7);
    RWKAR   = time_value.rwkcnt    | (uint8_t)(control.wk_enb  << 7);
    RMONAR  = time_value.rmoncnt   | (uint8_t)(control.mon_enb << 7);
    RYRAR   = time_value.ryrcnt;
    RYRAREN = (uint8_t)(control.yr_enb << 7);
    
    /* Read back to make sure value is set */
    for (bcount = 0; bcount < 4; bcount++)
    {
        tmp = RSECAR;
        tmp = RMINAR;
        tmp = RHRAR;
        tmp = RDAYAR;
        tmp = RWKAR;
        tmp = RMONAR;
        tmp = RYRAR;
        tmp = RYRAREN;
    }
    
    /* Must leave AIE = 0 when PIE = 1
     * If set, the vector may interfere and cause 1sec delay
    */
}

/***********************************************************************************************************************
* Function Name: R_RTC_Get_AlarmValue
* Description  : This function gets alarm value.
* Arguments    : alarm_val -
*                    the address to save alarm value(BCD code)
* Return Value : None
***********************************************************************************************************************/
void R_RTC_Get_AlarmValue(rtc_calendarcounter_value_t * const time_value, rtc_alarm_enable_t * const control)
{
    volatile uint8_t tmp;
    
    /* Supply clock to access RTC domain register */
    VRTCEN = 1U;
    
    /* Disable RTC alarm operation */
    tmp = RCR1;
    tmp &= (uint8_t)~_01_RTC_ALARM_ENABLE;
    RCR1 = tmp;
    while (RCR1 != tmp);

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
    
    /* Must leave AIE = 0 when PIE = 1
     * If set, the vector may interfere and cause 1sec delay
    */
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
