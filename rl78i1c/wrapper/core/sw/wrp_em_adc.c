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
* File Name    : wrp_em_adc.c
* Version      : 1.00
* Device(s)    : RL78/I1C
* Tool-Chain   : CCRL
* H/W Platform : RL78/I1C Energy Meter Platform
* Description  : ADC Wrapper for RL78/I1C Platform
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#ifdef __CCRL__
/* 
 * CCRL place those region automatically to near and far (place suffix _n or _f to output section name)
 * Function default place to far area
 * Name must limit to 8 chars for compatiblity with CA78K0R
 */
#pragma section text    WrpAdcText
#pragma section const   WrpAdcConst
/* Output RAM section only when debug:
 * In release mode, if using, user also need to set in linker and initialize
 * This will increase time for customer, so use internal when debug only
*/
#ifdef __DEBUG
//#pragma section bss     WrpAdcBss
//#pragma section data    WrpAdcData
#endif /* __DEBUG */
#endif /* __CCRL__ */

/* Driver */
#include "r_cg_macrodriver.h"
#include "r_cg_dsadc.h"

/* Other EM wrapper */
#include "wrp_em_sw_config.h"   /* Wrapper Shared Configuration */
#include "wrp_em_adc.h"         /* Wrapper ADC definitions */
#include "wrp_em_adc_data.h"
#include "wrp_em_adc_optimization.h"
#include "wrp_mcu.h"
#include "em_core.h"
#include "platform.h"
#include "r_calib.h"

/* Filter middleware */
#include "rl78i1c_sw_hpf.h"
#include "rl78i1c_hpf_config.h"
#include "r_dscl_iirbiquad_i32_1.h"
#include "r_dscl_iirbiquad_i32_2.h"
#include "r_dscl_iirbiquad_config.h"

/* Application */
//#include "em_fft.h"
#include "ct_compensation.h"

/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Macro definitions
******************************************************************************/
/* Definition for sampling chain broken 
 * Current sampling chain of ADC: (2) --> (3) --> (4) --> (5)
 * Corresponding to buffer index: [0] --> [1] --> [2] --> [3] (first round)
 * The condition below based on the experiment about value of non-use buffer[0]
 * Adjust accordingly if any hardware modification
*/
#define IS_SAMPLING_CHAIN_BROKEN()               (g_adc_data[0] < 65000)

#define EM_FAC_CHANGED_DETECTION_COUNT           (5)

/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/


/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/

/******************************************************************************
Private global variables and functions
******************************************************************************/
#ifdef __DEBUG
uint8_t g_enable_auto_fstep;

#else
const uint8_t g_enable_auto_fstep = 1;

#endif /* __DEBUG */

/* Static I/F for this module */
/* Changing gain, apply gain value to the port pin */
static void EM_ADC_GainPhaseSet(EM_LINE line);              /* Set a suitable gain phase level to a port */

/******************************************************************************
* Function Name    : static void EM_ADC_GainPhaseSet(EM_LINE line)
* Description      : Set a suitable gain phase level to a port
* Arguments        : None
* Functions Called : None
* Return Value     : None
******************************************************************************/
static void EM_ADC_GainPhaseSet(EM_LINE line)
{
    uint8_t current_level = g_EM_ADC_GainPhaseCurrentLevel[line];
    dsad_channel_t channel = g_wrp_adc_channel[line];
    dsad_gain_t gain = g_wrp_adc_channel_gain[line][current_level];
    
    R_DSADC_SetChannelGain(channel, gain);
    
}


/******************************************************************************
* Function Name    : int32_t RL78I1C_DoIntegrate24(int32_t input, uint8_t channel)
* Description      : Wrap around function for future integrator, not implement yet
* Arguments        : None
* Functions Called : None
* Return Value     : None
******************************************************************************/
int32_t RL78I1C_DoIntegrate24(int32_t input, uint8_t channel)
{
    /* Not implement yet */
    return input;
}

