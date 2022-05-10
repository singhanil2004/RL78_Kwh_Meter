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
* File Name    : 
* Version      : 1.00
* Device(s)    : RL78/I1C
* Tool-Chain   : CCRL
* H/W Platform : RL78/I1C Energy Meter Platform
* Description  : EM Shared Adapter Layer APIs
***********************************************************************************************************************/

#ifndef _RL78I1C_SW_HPF_CONFIG_H
#define _RL78I1C_SW_HPF_CONFIG_H

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include "typedef.h"                /* GSCE Standard Typedef */
#include "math.h"

/* Check to ensure RL78-S3 CPU core is using */
#ifdef __CCRL__
#if (__RL78_S3__ != 1)
#error "Must be using RL78-S3 CPU core"
#endif /* __RL78_S3__ != 1 */
#endif /* __CCRL__ */

#ifdef __ICCRL78__
#if (__CORE__ != __S3__)
#error "Must be using RL78-S3 CPU core"
#endif /* __RL78_S3__ != 1 */
#endif /* __ICCRL78__ */

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/
typedef struct tagSoftwareHPF16Buffer
{
    int16_t v_input_prev;
    int16_t v_output_prev;
    int32_t remainder_acc;
} RL78I1C_HPF16_BUFFER;

typedef enum tagSoftwareHPF16
{
    RL78I1C_HPF16_OK,
    RL78I1C_HPF16_ERROR,
    
} RL78I1C_HPF16_STATUS;
/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#define RL78I1C_HPF16_MAX_NUM_OF_CHANNEL                 (6)
#define RL78I1C_HPF_CUTOFF_FREQ_REG_SETTING_MAX          (3)

/***********************************************************************************************************************
Variable Externs
***********************************************************************************************************************/

/***********************************************************************************************************************
Functions Prototypes
***********************************************************************************************************************/
RL78I1C_HPF16_STATUS RL78I1C_InitSoftwareHPF16(uint8_t cutoff_def);
void RL78I1C_ResetSoftwareHPF16(uint8_t channel);
int16_t RL78I1C_DoSoftwareHPF16(int16_t input, uint8_t channel);

#endif /* _RL78I1C_SW_HPF_CONFIG_H */