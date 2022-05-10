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
* File Name    : platform.h
* Version      : 1.00
* Device(s)    : RL78/I1C
* Tool-Chain   : CCRL
* H/W Platform : RL78/I1C Energy Meter Platform
* Description  : Platform Property Source File
***********************************************************************************************************************/

#ifndef _EM_PLATFORM_PROPERTY_H
#define _EM_PLATFORM_PROPERTY_H

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include "em_type.h"

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
/* Control operation of application here */
#ifdef __DEBUG
//#define POWER_MANAGEMENT_ENABLE
#else
//#define POWER_MANAGEMENT_ENABLE
#endif /* __DEBUG */

/* Define this if want to ignore the calibration page in storage area
 * The default calibration information is stored in platform.h
*/
//#define METER_USE_DEFAULT_CALIBRATION_INFO


/* Define this if want to enable CT angle compensation
 *
 *
 */
//#define ENABLE_CT_COMPENSATION

/*
*/
#ifdef __DEBUG
//#define METER_ENABLE_MEASURE_CPU_LOAD
#endif /* __DEBUG */

/* Only enable delay, hpf and copy 90 degree sample when have library support reactive calculation
*/
#if defined(FEATURES_BQUR)   || \
    defined(FEATURES_WQUR)   || \
    defined(FEATURES_WQFR)   || \
    defined(FEATURES_BQFR)   || \
    defined(FEATURES_ALL)    || \
    (0)
#define METER_WRAPPER_ADC_COPY_REACTIVE_SAMPLE
#endif /* defined(FEATURES_BQUR)   || 
defined(FEATURES_WQUR)   ||
defined(FEATURES_WQFR)   ||
defined(FEATURES_BQFR)   ||
defined(FEATURES_ALL) */

/* Only enable filter and copy fundamental sample when have library support fundamental & THD calculation
*/
#if defined(FEATURES_WQFR)   || \
    defined(FEATURES_BQFR)   || \
    defined(FEATURES_ALL)    || \
    (0)
//#define METER_WRAPPER_ADC_COPY_FUNDAMENTAL_SAMPLE
#endif /* defined(FEATURES_WQFR)   ||
defined(FEATURES_BQFR)   ||
defined(FEATURES_ALL) */

/* This option below will do integration on input sample in case using the rogowski coil  */
#define METER_ENABLE_INTEGRATOR_ON_SAMPLE                      (0)                              /* Enable/Disable ADC Integrator  */

/***********************************************************************************************************************
*   Platform Default Property
*******************************************************************************/
#define EM_PLATFORM_PROPERTY_TARGET_AC_SOURCE_FREQUENCY         50                              /* Target AC Source Frequency */

/***********************************************************************************************************************
*   Platform Default Configuration
*******************************************************************************/
#define EM_CONFIG_DEFAULT_VOLTAGE_LOW                           120                             /* Volt */
#define EM_CONFIG_DEFAULT_VOLTAGE_HIGH                          280                             /* Volt */
#define EM_CONFIG_DEFAULT_CURRENT_HIGH                          60                              /* Ampere */
#define EM_CONFIG_DEFAULT_FREQ_LOW                              47                              /* Hz */
#define EM_CONFIG_DEFAULT_FREQ_HIGH                             53                              /* Hz */
/* Pulse */
#define EM_CONFIG_DEFAULT_PULSE0_SOURCE                          SRC_ACTIVE
#define EM_CONFIG_DEFAULT_PULSE0_LINE                            LINE_PHASE_R

#define EM_CONFIG_DEFAULT_PULSE1_SOURCE                          SRC_ACTIVE
#define EM_CONFIG_DEFAULT_PULSE1_LINE                            LINE_PHASE_Y

#define EM_CONFIG_DEFAULT_PULSE2_SOURCE                          SRC_ACTIVE
#define EM_CONFIG_DEFAULT_PULSE2_LINE                            LINE_PHASE_B