/******************************************************************************
* Function Name    : static void EM_ADC_DoPhaseDelayAllVoltage(void)
* Description      : Do phase delay for all voltage samples accordingly
* Arguments        : None
* Functions Called : None
* Return Value     : None
******************************************************************************/
#ifdef __CCRL__
#pragma noinline EM_ADC_DoPhaseDelayAllVoltage
#endif
#ifdef __ICCRL78__
#pragma inline=never
#endif
static void EM_ADC_DoPhaseDelayAllVoltage(void)
{
    volatile int16_t pop_pos;
    
    /* Phase adjustment for V signal */
    #if (EM_ADC_DELAY_STEP_VOLTAGE > 0)
    if (g_wrp_adc_voltage_circle_pos <= 0)
    {
        pop_pos = g_wrp_adc_voltage_circle_step;
    }
    else
    {
        pop_pos = g_wrp_adc_voltage_circle_pos - 1;
    }

    /* PUSH: Add the signal into the data circle at circlepos position */
    #if 1
    EM_ADC_Voltage_CirclePushAndPop();
    #else
    g_wrp_adc_voltage_circle_phase_r[g_wrp_adc_voltage_circle_pos] = g_wrp_adc_buffer_phase_r_v;
    g_wrp_adc_voltage_circle_phase_y[g_wrp_adc_voltage_circle_pos] = g_wrp_adc_buffer_phase_y_v;
    g_wrp_adc_voltage_circle_phase_b[g_wrp_adc_voltage_circle_pos] = g_wrp_adc_buffer_phase_b_v;
    
    g_wrp_adc_buffer_phase_r_v = g_wrp_adc_voltage_circle_phase_r[pop_pos];
    g_wrp_adc_buffer_phase_y_v = g_wrp_adc_voltage_circle_phase_y[pop_pos];
    g_wrp_adc_buffer_phase_b_v = g_wrp_adc_voltage_circle_phase_b[pop_pos];
    #endif /* 0 or 1 */

    /* POP: Get the signal from data circle from (pos + steps) position */
    
    /* Scroll the circle, scroll left */
    g_wrp_adc_voltage_circle_pos--;
    if (g_wrp_adc_voltage_circle_pos == -1)
    {
        g_wrp_adc_voltage_circle_pos = g_wrp_adc_voltage_circle_step;
    }

    #endif /* EM_ADC_DELAY_STEP_VOLTAGE > 0 */

    #ifdef METER_WRAPPER_ADC_COPY_REACTIVE_SAMPLE        
    /* Phase adjustment for V90 signal */
    #if (EM_ADC_DELAY_VOLTAGE90_MAX_STEP > 0)
    if (g_wrp_adc_voltage90_circle_pos <= 0)
    {
        pop_pos = g_wrp_adc_voltage90_circle_step;
    }
    else
    {
        pop_pos = g_wrp_adc_voltage90_circle_pos - 1;
    }

    #if 1
    EM_ADC_Voltage90_CirclePushAndPop();
    #else
    
    /* PUSH: Add the signal into the data circle at circlepos position */
    
    g_wrp_adc_voltage90_circle_phase_r[g_wrp_adc_voltage90_circle_pos] = g_wrp_adc_buffer_phase_r_v90;
    g_wrp_adc_voltage90_circle_phase_y[g_wrp_adc_voltage90_circle_pos] = g_wrp_adc_buffer_phase_y_v90;
    g_wrp_adc_voltage90_circle_phase_b[g_wrp_adc_voltage90_circle_pos] = g_wrp_adc_buffer_phase_b_v90;

    /* POP: Get the signal from data circle from (pos + steps) position */
    g_wrp_adc_buffer_phase_r_v90 = g_wrp_adc_voltage90_circle_phase_r[pop_pos];
    g_wrp_adc_buffer_phase_y_v90 = g_wrp_adc_voltage90_circle_phase_y[pop_pos];
    g_wrp_adc_buffer_phase_b_v90 = g_wrp_adc_voltage90_circle_phase_b[pop_pos];
    #endif /* 0 or 1 */
    
    /* Scroll the circle, scroll left */
    g_wrp_adc_voltage90_circle_pos--;
    if (g_wrp_adc_voltage90_circle_pos == -1)
    {
        g_wrp_adc_voltage90_circle_pos = g_wrp_adc_voltage90_circle_step;
    }
    #endif /* EM_ADC_DELAY_VOLTAGE90_MAX_STEP > 0 */

    #endif /* METER_WRAPPER_ADC_COPY_REACTIVE_SAMPLE */
}
/******************************************************************************
* Function Name    : static void EM_ADC_DoPhaseDelayAllCurrent(void)
* Description      : Do phase delay for all current samples accordingly
* Arguments        : None
* Functions Called : None
* Return Value     : None
******************************************************************************/
static void EM_ADC_DoPhaseDelayAllCurrent(void)
{
    /* Not implement for current channel */
}

