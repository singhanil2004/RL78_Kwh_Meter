/******************************************************************************
* DISCLAIMER

* This software is supplied by Renesas Electronics Corporation and is only
* intended for use with Renesas products. No other uses are authorized.

* This software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.

* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES
* REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY,
* INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
* PARTICULAR PURPOSE AND NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY
* DISCLAIMED.

* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES
* FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS
* AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.

* Renesas reserves the right, without notice, to make changes to this
* software and to discontinue the availability of this software.
* By using this software, you agree to the additional terms and
* conditions found by accessing the following link:
* http://www.renesas.com/disclaimer
******************************************************************************/
/* Copyright (C) 2011 Renesas Electronics Corporation. All rights reserved.  */
/******************************************************************************
* File Name    : r_calib.c
* Version      : 1.00
* Device(s)    : None
* Tool-Chain   :
* H/W Platform : Unified Energy Meter Platform
* Description  :
******************************************************************************
* History : DD.MM.YYYY Version Description
*         : 01.04.2013
******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_dsadc.h"
#include "r_calib.h"            /* Calibration */
#include "r_calib_sampling.h"   /* Accumulation of calibration */
#include "wrp_em_sw_config.h"   /* EM Software Config */
#include "wrp_em_adc_data.h"         /* EM Wrapper ADC */
#include "config_storage.h"            /* Storage Header File */
#include "r_cg_wdt.h"                   /* CG WDT Driver */


/* MW/Core, Common */
#include "em_core.h"            /* EM Core APIs */

/* Standards library */
#include <math.h>               /* Maths */
#include <string.h>             /* Standard String */

/* Application */
#include "wrp_em_adc_data.h"
#include "ct_compensation.h"

/******************************************************************************
Macro definitions
******************************************************************************/

/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/

/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/

/******************************************************************************
Private global variables and functions
******************************************************************************/
uint8_t g_calib_is_in_calib_mode = 0;

/******************************************************************************
* Function Name : R_CALIB_CalibrateVoltAndPhase
* Interface     : uint8_t R_CALIB_CalibrateVoltAndPhase(
*               :     uint16_t cycle,
*               :     uint16_t cycle_phase_error,
*               :     float32_t imax,
*               :     float32_t volt,
*               :     float32_t current,
*               :     calib_output_t *output
*               : );
* Description   : Execute auto calibration for voltage and phase current channels
* Arguments     : uint16_t cycle                : Total number of line cycle used to get sample
*               : uint16_t cycle_phase_error    : Total number of line cycle used for phase error calculation
*               : float32_t imax                : Max support measured current
*               : float32_t volt                : Calibrated voltage value
*               : float32_t current             : Calibrated current value
*               : calib_output_t * output       : Structure stores the calibrated result
* Function Calls: EM_CALIB_Init()
*               : EM_CALIB_Start()
*               : EM_CALIB_CalculateSamplingFrequency()
*               : EM_CALIB_CalculateAndSetCurrentGain()
*               : EM_CALIB_DoAccumulate()
* Return Value  : uint8_t, 0 is success, otherwise is error.
******************************************************************************/
uint8_t R_CALIB_CalibrateVoltAndPhase(
    uint16_t    cycle,
    uint16_t    cycle_phase_error,
    float32_t   imax,
    float32_t   volt,
    float32_t   current,
    calib_output_t* output,
    uint8_t     line
)
{
    uint8_t result;
    
    if (output == NULL)
    {
        return 1;
    }
    
    /* Mark outside enter calibration */
    g_calib_is_in_calib_mode = 1;
    
    EM_CALIB_Init();
    EM_CALIB_Start(imax, volt, current, cycle, cycle_phase_error, line);
    
    /* fs, gain */
    output->fs   = EM_CALIB_CalculateSamplingFrequency();
    
    if (*((uint32_t*)&imax) == 0)
    {
        R_DSADC_SetChannelGain((dsad_channel_t)line, g_wrp_adc_channel_gain[line][0]);
        
        /* Fixed Gain0 value to 1.0f */
        output->gain = 1.0f;
    }
    else
    {
        output->gain = EM_CALIB_CalculateAndSetCurrentGain();
    }
    
    /* vcoeff, icoeff, pcoeff, phase shift */
    result = EM_CALIB_DoAccumulate();
    
    output->vcoeff = (sqrt(g_calib_info.storage.vrms / g_calib_info.storage.nvrms)) / volt;
    output->icoeff = (sqrt(g_calib_info.storage.i1rms / g_calib_info.storage.ni1rms)) / current;
    output->pcoeff = output->vcoeff * output->icoeff;
    output->angle_error = g_calib_info.theta - EM_CT_GetPhaseAngleCompensation((EM_LINE)line, current);
    output->angle90_offset = g_calib_info.theta90_timer_offset;
    output->voffset = g_calib_info.offset.v.average;
    output->ioffset = g_calib_info.offset.i.average;
    
    if (*((uint32_t*)&imax) == 0)
    {
    
        R_DSADC_SetChannelGain((dsad_channel_t)line, g_wrp_adc_channel_gain[line][1]);
        
        /* Do 2nd accumulation (exclude timer offset calibration) for 2nd gain */
        
        result = EM_CALIB_DoAccumulate();
        
        /* Gain1 calculate by ratio between two coeffs */
        output->gain1  = ((sqrt(g_calib_info.storage.i1rms / g_calib_info.storage.ni1rms)) / current) / output->icoeff;
        output->angle_error1 = g_calib_info.theta;
    }
    
    EM_CALIB_Stop();
    
    /* Mark outside release calibration */
    g_calib_is_in_calib_mode = 0;
    
    if (result == EM_CALIB_OK)
    {
        /* Success */
        return 0;
    }
    else
    {
        /* Fail */
        return 1;
    }
}

