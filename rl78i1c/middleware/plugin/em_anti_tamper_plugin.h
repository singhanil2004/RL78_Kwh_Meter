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
* File Name    : em_anti_tamper_plugin.h
* Version      : 1.00
* Device(s)    : RL78/I1C
* Tool-Chain   : CCRL
* H/W Platform : RL78/I1C Energy Meter Platform
* Description  : EM Anti-tamper Plugin Header
***********************************************************************************************************************/

#ifndef _EM_ANTI_TAMPER_PLUGIN_H
#define _EM_ANTI_TAMPER_PLUGIN_H

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include "typedef.h"        /* GSCE Standard Typedef */
#include "em_adapter.h"      /* Shared Plugin Definition */

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

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
void EM_EXT_TAMPER_ResetStickCount(void);

/* Anti-tamper plugin registration */
uint8_t EM_EXT_TAMPER_RegisterPlugin(
    EM_EXT_CALLBACK     cb_neutral_missing,
    EM_EXT_CALLBACK     cb_MissingPotential_R,
    EM_EXT_CALLBACK     cb_MissingPotential_Y,
    EM_EXT_CALLBACK     cb_MissingPotential_B,
    EM_EXT_CALLBACK     cb_VoltageUnbalance,
    EM_EXT_CALLBACK     cb_ReverseCurrent_R,
    EM_EXT_CALLBACK     cb_ReverseCurrent_Y,
    EM_EXT_CALLBACK     cb_ReverseCurrent_B,
    EM_EXT_CALLBACK     cb_OpenCurrent_R,
    EM_EXT_CALLBACK     cb_OpenCurrent_Y,
    EM_EXT_CALLBACK     cb_OpenCurrent_B,
    EM_EXT_CALLBACK     cb_CurrentUnbalance,
    EM_EXT_CALLBACK     cb_CurrentBypass
);

#endif /* _EM_ANTI_TAMPER_PLUGIN_H */