/******************************************************************************
* Function Name    : void EM_ADC_Init(void)
* Description      : Set a suitable gain neutral level to a port
* Arguments        : None
* Functions Called : EM_ADC_DriverInit()
* Return Value     : None
******************************************************************************/
void EM_ADC_Init(void)
{
    /* Init ADC HW Module */
    EM_ADC_DriverInit();
    
    g_wrp_adc_voltage_circle_step = EM_ADC_DELAY_STEP_VOLTAGE;

    #ifdef METER_WRAPPER_ADC_COPY_REACTIVE_SAMPLE    
    g_wrp_adc_voltage90_circle_step = EM_ADC_DELAY_STEP_VOLTAGE90;
    #endif /* METER_WRAPPER_ADC_COPY_REACTIVE_SAMPLE */

    /* Enable the fstep compensation */
    #ifdef __DEBUG
    g_enable_auto_fstep = 1;
    #endif /* __DEBUG */

    /* Init the HPF16 */
    RL78I1C_InitSoftwareHPF16(EM_SW_PROPERTY_ADC_HPF_CUTOFF_FREQUENCY);
    
    /* Init the IIR Biquad Filter */
    
}

/******************************************************************************
* Function Name    : void EM_ADC_Start(void)
* Description      : Start ADC sampling
* Arguments        : None
* Functions Called : EM_ADC_DriverStart()
* Return Value     : None
******************************************************************************/
void EM_ADC_Start(void)
{
    EM_CALIBRATION calib = EM_GetCalibInfo();
    
    /* Start ADC HW Module */
    EM_ADC_DriverStart();
    
    /* Get the voltage offset */
    g_wrp_adc_offset_phase_r_v = calib.offset.phase_r.v;
    g_wrp_adc_offset_phase_y_v = calib.offset.phase_y.v;
    g_wrp_adc_offset_phase_b_v = calib.offset.phase_b.v;
}

/******************************************************************************
* Function Name    : void EM_ADC_Stop(void)
* Description      : Stop ADC sampling
* Arguments        : None
* Functions Called : EM_ADC_DriverStop()
* Return Value     : None
******************************************************************************/
void EM_ADC_Stop(void)
{
    /* Init ADC HW Module */
    EM_ADC_DriverStop();
    
    g_wrp_adc_voltage_circle_pos = 0;
    #ifdef METER_WRAPPER_ADC_COPY_REACTIVE_SAMPLE
    g_wrp_adc_voltage90_circle_pos = 0;
    #endif /* METER_WRAPPER_ADC_COPY_REACTIVE_SAMPLE */

    /* Clear filer buffer */
    RL78I1C_ResetSoftwareHPF16(0);
    RL78I1C_ResetSoftwareHPF16(1);
    RL78I1C_ResetSoftwareHPF16(2);
    RL78I1C_ResetSoftwareHPF16(3);
    RL78I1C_ResetSoftwareHPF16(4);
    RL78I1C_ResetSoftwareHPF16(5);
    
    /* Clear DSCL Filter */
    
}

