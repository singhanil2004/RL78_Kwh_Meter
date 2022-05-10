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
* File Name    : config_storage.c
* Version      : 1.00
* Device(s)    : RL78/I1C
* Tool-Chain   : CCRL
* H/W Platform : RL78/I1C Energy Meter Platform
* Description  : Storage Source File
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
/* Driver */
#include "r_cg_macrodriver.h"   /* CG Macro Driver */
//#include "r_cg_rtc.h"           /* CG RTC Driver */
#include "r_cg_wdt.h"           /* CG WDT Driver */
#include "r_cg_crc.h"           /* CG CRC Driver */
#include "r_cg_dsadc.h"         /* CG ADC Driver */

/* Library */
#include <string.h>             /* Compiler standard library */

/* MW */
#include "em_core.h"            /* EM Core */
#include "dataflash.h"          /* Logical Volume Manager */

/* WRP */
#include "wrp_em_adc_data.h"

/* Application */
#include "config_format.h"      /* Storage Format Header */
#include "config_storage.h"     /* Storage Header File */
#include "inst_read.h"
#include "user_config.h"

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/
/* Storage Module State */
typedef enum tagStorageState
{
    CONFIG_STATE_UNINITIALIZED = 0,        /* Uninitialized */
    CONFIG_STATE_INITIALIZED               /* Initialized */
    
} CONFIG_STATE;

typedef struct tagConfigStorageCrcData
{
    EM_CALIBRATION*   p_calibration;
    uint8_t*          p_driver_adc_gain0_list;
    uint8_t*          p_driver_adc_gain1_list;
    float32_t*        p_driver_timer_offset;
    
} config_crc_data;
/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#define CONFIG_STORAGE_INTERVAL_COUNT           (60)

/***********************************************************************************************************************
Imported global variables and functions (from other files)
***********************************************************************************************************************/

/***********************************************************************************************************************
Exported global variables and functions (to be accessed by other files)
***********************************************************************************************************************/
/* Configuration for the dataflash CRC checking area */
const uint16_t g_dataflash_crc_block_addr = CONFIG_STORAGE_CALIB_ADDR;
const uint16_t g_dataflash_crc_block_size = CONFIG_STORAGE_CALIB_SIZE;

/***********************************************************************************************************************
Private global variables and functions
***********************************************************************************************************************/
/* Variables */
#ifndef __DEBUG
static
#endif/* __DEBUG */
CONFIG_STORAGE_HEADER       g_config_storage_header;                               /* Storage Header */

#ifndef __DEBUG
static
#endif/* __DEBUG */
CONFIG_STATE                g_config_storage_state = CONFIG_STATE_UNINITIALIZED;   /* Storage State */

/* CRC interval checking calibration data in RAM */
#ifndef __DEBUG
static
#endif/* __DEBUG */
uint8_t                     g_config_storage_interval_flag;                        /* Storage interval checking CRC for RAM */

#ifndef __DEBUG
static
#endif /* __DEBUG */
uint8_t                     g_config_disable_CRC_checking;                         /* Enable CRC checking */

/* Sub-functions (internal used for module) */
static uint8_t CONFIG_ReadStorageHeader(void);          /* Read Storage Header to g_config_storage_header */
static uint8_t CONFIG_UpdateStorageHeader(void);        /* Update g_config_storage_header to Storage Header */

/***********************************************************************************************************************
* Function Name    : static uint8_t CONFIG_ReadStorageHeader(void)
* Description      : Read & Check Storage Header, store to g_config_storage_header
* Arguments        : None
* Functions Called : DATAFLASH_Read()
* Return Value     : Execution Status
*                  :    CONFIG_OK              Normal end, Storage is formatted
*                  :    CONFIG_ERROR           Storage Header error
*                  :    CONFIG_NOT_FORMATTED   Storage device is not formatted
***********************************************************************************************************************/
static uint8_t CONFIG_ReadStorageHeader(void)
{
    /* Read Storage header OK? */
    if (DATAFLASH_Read(CONFIG_STORAGE_HEADER_ADDR,
                       (uint8_t*)&g_config_storage_header,
                       CONFIG_STORAGE_HEADER_SIZE) != DATAFLASH_OK)
    {
        return CONFIG_ERROR;   /* Read error */
    }
    
    /* Is header code & usage ok? */
    else if (g_config_storage_header.usage == 0)
    {
        return CONFIG_NOT_FORMATTED;   /* Not formatted */
    }
    /* Is the usage = sum of all item? */
    else if (g_config_storage_header.usage != (sizeof(g_config_storage_header)                + \
             g_config_storage_header.num_of_config          + \
             g_config_storage_header.num_of_calib
                                              )
            )
    {
        return CONFIG_NOT_FORMATTED;   /* Not formatted */
    }
    
    /* Success */
    return CONFIG_OK;
}

/***********************************************************************************************************************
* Function Name    : static uint8_t CONFIG_UpdateStorageHeader(void)
* Description      : Update usage & g_config_storage_header to Storage Header
* Arguments        : None
* Functions Called : DATAFLASH_Write()
* Return Value     : Execution Status
*                  :    CONFIG_OK      Normal end, update success
*                  :    CONFIG_ERROR   Write error
***********************************************************************************************************************/
static uint8_t CONFIG_UpdateStorageHeader(void)
{
    /* Update usage */
    g_config_storage_header.usage =    sizeof(g_config_storage_header)                   + \
                                       g_config_storage_header.num_of_config             + \
                                       g_config_storage_header.num_of_calib ;
                                       
    /* Is write Storage OK? */
    if (DATAFLASH_Write(CONFIG_STORAGE_HEADER_ADDR,
                        (uint8_t*)&g_config_storage_header,
                        CONFIG_STORAGE_HEADER_SIZE) != DATAFLASH_OK)
    {
        return CONFIG_ERROR;   /* Write error */
    }
    
    /* Success */
    return CONFIG_OK;
}