/******************************************************************************
* Function Name : R_CALIB_CalibrateVoltAndNeutral
* Interface     : uint8_t R_CALIB_CalibrateVoltAndNeutral(
*               :     uint16_t cycle,
*               :     uint16_t cycle_phase_error,
*               :     float32_t imax,
*               :     float32_t volt,
*               :     float32_t current,
*               :     calib_output_t *output
*               : );
* Description   : Execute auto calibration for voltage and phase current channels
* Arguments     : uint16_t cycle                : Total number of line cycle used to get sample
*               : uint16_t cycle_phase_error    : Total number of line cycle used for phase error calculation
*               : float32_t imax                : Max support measured current
*               : float32_t volt                : Calibrated voltage value
*               : float32_t current             : Calibrated current value
*               : calib_output_t * output       : Structure stores the calibrated result
* Function Calls: EM_CALIB_Init()
*               : EM_CALIB_Start()
*               : EM_CALIB_CalculateSamplingFrequency()
*               : EM_CALIB_CalculateAndSetCurrentGain()
*               : EM_CALIB_DoAccumulate()
* Return Value  : uint8_t, 0 is success, otherwise is error.
******************************************************************************/
uint8_t R_CALIB_CalibrateVoltAndNeutral(
    uint16_t    cycle,
    uint16_t    cycle_phase_error,
    float32_t   imax,
    float32_t   volt,
    float32_t   current,
    calib_output_t* output
)
{
    uint8_t result;
    
    if (output == NULL)
    {
        return 1;
    }
    
    /* Mark outside enter calibration */
    g_calib_is_in_calib_mode = 1;
    
    EM_CALIB_Init();
    EM_CALIB_Start(imax, volt, current, cycle, cycle_phase_error, 3);
    
    memset(output, 0, sizeof(calib_output_t));
    
    /* fs, gain */
    output->gain = EM_CALIB_CalculateAndSetCurrentGain();
    
    /* vcoeff, icoeff, pcoeff, phase shift */
    result = EM_CALIB_DoAccumulate();
    output->icoeff = (sqrt(g_calib_info.storage.i1rms / g_calib_info.storage.ni1rms)) / current;
    
    EM_CALIB_Stop();
    
    /* Mark outside release calibration */
    g_calib_is_in_calib_mode = 0;
    
    if (result == EM_CALIB_OK)
    {
        /* Success */
        return 0;
    }
    else
    {
        /* Fail */
        return 1;
    }
}

/******************************************************************************
* Function Name : R_CALIB_IsInCalibMode
* Interface     : uint8_t R_CALIB_IsInCalibMode(void)
* Description   : Return the current operation mode of EM
* Arguments     : none
* Function Calls: none
* Return Value  : uint8_t, 1 is in calibration processing
******************************************************************************/
uint8_t R_CALIB_IsInCalibMode(void)
{
    return (g_calib_is_in_calib_mode);
}



