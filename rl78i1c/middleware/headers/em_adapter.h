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
* File Name    : em_adapter.h
* Version      : 1.00
* Device(s)    : RL78/I1C
* Tool-Chain   : CCRL
* H/W Platform : RL78/I1C Energy Meter Platform
* Description  : EM Shared Adapter Layer APIs
***********************************************************************************************************************/

#ifndef _EM_ADAPTER_H
#define _EM_ADAPTER_H

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include "typedef.h"                /* GSCE Standard Typedef */
#include "em_type.h"

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/
/* Enumeration of callback state */
typedef enum tagEMExtCallbackState
{
    EM_EXT_CALLBACK_STATE_ENTER,    /* Enter state */
    EM_EXT_CALLBACK_STATE_RELEASE   /* Release state */
} EM_EXT_CALLBACK_STATE;

/* Plugin/Event Callback API */
typedef void (*EM_EXT_CALLBACK)     (EM_EXT_CALLBACK_STATE);        /* Default plugin/event callback prototype */

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#ifndef EM_EXT_OK
#define EM_EXT_OK       0   /* OK */
#endif /* EM_EXT_OK */
#ifndef EM_EXT_ERROR
#define EM_EXT_ERROR    1   /* Error */
#endif /* EM_EXT_ERROR */

/***********************************************************************************************************************
Variable Externs
***********************************************************************************************************************/

/***********************************************************************************************************************
Functions Prototypes
***********************************************************************************************************************/

/* Shared Adapter Information Provider API */
float32_t EM_EXT_GetVRMS(EM_LINE line);             /* Get VRMS */
float32_t EM_EXT_GetIRMS(EM_LINE line);             /* Get IRMS */
float32_t EM_EXT_GetPhaseCurrentSumVector(void);    /* Get IVECTOR */

int8_t EM_EXT_GetActivePowerSign(EM_LINE line);     /* Get Active Power Sign (+, -, 0) */
int8_t EM_EXT_GetReactivePowerSign(EM_LINE line);   /* Get Reactive Power Sign (+, -, 0) */
int8_t EM_EXT_GetApparentPowerSign(EM_LINE line);   /* Get Apparent Power Sign (+, 0) */


#endif /* _EM_ADAPTER_H */