/***********************************************************************************************************************
* Function Name    : static uint8_t CONFIG_CalculateCRC16(void)
* Description      : Calculate the CRC16 based on required input data
* Arguments        : config_crc_data * p_crc_data: pointer to structure of data to calculate CRC
* Functions Called : TBD
* Return Value     : uint16_t: Calculated CRC value
***********************************************************************************************************************/
#ifndef __DEBUG
static
#endif /* __DEBUG */
uint16_t CONFIG_CalculateCRC16(config_crc_data* p_crc_data)
{
    uint16_t crc_value;
    
    if (p_crc_data != NULL)
    {
        if ((p_crc_data->p_calibration != NULL) &&
            (p_crc_data->p_driver_adc_gain0_list != NULL) &&
            (p_crc_data->p_driver_adc_gain1_list != NULL) &&
            (p_crc_data->p_driver_timer_offset != NULL)
           )
        {
        
            /* Clear old CRC to start calc. CRC for calib */
            R_CRC_Clear();
            
            /* Calibration data (no pointer) */
            R_CRC_Calculate(
                (uint8_t*)p_crc_data->p_calibration,
                CONFIG_STORAGE_CALIB_NO_ARRAY_SIZE
            );
            
            /* SW Phase */
            R_CRC_Calculate(
                (uint8_t*)p_crc_data->p_calibration->sw_phase_correction.phase_r.i_phase_degrees,
                CONFIG_STORAGE_CALIB_SW_DEGREE_ARRAY_PHASE_R_SIZE
            );
            R_CRC_Calculate(
                (uint8_t*)p_crc_data->p_calibration->sw_phase_correction.phase_y.i_phase_degrees,
                CONFIG_STORAGE_CALIB_SW_DEGREE_ARRAY_PHASE_Y_SIZE
            );
            R_CRC_Calculate(
                (uint8_t*)p_crc_data->p_calibration->sw_phase_correction.phase_b.i_phase_degrees,
                CONFIG_STORAGE_CALIB_SW_DEGREE_ARRAY_PHASE_B_SIZE
            );
            
            /* SW Gain */
            
            R_CRC_Calculate(
                (uint8_t*)p_crc_data->p_calibration->sw_gain.phase_r.i_gain_values,
                CONFIG_STORAGE_CALIB_SW_GAIN_ARRAY_PHASE_R_SIZE
            );
            R_CRC_Calculate(
                (uint8_t*)p_crc_data->p_calibration->sw_gain.phase_y.i_gain_values,
                CONFIG_STORAGE_CALIB_SW_GAIN_ARRAY_PHASE_Y_SIZE
            );
            R_CRC_Calculate(
                (uint8_t*)p_crc_data->p_calibration->sw_gain.phase_b.i_gain_values,
                CONFIG_STORAGE_CALIB_SW_GAIN_ARRAY_PHASE_B_SIZE
            );
            
            /* ADC Gain */
            
            R_CRC_Calculate(
                (uint8_t*)p_crc_data->p_driver_adc_gain0_list,
                CONFIG_STORAGE_CALIB_DRIVER_ADC_GAIN0_SIZE
            );
            R_CRC_Calculate(
                (uint8_t*)p_crc_data->p_driver_adc_gain1_list,
                CONFIG_STORAGE_CALIB_DRIVER_ADC_GAIN1_SIZE
            );
            
            /* Timer offset */
            
            R_CRC_Calculate(
                (uint8_t*)p_crc_data->p_driver_timer_offset,
                CONFIG_STORAGE_CALIB_DRIVER_ADC_TIMER_OFFSET_SIZE
            );
            
            crc_value = R_CRC_GetResult();
        }
    }
    
    return crc_value;
}

/***********************************************************************************************************************
* Function Name    : uint8_t CONFIG_Init(uint8_t is_checking)
* Description      : Storage Initialization
* Arguments        : None
* Functions Called : DATAFLASH_Init()
*                  : DATAFLASH_VerifyStatus()
*                  : CONFIG_ReadStorageHeader()
* Return Value     : Execution Status
*                  :    CONFIG_OK               Normal end
*                  :    CONFIG_ERROR            Error occurred
*                  :                            Read/Write error or data validity error
*                  :    CONFIG_NOT_FORMATTED    No valid bank found
***********************************************************************************************************************/
uint8_t CONFIG_Init(uint8_t is_checking)
{
    uint8_t     status;
    
    g_config_storage_state = CONFIG_STATE_UNINITIALIZED;   /* Uninitialized */
    
    if (is_checking)
    {
        /* Driver Initialization */
        if (DATAFLASH_Init() != DATAFLASH_OK)
        {
            return CONFIG_ERROR;
        }
    }
    
    /* Successful, at here STORAGE module is initial success */
    g_config_storage_state = CONFIG_STATE_INITIALIZED;     /* Initialized */
    
    /* Default use CRC or not */
#ifdef METER_USE_DEFAULT_CALIBRATION_INFO
    g_config_disable_CRC_checking = 1;      /* Disable CRC checking if using default calibration info */
#else
    g_config_disable_CRC_checking = 0;
#endif /* METER_USE_DEFAULT_CALIBRATION_INFO */
    
    if (is_checking)
    {
        /* Check data format */
        status = DATAFLASH_VerifyStatus();
        
        if (status == DATAFLASH_ERROR)
        {
            return CONFIG_ERROR;                            /* Error occurred when read Storage Header */
        }
        else if (status == DATAFLASH_NOT_FORMAT)
        {
            return CONFIG_NOT_FORMATTED;                    /* Storage data is not formatted: both bank invalid */
        }
    }
    
    /* Update and check Storage format */
    status = CONFIG_ReadStorageHeader();
    
    if (status == CONFIG_ERROR)
    {
        return CONFIG_ERROR;                            /* Error occurred when read Storage Header */
    }
    else if (status == CONFIG_NOT_FORMATTED)
    {
        return CONFIG_NOT_FORMATTED;                    /* Storage data is not formatted: wrong storage header content */
    }
    
    return CONFIG_OK;                                   /* OK, And storage is formatted */
}