/******************************************************************************
* Function Name : SERVICE_DoAutoCalibration
* Interface     : uint8_t SERVICE_DoAutoCalibration(
*               :     uint8_t step_id,
*               :     uint8_t *buff_in,
*               :     uint8_t *buff_out,
*               :     uint8_t len_in,
*               :     uint8_t *len_out
*               : );
* Description   : Read Firmware Version Service
* Arguments     : uint8_t step_id   : Step ID of the auto calibration
*               : uint8_t * buff_in : Input buffer
*               : uint8_t * buff_out: Output buffer
*               : uint8_t len_in    : Length of input buffer
*               : uint8_t *len_out  : Store the length of output buffer
* Function Calls: TBD
* Return Value  : uint8_t, success or not
******************************************************************************/
uint8_t SERVICE_DoAutoCalibration(void)
{
    uint8_t rlt;                                                    /* Calibration result */
    
    /* Calibration */
    calib_output_t result;                      /* Output data after calibration process */
    EM_CALIBRATION  calib;                      /* Calibrated data storage for EM core */
    
    /* Command parameters */
    volatile uint8_t line;                          /* Input supply line */
    dsad_channel_t  channel;                    /* Name the current channel for ADC gain setting */
    volatile uint8_t         is_calibrate_v90;           /* Indicate have timer phase shift offset calibration or not */
    volatile float32_t       f_result;                   /* Temporary data for encoding to output */
    float32_t       degree_list[EM_GAIN_PHASE_NUM_LEVEL_MAX];
    float32_t       gain_list[EM_GAIN_PHASE_NUM_LEVEL_MAX];
    
    /* Reference Parameters: Please match with Power Supply Setting during Auto-Calibration */
    static uint16_t cycle = 150;
    static uint16_t p_cycle = 150;
    static float32_t v_ref = (240.0f);
    static float32_t i_ref = (5.0f);
    
#if (EM_SW_PROPERTY_ADC_GAIN_NUMBER_LEVEL !=1)
    static float32_t imax_ref = (0.0f);
#else
    static float32_t imax_ref = (60.0f);
#endif
    
    /* Get current calib info */
    calib = EM_GetCalibInfo();
    
    /* Stop EM operation */
    if (EM_Stop() != EM_OK)
    {
        return 0x06;
    }
    
    for (line = 0U; line < 4U; line++)
    {
        memset(&degree_list[0], 0, sizeof(degree_list));
        memset(&gain_list[0], 0, sizeof(gain_list));
        
        /* Doing calibration */
        if (line < 3)
        {
            rlt = R_CALIB_CalibrateVoltAndPhase(cycle, p_cycle, imax_ref, v_ref, i_ref, &result, line);
        }
        else if (line == 3)
        {
            rlt = R_CALIB_CalibrateVoltAndNeutral(cycle, p_cycle, imax_ref, v_ref, i_ref, &result);
        }
        else
        {
            return 0x04;
        }
        
        if (rlt != EM_CALIB_OK)
        {
            return 0x05;
        }
        
        /* Checking calibration result to see whether include timer phase shift offset calibration or not */
        if (result.angle90_offset > 0 || result.angle90_offset < 0)
        {
            is_calibrate_v90 = 1;
        }
        else
        {
            is_calibrate_v90 = 0;
        }
        
        
        degree_list[0]                                              = result.angle_error;
        gain_list[0]                                                = result.gain;
        
        if (*((uint32_t*)&imax_ref) == 0)
        {
            degree_list[1]                                              = result.angle_error1;
            gain_list[1]                                                = result.gain1;
        }
        
        
        /* Modify calib info and output depends on step_id */
        switch (line)
        {
            case 0:
                /* Modify the calib info */
                calib.common.sampling_frequency                         = result.fs;
                calib.coeff.phase_r.vrms                                = result.vcoeff;
                calib.coeff.phase_r.irms                                = result.icoeff;
                calib.coeff.phase_r.active_power                        = result.pcoeff;
                calib.coeff.phase_r.reactive_power                      = result.pcoeff;
                calib.coeff.phase_r.apparent_power                      = result.pcoeff;
                calib.sw_phase_correction.phase_r.i_phase_degrees       = degree_list;
                calib.sw_gain.phase_r.i_gain_values                     = gain_list;
                calib.offset.phase_r.v                                  = result.voffset;
                calib.offset.phase_r.i                                  = result.ioffset;
                channel                                                 = EM_ADC_CURRENT_DRIVER_CHANNEL_PHASE_R;
                break;
                
            case 1:
                /* Modify the calib info */
                calib.common.sampling_frequency                         = result.fs;
                calib.coeff.phase_y.vrms                                = result.vcoeff;
                calib.coeff.phase_y.irms                                = result.icoeff;
                calib.coeff.phase_y.active_power                        = result.pcoeff;
                calib.coeff.phase_y.reactive_power                      = result.pcoeff;
                calib.coeff.phase_y.apparent_power                      = result.pcoeff;
                calib.sw_phase_correction.phase_y.i_phase_degrees       = degree_list;
                calib.sw_gain.phase_y.i_gain_values                     = gain_list;
                calib.offset.phase_y.v                                  = result.voffset;
                calib.offset.phase_y.i                                  = result.ioffset;
                channel                                                 = EM_ADC_CURRENT_DRIVER_CHANNEL_PHASE_Y;
                break;
                
            case 2:
                /* Modify the calib info */
                calib.common.sampling_frequency                         = result.fs;
                calib.coeff.phase_b.vrms                                = result.vcoeff;
                calib.coeff.phase_b.irms                                = result.icoeff;
                calib.coeff.phase_b.active_power                        = result.pcoeff;
                calib.coeff.phase_b.reactive_power                      = result.pcoeff;
                calib.coeff.phase_b.apparent_power                      = result.pcoeff;
                calib.sw_phase_correction.phase_b.i_phase_degrees       = degree_list;
                calib.sw_gain.phase_b.i_gain_values                     = gain_list;
                calib.offset.phase_b.v                                  = result.voffset;
                calib.offset.phase_b.i                                  = result.ioffset;
                channel                                                 = EM_ADC_CURRENT_DRIVER_CHANNEL_PHASE_B;
                break;
                
            case 3:
                /* Modify the calib info */
                calib.coeff.neutral.irms                                = calib.coeff.phase_b.irms;
                channel                                                 = EM_ADC_CURRENT_DRIVER_CHANNEL_NEUTRAL;
                break;
                
            default:
                return 0x04;
                
        }
        
        
        /* Timer phase shift offset values */
        if (is_calibrate_v90 == 1)
        {
            EM_ADC_SAMP_SetTimerOffset(result.angle90_offset);
        }
        
        /* Set the calib info to library */
        if (EM_SetCalibInfo(&calib) != EM_OK)
        {
            return 0x03;
        }
        
        /* Driver DSAD Gain */
        if (*((uint32_t*)&imax_ref) != 0)
        {
            R_DSADC_SetChannelGain(
                channel,
                R_DSADC_GetGainEnumValue((uint8_t)result.gain)
            );
        }
        
        R_WDT_Restart();
        
    }
    
    if (CONFIG_Backup(CONFIG_ITEM_CALIB) != CONFIG_OK)
    {
        /* Success */
        return 0x02;
    }
    
    /* Finish, now restart EM operation */
    if (EM_Start() != EM_OK)
    {
        return 0x01;
    }
    
    
    
    /* Successful */
    return EM_OK;
    
}

