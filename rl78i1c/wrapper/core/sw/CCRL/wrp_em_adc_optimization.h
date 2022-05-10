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
* File Name    : wrp_em_adc_optimization.h
* Version      : 1.00
* Device(s)    : RL78/I1C
* Tool-Chain   : CCRL
* H/W Platform : RL78/I1C Energy Meter Platform
* Description  : ADC Wrapper for RL78/I1C Platform
***********************************************************************************************************************/
#ifndef __WRAPPER_ADC_OPTIMIZATION_H
#define __WRAPPER_ADC_OPTIMIZATION_H

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"

#include "wrp_em_sw_config.h"   /* Wrapper Shared Configuration */
#include "wrp_em_adc_data.h"

#include "platform.h"

/******************************************************************************
Typedef definitions
******************************************************************************/

/* Section definition */
#pragma section text    WrpAdcText
#pragma section const   WrpAdcConst
#ifdef __DEBUG
#pragma section bss     WrpAdcBss
#pragma section data    WrpAdcData
#endif /* __DEBUG */

/******************************************************************************
Macro definitions
******************************************************************************/
/* CUSTOM DEFINITION FOR OPTIMIZATION */
#define DSADCR0_R           (0x3E0)
#define DSADCR1_R           (0x3E4)
#define DSADCR2_R           (0x3E8)
#define DSADCR3_R           (0x3EC)

#if (EM_ADC_CURRENT_DRIVER_CHANNEL_OPTIMIZATION_PHASE_R == 0)
    #define EM_ADC_CHANNEL_RESULT_REG_ADDR_PHASE_R      DSADCR0_R
#elif (EM_ADC_CURRENT_DRIVER_CHANNEL_OPTIMIZATION_PHASE_R == 1)
    #define EM_ADC_CHANNEL_RESULT_REG_ADDR_PHASE_R      DSADCR1_R
#elif (EM_ADC_CURRENT_DRIVER_CHANNEL_OPTIMIZATION_PHASE_R == 2)
    #define EM_ADC_CHANNEL_RESULT_REG_ADDR_PHASE_R      DSADCR2_R
#elif (EM_ADC_CURRENT_DRIVER_CHANNEL_OPTIMIZATION_PHASE_R == 3)
    #define EM_ADC_CHANNEL_RESULT_REG_ADDR_PHASE_R      DSADCR3_R
#endif /* EM_ADC_CURRENT_DRIVER_CHANNEL_OPTIMIZATION_PHASE_R == 0 */

#if (EM_ADC_CURRENT_DRIVER_CHANNEL_OPTIMIZATION_PHASE_Y == 0)
    #define EM_ADC_CHANNEL_RESULT_REG_ADDR_PHASE_Y      DSADCR0_R
#elif (EM_ADC_CURRENT_DRIVER_CHANNEL_OPTIMIZATION_PHASE_Y == 1)
    #define EM_ADC_CHANNEL_RESULT_REG_ADDR_PHASE_Y      DSADCR1_R
#elif (EM_ADC_CURRENT_DRIVER_CHANNEL_OPTIMIZATION_PHASE_Y == 2)
    #define EM_ADC_CHANNEL_RESULT_REG_ADDR_PHASE_Y      DSADCR2_R
#elif (EM_ADC_CURRENT_DRIVER_CHANNEL_OPTIMIZATION_PHASE_Y == 3)
    #define EM_ADC_CHANNEL_RESULT_REG_ADDR_PHASE_Y      DSADCR3_R
#endif /* EM_ADC_CURRENT_DRIVER_CHANNEL_OPTIMIZATION_PHASE_Y == 0 */

#if (EM_ADC_CURRENT_DRIVER_CHANNEL_OPTIMIZATION_PHASE_B == 0)
#define EM_ADC_CHANNEL_RESULT_REG_ADDR_PHASE_B      DSADCR0_R
#elif (EM_ADC_CURRENT_DRIVER_CHANNEL_OPTIMIZATION_PHASE_B == 1)
#define EM_ADC_CHANNEL_RESULT_REG_ADDR_PHASE_B      DSADCR1_R
#elif (EM_ADC_CURRENT_DRIVER_CHANNEL_OPTIMIZATION_PHASE_B == 2)
#define EM_ADC_CHANNEL_RESULT_REG_ADDR_PHASE_B      DSADCR2_R
#elif (EM_ADC_CURRENT_DRIVER_CHANNEL_OPTIMIZATION_PHASE_B == 3)
#define EM_ADC_CHANNEL_RESULT_REG_ADDR_PHASE_B      DSADCR3_R
#endif /* EM_ADC_CURRENT_DRIVER_CHANNEL_OPTIMIZATION_PHASE_B == 0 */