/******************************************************************************
* Function Name    : void EM_ADC_InterruptCallback(void)
* Description      : ADC Interrupt Sample processing and put to following:
*                  :    . EM_Middleware
*                  :    . Calibration App
*                  :    . Renesas protocol (Service data log)
* Arguments        : None
* Functions Called : TBD
* Return Value     : None
******************************************************************************/
void EM_ADC_InterruptCallback(void)
{
    /* Push MACRx registers */
    uint16_t bkup_macrh = MACRH;
    uint16_t bkup_macrl = MACRL;
    
    /* Read voltage samples first */
    EM_ADC_Voltage_DriverRead(EM_ADC_VOL_DRIVER_CHANNEL_PHASE_R, g_wrp_adc_buffer_phase_r_v);
    EM_ADC_Voltage_DriverRead(EM_ADC_VOL_DRIVER_CHANNEL_PHASE_Y, g_wrp_adc_buffer_phase_y_v);
    EM_ADC_Voltage_DriverRead(EM_ADC_VOL_DRIVER_CHANNEL_PHASE_B, g_wrp_adc_buffer_phase_b_v);
    
    #ifdef METER_WRAPPER_ADC_COPY_REACTIVE_SAMPLE   
    if (g_wrp_adc_tdr_ignore == 1)
    {
        /* Ignore the V90 samples, get integer steps only */
        g_wrp_adc_buffer_phase_r_v90 = g_wrp_adc_buffer_phase_r_v;
        g_wrp_adc_buffer_phase_y_v90 = g_wrp_adc_buffer_phase_y_v;
        g_wrp_adc_buffer_phase_b_v90 = g_wrp_adc_buffer_phase_b_v;
    }
    else
    {     
        EM_ADC_Voltage90_DriverRead(EM_ADC_VOL_DRIVER_CHANNEL_PHASE_R, g_wrp_adc_buffer_phase_r_v90);
        EM_ADC_Voltage90_DriverRead(EM_ADC_VOL_DRIVER_CHANNEL_PHASE_Y, g_wrp_adc_buffer_phase_y_v90);
        EM_ADC_Voltage90_DriverRead(EM_ADC_VOL_DRIVER_CHANNEL_PHASE_B, g_wrp_adc_buffer_phase_b_v90);
    } 
    #endif /* METER_WRAPPER_ADC_COPY_REACTIVE_SAMPLE */
    
    /*  Get current samples */
    EM_ADC_Current_DriverRead();
        
    /* Do phase delay: for all voltage channels */
    EM_ADC_DoPhaseDelayAllVoltage();
    
    /* Do phase delay: for all current channels */
    EM_ADC_DoPhaseDelayAllCurrent();
    
    /* Do HPF to maintain the response with I channel + */
    /* Copy the voltage signal to output buffer */
    if (g_calib_is_in_calib_mode == 0)
    {
        /* Substract the signal offset and do Software HPF*/
        g_wrp_adc_samples.phase_r.v = 
        g_wrp_adc_buffer_phase_r_v = RL78I1C_DoSoftwareHPF16(g_wrp_adc_buffer_phase_r_v-g_wrp_adc_offset_phase_r_v, 0);
        
        g_wrp_adc_samples.phase_y.v = 
        g_wrp_adc_buffer_phase_y_v = RL78I1C_DoSoftwareHPF16(g_wrp_adc_buffer_phase_y_v-g_wrp_adc_offset_phase_y_v, 1);
        
        g_wrp_adc_samples.phase_b.v = 
        g_wrp_adc_buffer_phase_b_v = RL78I1C_DoSoftwareHPF16(g_wrp_adc_buffer_phase_b_v-g_wrp_adc_offset_phase_b_v, 2);
        
        #ifdef METER_WRAPPER_ADC_COPY_REACTIVE_SAMPLE
        g_wrp_adc_samples.phase_r.v90 = 
        g_wrp_adc_buffer_phase_r_v90 = RL78I1C_DoSoftwareHPF16(g_wrp_adc_buffer_phase_r_v90-g_wrp_adc_offset_phase_r_v, 3);
        
        g_wrp_adc_samples.phase_y.v90 = 
        g_wrp_adc_buffer_phase_y_v90 = RL78I1C_DoSoftwareHPF16(g_wrp_adc_buffer_phase_y_v90-g_wrp_adc_offset_phase_y_v, 4);
        
        g_wrp_adc_samples.phase_b.v90 = 
        g_wrp_adc_buffer_phase_b_v90 = RL78I1C_DoSoftwareHPF16(g_wrp_adc_buffer_phase_b_v90-g_wrp_adc_offset_phase_b_v, 5);
        #endif /* METER_WRAPPER_ADC_COPY_REACTIVE_SAMPLE */
    }
    else
    {
        /* Pass sample to output directly, calibration will extract the offset from signal */
        g_wrp_adc_samples.phase_r.v = g_wrp_adc_buffer_phase_r_v;
        g_wrp_adc_samples.phase_y.v = g_wrp_adc_buffer_phase_y_v;
        g_wrp_adc_samples.phase_b.v = g_wrp_adc_buffer_phase_b_v;
        
        #ifdef METER_WRAPPER_ADC_COPY_REACTIVE_SAMPLE
        g_wrp_adc_samples.phase_r.v90 = g_wrp_adc_buffer_phase_r_v90;
        g_wrp_adc_samples.phase_y.v90 = g_wrp_adc_buffer_phase_y_v90;
        g_wrp_adc_samples.phase_b.v90 = g_wrp_adc_buffer_phase_b_v90;
        #endif /* METER_WRAPPER_ADC_COPY_REACTIVE_SAMPLE */
    }
    
    /* Call to middleware integrator for signals: enable this if used with Rogowski coil */
    #if (METER_ENABLE_INTEGRATOR_ON_SAMPLE == 1)
    /* Backup the sample before doing integrator for data logging service */
    #ifndef __DEBUG
        g_datalog_before_integrate_phase_r_i = g_wrp_adc_samples.phase_r.i;
        g_datalog_before_integrate_phase_y_i = g_wrp_adc_samples.phase_y.i;
        g_datalog_before_integrate_phase_b_i = g_wrp_adc_samples.phase_b.i;
        g_datalog_before_integrate_neutral_i = g_wrp_adc_samples.neutral.i;
    #endif /* __DEBUG */
    
    /* Integrate the signal from Rogowski coil, API will append later */
    g_wrp_adc_samples.phase_r.i = RL78I1C_DoIntegrate24(g_wrp_adc_samples.phase_r.i, 0);
    g_wrp_adc_samples.phase_y.i = RL78I1C_DoIntegrate24(g_wrp_adc_samples.phase_y.i, 1);
    g_wrp_adc_samples.phase_b.i = RL78I1C_DoIntegrate24(g_wrp_adc_samples.phase_b.i, 2);
    g_wrp_adc_samples.neutral.i = RL78I1C_DoIntegrate24(g_wrp_adc_samples.neutral.i, 3);
    
    #endif /* METER_ENABLE_INTEGRATOR_ON_SAMPLE == 1 */

    #ifdef METER_WRAPPER_ADC_COPY_FUNDAMENTAL_SAMPLE
    /* Do the IIRBiquad to filter the input signals, put filterd signal to buffer and the sequence of fundamental samples*/
    
    /* Run the filter on voltage channel at 3906Hz sampling frequency */
    R_DSCL_IIRBiquad_i32i32_c_nr_1(g_wrp_adc_samples.phase_r.v, &g_wrp_adc_samples.phase_r.v_fund, 0);
    R_DSCL_IIRBiquad_i32i32_c_nr_1(g_wrp_adc_samples.phase_y.v, &g_wrp_adc_samples.phase_y.v_fund, 1);
    R_DSCL_IIRBiquad_i32i32_c_nr_1(g_wrp_adc_samples.phase_b.v, &g_wrp_adc_samples.phase_b.v_fund, 2);
    
    /* Run the filter on current channel at 1302Hz sampling frequency */
    switch (g_wrp_adc_samples.status.fund_sequence)
    {
        case EM_SW_FUND_SEQUENCE_PHASE_B:
            R_DSCL_IIRBiquad_i32i32_c_nr_2(g_wrp_adc_samples.phase_r.i, &g_wrp_adc_samples.phase_r.i_fund, 0);
            g_wrp_adc_samples.status.fund_sequence = EM_SW_FUND_SEQUENCE_PHASE_R;
            break;
        case EM_SW_FUND_SEQUENCE_PHASE_R:
            R_DSCL_IIRBiquad_i32i32_c_nr_2(g_wrp_adc_samples.phase_y.i, &g_wrp_adc_samples.phase_y.i_fund, 1);
            g_wrp_adc_samples.status.fund_sequence = EM_SW_FUND_SEQUENCE_PHASE_Y;
            break;
        case EM_SW_FUND_SEQUENCE_PHASE_Y:
            R_DSCL_IIRBiquad_i32i32_c_nr_2(g_wrp_adc_samples.phase_b.i, &g_wrp_adc_samples.phase_b.i_fund, 2);
            g_wrp_adc_samples.status.fund_sequence = EM_SW_FUND_SEQUENCE_PHASE_B;
            break;
        default:
            g_wrp_adc_samples.status.fund_sequence = EM_SW_FUND_SEQUENCE_PHASE_B;
            break;
    }
    
    #endif /* METER_WRAPPER_ADC_COPY_FUNDAMENTAL_SAMPLE */

    /* Debug the signal */
    #ifdef __DEBUG
    #ifndef METER_ENABLE_MEASURE_CPU_LOAD
    if (g_wrp_sample_count < EM_ADC_MAX_DEBUG_SAMPLE)
    {
        g_wrp_sample_dsad[g_wrp_sample_count][0]    = g_wrp_adc_samples.phase_r.i;
        g_wrp_sample_adc[g_wrp_sample_count][0]     = g_wrp_adc_samples.phase_r.v;
        g_wrp_sample_v90[g_wrp_sample_count][0]     = g_wrp_adc_buffer_phase_r_v90;
        
        g_wrp_sample_dsad[g_wrp_sample_count][1]    = g_wrp_adc_samples.phase_y.i;
        g_wrp_sample_adc[g_wrp_sample_count][1]     = g_wrp_adc_samples.phase_y.v;
        g_wrp_sample_v90[g_wrp_sample_count][1]     = g_wrp_adc_buffer_phase_y_v90;
        
        g_wrp_sample_dsad[g_wrp_sample_count][2]    = g_wrp_adc_samples.phase_b.i;
        g_wrp_sample_adc[g_wrp_sample_count][2]     = g_wrp_adc_buffer_phase_b_v;
        g_wrp_sample_v90[g_wrp_sample_count][2]     = g_wrp_adc_buffer_phase_b_v90;
        
        g_wrp_sample_dsad[g_wrp_sample_count][3]    = g_wrp_adc_samples.neutral.i;
        
        g_wrp_sample_count++;
    }
    #endif /* METER_ENABLE_MEASURE_CPU_LOAD */
    #endif /* __DEBUG */
    
    if (IS_SAMPLING_CHAIN_BROKEN() == 1)
    {
        g_sampling_chain_corrupt = 1;
    }
    
    /* 
     * METROLOGY AND EXTERNAL APPLICATION PROCESSING:
     * The sample is ready. Put it to each application need
    */
    
    #ifdef EM_FFT_ENABLE
    EM_FFT_AddSample();
    #endif /* EM_FFT_ENABLE */
    
    if (g_calib_is_in_calib_mode == 0)
    {
                  /* Normal processing */
            EM_ADC_IntervalProcessing(&g_wrp_adc_samples);

    }
    else
    {
        /* Calibration processing */
        EM_CALIB_Sampling(&g_wrp_adc_samples);
    }
    
    /* Pop MACRx registers */
    MACRH = bkup_macrh;
    MACRL = bkup_macrl;
}