/***********************************************************************************************************************
* Function Name    : uint8_t CONFIG_Format(void)
* Description      : Storage Format Storage Device to store data
* Arguments        : None
* Functions Called : DATAFLASH_BankErase()
                   : DATAFLASH_Write()
* Return Value     : Execution Status
*                  :    CONFIG_OK          Formatted successfull, device is ready to store data
*                  :    CONFIG_ERROR       Storage device is not initialized
*                  :                        Or, error occurred when write data to Storage
***********************************************************************************************************************/
uint8_t CONFIG_Format(void)
{
    uint16_t    count;
    uint32_t    config_addr;
    
    CONFIG_STORAGE_HEADER formatted_header =
    {
        sizeof(CONFIG_STORAGE_HEADER),      /* Usage (number of bytes)                 */
        0,                                  /* Number of EM Configuration bytes        */
        0,                                  /* Number of EM Calibration bytes          */
    };
    
    /* Check storage state, only call when initial successfull */
    if (g_config_storage_state != CONFIG_STATE_INITIALIZED)
    {
        return CONFIG_ERROR;
    }
    
    /* Format the dataflash */
    DATAFLASH_BankErase(0);
    DATAFLASH_BankErase(1);
    
    /* Write initial Storage Header */
    if (DATAFLASH_Write(CONFIG_STORAGE_HEADER_ADDR,
                        (uint8_t*)&formatted_header,
                        CONFIG_STORAGE_HEADER_SIZE) != DATAFLASH_OK)
    {
        return CONFIG_ERROR;
    }
    
    /* Update current Storage header on RAM */
    g_config_storage_header = formatted_header;
    /* Reset Energies */
    INST_ResetEnergies();
    
    return CONFIG_OK;  /* Formatted */
}