#if (EM_ADC_CURRENT_DRIVER_CHANNEL_OPTIMIZATION_NEUTRAL == 0)
#define EM_ADC_CHANNEL_RESULT_REG_ADDR_NEUTRAL      DSADCR0_R
#elif (EM_ADC_CURRENT_DRIVER_CHANNEL_OPTIMIZATION_NEUTRAL == 1)
#define EM_ADC_CHANNEL_RESULT_REG_ADDR_NEUTRAL      DSADCR1_R
#elif (EM_ADC_CURRENT_DRIVER_CHANNEL_OPTIMIZATION_NEUTRAL == 2)
#define EM_ADC_CHANNEL_RESULT_REG_ADDR_NEUTRAL      DSADCR2_R
#elif (EM_ADC_CURRENT_DRIVER_CHANNEL_OPTIMIZATION_NEUTRAL == 3)
#define EM_ADC_CHANNEL_RESULT_REG_ADDR_NEUTRAL      DSADCR3_R
#endif /* EM_ADC_CURRENT_DRIVER_CHANNEL_OPTIMIZATION_NEUTRAL == 0 */

/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/

/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/

/******************************************************************************
Private global variables and functions
******************************************************************************/

/******************************************************************************
* Function Name    : static void EM_ADC_Voltage_CirclePushAndPop(void)
* Description      : Shift input signal as expected step for voltage channels
* Arguments        : None
* Functions Called : None
* Return Value     : None
******************************************************************************/
#pragma inline_asm EM_ADC_Voltage_CirclePushAndPop
static void EM_ADC_Voltage_CirclePushAndPop(void)
{
    #ifdef __CCRL__
    ;Push_pos
    MOVW    AX, !_g_wrp_adc_voltage_circle_pos
    ADDW    AX, AX
    MOVW    BC, AX
    
    MOVW    AX, !_g_wrp_adc_buffer_phase_r_v
    MOVW    _g_wrp_adc_voltage_circle_phase_r[BC], AX
    
    MOVW    AX, !_g_wrp_adc_buffer_phase_y_v
    MOVW    _g_wrp_adc_voltage_circle_phase_y[BC], AX
    
    MOVW    AX, !_g_wrp_adc_buffer_phase_b_v
    MOVW    _g_wrp_adc_voltage_circle_phase_b[BC], AX
    
    ;Pop_pos
    MOVW    AX, [SP+0x00]
    ADDW    AX, AX
    MOVW    BC, AX
    
    MOVW    AX, _g_wrp_adc_voltage_circle_phase_r[BC]
    MOVW    !_g_wrp_adc_buffer_phase_r_v, AX
    
    MOVW    AX, _g_wrp_adc_voltage_circle_phase_y[BC]
    MOVW    !_g_wrp_adc_buffer_phase_y_v, AX
    
    MOVW    AX, _g_wrp_adc_voltage_circle_phase_b[BC]
    MOVW    !_g_wrp_adc_buffer_phase_b_v, AX
    #endif /* __CCRL__ */
}