/******************************************************************************
* Function Name   : COMMAND_InvokeCalibration
* Interface       : static void COMMAND_InvokeCalibration(uint8_t *arg_str)
* Description     : Command Invoke Calibration
* Arguments       : uint8_t * arg_str: Arguments string
* Function Calls  : None
* Return Value    : None
******************************************************************************/
uint8_t COMMAND_InvokeCalibration(void)
{
    uint8_t rlt, count;
    
    /* RTC time */
    //rtc_calendarcounter_value_t rtctime;
    //uint32_t start_second, end_second;
    
    /* Result of calib */
    calib_output_t result;
    EM_CALIBRATION  calib;                  /* Calibrated data storage for EM core */
    
    /* Command parameters */
    uint8_t         line;
    dsad_channel_t  channel;                /* Choose calibrated channel */
    uint8_t         is_calibrate_v90;
    
    float32_t       degree_list[EM_GAIN_PHASE_NUM_LEVEL_MAX];
    float32_t       gain_list[EM_GAIN_PHASE_NUM_LEVEL_MAX];
    
    /* Reference Parameters: Please match with Power Supply Setting during Auto-Calibration */
    static uint16_t c = 150;
    static uint16_t cp = 150;
    static float32_t v = (240.0f);
    static float32_t i = (5.0f);
    float32_t       imax /*, v, i*/;             /* Voltage and Current for calibrating */
    //uint16_t        c, cp;                  /* Number of line cycle for calibrating & phase error loop */
    
    uint8_t         line_name;
    uint8_t         buffer[20];             /* Data buffer to store input arguments */
    
    
    
    //CMD_SendString((uint8_t *)"\n\rParameter(s): ");
    //CMD_SendString((uint8_t *)arg_str);
    //CMD_SendString((uint8_t *)"\n\r");
    
    
    
    
    
    for (line = 0U; line < 4U; line++)
    {
        imax = 60;
        //v = 240;
        //i = 5;
        //c = 50;
        //cp = 50;
        memset(&degree_list[0], 0, sizeof(degree_list));
        memset(&gain_list[0], 0, sizeof(gain_list));
        
        /* For neutral line, keep the imax information
         * For phase line, set imax=0 to let calib module do in dual gain
        */
#if (EM_SW_PROPERTY_ADC_GAIN_NUMBER_LEVEL_PHASE_R != 1)
        
        if (line == (uint8_t)LINE_PHASE_R)
        {
            imax = 0.0f;
        }
        
#endif /* EM_SW_PROPERTY_ADC_GAIN_NUMBER_LEVEL_PHASE_R != 1*/
        
#if (EM_SW_PROPERTY_ADC_GAIN_NUMBER_LEVEL_PHASE_Y != 1)
        
        if (line == (uint8_t)LINE_PHASE_Y)
        {
            imax = 0.0f;
        }
        
#endif /* EM_SW_PROPERTY_ADC_GAIN_NUMBER_LEVEL_PHASE_Y != 1 */
        
#if (EM_SW_PROPERTY_ADC_GAIN_NUMBER_LEVEL_PHASE_B != 1)
        
        if (line == (uint8_t)LINE_PHASE_B)
        {
            imax = 0.0f;
        }
        
#endif /* EM_SW_PROPERTY_ADC_GAIN_NUMBER_LEVEL_PHASE_B != 1*/
        
        /* Stop EM operation */
        //CMD_Printf((uint8_t *)"\n\rStop EM Core...");
        EM_Stop();
        
        
        
        calib = EM_GetCalibInfo();
        
        /* Calibrating */
        if (line < 3)
        {
            //CMD_Printf((uint8_t *)"Calibrating volt and current of phase %c...", line_name);
            rlt = R_CALIB_CalibrateVoltAndPhase(c, cp, imax, v, i, &result, line);
            
            if (rlt != 0)
            {
                //CMD_Printf((uint8_t *)"FAILED\n\r");
                
                //CMD_Printf((uint8_t *)"Start EM Core...");
                EM_Start();
                
                return 1;
            }
        }
        else
        {
            //CMD_Printf((uint8_t *)"Calibrating line neutral...");
            rlt = R_CALIB_CalibrateVoltAndNeutral(c, cp, imax, v, i, &result);
            
            if (rlt != 0)
            {
                //CMD_Printf((uint8_t *)"FAILED\n\r");
                //CMD_Printf((uint8_t *)"Start EM Core...");
                EM_Start();
                return 1;
            }
        }
        
        if (result.angle90_offset > 0 || result.angle90_offset < 0)
        {
            is_calibrate_v90 = 1;
        }
        else
        {
            is_calibrate_v90 = 0;
        }
        
        
        
        /* Backup data and check auto calibration result */
        degree_list[0]                                              = result.angle_error;
        gain_list[0]                                                = result.gain;
        
        if (*((uint32_t*)&imax) == 0)
        {
            degree_list[1]                                              = result.angle_error1;
            gain_list[1]                                                = result.gain1;
        }
        
        if (line == 0)
        {
            calib.common.sampling_frequency                         = result.fs;
            calib.coeff.phase_r.vrms                                = result.vcoeff;
            calib.coeff.phase_r.irms                                = result.icoeff;
            calib.coeff.phase_r.active_power                        = result.pcoeff;
            calib.coeff.phase_r.reactive_power                      = result.pcoeff;
            calib.coeff.phase_r.apparent_power                      = result.pcoeff;
            calib.sw_phase_correction.phase_r.i_phase_degrees       = degree_list;
            calib.sw_gain.phase_r.i_gain_values                     = gain_list;
            calib.offset.phase_r.v                                  = (EM_SW_SAMP_TYPE)result.voffset;
            calib.offset.phase_r.i                                  = (EM_SW_SAMP_TYPE)result.ioffset;
            channel                                                 = EM_ADC_CURRENT_DRIVER_CHANNEL_PHASE_R;
        }
        else if (line == 1)
        {
            calib.common.sampling_frequency                         = result.fs;
            calib.coeff.phase_y.vrms                                = result.vcoeff;
            calib.coeff.phase_y.irms                                = result.icoeff;
            calib.coeff.phase_y.active_power                        = result.pcoeff;
            calib.coeff.phase_y.reactive_power                      = result.pcoeff;
            calib.coeff.phase_y.apparent_power                      = result.pcoeff;
            calib.sw_phase_correction.phase_y.i_phase_degrees       = degree_list;
            calib.sw_gain.phase_y.i_gain_values                     = gain_list;
            calib.offset.phase_y.v                                  = (EM_SW_SAMP_TYPE)result.voffset;
            calib.offset.phase_y.i                                  = (EM_SW_SAMP_TYPE)result.ioffset;
            channel                                                 = EM_ADC_CURRENT_DRIVER_CHANNEL_PHASE_Y;
        }
        else if (line == 2)
        {
            calib.common.sampling_frequency                         = result.fs;
            calib.coeff.phase_b.vrms                                = result.vcoeff;
            calib.coeff.phase_b.irms                                = result.icoeff;
            calib.coeff.phase_b.active_power                        = result.pcoeff;
            calib.coeff.phase_b.reactive_power                      = result.pcoeff;
            calib.coeff.phase_b.apparent_power                      = result.pcoeff;
            calib.sw_phase_correction.phase_b.i_phase_degrees       = degree_list;
            calib.sw_gain.phase_b.i_gain_values                     = gain_list;
            calib.offset.phase_b.v                                  = (EM_SW_SAMP_TYPE)result.voffset;
            calib.offset.phase_b.i                                  = (EM_SW_SAMP_TYPE)result.ioffset;
            channel                                                 = EM_ADC_CURRENT_DRIVER_CHANNEL_PHASE_B;
        }
        else
        {
            calib.coeff.neutral.irms                                = result.icoeff;
            channel                                                 = EM_ADC_CURRENT_DRIVER_CHANNEL_NEUTRAL;
        }
        
        if (is_calibrate_v90 == 1)
        {
            EM_ADC_SAMP_SetTimerOffset(result.angle90_offset);
        }
        
        /* Set calibrated data */
        //CMD_Printf((uint8_t *)"Seting EM Core Calibration Info...");
        if (EM_SetCalibInfo(&calib) != EM_OK)
        {
            //CMD_Printf((uint8_t *)"FAILED\n\r");
            /* Restart the EM */
            EM_Start();
            return 1;
        }
        else
        {
            //CMD_Printf((uint8_t *)"OK\n\r");
        }
        
        /* Driver ADC Gain */
        if (*((uint32_t*)&imax) != 0)
        {
            R_DSADC_SetChannelGain(
                channel,
                R_DSADC_GetGainEnumValue((uint8_t)result.gain)
            );
        }
        
        /* Backup calibrated data into Memory */
        //CMD_Printf((uint8_t *)"Backup EM Core Calibration Into MCU Configuration Page...");
        if (CONFIG_Backup(CONFIG_ITEM_CALIB) != EM_OK)
        {
            //CMD_Printf((uint8_t *)"FAILED\n\r");
            /* Restart the EM */
            EM_Start();
            return 1;
        }
        else
        {
            //CMD_Printf((uint8_t *)"OK\n\r");
        }
        
        /* Start EM operation */
        //CMD_Printf((uint8_t *)"Start EM Core...");
        EM_Start();
        
        
        
    }
    
    return EM_OK;
}