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
* File Name    : wrp_em_adc_data.h
* Version      : 1.00
* Device(s)    : RL78/I1C
* Tool-Chain   : CCRL
* H/W Platform : RL78/I1C Energy Meter Platform
* Description  : ADC Wrapper for RL78/I1C Platform
***********************************************************************************************************************/

#ifndef _WRAPPER_EM_ADC_DATA_H
#define _WRAPPER_EM_ADC_DATA_H

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include "typedef.h"            /* GSCE Standard Typedef */
#include "em_type.h"            /* EM Core Type Definitions */
#include "wrp_em_sw_config.h"
#include "platform.h"

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/
typedef struct
{
    int16_t     pos;        /* Position */
    uint16_t    step;       /* Step */
    
    int16_t     circle_phase_r[EM_ADC_DELAY_STEP_VOLTAGE+1];
    int16_t     circle_phase_y[EM_ADC_DELAY_STEP_VOLTAGE+1];
    int16_t     circle_phase_b[EM_ADC_DELAY_STEP_VOLTAGE+1];

} sw_delay_voltage;

typedef struct
{
    int16_t     pos;        /* Position */
    uint16_t    step;       /* Step */
    
    int16_t     circle_phase_r[EM_ADC_DELAY_VOLTAGE90_MAX_STEP+1];
    int16_t     circle_phase_y[EM_ADC_DELAY_VOLTAGE90_MAX_STEP+1];
    int16_t     circle_phase_b[EM_ADC_DELAY_VOLTAGE90_MAX_STEP+1];

} sw_delay_voltage90;

typedef struct tagEMTimerTable
{
    uint16_t    fsteps_x100;            /* x100 scaled fractional steps */
    uint16_t    timer_value;            /* pre-calculated timer value */
} TIMER_TABLE;

typedef struct tagEM
{
    /* Current signal */
    struct
    {
        EM_SW_SAMP_TYPE i;
        int16_t v;
        int16_t v90;
    } phase_r;                      /* Phase_R */
    
    struct
    {
        EM_SW_SAMP_TYPE i;
        int16_t v;
        int16_t v90;
    } phase_y;                      /* Phase_Y */
    
    struct
    {
        EM_SW_SAMP_TYPE i;
        int16_t v;
        int16_t v90;
    } phase_b;                      /* Phase_B */
    
    struct 
    {
        EM_SW_SAMP_TYPE i;
    } neutral;                      /* Neutral */
     
} WRP_ADC_SAMPLE;

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#define TIMER_TABLE_SIZE                (96)

#ifdef __DEBUG
//#define EM_ADC_MAX_DEBUG_SAMPLE         (140)
#define EM_ADC_MAX_DEBUG_SAMPLE         (40) //6KB
#endif /* __DEBUG */


/* Sample structure: for direct accessing with assembly code */
#define OFFSET_SAMPLE_PHASE_R_I                  (0x0000)
#define OFFSET_SAMPLE_PHASE_R_V                  (0x0004)
#define OFFSET_SAMPLE_PHASE_R_V90                (0x0008)
#define OFFSET_SAMPLE_PHASE_R_FILTERED_I         (0x000C)
#define OFFSET_SAMPLE_PHASE_R_FILTERED_V         (0x0010)
        
#define OFFSET_SAMPLE_PHASE_Y_I                  (0x0014)
#define OFFSET_SAMPLE_PHASE_Y_V                  (0x0018)
#define OFFSET_SAMPLE_PHASE_Y_V90                (0x001C)
#define OFFSET_SAMPLE_PHASE_Y_FILTERED_I         (0x0020)
#define OFFSET_SAMPLE_PHASE_Y_FILTERED_V         (0x0024)
        
#define OFFSET_SAMPLE_PHASE_B_I                  (0x0028)
#define OFFSET_SAMPLE_PHASE_B_V                  (0x002C)
#define OFFSET_SAMPLE_PHASE_B_V90                (0x0030)
#define OFFSET_SAMPLE_PHASE_B_FILTERED_I         (0x0034)
#define OFFSET_SAMPLE_PHASE_B_FILTERED_V         (0x0038)

#define OFFSET_SAMPLE_NEUTRAL_I                  (0x003C)

/***********************************************************************************************************************
Variable Externs
***********************************************************************************************************************/
/* Import from wrp_em_adc_data.c */
#if (EM_ADC_DELAY_STEP_VOLTAGE > 0)

extern int16_t     g_wrp_adc_voltage_circle_pos; 
extern uint16_t    g_wrp_adc_voltage_circle_step;

extern int16_t g_wrp_adc_voltage_circle_phase_r[];
extern int16_t g_wrp_adc_voltage_circle_phase_y[];
extern int16_t g_wrp_adc_voltage_circle_phase_b[];

#endif /* EM_ADC_DELAY_STEP_VOLTAGE > 0 */

#if (EM_ADC_DELAY_VOLTAGE90_MAX_STEP > 0)

extern int16_t     g_wrp_adc_voltage90_circle_pos; 
extern uint16_t    g_wrp_adc_voltage90_circle_step;

#ifdef METER_WRAPPER_ADC_COPY_REACTIVE_SAMPLE
extern int16_t g_wrp_adc_voltage90_circle_phase_r[];
extern int16_t g_wrp_adc_voltage90_circle_phase_y[];
extern int16_t g_wrp_adc_voltage90_circle_phase_b[];
#endif /* METER_WRAPPER_ADC_COPY_REACTIVE_SAMPLE */

#endif /* EM_ADC_DELAY_VOLTAGE90_MAX_STEP > 0 */

extern int16_t g_wrp_adc_buffer_phase_r_v;
extern int16_t g_wrp_adc_buffer_phase_y_v;
extern int16_t g_wrp_adc_buffer_phase_b_v;

extern int16_t g_wrp_adc_buffer_phase_r_v90;
extern int16_t g_wrp_adc_buffer_phase_y_v90;
extern int16_t g_wrp_adc_buffer_phase_b_v90;

extern int16_t g_wrp_adc_offset_phase_r_v;
extern int16_t g_wrp_adc_offset_phase_y_v;
extern int16_t g_wrp_adc_offset_phase_b_v;

extern EM_SAMPLES g_wrp_adc_samples;

#ifdef __DEBUG

extern uint16_t g_wrp_sample_count;
extern int16_t g_wrp_sample_v90[][3];
extern int16_t g_wrp_sample_adc[][3];
extern int32_t g_wrp_sample_dsad[][4];

#endif /* __DEBUG */

extern volatile uint8_t g_wrp_adc_tdr_ignore;
extern float32_t g_fsteps_timer_offset;
extern float32_t g_previous_fac;
extern uint8_t g_fac_update_counter;

extern uint8_t g_sampling_chain_corrupt;

extern const TIMER_TABLE    g_wrp_adc_timer_table[TIMER_TABLE_SIZE]; 
extern uint8_t              g_EM_ADC_GainPhaseCurrentLevel[];
extern const dsad_channel_t g_wrp_adc_channel[];
extern const dsad_gain_t    g_wrp_adc_channel_gain[][EM_GAIN_PHASE_NUM_LEVEL_MAX];

#define TIMER_TABLE_OFFSET      (g_wrp_adc_timer_table[0].fsteps_x100)

/***********************************************************************************************************************
Functions Prototypes
***********************************************************************************************************************/

float32_t EM_ADC_SAMP_GetTimerOffset(void);
void EM_ADC_SAMP_SetTimerOffset(float32_t value);
void EM_ADC_SAMP_PollingProcessing(void);


#endif /* _WRAPPER_EM_ADC_DATA_H */