#ifdef METER_WRAPPER_ADC_COPY_REACTIVE_SAMPLE
/******************************************************************************
* Function Name    : static void EM_ADC_Voltage90_CirclePushAndPop(void)
* Description      : Shift input signal as expected step for 90 degree voltage channels
* Arguments        : None
* Functions Called : None
* Return Value     : None
******************************************************************************/
#pragma inline_asm EM_ADC_Voltage90_CirclePushAndPop
static void EM_ADC_Voltage90_CirclePushAndPop(void)
{
    #ifdef __CCRL__
    ;Push_pos
    MOVW    AX, !_g_wrp_adc_voltage90_circle_pos
    ADDW    AX, AX
    MOVW    BC, AX
    
    MOVW    AX, !_g_wrp_adc_buffer_phase_r_v90
    MOVW    _g_wrp_adc_voltage90_circle_phase_r[BC], AX
    
    MOVW    AX, !_g_wrp_adc_buffer_phase_y_v90
    MOVW    _g_wrp_adc_voltage90_circle_phase_y[BC], AX
    
    MOVW    AX, !_g_wrp_adc_buffer_phase_b_v90
    MOVW    _g_wrp_adc_voltage90_circle_phase_b[BC], AX
    
    ;Pop_pos
    MOVW    AX, [SP+0x00]
    ADDW    AX, AX
    MOVW    BC, AX
    
    MOVW    AX, _g_wrp_adc_voltage90_circle_phase_r[BC]
    MOVW    !_g_wrp_adc_buffer_phase_r_v90, AX
    
    MOVW    AX, _g_wrp_adc_voltage90_circle_phase_y[BC]
    MOVW    !_g_wrp_adc_buffer_phase_y_v90, AX
    
    MOVW    AX, _g_wrp_adc_voltage90_circle_phase_b[BC]
    MOVW    !_g_wrp_adc_buffer_phase_b_v90, AX
    #endif /* __CCRL__ */
    
}
#endif /* METER_WRAPPER_ADC_COPY_REACTIVE_SAMPLE */

/******************************************************************************
* Function Name    : static void EM_ADC_Current_DriverRead(void)
* Description      : Directly read the 4 DSAD sample into ADC buffer
* Arguments        : None
* Functions Called : None
* Return Value     : None
******************************************************************************/
#pragma inline_asm EM_ADC_Current_DriverRead
static void EM_ADC_Current_DriverRead(void)
{
    #ifdef __CCRL__
    /* Phase_R Current */  
;READ_PHASE_R
    MOVW    AX, !(EM_ADC_CHANNEL_RESULT_REG_ADDR_PHASE_R)
    MOVW    !(_g_wrp_adc_samples+OFFSET_SAMPLE_PHASE_R_I), AX
    MOV     A, !(EM_ADC_CHANNEL_RESULT_REG_ADDR_PHASE_R+0x02)
    MOV     !(_g_wrp_adc_samples+OFFSET_SAMPLE_PHASE_R_I+0x02), A
    SAR     A, 7
    MOV     !(_g_wrp_adc_samples+OFFSET_SAMPLE_PHASE_R_I+0x03), A

    /* Phase_Y Current */  
;READ_PHASE_Y    
    MOVW    AX, !(EM_ADC_CHANNEL_RESULT_REG_ADDR_PHASE_Y)
    MOVW    !(_g_wrp_adc_samples+OFFSET_SAMPLE_PHASE_Y_I), AX
    MOV     A, !(EM_ADC_CHANNEL_RESULT_REG_ADDR_PHASE_Y+0x02)
    MOV     !(_g_wrp_adc_samples+OFFSET_SAMPLE_PHASE_Y_I+0x02), A
    SAR     A, 7
    MOV     !(_g_wrp_adc_samples+OFFSET_SAMPLE_PHASE_Y_I+0x03), A
    
    /* Phase_B Current */  
;READ_PHASE_B
    MOVW    AX, !(EM_ADC_CHANNEL_RESULT_REG_ADDR_PHASE_B)
    MOVW    !(_g_wrp_adc_samples+OFFSET_SAMPLE_PHASE_B_I), AX
    MOV     A, !(EM_ADC_CHANNEL_RESULT_REG_ADDR_PHASE_B+0x02)
    MOV     !(_g_wrp_adc_samples+OFFSET_SAMPLE_PHASE_B_I+0x02), A
    SAR     A, 7
    MOV     !(_g_wrp_adc_samples+OFFSET_SAMPLE_PHASE_B_I+0x03), A

    /* Neutral Current */
;READ_NEUTRAL
    MOVW    AX, !(EM_ADC_CHANNEL_RESULT_REG_ADDR_NEUTRAL)
    MOVW    !(_g_wrp_adc_samples+OFFSET_SAMPLE_NEUTRAL_I), AX
    MOV     A, !(EM_ADC_CHANNEL_RESULT_REG_ADDR_NEUTRAL+0x02)
    MOV     !(_g_wrp_adc_samples+OFFSET_SAMPLE_NEUTRAL_I+0x02), A
    SAR     A, 7
    MOV     !(_g_wrp_adc_samples+OFFSET_SAMPLE_NEUTRAL_I+0x03), A
    #endif /* __CCRL__ */
}

#endif /* __WRAPPER_ADC_OPTIMIZATION_H */