/******************************************************************************
* Function Name    : void EM_ADC_GainPhaseReset(EM_LINE line)
* Description      : Reset phase gain to lowest level
* Arguments        : None
* Functions Called : None
* Return Value     : None
******************************************************************************/
void EM_ADC_GainPhaseReset(EM_LINE line)
{
    g_EM_ADC_GainPhaseCurrentLevel[line] = 0;       /* Lowest */
    EM_ADC_GainPhaseSet(line);
}

/******************************************************************************
* Function Name    : void EM_ADC_GainPhaseIncrease(void)
* Description      : Increase phase gain 1 level & set the gain level to port
* Arguments        : None
* Functions Called : EM_ADC_GainPhaseSet()
* Return Value     : None
******************************************************************************/
void EM_ADC_GainPhaseIncrease(EM_LINE line)
{
    if (g_EM_ADC_GainPhaseCurrentLevel[line] >= EM_GAIN_PHASE_NUM_LEVEL_MAX-1)  /* Is highest gain? */
    {
        return; /* Do nothing */
    }
    else    /* Still exist higher gain */
    {
        g_EM_ADC_GainPhaseCurrentLevel[line]++; /* Increase level count */
        EM_ADC_GainPhaseSet(line);              /* Apply gain */
    }
}

/******************************************************************************
* Function Name    : void EM_ADC_GainPhaseDecrease(void)
* Description      : Decrease phase gain 1 level & set the gain level to port
* Arguments        : None
* Functions Called : EM_ADC_GainPhaseSet()
* Return Value     : None
******************************************************************************/
void EM_ADC_GainPhaseDecrease(EM_LINE line)
{
    if (g_EM_ADC_GainPhaseCurrentLevel[line] == 0)  /* Is lowest gain? */
    {
        return; /* Do nothing */
    }
    else    /* Still exist lower gain */
    {
        g_EM_ADC_GainPhaseCurrentLevel[line]--; /* Decrease level count */
        EM_ADC_GainPhaseSet(line);              /* Apply gain */
    }
}
    
