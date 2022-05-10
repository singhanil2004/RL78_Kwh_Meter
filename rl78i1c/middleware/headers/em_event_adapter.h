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
* File Name    : em_event_adapter.h
* Version      : 1.00
* Device(s)    : RL78/I1C
* Tool-Chain   : CCRL
* H/W Platform : RL78/I1C Energy Meter Platform
* Description  : EM Event Adapter Layer APIs
***********************************************************************************************************************/

#ifndef _EM_EVENT_ADAPTER_H
#define _EM_EVENT_ADAPTER_H

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include "typedef.h"                /* GSCE Standard Typedef */
#include "em_adapter.h"             /* Shared Adapter Layer */  

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/
/* Event Plugin Result */
typedef union tagEMExtEventResult
{
    uint32_t        value;
    struct
    {
        uint8_t         is_active_noload_R      :1;     /* Is active noload occurred or not? */
        uint8_t         is_active_noload_Y      :1;     /* Is active noload occurred or not? */
        uint8_t         is_active_noload_B      :1;     /* Is active noload occurred or not? */
        uint8_t         is_reactive_noload_R    :1;     /* Is reactive noload occurred or not? */
        uint8_t         is_reactive_noload_Y    :1;     /* Is reactive noload occurred or not? */
        uint8_t         is_reactive_noload_B    :1;     /* Is reactive noload occurred or not? */
        uint8_t         is_apparent_noload_R    :1;     /* Is apparent noload occurred or not? */
        uint8_t         is_apparent_noload_Y    :1;     /* Is apparent noload occurred or not? */
        
        uint8_t         is_apparent_noload_B    :1;     /* Is apparent noload occurred or not? */
        uint8_t         is_peak_voltage_R       :1;     /* Is peak voltage occurred or not? */
        uint8_t         is_peak_voltage_Y       :1;     /* Is peak voltage occurred or not? */
        uint8_t         is_peak_voltage_B       :1;     /* Is peak voltage occurred or not? */
        uint8_t         is_peak_current_R       :1;     /* Is peak current occurred or not? */
        uint8_t         is_peak_current_Y       :1;     /* Is peak current occurred or not? */
        uint8_t         is_peak_current_B       :1;     /* Is peak current occurred or not? */
        uint8_t         is_freq_out_range_R     :1;     /* Is frequency out of range occurred or not? */
        
        uint8_t         is_freq_out_range_Y     :1;     /* Is frequency out of range occurred or not? */
        uint8_t         is_freq_out_range_B     :1;     /* Is frequency out of range occurred or not? */
        
        uint8_t         reserve                 :6;
        uint8_t         reserved1               :8;    /* (NO USE) Reserved */
    } details;
    
} EM_EXT_EVENT_RESULT;

/* Event Plugin Detection API */
typedef EM_EXT_EVENT_RESULT (*EM_EXT_EVENT_DETECTION_API)(void);                /* Detection API prototype */

/* Event Plugin Callback API */
typedef void    (*EM_EXT_EVENT_CALLBACK_MDM_CYCEND)     (uint8_t);                              /* Max demand cycle end callback prototype             */
typedef void    (*EM_EXT_EVENT_CALLBACK_TARIFF_CHANGE)  (uint8_t);                              /* Tariff changed callback prototype                   */
typedef void    (*EM_EXT_EVENT_CALLBACK_ENERGY_CYCEND)  (void);                                 /* Energy cycle end callback prototype                 */

/* Event Plugin Information */
typedef struct tagEMExtEventPluginInfo
{   
    /* Plugin Detection (Connector) API, NULL to deactive the plugin */
    EM_EXT_EVENT_DETECTION_API              fp_detection;               /* Function pointer of event detection API */

    /* Callback for event, specify NULL to no use callback */
    EM_EXT_CALLBACK                         fp_cb_active_noload_R;      /* Active Noload Event Callback */
    EM_EXT_CALLBACK                         fp_cb_active_noload_Y;      /* Active Noload Event Callback */
    EM_EXT_CALLBACK                         fp_cb_active_noload_B;      /* Active Noload Event Callback */
    EM_EXT_CALLBACK                         fp_cb_reactive_noload_R;    /* Reactive Noload Event Callback */
    EM_EXT_CALLBACK                         fp_cb_reactive_noload_Y;    /* Reactive Noload Event Callback */
    EM_EXT_CALLBACK                         fp_cb_reactive_noload_B;    /* Reactive Noload Event Callback */
    EM_EXT_CALLBACK                         fp_cb_apparent_noload_R;    /* Apparent Noload Event Callback */
    EM_EXT_CALLBACK                         fp_cb_apparent_noload_Y;    /* Apparent Noload Event Callback */
    EM_EXT_CALLBACK                         fp_cb_apparent_noload_B;    /* Apparent Noload Event Callback */
    EM_EXT_CALLBACK                         fp_cb_peak_voltage_R;       /* Peak Voltage Event Callback */
    EM_EXT_CALLBACK                         fp_cb_peak_voltage_Y;       /* Peak Voltage Event Callback */
    EM_EXT_CALLBACK                         fp_cb_peak_voltage_B;       /* Peak Voltage Event Callback */
    EM_EXT_CALLBACK                         fp_cb_peak_current_R;       /* Peak Current Event Callback */
    EM_EXT_CALLBACK                         fp_cb_peak_current_Y;       /* Peak Current Event Callback */
    EM_EXT_CALLBACK                         fp_cb_peak_current_B;       /* Peak Current Event Callback */
    EM_EXT_CALLBACK                         fp_cb_freq_out_range_R;     /* Freq. Out of range Event Callback */
    EM_EXT_CALLBACK                         fp_cb_freq_out_range_Y;     /* Freq. Out of range Event Callback */
    EM_EXT_CALLBACK                         fp_cb_freq_out_range_B;     /* Freq. Out of range Event Callback */
    
    EM_EXT_CALLBACK                         fp_cb_sag_voltage_R;        /* SAG (Low) voltage Event Callback */
    EM_EXT_CALLBACK                         fp_cb_sag_voltage_Y;        /* SAG (Low) voltage Event Callback */
    EM_EXT_CALLBACK                         fp_cb_sag_voltage_B;        /* SAG (Low) voltage Event Callback */
    EM_EXT_CALLBACK                         fp_cb_swell_voltage_R;      /* SWELL (High) voltage Event Callback */
    EM_EXT_CALLBACK                         fp_cb_swell_voltage_Y;      /* SWELL (High) voltage Event Callback */
    EM_EXT_CALLBACK                         fp_cb_swell_voltage_B;      /* SWELL (High) voltage Event Callback */
    EM_EXT_EVENT_CALLBACK_MDM_CYCEND        fp_cb_mdm_end;              /* Max Demand End Event Callback */
    EM_EXT_EVENT_CALLBACK_TARIFF_CHANGE     fp_cb_tariff_changed;       /* Tariff Changed Event Callback */
    EM_EXT_EVENT_CALLBACK_ENERGY_CYCEND     fp_cb_energy_cycend;        /* Energy Update End Event Callback */
} EM_EXT_EVENT_PLUGIN_INFO;

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
uint8_t EM_EXT_EVENT_Register(NEAR_PTR const EM_EXT_EVENT_PLUGIN_INFO *p_plugin_info);

#endif /* _EM_EVENT_ADAPTER_H */
