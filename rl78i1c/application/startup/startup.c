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
* File Name    : startup.c
* Version      : 1.00
* Device(s)    : RL78/I1C
* Tool-Chain   : CCRL
* H/W Platform : RL78/I1C Energy Meter Platform
* Description  : Start-up source File
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
/* Driver */
#include "r_cg_macrodriver.h"   /* CG Macro Driver */
#include "r_cg_sau.h"           /* Serial Driver */
#include "r_cg_lcd.h"           /* LCD Driver */
#include "r_cg_rtc.h"           /* RTC Driver */
//#include "r_cg_intp.h"          /* INTP Driver */
#include "r_cg_dsadc.h"
#include "r_cg_adc.h"
#include "r_cg_tau.h"
#include "r_cg_it.h"           /* Interval timer */
#include "r_cg_lvd.h"
//#include "r_cg_vbatt.h"
//#include "r_cg_iica.h"
#include "r_cg_wdt.h"

/* Wrapper/User */
#include "wrp_em_adc_data.h"

/* MW/Core */
#include "em_core.h"            /* EM Core APIs */

/* Application */
#include "platform.h"           /* Default Platform Information Header */
#include "led.h"                /* LED Interface */
#include "config_storage.h"    /* Configuration on chip */
#include "startup.h"            /* Startup Header File */
#include "dataflash.h"

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#define PHM_INIT_OK         0   /* Initial OK */
#define PHM_INIT_ERROR      1   /* Initial Error */
#define PHM_START_OK        0   /* Start OK */
#define PHM_START_ERROR     1   /* Start Error */

#define TRAP                0x80
#define WDTRF               0x10
#define RPERF               0x04
#define IAWRF               0x02
#define LVIRF               0x01

/***********************************************************************************************************************
Imported global variables and functions (from other files)
***********************************************************************************************************************/

/***********************************************************************************************************************
Exported global variables and functions (to be accessed by other files)
***********************************************************************************************************************/
uint8_t g_reset_flag;
uint8_t g_softreset_flag;

/***********************************************************************************************************************
Private global variables and functions
***********************************************************************************************************************/
#ifdef __DEBUG
static const uint8_t line_break[]   = "+------------------------------------------------------------------------+\n\r";
static const uint8_t param_intro[] = "| %-21s %26s %-21s |\n\r";
static const uint8_t param_text[] = "|     %-67s|\n\r";
static const uint8_t param_info_s[] = "|      . %-22s: %-40s|\n\r";
static const uint8_t param_info_i[] = "|      . %-22s: %-40d|\n\r";
static const uint8_t param_info_date[] = "|      . %-22s: %02x/%02x/20%02x %-29s|\n\r";


static const uint8_t param_itemgroup_start[] = "| %2d. %-54s             |\n\r";
static const uint8_t param_item_start[] = "| %2d. %-51s ";
static const uint8_t param_subitem_start[] = "|     . %-49s ";
static const uint8_t param_subitem_info_start[] = "|     . %-21s ";
static const uint8_t param_subitem_info_middle_rtc[] = "%02x/%02x/20%02x %02x:%02x:%02x %-7s ";
static const uint8_t param_item_end_normal[] = "\x1b[32m[%12s]\x1b[0m |\n\r";
static const uint8_t param_item_end_abnormal[] = "\x1b[31m[%12s]\x1b[0m |\n\r";

#else
static const uint8_t line_break[1];
static const uint8_t param_intro[1];
static const uint8_t param_text[1];
static const uint8_t param_info_s[1];
static const uint8_t param_info_i[1] ;
static const uint8_t param_info_date[1] ;


static const uint8_t param_itemgroup_start[1] ;
static const uint8_t param_item_start[1];
static const uint8_t param_subitem_start[1] ;
static const uint8_t param_subitem_info_start[1] ;
static const uint8_t param_subitem_info_middle_rtc[1];
static const uint8_t param_item_end_normal[1];
static const uint8_t param_item_end_abnormal[1];
#endif /* __DEBUG */

static uint8_t phminit(void);
static uint8_t phmstart(void);
static uint8_t meter_data_load(uint8_t init_status);
static uint8_t config_data_load(uint8_t init_status);

/***********************************************************************************************************************
* Function Name    : static uint8_t phminit(void)
* Description      : Initial Peripheral device & module
* Arguments        : None
* Functions Called : EM_IEC1107_Init()
* Return Value     : Execution Status
*                  :    PHM_INIT_OK             Initial peripheral OK
*                  :    PHM_INIT_ERROR          Initial peripheral error
***********************************************************************************************************************/
static uint8_t phminit(void)
{
    return PHM_INIT_OK;
}