/******************************************************************************
* Function Name    : uint8_t EM_ADC_GainPhaseGetLevel(void)
* Description      : Get the current phase gain level
* Arguments        : None
* Functions Called : None
* Return Value     : None
******************************************************************************/
uint8_t EM_ADC_GainPhaseGetLevel(EM_LINE line)
{
    return g_EM_ADC_GainPhaseCurrentLevel[line];
}

/******************************************************************************
* Function Name    : void EM_ADC_SetPhaseCorrection(EM_LINE line, dsad_channel_t channel, float32_t degree)
* Description      : Core function to set phase correction
* Arguments        : EM_LINE line			: phase selection
*				   : dsad_channel_t channel	: DSAD channel of selected phase
*				   : float32_t degree		: degree to compensate
* Functions Called : EM_ADC_DriverSetPhaseStep
* Return Value     : None
******************************************************************************/
void EM_ADC_SetPhaseCorrection(EM_LINE line, dsad_channel_t channel, float32_t degree)
{
    /* negative ? the current lead voltage */
    /* Compensate the frequency variation + sampling frequency*/
    float32_t shift_step;
    float32_t fac = EM_GetLineFrequency(line);
    
    #if 0
    //degree = (fac - EM_PLATFORM_PROPERTY_TARGET_AC_SOURCE_FREQUENCY) * 0.17 ;
    degree = degree - ((fac-EM_PLATFORM_PROPERTY_TARGET_AC_SOURCE_FREQUENCY)*0.17);
    
    //shift_step = (360 * target_ac)/(384*3906.25);
    shift_step = (0.00024 * fac);       //(360/(384*3906.25) = 0.00024)
    #else
    shift_step = EM_ADC_DRIVER_PHASE_SHIFT_STEP_BY_50HZ;
    
    #endif /* 0 or 1 */

	degree += EM_CT_GetPhaseAngleCompensation(line, EM_GetCurrentRMS(line));


    if (g_enable_auto_fstep == 1)
    {
        if (degree <= 0.0f)
        {
            EM_ADC_DriverSetPhaseStep(
                channel,
                (uint16_t)(
                    (-degree) /
                    shift_step
                )
            );
        }
        else
        {
            
        }
    }
}