/***********************************************************************************************************************
* Function Name    : uint8_t CONFIG_Backup(uint8_t selection)
* Description      : Storage Backup (from EM Core to Storage)
* Note             : When CONFIG_ERROR is returned, maybe some data on selected item
*                  : (of [selection] parameter) have been stored to Storage
* Arguments        : uint8_t selection: Backup item selection
*                  :      CONFIG_ITEM_CONFIG       Select EM Configuration
*                  :      CONFIG_ITEM_CALIB        Select EM Calibration
*                  :      CONFIG_ITEM_ALL          Select all above items
* Functions Called : CONFIG_ReadStorageHeader()
*                  : DATAFLASH_Write()
*                  : EM_GetConfig()
*                  : EM_GetCalibInfo()
*                  : CONFIG_UpdateStorageHeader()
* Return Value     : Execution Status
*                  :    CONFIG_OK      Backup successfull
*                  :    CONFIG_ERROR   Storage device is not initialized or not formatted
*                  :                    Or, error occurred when write Storage
*                  :                    Or, selection = 0 (CONFIG_ITEM_NONE)
***********************************************************************************************************************/
uint8_t CONFIG_Backup(uint8_t selection)
{
    uint8_t                 status;
    
    /* Check parameter */
    if (selection == CONFIG_ITEM_NONE)
    {
        return CONFIG_ERROR;   /* Params error */
    }
    
    /* Check Storage State */
    if (g_config_storage_state != CONFIG_STATE_INITIALIZED)
    {
        return CONFIG_ERROR;   /* Device is not initialized */
    }
    
    /* Get Storage Header */
    if (CONFIG_ReadStorageHeader() != CONFIG_OK)
    {
        return CONFIG_ERROR;   /* Storage Header error (not formatted) */
    }
    
    /* Is CONFIG_ITEM_CONFIG selected? */
    if ((selection & CONFIG_ITEM_CONFIG) != 0)
    {
        EM_CONFIG config;
        
        /* Get current EM Core config */
        config = EM_GetConfig();
        
        /* Store config to Storage */
        if (DATAFLASH_Write(CONFIG_STORAGE_CONFIG_ADDR,
                            (uint8_t*)&config,
                            sizeof(config) - sizeof(EM_TIME_SLOT*)) != DATAFLASH_OK)
        {
            return CONFIG_ERROR;   /* Write error */
        }
        
        /* Store time slot array */
        if (DATAFLASH_Write(CONFIG_STORAGE_CONFIG_TIMESLOT_ARRAY_ADDR,
                            (uint8_t*)config.timeslot_list,
                            CONFIG_STORAGE_CONFIG_TIMESLOT_ARRAY_SIZE) != DATAFLASH_OK)
        {
            return CONFIG_ERROR;   /* Write error */
        }
        
        /* Update number used bytes of config to storage header */
        g_config_storage_header.num_of_config = CONFIG_STORAGE_CONFIG_SIZE;
    }
    
    /* Is CONFIG_ITEM_CALIB selected? */
    if ((selection & CONFIG_ITEM_CALIB) != 0)
    {
        EM_CALIBRATION      calib;
        dsad_reg_setting_t  regs;
        uint8_t             driver_adc_gain0;
        uint8_t             driver_adc_gain1;
        float32_t           driver_timer_offset;
        
        /* Get EM Core calib */
        calib = EM_GetCalibInfo();
        
        /* Clear the temp driver gain list */
        memset(&driver_adc_gain0, 0, sizeof(driver_adc_gain0));
        memset(&driver_adc_gain1, 0, sizeof(driver_adc_gain1));
        
        /* Store calib to Storage */
        if (DATAFLASH_Write(CONFIG_STORAGE_CALIB_ADDR,
                            (uint8_t*)&calib,
                            CONFIG_STORAGE_CALIB_NO_ARRAY_SIZE) != DATAFLASH_OK)
        {
            return CONFIG_ERROR;   /* Write error */
        }
        
        R_WDT_Restart();
        
        /*
         * Store SW Phase Correction array
        */
        /* Phase_R */
        if (DATAFLASH_Write(CONFIG_STORAGE_CALIB_SW_DEGREE_ARRAY_PHASE_R_ADDR,
                            (uint8_t*)calib.sw_phase_correction.phase_r.i_phase_degrees,
                            CONFIG_STORAGE_CALIB_SW_DEGREE_ARRAY_PHASE_R_SIZE) != DATAFLASH_OK)
        {
            return CONFIG_ERROR;   /* Write error */
        }
        
        /* Phase_Y */
        if (DATAFLASH_Write(CONFIG_STORAGE_CALIB_SW_DEGREE_ARRAY_PHASE_Y_ADDR,
                            (uint8_t*)calib.sw_phase_correction.phase_y.i_phase_degrees,
                            CONFIG_STORAGE_CALIB_SW_DEGREE_ARRAY_PHASE_Y_SIZE) != DATAFLASH_OK)
        {
            return CONFIG_ERROR;   /* Write error */
        }
        
        /* Phase_B */
        if (DATAFLASH_Write(CONFIG_STORAGE_CALIB_SW_DEGREE_ARRAY_PHASE_B_ADDR,
                            (uint8_t*)calib.sw_phase_correction.phase_b.i_phase_degrees,
                            CONFIG_STORAGE_CALIB_SW_DEGREE_ARRAY_PHASE_B_SIZE) != DATAFLASH_OK)
        {
            return CONFIG_ERROR;   /* Write error */
        }
        
        R_WDT_Restart();
        
        /*
        * Store SW Gain I Array
        */
        /* Phase_R */
        if (DATAFLASH_Write(CONFIG_STORAGE_CALIB_SW_GAIN_ARRAY_PHASE_R_ADDR,
                            (uint8_t*)calib.sw_gain.phase_r.i_gain_values,
                            CONFIG_STORAGE_CALIB_SW_GAIN_ARRAY_PHASE_R_SIZE) != DATAFLASH_OK)
        {
            return CONFIG_ERROR;   /* Write error */
        }
        
        /* Phase_Y */
        if (DATAFLASH_Write(CONFIG_STORAGE_CALIB_SW_GAIN_ARRAY_PHASE_Y_ADDR,
                            (uint8_t*)calib.sw_gain.phase_y.i_gain_values,
                            CONFIG_STORAGE_CALIB_SW_GAIN_ARRAY_PHASE_Y_SIZE) != DATAFLASH_OK)
        {
            return CONFIG_ERROR;   /* Write error */
        }
        
        /* Phase_B */
        if (DATAFLASH_Write(CONFIG_STORAGE_CALIB_SW_GAIN_ARRAY_PHASE_B_ADDR,
                            (uint8_t*)calib.sw_gain.phase_b.i_gain_values,
                            CONFIG_STORAGE_CALIB_SW_GAIN_ARRAY_PHASE_B_SIZE) != DATAFLASH_OK)
        {
            return CONFIG_ERROR;   /* Write error */
        }
        
        R_WDT_Restart();
        /*
         * Store Driver ADC Gain Array
        */
        /* Get the gain first */
        R_DSADC_GetGainAndPhase(&regs);
        
        /* When using dual gain, ignore current ADC Gain,
         * Using default value only (same as when backup, GAIN_X1)
        */
#if (EM_SW_PROPERTY_ADC_GAIN_NUMBER_LEVEL_PHASE_R != 1)
        R_DSADC_ClearGainSetting(&regs, EM_ADC_CURRENT_DRIVER_CHANNEL_PHASE_R);
#endif /* EM_SW_PROPERTY_ADC_GAIN_NUMBER_LEVEL_PHASE_R != 1 */
        
#if (EM_SW_PROPERTY_ADC_GAIN_NUMBER_LEVEL_PHASE_Y != 1)
        R_DSADC_ClearGainSetting(&regs, EM_ADC_CURRENT_DRIVER_CHANNEL_PHASE_Y);
#endif /* EM_SW_PROPERTY_ADC_GAIN_NUMBER_LEVEL_PHASE_Y != 1 */
        
#if (EM_SW_PROPERTY_ADC_GAIN_NUMBER_LEVEL_PHASE_B != 1)
        R_DSADC_ClearGainSetting(&regs, EM_ADC_CURRENT_DRIVER_CHANNEL_PHASE_B);
#endif /* EM_SW_PROPERTY_ADC_GAIN_NUMBER_LEVEL_PHASE_B != 1 */
        
        driver_adc_gain0 = regs.gain0;
        driver_adc_gain1 = regs.gain1;
        
        if (DATAFLASH_Write(CONFIG_STORAGE_CALIB_DRIVER_ADC_GAIN0_ADDR,
                            (uint8_t*)&driver_adc_gain0,
                            CONFIG_STORAGE_CALIB_DRIVER_ADC_GAIN0_SIZE) != DATAFLASH_OK)
        {
            return CONFIG_ERROR;   /* Write error */
        }
        
        if (DATAFLASH_Write(CONFIG_STORAGE_CALIB_DRIVER_ADC_GAIN1_ADDR,
                            (uint8_t*)&driver_adc_gain1,
                            CONFIG_STORAGE_CALIB_DRIVER_ADC_GAIN1_SIZE) != DATAFLASH_OK)
        {
            return CONFIG_ERROR;   /* Write error */
        }
        
        R_WDT_Restart();
        
        /* Store Driver Timer Offset */
        driver_timer_offset = EM_ADC_SAMP_GetTimerOffset();
        
        if (DATAFLASH_Write(CONFIG_STORAGE_CALIB_DRIVER_ADC_TIMER_OFFSET_ADDR,
                            (uint8_t*)&driver_timer_offset,
                            CONFIG_STORAGE_CALIB_DRIVER_ADC_TIMER_OFFSET_SIZE) != DATAFLASH_OK)
        {
            return CONFIG_ERROR;   /* Write error */
        }
        
        R_WDT_Restart();
        
        /* Update number used bytes of calib to storage header */
        g_config_storage_header.num_of_calib = CONFIG_STORAGE_CALIB_SIZE;
    }
    
    /* Is CONFIG_ITEM_USER selected? */
    if ((selection & CONFIG_ITEM_USER) != 0)
    {
        /* Store config to Storage */
        if (DATAFLASH_Write(CONFIG_STORAGE_USER_CONFIG_ADDR,
                            (uint8_t*)&user_config,
                            CONFIG_STORAGE_USER_CONFIG_SIZE) != DATAFLASH_OK)
        {
            return CONFIG_ERROR;   /* Write error */
        }
    }
    
    /* Is CONFIG_ITEM_MAINS_ENG selected? */
    if ((selection & CONFIG_ITEM_MAINS_ENG) != 0)
    {
        /* Store config to Storage */
        if (DATAFLASH_Write(CONFIG_STORAGE_MAINS_ENERGY_ADDR,
                            (uint8_t*)&MainsEnergies,
                            CONFIG_STORAGE_MAINS_ENERGY_SIZE) != DATAFLASH_OK)
        {
            return CONFIG_ERROR;   /* Write error */
        }
    }
    
    /* Is CONFIG_ITEM_MAINS_ENG selected? */
    if ((selection & CONFIG_ITEM_PERIODIC_ENG) != 0)
    {
        /* Store config to Storage */
        if (DATAFLASH_Write(CONFIG_STORAGE_DG_ENERGY_ADDR,
                            (uint8_t*)&MainsEnergies,
                            CONFIG_STORAGE_DG_ENERGY_SIZE) != DATAFLASH_OK)
        {
            return CONFIG_ERROR;   /* Write error */
        }
    }
    
    
    /* Update Storage Header */
    if (CONFIG_UpdateStorageHeader() != CONFIG_OK)
    {
        return CONFIG_ERROR;   /* Write error */
    }
    
    /* Success */
    return CONFIG_OK;
}