#define EM_CONFIG_DEFAULT_PULSE3_SOURCE                          SRC_ACTIVE
#define EM_CONFIG_DEFAULT_PULSE3_LINE                            LINE_TOTAL

#define EM_CONFIG_DEFAULT_PULSE4_SOURCE                          SRC_REACTIVE
#define EM_CONFIG_DEFAULT_PULSE4_LINE                            LINE_TOTAL

#define EM_CONFIG_DEFAULT_MAX_DEMAND_PERIOD                     30                              /* Minute */
#define EM_CONFIG_DEFAULT_NUMBER_TARIFF                         6                               /* Number tariff. Max is 6. Min is 1 */
#define EM_CONFIG_DEFAULT_NUMBER_TIMESLOT                       6                               /* Number time slot. Max is 6. Min is 1 */

/* Time Slots Information
 * Please specify time slot infors only enough for the number time slot that specified above
 *      -----------------------------------------------------------------------------------------------------------
 *      | Time slot no.                         | Start time (Hour, Minute) | Tariff No (for energy accumulation) |
 *      -----------------------------------------------------------------------------------------------------------*/
#define EM_CONFIG_DEFAULT_TIMESLOT0_INFO                        {   {0      , 0 }   ,   0   }   /* Time slot 0 */
#define EM_CONFIG_DEFAULT_TIMESLOT1_INFO                        {   {4      , 0 }   ,   1   }   /* Time slot 1 */
#define EM_CONFIG_DEFAULT_TIMESLOT2_INFO                        {   {8      , 0 }   ,   2   }   /* Time slot 2 */
#define EM_CONFIG_DEFAULT_TIMESLOT3_INFO                        {   {12     , 0 }   ,   3   }   /* Time slot 3 */
#define EM_CONFIG_DEFAULT_TIMESLOT4_INFO                        {   {16     , 0 }   ,   4   }   /* Time slot 4 */
#define EM_CONFIG_DEFAULT_TIMESLOT5_INFO                        {   {18     , 0 }   ,   5   }   /* Time slot 5 */


/***********************************************************************************************************************
*   Platform Default Calibration Informations
*******************************************************************************/
/* Meter Constant */
#define EM_CALIB_DEFAULT_SAMPLING_FREQUENCY                     3897.000000                         /* Actual sampling frequency of the meter    */
#define EM_CALIB_DEFAULT_METER_CONSTANT_PHASE                   5000                                /* Meter Constant. Pulses/KWh                */
#define EM_CALIB_DEFAULT_METER_CONSTANT_TOTAL                   5000                                /* Meter Constant. Pulses/KWh                */
#define EM_CALIB_DEFAULT_PULSE_ON_TIME                          30                                  /* Pulse On Time                             */
#define EM_CALIB_DEFAULT_RTC_COMP_OFFSET                        0                                   /* RTC compensation default offset           */

/* Co-efficient */
#define EM_CALIB_DEFAULT_COEFF_VRMS_PHASE_R                     (33.0369 )                        /* VRMS Co-efficient  At Gain 1.0f           */
#define EM_CALIB_DEFAULT_COEFF_IRMS_PHASE_R                     (33604.8200 )                     /* IRMS Co-efficient At Gain 1.0f           */
#define EM_CALIB_DEFAULT_COEFF_ACTIVE_POWER_PHASE_R             (EM_CALIB_DEFAULT_COEFF_VRMS_PHASE_R*EM_CALIB_DEFAULT_COEFF_IRMS_PHASE_R)                   /* Active Power Co-efficient At Gain 1.0f    */
#define EM_CALIB_DEFAULT_COEFF_REACTIVE_POWER_PHASE_R           (EM_CALIB_DEFAULT_COEFF_VRMS_PHASE_R*EM_CALIB_DEFAULT_COEFF_IRMS_PHASE_R)                   /* Reactive Power Co-efficient At Gain 1.0f  */
#define EM_CALIB_DEFAULT_COEFF_APPARENT_POWER_PHASE_R           (EM_CALIB_DEFAULT_COEFF_VRMS_PHASE_R*EM_CALIB_DEFAULT_COEFF_IRMS_PHASE_R)                   /* Apparent Power Co-efficient At Gain 1.0f  */

