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
* File Name    : em_measurement.h
* Version      : 1.00
* Device(s)    : RL78/I1C
* Tool-Chain   : CCRL
* H/W Platform : RL78/I1C Energy Meter Platform
* Description  : EM Core Measurement Middleware Header file
***********************************************************************************************************************/

#ifndef _EM_MEASUREMENT_H
#define _EM_MEASUREMENT_H

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
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
/* Phase angle (in degree) */
float32_t EM_GetPhaseAngleRtoY(void);
float32_t EM_GetPhaseAngleYtoB(void);
float32_t EM_GetPhaseAngleBtoR(void);

/* Power */
float32_t EM_GetActivePower(EM_LINE line);
float32_t EM_GetFundamentalActivePower(EM_LINE line);
float32_t EM_GetReactivePower(EM_LINE line);
float32_t EM_GetApparentPower(EM_LINE line);

/* Max demand */
/* All running time */
uint32_t EM_GetActiveMaxDemand(EM_LINE line);
uint32_t EM_GetReactiveMaxDemand(EM_LINE line);
uint32_t EM_GetApparentMaxDemand(EM_LINE line);

/* Per Period */
uint32_t EM_GetActiveMaxDemandPerPeriod(EM_LINE line);
uint32_t EM_GetReactiveMaxDemandPerPeriod(EM_LINE line);
uint32_t EM_GetApparentMaxDemandPerPeriod(EM_LINE line);

/* Energy */
uint8_t EM_GetActiveEnergyTariff(uint8_t tariff, float32_t NEAR_PTR *energy, EM_LINE line);
uint8_t EM_GetCapacitiveReactiveEnergyTariff(uint8_t tariff, float32_t NEAR_PTR *energy, EM_LINE line);
uint8_t EM_GetInductiveReactiveEnergyTariff(uint8_t tariff, float32_t NEAR_PTR *energy, EM_LINE line);
uint8_t EM_GetApparentEnergyTariff(uint8_t tariff, float32_t NEAR_PTR *energy, EM_LINE line);
float32_t EM_GetActiveEnergyTotal(EM_LINE line);
float32_t EM_GetCapacitiveReactiveEnergyTotal(EM_LINE line);
float32_t EM_GetInductiveReactiveEnergyTotal(EM_LINE line);
float32_t EM_GetApparentEnergyTotal(EM_LINE line);

/* VRMS */
float32_t EM_GetVoltageRMS(EM_LINE line);
float32_t EM_GetFundamentalVoltageRMS(EM_LINE line);

/* IRMS */
float32_t EM_GetCurrentRMS(EM_LINE line);
float32_t EM_GetFundamentalCurrentRMS(EM_LINE line);

/* PF */
float32_t EM_GetPowerFactor(EM_LINE line);
EM_PF_SIGN EM_GetPowerFactorSign(EM_LINE line);

/* Line Frequency */
float32_t EM_GetLineFrequency(EM_LINE line);

/* Signal Offset */
int32_t EM_GetCurrentOffsetAverage(EM_LINE line);

/* Vector Value */
float32_t EM_GetPhaseCurrentSumVector(void);

/* Total Harmonic Distortion */
float32_t EM_GetVoltageTHD(EM_LINE line);
float32_t EM_GetCurrentTHD(EM_LINE line);
float32_t EM_GetActivePowerTHD(EM_LINE line);

float32_t EM_GetCurrentPhaseAngle(EM_LINE base_line, EM_LINE relative_line);

#endif /* _EM_MEASUREMENT_H */