/***********************************************************************************************************************
* Function Name    : uint8_t CONFIG_Restore(uint8_t selection)
* Description      : Storage Restore (from Storage to EM Core)
* Arguments        : uint8_t selection: Restore item selection
*                  :      CONFIG_ITEM_CONFIG       Select EM Configuration
*                  :      CONFIG_ITEM_CALIB        Select EM Calibration
*                  :      CONFIG_ITEM_SYS_STATE    Select EM System State
*                  :      CONFIG_ITEM_RTC_TIME     Select RTC Time
*                  :      CONFIG_ITEM_ALL          Select all above items
* Functions Called : CONFIG_ReadStorageHeader()
*                  : DATAFLASH_Read()
*                  : EM_SetConfig()
*                  : EM_SetCalibInfo()
*                  : EM_SetState()
*                  : RTC_CounterSet()
* Return Value     : Execution Status
*                  :    CONFIG_OK                  Restore successfull
*                  :    CONFIG_ERROR_DATA_CORRUPT  Related data on Storage of selected item is corrupt
*                  :    CONFIG_ERROR               Storage device is not initialized or not formatted
*                  :                                Or, error occurred when read Storage,
*                  :                                Or, selection = 0 (CONFIG_ITEM_NONE)
***********************************************************************************************************************/
uint8_t CONFIG_Restore(uint8_t selection)
{
    uint8_t                 status;
    
    /* Check parameter */
    if (selection == CONFIG_ITEM_NONE)
    {
        return CONFIG_ERROR;   /* Params error */
    }
    
    /* Check Storage State */
    if (g_config_storage_state != CONFIG_STATE_INITIALIZED)
    {
        return CONFIG_ERROR;   /* Device is not initialized */
    }
    
    /* Get Storage Header */
    if (CONFIG_ReadStorageHeader() != CONFIG_OK)
    {
        return CONFIG_ERROR;   /* Storage Header error */
    }
    
    /* Is CONFIG_ITEM_CONFIG selected? */
    if ((selection & CONFIG_ITEM_CONFIG) != 0)
    {
        /* Is EM Configuration data valid in Storage? */
        if (g_config_storage_header.num_of_config == CONFIG_STORAGE_CONFIG_SIZE)
        {
            EM_CONFIG       config;
            EM_TIME_SLOT    timeslot_list[6];
            
            /* Get config from Storage, accept the timeslot_list */
            if (DATAFLASH_Read(CONFIG_STORAGE_CONFIG_ADDR,
                               (uint8_t*)&config,
                               sizeof(config) - sizeof(EM_TIME_SLOT*)) != DATAFLASH_OK)
            {
                return CONFIG_ERROR;   /* Read error */
            }
            
            /* Get time slot array */
            if (DATAFLASH_Read(CONFIG_STORAGE_CONFIG_TIMESLOT_ARRAY_ADDR,
                               (uint8_t*)timeslot_list,
                               CONFIG_STORAGE_CONFIG_TIMESLOT_ARRAY_SIZE) != DATAFLASH_OK)
            {
                return CONFIG_ERROR;   /* Read error */
            }
            
            /* Link the timeslot_list to config */
            config.timeslot_list = timeslot_list;
            
            /* Set to EM Core */
            if (EM_SetConfig(&config) != EM_OK)
            {
                return CONFIG_ERROR_DATA_CORRUPT;  /* Data corrupt */
            }
        }
    }
    
    /* Is CONFIG_ITEM_CALIB selected? */
    if ((selection & CONFIG_ITEM_CALIB) != 0)
    {
        /* Is EM Calibration data valid in Storage? */
        if (g_config_storage_header.num_of_calib == CONFIG_STORAGE_CALIB_SIZE)
        {
            EM_CALIBRATION  calib;
            dsad_reg_setting_t regs;
            float32_t       degree_list_r[EM_GAIN_PHASE_NUM_LEVEL_MAX];
            float32_t       degree_list_y[EM_GAIN_PHASE_NUM_LEVEL_MAX];
            float32_t       degree_list_b[EM_GAIN_PHASE_NUM_LEVEL_MAX];
            float32_t       gain_list_r[EM_GAIN_PHASE_NUM_LEVEL_MAX];
            float32_t       gain_list_y[EM_GAIN_PHASE_NUM_LEVEL_MAX];
            float32_t       gain_list_b[EM_GAIN_PHASE_NUM_LEVEL_MAX];
            uint8_t         driver_adc_gain0;
            uint8_t         driver_adc_gain1;
            float32_t       driver_timer_offset;
            
            /* Clear the temp driver gain list */
            memset(&degree_list_r, 0, sizeof(degree_list_r));
            memset(&degree_list_y, 0, sizeof(degree_list_y));
            memset(&degree_list_b, 0, sizeof(degree_list_b));
            memset(&gain_list_r, 0, sizeof(gain_list_r));
            memset(&gain_list_y, 0, sizeof(gain_list_y));
            memset(&gain_list_b, 0, sizeof(gain_list_b));
            memset(&driver_adc_gain0, 0, sizeof(driver_adc_gain0));
            memset(&driver_adc_gain1, 0, sizeof(driver_adc_gain1));
            
            /* Get calib from Storage, accept sw phase & gain value list pointer */
            if (DATAFLASH_Read(CONFIG_STORAGE_CALIB_ADDR,
                               (uint8_t*)&calib,
                               CONFIG_STORAGE_CALIB_NO_ARRAY_SIZE) != DATAFLASH_OK)
            {
                return CONFIG_ERROR;   /* Read error */
            }
            
            /*
             * Get SW Phase Correction array
            */
            /* Phase_R */
            if (DATAFLASH_Read(CONFIG_STORAGE_CALIB_SW_DEGREE_ARRAY_PHASE_R_ADDR,
                               (uint8_t*)degree_list_r,
                               CONFIG_STORAGE_CALIB_SW_DEGREE_ARRAY_PHASE_R_SIZE) != DATAFLASH_OK)
            {
                return CONFIG_ERROR;   /* Read error */
            }
            
            calib.sw_phase_correction.phase_r.i_phase_degrees = degree_list_r;
            
            /* Phase_Y */
            if (DATAFLASH_Read(CONFIG_STORAGE_CALIB_SW_DEGREE_ARRAY_PHASE_Y_ADDR,
                               (uint8_t*)degree_list_y,
                               CONFIG_STORAGE_CALIB_SW_DEGREE_ARRAY_PHASE_Y_SIZE) != DATAFLASH_OK)
            {
                return CONFIG_ERROR;   /* Read error */
            }
            
            calib.sw_phase_correction.phase_y.i_phase_degrees = degree_list_y;
            
            /* Phase_B */
            if (DATAFLASH_Read(CONFIG_STORAGE_CALIB_SW_DEGREE_ARRAY_PHASE_B_ADDR,
                               (uint8_t*)degree_list_b,
                               CONFIG_STORAGE_CALIB_SW_DEGREE_ARRAY_PHASE_B_SIZE) != DATAFLASH_OK)
            {
                return CONFIG_ERROR;   /* Read error */
            }
            
            calib.sw_phase_correction.phase_b.i_phase_degrees = degree_list_b;
            
            /*
            * Get SW Gain array
            */
            /* Phase_R */
            if (DATAFLASH_Read(CONFIG_STORAGE_CALIB_SW_GAIN_ARRAY_PHASE_R_ADDR,
                               (uint8_t*)gain_list_r,
                               CONFIG_STORAGE_CALIB_SW_GAIN_ARRAY_PHASE_R_SIZE) != DATAFLASH_OK)
            {
                return CONFIG_ERROR;   /* Read error */
            }
            
            calib.sw_gain.phase_r.i_gain_values = gain_list_r;
            
            /* Phase_Y */
            if (DATAFLASH_Read(CONFIG_STORAGE_CALIB_SW_GAIN_ARRAY_PHASE_Y_ADDR,
                               (uint8_t*)gain_list_y,
                               CONFIG_STORAGE_CALIB_SW_GAIN_ARRAY_PHASE_Y_SIZE) != DATAFLASH_OK)
            {
                return CONFIG_ERROR;   /* Read error */
            }
            
            calib.sw_gain.phase_y.i_gain_values = gain_list_y;
            
            /* Phase_B */
            if (DATAFLASH_Read(CONFIG_STORAGE_CALIB_SW_GAIN_ARRAY_PHASE_B_ADDR,
                               (uint8_t*)gain_list_b,
                               CONFIG_STORAGE_CALIB_SW_GAIN_ARRAY_PHASE_B_SIZE) != DATAFLASH_OK)
            {
                return CONFIG_ERROR;   /* Read error */
            }
            
            calib.sw_gain.phase_b.i_gain_values = gain_list_b;
            
            /*
             * Get Driver ADC Gain Array
            */
            /* Gain0 */
            if (DATAFLASH_Read(CONFIG_STORAGE_CALIB_DRIVER_ADC_GAIN0_ADDR,
                               (uint8_t*)&driver_adc_gain0,
                               CONFIG_STORAGE_CALIB_DRIVER_ADC_GAIN0_SIZE) != DATAFLASH_OK)
            {
                return CONFIG_ERROR;   /* Read error */
            }
            
            /* Gain1 */
            if (DATAFLASH_Read(CONFIG_STORAGE_CALIB_DRIVER_ADC_GAIN1_ADDR,
                               (uint8_t*)&driver_adc_gain1,
                               CONFIG_STORAGE_CALIB_DRIVER_ADC_GAIN1_SIZE) != DATAFLASH_OK)
            {
                return CONFIG_ERROR;   /* Read error */
            }
            
            /* Timer offset */
            if (DATAFLASH_Read(CONFIG_STORAGE_CALIB_DRIVER_ADC_TIMER_OFFSET_ADDR,
                               (uint8_t*)&driver_timer_offset,
                               CONFIG_STORAGE_CALIB_DRIVER_ADC_TIMER_OFFSET_SIZE) != DATAFLASH_OK)
            {
                return CONFIG_ERROR;   /* Read error */
            }
            
            /* Run internal CRC verify on dataflash, no need to get crc value */
            if (DATAFLASH_VerifyCRC(NULL) != DATAFLASH_OK)
            {
                return CONFIG_ERROR;
            }
            
            /* Set ADC Gain value into driver */
            regs.gain0 = driver_adc_gain0;
            regs.gain1 = driver_adc_gain1;
            R_DSADC_SetGain(regs);
            
            /* Set timer offset into wrapper */
            EM_ADC_SAMP_SetTimerOffset(driver_timer_offset);
            
            /* Set to EM Core */
            if (EM_SetCalibInfo(&calib) != EM_OK)
            {
                return CONFIG_ERROR_DATA_CORRUPT;  /* Data corrupt */
            }
        }
        else
        {
            return CONFIG_ERROR;   /* Header not match */
        }
    }
    
    /* Is CONFIG_ITEM_USER selected? */
    if ((selection & CONFIG_ITEM_USER) != 0)
    {
        /* Store config to Storage */
        if (DATAFLASH_Read(CONFIG_STORAGE_USER_CONFIG_ADDR,
                           (uint8_t*)&user_config,
                           CONFIG_STORAGE_USER_CONFIG_SIZE) != DATAFLASH_OK)
        {
            return CONFIG_ERROR;   /* Write error */
        }
    }
    
    /* Is CONFIG_ITEM_MAINS_ENG selected? */
    if ((selection & CONFIG_ITEM_MAINS_ENG) != 0)
    {
        /* Store config to Storage */
        if (DATAFLASH_Read(CONFIG_STORAGE_MAINS_ENERGY_ADDR,
                           (uint8_t*)&MainsEnergies,
                           CONFIG_STORAGE_MAINS_ENERGY_SIZE) != DATAFLASH_OK)
        {
            return CONFIG_ERROR;   /* Write error */
        }
        
        INST_UpdateEnergiesAtStartup();
    }

    /* Is CONFIG_ITEM_MAINS_ENG selected? */
    if ((selection & CONFIG_ITEM_PERIODIC_ENG) != 0)
    {
        /* Store config to Storage */
        if (DATAFLASH_Read(CONFIG_STORAGE_DG_ENERGY_ADDR,
                           (uint8_t*)&PeriodicEnergies,
                           CONFIG_STORAGE_DG_ENERGY_SIZE) != DATAFLASH_OK)
        {
            return CONFIG_ERROR;   /* Write error */
        }
        
        INST_UpdateEnergiesAtStartup();
    }
    
    /* Success */
    return CONFIG_OK;
}

