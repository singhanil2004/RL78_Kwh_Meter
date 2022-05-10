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
* File Name    : em_operation.h
* Version      : 1.00
* Device(s)    : RL78/I1C
* Tool-Chain   : CCRL
* H/W Platform : RL78/I1C Energy Meter Platform
* Description  : EM Core Operation Middleware Header file
***********************************************************************************************************************/

#ifndef _EM_OPERATION_H
#define _EM_OPERATION_H

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include "em_constraint.h"  /* EM Constraint Definitions */
#include "em_errcode.h"     /* EM Error Code Definitions */
#include "em_type.h"        /* EM Type Definitions */

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Variable Externs
***********************************************************************************************************************/

/***********************************************************************************************************************
Functions Prototypes
***********************************************************************************************************************/
/* Core Operation */
uint8_t EM_Init(NEAR_PTR  EM_PLATFORM_PROPERTY *p_property, NEAR_PTR  EM_CONFIG * p_config, NEAR_PTR EM_CALIBRATION * p_calib);
uint8_t EM_Start(void);
uint8_t EM_Stop(void);
EM_SYSTEM_STATE EM_GetSystemState(void);

/* Configuration */
EM_CONFIG EM_GetConfig(void);
uint8_t EM_SetConfig(NEAR_PTR EM_CONFIG * p_config);

/* Recovery */
uint8_t EM_GetState(NEAR_PTR uint8_t *buf, uint16_t bufsize, NEAR_PTR uint16_t *statesize);
uint8_t EM_SetState(NEAR_PTR uint8_t *buf, uint16_t bufsize, uint16_t statesize);

/* Status */
EM_STATUS EM_GetStatus(void);

/* Max demand */
uint8_t EM_ResetMaxDemand(EM_SOURCE_MAX_DEMAND source, EM_LINE line);

/* Energy */
uint8_t EM_ResetEnergyTariff(uint8_t tariff, EM_SOURCE_ENERGY source, EM_LINE line);
uint8_t EM_ResetEnergyTotal(EM_SOURCE_ENERGY source, EM_LINE line);
uint8_t EM_AddEnergyToCurrentTariff(float32_t power, float32_t second, EM_LINE line);

/* Tariff */
uint8_t EM_GetActiveTariff(void);

/* Sag Swell */
uint16_t EM_GetLastSagDuration(EM_LINE line);
uint16_t EM_GetLastSwellDuration(EM_LINE line);

/* Configure pulse power */
void EM_PULSE0_SetPower(float32_t input_power);
void EM_PULSE1_SetPower(float32_t input_power);
void EM_PULSE2_SetPower(float32_t input_power);
void EM_PULSE3_SetPower(float32_t input_power);
void EM_PULSE4_SetPower(float32_t input_power);

#endif /* _EM_OPERATION_H */