/***********************************************************************************************************************
* Function Name    : static uint8_t phmstart(void)
* Description      : Start Peripheral device & module
* Arguments        : None
* Functions Called : DEBUG_Printf()
*                  : RTC_CounterEnable()
*                  : RTC_CounterGet()
*                  : INTP0_Enable()
*                  : KEY_Enable()
*                  : EM_IEC1107_Start()
* Return Value     : Execution Status
*                  :    PHM_START_OK            Start peripheral OK
*                  :    PHM_START_ERROR         Start peripheral error
***********************************************************************************************************************/
static uint8_t phmstart(void)
{
    uint8_t                         status;
    uint16_t                        timeout;
    //    rtc_calendarcounter_value_t     rtctime;
    
    /* Start & check RTC */
    //  R_RTC_Start();
    //    R_RTC_Set_ConstPeriodInterruptOn(PES_1_2_SEC);
    
    /* Check RTC module */
    //R_RTC_Get_CalendarCounterValue(&rtctime);
    status = MD_OK;
    
    if (status != MD_OK)    /* RTC start OK? */
    {
    
        /* When RTC can start, we need to beak the start-up process here
         * because the EM can't run without RTC */
        return PHM_START_ERROR;
    }
    
    
    /* Success */
    return PHM_START_OK;
}
/***********************************************************************************************************************
* Function Name    : static uint8_t config_data_load(uint8_t init_status)
* Description      : Load Configuration Page from MCU DataFlash memory
* Arguments        : uint8_t init_status: initialization status
* Return Value     : Execution Status
*                  :    CONFIG_OK         Load data OK
*                  :    CONFIG_ERROR         Load data error
***********************************************************************************************************************/
static uint8_t config_data_load(uint8_t init_status)
{
    /* Check device format to get out config data */
    
    /* Check device format */
    if (init_status == CONFIG_OK)       /* Initial successful, already formatted */
    {
        if (CONFIG_Restore(CONFIG_ITEM_CONFIG) != CONFIG_OK)
        {
            NOP();
        }
        
        if (CONFIG_Restore(CONFIG_ITEM_CALIB) != CONFIG_OK)
        {
            NOP();
        }
        
        
        if (CONFIG_Restore(CONFIG_ITEM_USER) != CONFIG_OK)
        {
            NOP();
        }
        
        if (CONFIG_Restore(CONFIG_ITEM_MAINS_ENG) != CONFIG_OK)
        {
            NOP();
        }
        
        if (CONFIG_Restore(CONFIG_ITEM_PERIODIC_ENG) != CONFIG_OK)
        {
            NOP();
        }
        
    }
    /* Initial successful, but not formatted */
    else if (init_status == CONFIG_NOT_FORMATTED)
    {
        /* Format device */
        if (CONFIG_Format() != CONFIG_OK)
        {
            /* When format fail,
             * we need to beak the start-up process here */
            return CONFIG_ERROR;   /* Format fail */
        }
        else    /* format ok */
        {
            /* First backup all configuration data */
            if (CONFIG_Backup(CONFIG_ITEM_ALL) != CONFIG_OK)
            {
                return CONFIG_ERROR;   /* Fisrt backup not success */
            }
        }
    }
    
    /* Load data sucessfully */
    return CONFIG_OK;
}

