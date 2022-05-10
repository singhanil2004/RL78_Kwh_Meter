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
* File Name    : em_event_plugin.h
* Version      : 1.00
* Device(s)    : RL78/I1C
* Tool-Chain   : CCRL
* H/W Platform : RL78/I1C Energy Meter Platform
* Description  : EM Event Plugin Header
***********************************************************************************************************************/

#ifndef _EM_EVENT_PLUGIN_H
#define _EM_EVENT_PLUGIN_H

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include "typedef.h"        /* GSCE Standard Typedef */
#include "em_adapter.h"      /* Shared Plugin Definition */

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/
/* Event Plugin Callback API */
typedef void    (*EM_EXT_EVENT_CALLBACK_MDM_CYCEND)     (uint8_t);                              /* Max demand cycle end callback prototype             */
typedef void    (*EM_EXT_EVENT_CALLBACK_TARIFF_CHANGE)  (uint8_t);                              /* Tariff changed callback prototype                   */
typedef void    (*EM_EXT_EVENT_CALLBACK_ENERGY_CYCEND)  (void);                                 /* Energy cyc */

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Variable Externs
***********************************************************************************************************************/

/***********************************************************************************************************************
Functions Prototypes
***********************************************************************************************************************/
/* Reset event plugin stick count */
void EM_EXT_EVENT_ResetStickCount(void);

/* Event plugin registration */
uint8_t EM_EXT_EVENT_RegisterPlugin(
                                    EM_EXT_CALLBACK cb_active_noload_R,                    /* Active Noload Callback */
                                    EM_EXT_CALLBACK cb_active_noload_Y,                    /* Active Noload Callback */
                                    EM_EXT_CALLBACK cb_active_noload_B,                    /* Active Noload Callback */
                                    EM_EXT_CALLBACK cb_reactive_noload_R,                  /* Reactive Noload Callback */
                                    EM_EXT_CALLBACK cb_reactive_noload_Y,                  /* Reactive Noload Callback */
                                    EM_EXT_CALLBACK cb_reactive_noload_B,                  /* Reactive Noload Callback */
                                    EM_EXT_CALLBACK cb_apparent_noload_R,                  /* Apparent Noload Callback */
                                    EM_EXT_CALLBACK cb_apparent_noload_Y,                  /* Apparent Noload Callback */
                                    EM_EXT_CALLBACK cb_apparent_noload_B,                  /* Apparent Noload Callback */
                                    EM_EXT_CALLBACK cb_peak_voltage_R,                     /* Peak Voltage Callback */
                                    EM_EXT_CALLBACK cb_peak_voltage_Y,                     /* Peak Voltage Callback */
                                    EM_EXT_CALLBACK cb_peak_voltage_B,                     /* Peak Voltage Callback */
                                    EM_EXT_CALLBACK cb_peak_current_R,                     /* Peak Current Callback */
                                    EM_EXT_CALLBACK cb_peak_current_Y,                     /* Peak Current Callback */
                                    EM_EXT_CALLBACK cb_peak_current_B,                     /* Peak Current Callback */
                                    EM_EXT_CALLBACK cb_freq_out_range_R,                   /* Frequency Out Of Range Callback */
                                    EM_EXT_CALLBACK cb_freq_out_range_Y,                   /* Frequency Out Of Range Callback */
                                    EM_EXT_CALLBACK cb_freq_out_range_B,                   /* Frequency Out Of Range Callback */
                                    EM_EXT_CALLBACK cb_sag_voltage_R,                       /* SAG (Low) voltage Callback */
                                    EM_EXT_CALLBACK cb_sag_voltage_Y,                       /* SAG (Low) voltage Callback */
                                    EM_EXT_CALLBACK cb_sag_voltage_B,                       /* SAG (Low) voltage Callback */
                                    EM_EXT_CALLBACK cb_swell_voltage_R,                     /* SWELL (High) voltage Callback */
                                    EM_EXT_CALLBACK cb_swell_voltage_Y,                     /* SWELL (High) voltage Callback */
                                    EM_EXT_CALLBACK cb_swell_voltage_B,                     /* SWELL (High) voltage Callback */
                                    EM_EXT_EVENT_CALLBACK_MDM_CYCEND cb_max_demand,        /* Max Demand Update End Callback */
                                    EM_EXT_EVENT_CALLBACK_TARIFF_CHANGE cb_tariff_changed, /* Tariff Changed Callback */
                                    EM_EXT_EVENT_CALLBACK_ENERGY_CYCEND cb_energy_cycend   /* Energy Update End Callback */
                                    );

#endif /* _EM_EVENT_PLUGIN_H */