/******************************************************************************
* Function Name : CONFIG_EnableCRCChecking
* Interface     : void CONFIG_EnableCRCChecking(void)
* Description   : Enable to check CRC of storage
* Arguments     : None
* Return Value  : None
******************************************************************************/
void CONFIG_EnableCRCChecking(void)
{
    g_config_disable_CRC_checking = 0;
}

/******************************************************************************
* Function Name : CONFIG_DisableCRCChecking
* Interface     : void CONFIG_DisableCRCChecking(void)
* Description   : Disable to check CRC of storage
* Arguments     : None
* Return Value  : None
******************************************************************************/
void CONFIG_DisableCRCChecking(void)
{
    g_config_disable_CRC_checking = 1;
}

/***********************************************************************************************************************
* Function Name : CONFIG_PollingProcessing
* Interface     : void CONFIG_PollingProcessing(void)
* Description   : Polling processing for storage
*               : Calculate CRC for calibration content on RAM
*               : Compare with CRC value on Storage
*               : If not matched, consider RAM corrupt, restore from dataflash
* Arguments     : None
* Return Value  : None
***********************************************************************************************************************/
void CONFIG_PollingProcessing(void)
{
    config_crc_data     crc_data;
    EM_CALIBRATION      calib;
    dsad_reg_setting_t  regs;
    uint8_t             driver_adc_gain0;
    uint8_t             driver_adc_gain1;
    float32_t           driver_timer_offset;
    uint16_t            ram_calculated_crc;
    uint16_t            memory_calculated_crc;
    
    if (g_config_disable_CRC_checking == 1)
    {
        return;
    }
    
    /* CRC check for the current calibration data */
    if (g_config_storage_interval_flag == 1)
    {
        /* ACK */
        g_config_storage_interval_flag = 0;
        
        if (g_config_storage_state == CONFIG_STATE_INITIALIZED &&
            g_config_storage_header.num_of_calib > 0)
        {
            calib = EM_GetCalibInfo();
            
            /* Clear the temp driver gain list */
            memset(&driver_adc_gain0, 0, sizeof(driver_adc_gain0));
            memset(&driver_adc_gain1, 0, sizeof(driver_adc_gain1));
            
            /*
             * ADC Gain Array
            */
            /* Get the gain first */
            R_DSADC_GetGainAndPhase(&regs);
            
            /* When using dual gain, ignore current ADC Gain,
             * Using default value only (same as when backup, GAIN_X1)
            */
#if (EM_SW_PROPERTY_ADC_GAIN_NUMBER_LEVEL_PHASE_R != 1)
            R_DSADC_ClearGainSetting(&regs, EM_ADC_CURRENT_DRIVER_CHANNEL_PHASE_R);
#endif /* EM_SW_PROPERTY_ADC_GAIN_NUMBER_LEVEL_PHASE_R != 1 */
            
#if (EM_SW_PROPERTY_ADC_GAIN_NUMBER_LEVEL_PHASE_Y != 1)
            R_DSADC_ClearGainSetting(&regs, EM_ADC_CURRENT_DRIVER_CHANNEL_PHASE_Y);
#endif /* EM_SW_PROPERTY_ADC_GAIN_NUMBER_LEVEL_PHASE_Y != 1 */
            
#if (EM_SW_PROPERTY_ADC_GAIN_NUMBER_LEVEL_PHASE_B != 1)
            R_DSADC_ClearGainSetting(&regs, EM_ADC_CURRENT_DRIVER_CHANNEL_PHASE_B);
#endif /* EM_SW_PROPERTY_ADC_GAIN_NUMBER_LEVEL_PHASE_B != 1 */
            
            driver_adc_gain0 = regs.gain0;
            driver_adc_gain1 = regs.gain1;
            
            /* Store Driver Timer Offset */
            driver_timer_offset = EM_ADC_SAMP_GetTimerOffset();
            
            /* Calculate the CRC on RAM calibration data */
            crc_data.p_calibration = &calib;
            crc_data.p_driver_adc_gain0_list = (uint8_t*)&driver_adc_gain0;
            crc_data.p_driver_adc_gain1_list = (uint8_t*)&driver_adc_gain1;
            crc_data.p_driver_timer_offset = &driver_timer_offset;
            
            ram_calculated_crc = CONFIG_CalculateCRC16(&crc_data);
            
            /* Run internal CRC verify of dataflash and get the CRC result for comparison with RAM content
            *  This assume dataflash memory correct as it already passed in bios checking
            */
            DATAFLASH_VerifyCRC(&memory_calculated_crc);
            
            /* Check matching of RAM and Memory data */
            if (memory_calculated_crc != ram_calculated_crc)
            {
                /* CRC not matched, this assume the RAM content wrong, will restore from memory */
                
                if (EM_Stop() != EM_OK)
                {
                
                }
                
                if (CONFIG_Restore(CONFIG_ITEM_CALIB) != CONFIG_OK)
                {
                
                }
                
                if (EM_Start() != EM_OK)
                {
                
                }
            }
            
        }
    }
}

/***********************************************************************************************************************
* Function Name : CONFIG_RtcCallback
* Interface     : void CONFIG_RtcCallback()
* Description   : RTC Callback to count time
* Arguments     : None
* Return Value  : None
***********************************************************************************************************************/
void CONFIG_RtcCallback(void)
{
    static uint16_t counter = 0;
    
    /* Check to signal the interval flag for CRC check */
    counter++;
    
    if (counter >= CONFIG_STORAGE_INTERVAL_COUNT)
    {
        g_config_storage_interval_flag = 1;
        counter = 0;
    }
}
