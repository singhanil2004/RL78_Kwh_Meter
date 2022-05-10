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
* File Name    : wrp_em_shared_config.h
* Version      : 1.00
* Device(s)    : RL78/I1C
* Tool-Chain   : CCRL
* H/W Platform : RL78/I1C Energy Meter Platform
* Description  : Wrapper EM Shared Configuration Header file
***********************************************************************************************************************/

#ifndef _WRAPPER_EM_SHARED_CONFIG_H
#define _WRAPPER_EM_SHARED_CONFIG_H

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"

/***********************************************************************************************************************
*   EM Core Driver Shared Block
************************************************************************************************************************/
    /**************************************************************************
    *   MCU Driver Header
    ***************************************************************************/
    
    /**************************************************************************
    *   WDT Driver Header
    ***************************************************************************/
    #include "r_cg_wdt.h"
    
    /**************************************************************************
    *   EM TIMER Driver Header
    ***************************************************************************/
    #include "r_cg_tau.h"
    
    /**************************************************************************
    *   RTC Driver Header
    ***************************************************************************/
//    #include "r_cg_rtc.h"
    
    
/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
*   EM Core Shared Block
************************************************************************************************************************/
    /**************************************************************************
    *   MCU Wrapper Configuration for Settings & I/F mapping
    ***************************************************************************/
    /* I/Fs mapping
     *      Macro Name / I/Fs                                   Setting/Description */
    #define EM_MCU_MULTIPLE_INT_ENABLE_STATEMENT                {EI();}                         /* Multiple interrupt enable statement */
    #define EM_MCU_MULTIPLE_INT_DISABLE_STATEMENT               {DI();}                         /* Multiple interrupt disable statement */

    /**************************************************************************
    *   WDT Wrapper Configuration for Settings & I/F mapping
    ***************************************************************************/
    /* I/Fs mapping
     *      Macro Name / I/Fs                                   Setting/Description */
    #define EM_WDT_DriverInit()                                 R_WDT_Create()                  /* WDT Init */
    #define EM_WDT_DriverStart()                                {;}                             /* WDT Start */
    #define EM_WDT_DriverStop()                                 {;}                             /* WDT Stop */
    #define EM_WDT_DriverRestart()                              R_WDT_Restart()                 /* WDT Restart */

    /**************************************************************************
    *   EM TIMER Wrapper Configuration for Settings & I/F mapping
    ***************************************************************************/
    /* I/Fs mapping
     *      Macro Name / I/Fs                                   Setting/Description */
    #define EM_TIMER_DriverInit()                               {;}                             /* Init */
    #define EM_TIMER_DriverStart()                              R_TAU0_Channel2_Start()         /* Start */
    #define EM_TIMER_DriverStop()                               R_TAU0_Channel2_Stop()          /* Stop */

    /**************************************************************************
    *   RTC Wrapper Configuration for Settings & I/F mapping
    ***************************************************************************/
    /* I/Fs mapping
     *      Macro Name / I/Fs                                   Setting/Description */
    #define EM_RTC_DriverInit()                                 {;}                             /* Init */
    #define EM_RTC_DriverStart()                                R_RTC_Start()                   /* Start */
    #define EM_RTC_DriverStop()                                 {;}                             /* Stop */
    #define EM_RTC_DriverCounterGet(datetime)                   R_RTC_Get_CalendarCounterValue(datetime)                            /* RTC Get Counter Value BCD Format */

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Variable Externs
***********************************************************************************************************************/

/***********************************************************************************************************************
Functions Prototypes
***********************************************************************************************************************/

#endif /* _WRAPPER_CORE_CONFIG_H */