#define EM_CALIB_DEFAULT_COEFF_VRMS_PHASE_Y                     (33.0453 )                        /* VRMS Co-efficient  At Gain 1.0f           */
#define EM_CALIB_DEFAULT_COEFF_IRMS_PHASE_Y                     (33438.1520 )                     /* IRMS Co-efficient At Gain 1.0f           */
#define EM_CALIB_DEFAULT_COEFF_ACTIVE_POWER_PHASE_Y             (EM_CALIB_DEFAULT_COEFF_VRMS_PHASE_Y*EM_CALIB_DEFAULT_COEFF_IRMS_PHASE_Y)                   /* Active Power Co-efficient At Gain 1.0f    */
#define EM_CALIB_DEFAULT_COEFF_REACTIVE_POWER_PHASE_Y           (EM_CALIB_DEFAULT_COEFF_VRMS_PHASE_Y*EM_CALIB_DEFAULT_COEFF_IRMS_PHASE_Y)                   /* Reactive Power Co-efficient At Gain 1.0f  */
#define EM_CALIB_DEFAULT_COEFF_APPARENT_POWER_PHASE_Y           (EM_CALIB_DEFAULT_COEFF_VRMS_PHASE_Y*EM_CALIB_DEFAULT_COEFF_IRMS_PHASE_Y)                   /* Apparent Power Co-efficient At Gain 1.0f  */

#define EM_CALIB_DEFAULT_COEFF_VRMS_PHASE_B                     (33.0529 )                        /* VRMS Co-efficient  At Gain 1.0f           */
#define EM_CALIB_DEFAULT_COEFF_IRMS_PHASE_B                     (33522.9530 )                     /* IRMS Co-efficient At Gain 1.0f           */
#define EM_CALIB_DEFAULT_COEFF_ACTIVE_POWER_PHASE_B             (EM_CALIB_DEFAULT_COEFF_VRMS_PHASE_B*EM_CALIB_DEFAULT_COEFF_IRMS_PHASE_B)                   /* Active Power Co-efficient At Gain 1.0f    */
#define EM_CALIB_DEFAULT_COEFF_REACTIVE_POWER_PHASE_B           (EM_CALIB_DEFAULT_COEFF_VRMS_PHASE_B*EM_CALIB_DEFAULT_COEFF_IRMS_PHASE_B)                   /* Reactive Power Co-efficient At Gain 1.0f  */
#define EM_CALIB_DEFAULT_COEFF_APPARENT_POWER_PHASE_B           (EM_CALIB_DEFAULT_COEFF_VRMS_PHASE_B*EM_CALIB_DEFAULT_COEFF_IRMS_PHASE_B)                   /* Apparent Power Co-efficient At Gain 1.0f  */

#define EM_CALIB_DEFAULT_COEFF_IRMS_NEUTRAL                     (33626.750684985 )                     /* IRMS Co-efficient At Gain 1.0f           */

/* Offset */
#define EM_CALIB_DEFAULT_OFFSET_V_PHASE_R                       (18326)                             /* Voltage offset */
#define EM_CALIB_DEFAULT_OFFSET_V_PHASE_Y                       (18321)                             /* Voltage offset */
#define EM_CALIB_DEFAULT_OFFSET_V_PHASE_B                       (18325)                             /* Voltage offset */

#define EM_CALIB_DEFAULT_OFFSET_I_PHASE_R                       (10)                                /* Current offset */
#define EM_CALIB_DEFAULT_OFFSET_I_PHASE_Y                       (10)                                /* Current offset */
#define EM_CALIB_DEFAULT_OFFSET_I_PHASE_B                       (10)                                /* Current offset */

