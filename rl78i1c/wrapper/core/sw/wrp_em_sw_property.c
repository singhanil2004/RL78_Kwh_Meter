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
* File Name    : wrp_em_sw_property.c
* Version      : 1.00
* Device(s)    : RL78/I1C
* Tool-Chain   : CCRL
* H/W Platform : RL78/I1C Energy Meter Platform
* Description  : Wrapper setting SW Property APIs
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include "wrp_em_sw_config.h"
#include "wrp_em_sw_property.h"

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Imported global variables and functions (from other files)
***********************************************************************************************************************/

/***********************************************************************************************************************
Exported global variables and functions (to be accessed by other files)
***********************************************************************************************************************/
static const EM_SW_PROPERTY em_sw_property = 
{
    {
        /* ADC */
        /* Phase_R */
        {
            EM_SW_PROPERTY_ADC_GAIN_NUMBER_LEVEL_PHASE_R,
            0,
            EM_SW_PROPERTY_ADC_GAIN_UPPER_THRESHOLD_PHASE_R,
            EM_SW_PROPERTY_ADC_GAIN_LOWER_THRESHOLD_PHASE_R,
        },
        
        /* Phase_Y */
        {
            EM_SW_PROPERTY_ADC_GAIN_NUMBER_LEVEL_PHASE_Y,
            0,
            EM_SW_PROPERTY_ADC_GAIN_UPPER_THRESHOLD_PHASE_Y,
            EM_SW_PROPERTY_ADC_GAIN_LOWER_THRESHOLD_PHASE_Y,
        },
        
        /* Phase_B */
        {
            EM_SW_PROPERTY_ADC_GAIN_NUMBER_LEVEL_PHASE_B,
            0,
            EM_SW_PROPERTY_ADC_GAIN_UPPER_THRESHOLD_PHASE_B,
            EM_SW_PROPERTY_ADC_GAIN_LOWER_THRESHOLD_PHASE_B,
        },
        
        /* SAMP */
        EM_SW_PROPERTY_ADC_CONVERSION_TIME,
     },
     
     {
         /* Operation */
         EM_SW_PROPERTY_OPERATION_IRMS_NOLOAD_THRESHOLD,
         EM_SW_PROPERTY_OPERATION_POWER_NOLOAD_THRESHOLD,
         EM_SW_PROPERTY_OPERATION_DEFAULT_NM_VOLTAGE,
         0.0f, /* reserved */
     },
     
     {
         /* Option bytes */
         EM_SW_PROPERTY_IS_UPDATE_PULSE_EXTERNALLY,
         EM_SW_PROPERTY_IS_MANAGE_ENERGY_EXTERNALLY,
         0, /* Reserved bits */
         0, /* Reserved byte */
     },
     
     {
         /* SAG SWELL */
         EM_SW_PROPERTY_SAG_RMS_THRESHOLD,
         EM_SW_PROPERTY_SWELL_RMS_THRESHOLD,
         EM_SW_PROPERTY_SAG_SWELL_DETECTION_CYCLE,
     },

    /* Pulse */
    {
        EM_PULSE_RIPPLE_REJECTION_THRESHOLD,
    },
};

/***********************************************************************************************************************
Private global variables and functions
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name : EM_SW_GetProperty
* Interface     : const EM_SW_PROPERTY* EM_SW_GetProperty(void)
* Description   : Get SW Property Setting From Wrapper Layer
* Arguments     : None
* Function Calls: None
* Return Value  : const EM_SW_PROPERTY*, point to the SW Property Structure
***********************************************************************************************************************/
NEAR_PTR EM_SW_PROPERTY * EM_SW_GetProperty(void)
{
    return (NEAR_PTR EM_SW_PROPERTY*)&em_sw_property;
}
