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
* File Name    : wrp_em_rtc.c
* Version      : 1.00
* Device(s)    : RL78/I1C
* Tool-Chain   : CCRL
* H/W Platform : RL78/I1C Energy Meter Platform
* Description  : Wrapper RTC APIs
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include "wrp_em_shared_config.h"   /* Wrapper Shared Configuration */
#include "wrp_em_rtc.h"             /* EM RTC Wrapper Layer */
#include "r_cg_rtc.h"
/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

/* Internal use of module (do not change) */
#define     _BCD2DEC(x)     (x = (x & 0xF) + (x >> 4) * 10)     /* 1 byte BCD to DEC Conversion */

/***********************************************************************************************************************
Macro Definitions Checking
***********************************************************************************************************************/

/***********************************************************************************************************************
Imported global variables and functions (from other files)
***********************************************************************************************************************/

/***********************************************************************************************************************
Exported global variables and functions (to be accessed by other files)
***********************************************************************************************************************/

/***********************************************************************************************************************
Private global variables and functions
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name    : void EM_RTC_Init(void)
* Description      : EM RTC Initialization
* Arguments        : None
* Functions Called : EM_RTC_DriverInit()
* Return Value     : None
***********************************************************************************************************************/
void EM_RTC_Init(void)
{
    EM_RTC_DriverInit();
}

/***********************************************************************************************************************
* Function Name    : void EM_RTC_Start(void)
* Description      : EM RTC Start
* Arguments        : None
* Functions Called : EM_RTC_DriverStart()
* Return Value     : None
***********************************************************************************************************************/
void EM_RTC_Start(void)
{
    //EM_RTC_DriverStart();
    R_TAU0_Channel0_Start();
}

/***********************************************************************************************************************
* Function Name    : void EM_RTC_Stop(void)
* Description      : EM RTC Stop
* Arguments        : None
* Functions Called : EM_RTC_DriverStop()
* Return Value     : None
***********************************************************************************************************************/
void EM_RTC_Stop(void)
{
    //EM_RTC_DriverStop();
    R_TAU0_Channel0_Stop();
}

/***********************************************************************************************************************
* Function Name    : void EM_RTC_GetDateTime(EM_RTC_DATE_TIME *datetime)
* Description      : Get RTC Date time value. The format of [datetime] element value is in
*                  : decimal number format.
* Arguments        : EM_RTC_DATE_TIME* datetime
*                  :    Pointer to a EM_RTC_DATE_TIME struct to get the date time
*                  :    value. All elements values is in decimal format
* Functions Called : EM_RTC_DriverCounterGet()
* Return Value     : None
***********************************************************************************************************************/

void EM_RTC_GetDateTime(EM_RTC_DATE_TIME *datetime)
{
    #if 0
    rtc_calendarcounter_value_t rtctime;
        
    /* Checking input argument */
    if (datetime == NULL)
    {
        return;
    }
    
    /* If valid, get the RTC counter */
    EM_RTC_DriverCounterGet(&rtctime);
    
    datetime->Sec = rtctime.rseccnt;
    datetime->Min = rtctime.rmincnt;
    datetime->Hour = rtctime.rhrcnt; 
    datetime->Day = rtctime.rdaycnt; 
    datetime->Week = rtctime.rwkcnt;
    datetime->Month = rtctime.rmoncnt;
    datetime->Year = (uint8_t)rtctime.ryrcnt;
    
    /* Convert all to decimal */
    _BCD2DEC(datetime->Sec);  
    _BCD2DEC(datetime->Min);  
    _BCD2DEC(datetime->Hour); 
    _BCD2DEC(datetime->Day);  
    _BCD2DEC(datetime->Week); 
    _BCD2DEC(datetime->Month);
    _BCD2DEC(datetime->Year);
    #else
    datetime->Sec = 00;
    datetime->Min = 00;
    datetime->Hour = 12; 
    datetime->Day = 24; 
    datetime->Week = 43;
    datetime->Month = 10;
    datetime->Year = 19;
    #endif
    
}