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
* File Name    : em_anti_tamper_adapter.h
* Version      : 1.00
* Device(s)    : RL78/I1C
* Tool-Chain   : CCRL
* H/W Platform : RL78/I1C Energy Meter Platform
* Description  : EM Anti-tamper Adapter Layer APIs
***********************************************************************************************************************/

#ifndef _EM_ANTI_TAMPER_ADAPTER_H
#define _EM_ANTI_TAMPER_ADAPTER_H

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include "typedef.h"                /* GSCE Standard Typedef */
#include "em_adapter.h"             /* Shared Adapter Layer */

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/* Energy select when tamper occured */
typedef enum tagEMExtAntiTamperEnergySelect
{
    EM_EXT_TAMPER_ENERGY_SELECT_UPDATE_NORMAL = 0,      /* Update (accumulate) normally */
    EM_EXT_TAMPER_ENERGY_SELECT_NO_UPDATE,              /* No update */
    EM_EXT_TAMPER_ENERGY_SELECT_MAX_DEMAND,             /* Update by max demand */
    EM_EXT_TAMPER_ENERGY_SELECT_SPECIFIED_POWER,        /* Update by a specified power value */
} EM_EXT_TAMPER_ENERGY_SELECT;

/* Tamper Plugin Result */
typedef struct tagEMExtAntiTamperEventResult
{
    float32_t   specified_power;                    /* Specified power to update when tamper happen */

    union
    {
        uint32_t value;
        struct
        {
            uint8_t  is_neutral_missing             :1;     /* Is Neutral missing occurred or not? */
            uint8_t  is_missing_potential_R         :1;     /* Missing Potential */
            uint8_t  is_missing_potential_Y         :1;     /* Missing Potential */
            uint8_t  is_missing_potential_B         :1;     /* Missing Potential */
            uint8_t  is_voltage_unbalance           :1;     /* Voltage Unbalance */
            uint8_t  is_reverse_current_R           :1;     /* Reverse Current */
            uint8_t  is_reverse_current_Y           :1;     /* Reverse Current */
            uint8_t  is_reverse_current_B           :1;     /* Reverse Current */
            
            uint8_t  is_open_current_R              :1;     /* Open Current */
            uint8_t  is_open_current_Y              :1;     /* Open Current */
            uint8_t  is_open_current_B              :1;     /* Open Current */
            uint8_t  is_current_unbalance           :1;     /* Current Unbalance */
            uint8_t  is_current_bypass              :1;     /* Current Bypass */
            uint8_t  reserved1                      :3;     /* reserved */
            
            uint8_t  reserved2                      :8;     /* reserved */
            
            uint8_t  reserved3                      :8;     /* reserved */
        } details;
    } tamper_status;
    
} EM_EXT_TAMPER_RESULT;

/* Tamper Plugin Detection API */
typedef EM_EXT_TAMPER_RESULT (*EM_EXT_TAMPER_DETECTION_API)(void);  /* Detection API prototype */

/* Tamper Plugin Information */
typedef struct tagEMExtAntiTamperPluginInfo
{
    /*
     * Plugin Configuration
     */

    /* Selection to update energy when Neutral missing tamper occured */
    EM_EXT_TAMPER_ENERGY_SELECT     neutral_missing_energy_selection;

    /* Selection to update energy when missing potential tamper occured */
    EM_EXT_TAMPER_ENERGY_SELECT     missing_potential_energy_selection;
    
    /* Selection to update energy when voltage unbalance tamper occured */
    EM_EXT_TAMPER_ENERGY_SELECT     voltage_unbalance_energy_selection;

    /* Selection to update energy when REV tamper occured */
    EM_EXT_TAMPER_ENERGY_SELECT     rev_energy_selection;

    /* Selection to update energy when CT open tamper occured */
    EM_EXT_TAMPER_ENERGY_SELECT     current_open_energy_selection;

    /* Selection to update energy when current unbalance tamper occured */
    EM_EXT_TAMPER_ENERGY_SELECT     current_unbalance_energy_selection;

    /* Selection to update energy when current bypass tamper occured */
    EM_EXT_TAMPER_ENERGY_SELECT     current_bypass_energy_selection;
    
    /* Reserved */
    EM_EXT_TAMPER_ENERGY_SELECT     reserved;

    /* Plugin Detection (Connector) API, NULL to deactive the plugin */
    EM_EXT_TAMPER_DETECTION_API     fp_detection;       /* Function pointer of detection API */
    
    /* Callback for tamper event, specify NULL to no use callback */
    EM_EXT_CALLBACK                 fp_cb_NeutralMissing;
    EM_EXT_CALLBACK                 fp_cb_MissingPotential_R;
    EM_EXT_CALLBACK                 fp_cb_MissingPotential_Y;
    EM_EXT_CALLBACK                 fp_cb_MissingPotential_B;
    EM_EXT_CALLBACK                 fp_cb_VoltageUnbalance;
    EM_EXT_CALLBACK                 fp_cb_ReverseCurrent_R;
    EM_EXT_CALLBACK                 fp_cb_ReverseCurrent_Y;
    EM_EXT_CALLBACK                 fp_cb_ReverseCurrent_B;
    EM_EXT_CALLBACK                 fp_cb_OpenCurrent_R;
    EM_EXT_CALLBACK                 fp_cb_OpenCurrent_Y;
    EM_EXT_CALLBACK                 fp_cb_OpenCurrent_B;
    EM_EXT_CALLBACK                 fp_cb_CurrentUnbalance;
    EM_EXT_CALLBACK                 fp_cb_CurrentBypass;
    
} EM_EXT_TAMPER_PLUGIN_INFO;

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Variable Externs
***********************************************************************************************************************/

/***********************************************************************************************************************
Functions Prototypes
***********************************************************************************************************************/
/* Plugin Registration API
 * Please use this API to register the plugin into the EM Core
 * Note that, all plugin information must be correct,
 * if not, EM_EXT_ERROR will be returned, otherwise, EM_EXT_OK.
 */
uint8_t EM_EXT_TAMPER_Register(NEAR_PTR const EM_EXT_TAMPER_PLUGIN_INFO *p_plugin_info);

#endif /* _EM_ANTI_TAMPER_ADAPTER_H */