/******************************************************************************
* Function Name    : void EM_ADC_SetPhaseCorrection_Phase_R(float32_t degree)
* Description      : Set phase correction between I1 and V
* Arguments        : degree: Phase (in degree) of V based on I1
* Functions Called : TBD
* Return Value     : None
******************************************************************************/
void EM_ADC_SetPhaseCorrection_Phase_R(float32_t degree)
{
    EM_ADC_SetPhaseCorrection(LINE_PHASE_R, EM_ADC_CURRENT_DRIVER_CHANNEL_PHASE_R, degree);
}

/******************************************************************************
* Function Name    : void EM_ADC_SetPhaseCorrection(float32_t degree)
* Description      : Set phase correction between I1 and V
* Arguments        : degree: Phase (in degree) of V based on I1
* Functions Called : TBD
* Return Value     : None
******************************************************************************/
void EM_ADC_SetPhaseCorrection_Phase_Y(float32_t degree)
{
    EM_ADC_SetPhaseCorrection(LINE_PHASE_Y, EM_ADC_CURRENT_DRIVER_CHANNEL_PHASE_Y, degree);
}

/******************************************************************************
* Function Name    : void EM_ADC_SetPhaseCorrection_Phase_B(float32_t degree)
* Description      : Set phase correction between I1 and V
* Arguments        : degree: Phase (in degree) of V based on I1
* Functions Called : TBD
* Return Value     : None
******************************************************************************/
void EM_ADC_SetPhaseCorrection_Phase_B(float32_t degree)
{
    EM_ADC_SetPhaseCorrection(LINE_PHASE_B, EM_ADC_CURRENT_DRIVER_CHANNEL_PHASE_B, degree);
}

/******************************************************************************
* Function Name    : void EM_ADC_SetPhaseCorrection_Phase_B(float32_t degree)
* Description      : Set phase correction between I1 and V
* Arguments        : degree: Phase (in degree) of V based on I1
* Functions Called : TBD
* Return Value     : None
******************************************************************************/
void EM_ADC_SetPhaseCorrection_Neutral(float32_t degree)
{
    EM_ADC_SetPhaseCorrection(LINE_NEUTRAL, EM_ADC_CURRENT_DRIVER_CHANNEL_NEUTRAL, degree);
}