/***********************************************************************************************************************
* Function Name    : uint8_t startup(void)
* Description      : Start-up energy meter
* Arguments        : None
* Functions Called : DEBUG_Printf()
*                  : EM_Init()
*                  : phminit()
*                  : phmstart()
*                  : STORAGE_Init()
*                  : EVENT_RegisterAll()
*                  : RTC_CounterGet()
*                  : STORAGE_CheckDeviceFormat()
*                  : STORAGE_FormatDevice()
*                  : STORAGE_Backup()
*                  : STORAGE_Restore()
*                  : EM_Start()
* Return Value     : Execution Status
*                  :    STARTUP_OK              Start-up Ok
*                  :    STARTUP_ERROR           Startup error
***********************************************************************************************************************/
uint8_t startup_bios_check(void)
{
    //    volatile rtc_calendarcounter_value_t     rtctime;
    volatile uint8_t                         config_initial_status;
    
    
    /*****************************************************************
    * Initial all
    ******************************************************************/
    /* Initial all peripheral & device */
    if (phminit() != PHM_INIT_OK)
    {
        return STARTUP_ERROR;
    }
    
    /* At here, Peripheral OK */
    
    /* Initial configuration page */
    /* Not using dataflash if power by battery */
    if (VBATCMPM == 0)
    {
        config_initial_status = CONFIG_Init(1);
    }
    else
    {
    
        return STARTUP_ERROR;
    }
    
    if (config_initial_status == CONFIG_ERROR)
    {
    
    
        /* When CONFIG_Init fail, we need to beak the start-up process here
         * because the meter can't without dataflash memory */
        return STARTUP_ERROR;
    }
    
    /* Init default DSAD Channel Gain */
#if (EM_SW_PROPERTY_ADC_GAIN_NUMBER_LEVEL_PHASE_R == 1)
    R_DSADC_SetChannelGain(EM_ADC_CURRENT_DRIVER_CHANNEL_PHASE_R, R_DSADC_GetGainEnumValue(EM_CALIB_DEFAULT_GAIN_PHASE_R));
#endif /* EM_SW_PROPERTY_ADC_GAIN_NUMBER_LEVEL_PHASE_R == 1 */
#if (EM_SW_PROPERTY_ADC_GAIN_NUMBER_LEVEL_PHASE_Y == 1)
    R_DSADC_SetChannelGain(EM_ADC_CURRENT_DRIVER_CHANNEL_PHASE_Y, R_DSADC_GetGainEnumValue(EM_CALIB_DEFAULT_GAIN_PHASE_Y));
#endif /* EM_SW_PROPERTY_ADC_GAIN_NUMBER_LEVEL_PHASE_Y == 1 */
#if (EM_SW_PROPERTY_ADC_GAIN_NUMBER_LEVEL_PHASE_B == 1)
    R_DSADC_SetChannelGain(EM_ADC_CURRENT_DRIVER_CHANNEL_PHASE_B, R_DSADC_GetGainEnumValue(EM_CALIB_DEFAULT_GAIN_PHASE_B));
#endif /* EM_SW_PROPERTY_ADC_GAIN_NUMBER_LEVEL_PHASE_B == 1 */
    R_DSADC_SetChannelGain(EM_ADC_CURRENT_DRIVER_CHANNEL_NEUTRAL, R_DSADC_GetGainEnumValue(EM_CALIB_DEFAULT_GAIN_NEUTRAL));
    
    /* Init default ADC Timer offset value */
    EM_ADC_SAMP_SetTimerOffset(EM_CALIB_DEFAULT_TIMER_OFFSET);
    
    /* Initial EM by default property, configuration, calibration from MCU ROM */
    if (EM_Init((EM_PLATFORM_PROPERTY*)&g_EM_DefaultProperty,
                (EM_CONFIG*)&g_EM_DefaultConfig,
                (EM_CALIBRATION*)&g_EM_DefaultCalibration) != EM_OK)        /* EM initial with default info on MCU OK? */
    {
    
    
        /* When EM_Init fail, we need to beak the start-up process here
         * because the EM default information (property, configuration, calibration)
         * on MCU has been corruptted, need to check & modify the platform.h information */
        return STARTUP_ERROR;
    }
    
    
    /*****************************************************************
    * Start all peripheral, device, EM Core
    ******************************************************************/
    
    if (phmstart() != PHM_START_OK)
    {
        /* When phmstart() fail, we need to beak the start-up process here */
        return STARTUP_ERROR;
    }
    
    /* At here, Peripheral OK, RTC OK, EM Init (default) OK, Storage Device OK */
    
    /*****************************************************************
    * Load meter and configuration data
    ******************************************************************/
    
    /* Load the meter configuration data */
    if (config_data_load(config_initial_status) != CONFIG_OK)
    {
        return STARTUP_ERROR;
    }
    
    /*****************************************************************
    * Start the EM
    ******************************************************************/
    /* Start EM Core */
    //R_TAU0_Channel0_Start();
    if (EM_Start() != EM_OK)
    {
        return STARTUP_ERROR;
    }
    
    /* Start other peripherals */
    //R_RTC_Start();
    //R_RTC_Set_ConstPeriodInterruptOn(PES_1_2_SEC);
    /* LVD */
    //R_LVD_Start_VRTC();
    /* UART */
    R_UART2_Start();
    /* Interval timer of 1 ms */
    R_IT_Start();
    
    return STARTUP_OK;
}

/***********************************************************************************************************************
* Function Name    : uint8_t startup(void)
* Description      : Start-up energy meter
* Arguments        : None
* Functions Called : State selection for startup
* Return Value     : Execution Status
*                  :    STARTUP_OK              Start-up Ok
*                  :    STARTUP_ERROR           Startup error
***********************************************************************************************************************/
uint8_t startup(void)
{
    uint8_t status;
    
    g_reset_flag = RESF;
    
    /* Power-on reset, startup with bios checking */
    status = startup_bios_check();
    
    return status;
}