/* Reactive timer Offset */
#define EM_CALIB_DEFAULT_TIMER_OFFSET                           (-200)

/* DSAD Gain */
#define EM_CALIB_DEFAULT_GAIN_PHASE_R                           (1)
#define EM_CALIB_DEFAULT_GAIN_PHASE_Y                           (1)
#define EM_CALIB_DEFAULT_GAIN_PHASE_B                           (1)
#define EM_CALIB_DEFAULT_GAIN_NEUTRAL                           (1)

/* SW Phase Correction */
/* Phase Gain Phase Shift Angle */
#define EM_CALIB_DEFAULT_GAIN_PHASE_LEVEL0_PHASE_SHIFT_PHASE_R  (-2.115543f)                 /* Phase Gain Level 0 Phase Shift Angle (in degree) */
#define EM_CALIB_DEFAULT_GAIN_PHASE_LEVEL1_PHASE_SHIFT_PHASE_R  (-2.104473f)               /* Phase Gain Level 2 Phase Shift Angle (in degree) */


/* Phase Gain Phase Shift Angle */
#define EM_CALIB_DEFAULT_GAIN_PHASE_LEVEL0_PHASE_SHIFT_PHASE_Y  (-2.048136f)                 /* Phase Gain Level 0 Phase Shift Angle (in degree) */
#define EM_CALIB_DEFAULT_GAIN_PHASE_LEVEL1_PHASE_SHIFT_PHASE_Y  (-2.037581f)                /* Phase Gain Level 2 Phase Shift Angle (in degree) */

/* Phase Gain Phase Shift Angle */
#define EM_CALIB_DEFAULT_GAIN_PHASE_LEVEL0_PHASE_SHIFT_PHASE_B  (-1.919932f)                 /* Phase Gain Level 0 Phase Shift Angle (in degree) */
#define EM_CALIB_DEFAULT_GAIN_PHASE_LEVEL1_PHASE_SHIFT_PHASE_B  (-1.915074f)              /* Phase Gain Level 2 Phase Shift Angle (in degree) */


/* Gain Value List */
/* Phase Gain Value List */
#define EM_CALIB_DEFAULT_GAIN_PHASE_LEVEL0_VALUE_PHASE_R        (1.0f)                      /* Phase Gain Level 0 Value (lowest, value is 1.0, fixed)   */
#define EM_CALIB_DEFAULT_GAIN_PHASE_LEVEL1_VALUE_PHASE_R        (4.005f)                  /* Phase Gain Level 2 Value     |                           */

/* Phase Gain Value List */
#define EM_CALIB_DEFAULT_GAIN_PHASE_LEVEL0_VALUE_PHASE_Y        (1.0f)                      /* Phase Gain Level 0 Value (lowest, value is 1.0, fixed)   */
#define EM_CALIB_DEFAULT_GAIN_PHASE_LEVEL1_VALUE_PHASE_Y        (3.997f)                /* Phase Gain Level 2 Value     |                           */

/* Phase Gain Value List */
#define EM_CALIB_DEFAULT_GAIN_PHASE_LEVEL0_VALUE_PHASE_B        (1.0f)                      /* Phase Gain Level 0 Value (lowest, value is 1.0, fixed)   */
#define EM_CALIB_DEFAULT_GAIN_PHASE_LEVEL1_VALUE_PHASE_B        (4.003f)            /* Phase Gain Level 2 Value     |                           */

/******************************************************************************
Variable Externs
******************************************************************************/
extern const EM_PLATFORM_PROPERTY   g_EM_DefaultProperty;       /* Default Platform Property */
extern const EM_CONFIG              g_EM_DefaultConfig;         /* Default Platform Configuration */
extern const EM_CALIBRATION         g_EM_DefaultCalibration;    /* Default Platform Calibration */

/***********************************************************************************************************************
Functions Prototypes
***********************************************************************************************************************/

#endif /* _EM_PLATFORM_PROPERTY_H */