#ifdef METER_WRAPPER_ADC_COPY_REACTIVE_SAMPLE
/******************************************************************************
* Function Name    : uint16_t EM_ADC_QuarterShiftTimer_LookupValue(uint16_t fsteps_x100)
* Description      : Lookup for timer value according to scaled x100 fraction steps
* Arguments        : fsteps_x100: scaled fractional step (x100)
* Functions Called : None
* Return Value     : None
******************************************************************************/
uint16_t EM_ADC_QuarterShiftTimer_LookupValue(uint16_t fsteps_x100)
{
    uint16_t value;
    
    /* Check the table high index */
    if (fsteps_x100 >= TIMER_TABLE_SIZE)
    {
        fsteps_x100 = TIMER_TABLE_SIZE-1;
    }
    
    /* Get value from lookup table */
    value = g_wrp_adc_timer_table[fsteps_x100].timer_value;
    
    /* Compensate with calibration offset: Add? */
    /* Ignore calculation when value is 0 */
    if (value != 0)
    {
    	value -= g_fsteps_timer_offset;
    }
    
    if (value > EM_ADC_DRIVER_TAU_MAX_THRESHOLD)
    {
        if (value > ((EM_ADC_DRIVER_TAU_256US_SETTING - EM_ADC_DRIVER_TAU_MAX_THRESHOLD)/2))
        {
            g_wrp_adc_tdr_ignore = 1;
            g_wrp_adc_voltage90_circle_step -= 1;
        }
        else
        {
            g_wrp_adc_tdr_ignore = 0;
        }
        
        value = EM_ADC_DRIVER_TAU_MAX_THRESHOLD;
    }
    else if (value < EM_ADC_DRIVER_TAU_MIN_THRESHOLD)
    {
        if (value < (EM_ADC_DRIVER_TAU_MIN_THRESHOLD/2))
        {
            g_wrp_adc_tdr_ignore = 1;
            
        }
        else
        {
            g_wrp_adc_tdr_ignore = 0;
        }
        
        value = EM_ADC_DRIVER_TAU_MIN_THRESHOLD;
    }
    else
    {
        g_wrp_adc_tdr_ignore = 0;
    }
    
    return value;
}
#endif /* METER_WRAPPER_ADC_COPY_REACTIVE_SAMPLE */

/******************************************************************************
* Function Name    : void EM_ADC_SAMP_UpdateStep(float32_t fac)
* Description      : Update the step delay into sw delay circle and hw timer delay
* Arguments        : float32_t fac: measured line frequency
* Functions Called : EM_ADC_QuarterShiftTimer_LookupValue(): lookup timer value
*                  : EM_ADC_TAU_QuarterShiftTimer_SetValue(): set TDR value
* Return Value     : None
******************************************************************************/
void EM_ADC_SAMP_UpdateStep(float32_t fac)
{
    #ifdef METER_WRAPPER_ADC_COPY_REACTIVE_SAMPLE
    /* Update software integer  */  
    uint8_t     sw_delay_steps;
    uint16_t    fsteps_x100;
    float32_t   total_steps;
    uint16_t    tdr_value;
    
    /* Delay frequency updating */
    if (fac == g_previous_fac)
    {
        g_fac_update_counter = 0;
        return;
    }
    else
    {
        g_fac_update_counter++;
        if (g_fac_update_counter < EM_FAC_CHANGED_DETECTION_COUNT)
        {
            return;
        }
        else
        {
            g_previous_fac = fac;
            g_fac_update_counter = 0;
            /* Update by below routine */
        }
    }
    
    /*
     *                                  EM_ADC_DRIVER_IDEAL_SAMPLING_FREQUENCY
     * Calculate ideal total step: = ---------------------------------------------
     *                                              fac * 4.0f
    */
    
    total_steps = ((90.0f/360.0f) * EM_ADC_DRIVER_IDEAL_SAMPLING_FREQUENCY)/ (fac);
                        
    sw_delay_steps = (uint8_t)total_steps;
    
    /* Reverse the fsteps as required */
    total_steps = 1.0f - (total_steps - sw_delay_steps); 
    
    total_steps = ((uint16_t)((total_steps + 0.005f) * 100.0f)) / 100.0f;
    
    /* Scale to fit lookup table */
    fsteps_x100 = (uint16_t)(total_steps * 100.0f);
    
    /* Increase 1 step software delay */
    sw_delay_steps += 1;
    
    /* Set to software delay */
    sw_delay_steps += EM_ADC_DELAY_STEP_VOLTAGE;
    
    /* Make sure steps still within maximum buffer length */
    if (sw_delay_steps > EM_ADC_DELAY_VOLTAGE90_MAX_STEP)
    {
        sw_delay_steps = EM_ADC_DELAY_VOLTAGE90_MAX_STEP;
    }
    g_wrp_adc_voltage90_circle_step = sw_delay_steps;

    if (g_enable_auto_fstep == 1)
    {
        /* 
         * Adjust HW Timer Fsteps 
        */
        
        /* Lookup the timer register value table */
        tdr_value = EM_ADC_QuarterShiftTimer_LookupValue(fsteps_x100);
        
        /* Control timer delay, convert to us */
        EM_ADC_TAU_QuarterShiftTimer_SetValue(tdr_value);
    }
        
    #endif /* METER_WRAPPER_ADC_COPY_REACTIVE_SAMPLE */
}

