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
* File Name    : wrp_em_hw_property.h
* Version      : 1.00
* Device(s)    : RL78/I1C
* Tool-Chain   : CCRL
* H/W Platform : RL78/I1C Energy Meter Platform
* Description  : Wrapper for Setting SW Propery
***********************************************************************************************************************/

#ifndef _WRAPPER_EM_SW_PROPERTY_H
#define _WRAPPER_EM_SW_PROPERTY_H

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include "typedef.h"        /* GCSE Standard Definitions */

/******************************************************************************
Typedef definitions
***********************************************************************************************************************/
typedef struct tagEMSoftwareProperty
{   
    struct
    {
        struct
        {
            uint8_t     gain_num_level;                             /* (SW Support Only) ADC gain phase number of level */
            uint8_t     reserved;                                   /* Reserved (for padding) */
            uint32_t    gain_upper_threshold;                       /* (SW Support Only) ADC gain phase upper threshold */
            uint32_t    gain_lower_threshold;                       /* (SW Support Only) ADC gain phase lower threshold */
        } phase_r;              
                        
        struct              
        {               
            uint8_t     gain_num_level;                             /* (SW Support Only) ADC gain phase number of level */
            uint8_t     reserved;                                   /* Reserved (for padding) */
            uint32_t    gain_upper_threshold;                       /* (SW Support Only) ADC gain phase upper threshold */
            uint32_t    gain_lower_threshold;                       /* (SW Support Only) ADC gain phase lower threshold */
        } phase_y;              
                        
        struct              
        {               
            uint8_t     gain_num_level;                             /* (SW Support Only) ADC gain phase number of level */
            uint8_t     reserved;                                   /* Reserved (for padding) */
            uint32_t    gain_upper_threshold;                       /* (SW Support Only) ADC gain phase upper threshold */
            uint32_t    gain_lower_threshold;                       /* (SW Support Only) ADC gain phase lower threshold */
        } phase_b;
        
        float32_t       conversion_time;                            /* (SW Support Only) ADC conversion time */ 
    
    } adc;          /* ADC Module */
    
    struct
    {
        float32_t       irms_noload_threshold;                      /* (SW Support Only) Set the threshold for IRMS No Load Detection (Ampere) */
        float32_t       power_noload_threshold;                     /* (SW Support Only) Set the threshold for Power No Load Detection (Watt) */
        float32_t       default_neutral_missing_voltage;            /* (SW Support Only) Set the default Voltage when neutral msising (Volt) */
        float32_t       reserved;                                   /* Reserved (for future expanding) */
    } operation;    /* Metrology Operation */
    
    struct
    {
        uint8_t         is_update_pulse_external    : 1;            /* (SW Support Only) Is call to update pulse externally? */
        uint8_t         is_manage_energy_external   : 1;            /* (SW Support Only) Is manage energy accumulation externally? */
        uint8_t         reserved                    : 6;            /* Reserved bits */
        uint8_t         reserved1;                                  /* Reserved byte (for padding) */
    } option_bytes;
    
    struct
    {
        float32_t       sag_rms_threshold;                          /* (SW Support Only) The VRMS threshold to detect Sag Event */
        float32_t       swell_rms_threshold;                        /* (SW Support Only) The VRMS threshold to detect Swell Event */
        uint16_t        detection_cycle;                            /* (SW Support Only) Number of signal cycle to detect the Swell Event, 0 means no detection */
    } sag_swell;

    struct
    {
        float32_t    ripple_rejecttion_threshold;                   /* The rejection threshold to eliminate ripple on pulse output */
    } pulse;

} EM_SW_PROPERTY;

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Variable Externs
***********************************************************************************************************************/

/***********************************************************************************************************************
Functions Prototypes
***********************************************************************************************************************/
/* Get all HW property information */
NEAR_PTR EM_SW_PROPERTY* EM_SW_GetProperty(void);

#endif  /* _WRAPPER_EM_SW_PROPERTY_H */

