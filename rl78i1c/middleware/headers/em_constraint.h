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
* File Name    : em_constraint.h
* Version      : 1.00
* Device(s)    : RL78/I1C
* Tool-Chain   : CCRL
* H/W Platform : RL78/I1C Energy Meter Platform
* Description  : EM Core Constraint Middleware Header file
***********************************************************************************************************************/

#ifndef _EM_CONSTRAINT_H
#define _EM_CONSTRAINT_H

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
/* Platform property constant */
#define EM_GAIN_PHASE_NUM_LEVEL_MIN                 (1U)                    /* Gain phase number level min */
#define EM_GAIN_PHASE_NUM_LEVEL_MAX                 (2U)                    /* Gain phase number level max */
#define EM_ANGLE_PHASE_NUM_LEVEL_MIN                (1U)                    /* Gain phase number level min */
#define EM_ANGLE_PHASE_NUM_LEVEL_MAX                (2U)                    /* Gain phase number level max */
#define EM_SAMPLING_FREQUENCY_MIN                   (1200U)                 /* Sampling Frequency Min (Hz) */
#define EM_SAMPLING_FREQUENCY_MAX                   (4000U)                 /* Sampling Frequency Max (Hz) */
#define EM_ADC_CONVERSION_TIME_MAX                  (40.0f)                 /* ADC Max. Conversion time */
#define EM_TARGET_AC_SOURCE_FREQ_SELECTION0         (50U)                   /* Target AC Source Frequency Selection 0 (Hz) */
#define EM_TARGET_AC_SOURCE_FREQ_SELECTION1         (60U)                   /* Target AC Source Frequency Selection 1 (Hz) */

/* Configuration Constant */
#define EM_NUM_OF_PHASE                             (3U)                    /* Phase number */
#define EM_NUM_OF_LINE                              (4U)                    /* Line number */
#define EM_CALC_NUM_OF_LINE                         (EM_NUM_OF_LINE + 1)    /* Included Total Calculation*/
#define EM_VOL_CHANNEL_NUM                          (EM_NUM_OF_PHASE)       /* Voltage channel number */
#define EM_CURRENT_CHANNEL_NUM                      (EM_NUM_OF_LINE)        /* Current channel number */

#define EM_PULSE_NUM                                (5)                     /* Current pulse output number */

/* Configuration Constant */
#define EM_VOL_LOW_MIN                              (10U)                   /* Voltage low min */
#define EM_VOL_LOW_MAX                              (200U)                  /* Voltage low max */
#define EM_VOL_HIGH_MIN                             (220U)                  /* Voltage high min */
#define EM_VOL_HIGH_MAX                             (400U)                  /* Voltage high max */
#define EM_CUR_HIGH_MIN                             (10U)                   /* Current high min */
#define EM_CUR_HIGH_MAX                             (60U)                   /* Current high max */
#define EM_FREQ_LOW_MIN                             (40U)                   /* Frequency low min */
#define EM_FREQ_LOW_MAX                             (60U)                   /* Frequency low max */
#define EM_FREQ_HIGH_MIN                            (50U)                   /* Frequency low min */
#define EM_FREQ_HIGH_MAX                            (70U)                   /* Frequency low max */
#define EM_MAX_DEMAND_MIN                           (15U)                   /* Max demand min */
#define EM_MAX_DEMAND_MAX                           (90U)                   /* Max demand max */
#define EM_NUM_TARIFF_MIN                           (1U)                    /* Tariffs min. */
#define EM_NUM_TARIFF_MAX                           (6U)                    /* Tariffs max. */
#define EM_NUM_TIMESLOT_MIN                         (1U)                    /* Time slot min. */
#define EM_NUM_TIMESLOT_MAX                         (6U)                    /* Time slot max. */

/* Calibration information */
#define EM_METER_CONST_MIN                          (500U)                  /* Meter Constant min */
#define EM_METER_CONST_MAX                          (5000U)                 /* Meter Constant max */
#define EM_PULSE_ON_TIME_MIN                        (30U)                   /* Pulse On Time Min */

/* Co-efficient Constant */
#define EM_VRMS_COEFF_MIN                           (0.1f)                  /* VRMS Co-efficient min */
#define EM_IRMS_COEFF_MIN                           (0.1f)                  /* IRMS Co-efficient min */
#define EM_ACT_POWER_COEFF_MIN                      (0.1f)                  /* Active Power Co-efficient min */
#define EM_REA_POWER_COEFF_MIN                      (0.1f)                  /* Reactive Power Co-efficient min */
#define EM_APP_POWER_COEFF_MIN                      (0.1f)                  /* Apparent Power Co-efficient min */

/* Offset information */
#define EM_V_OFFSET_MAX                             (32768U)                /* Voltage Offset max */

/* HW Phase Correction Constant */
#define EM_HW_PHASE_CORRECTION_DEGREE_MIN           (0.0f)                  /* Min delay degree */
#define EM_HW_PHASE_CORRECTION_DEGREE_MAX           (10.0f)                 /* Max delay degree */
#define EM_HW_PHASE_CORRECTION_DELAY_SELECT_MIN     (0U)                    /* Min value of delay correction select */
#define EM_HW_PHASE_CORRECTION_DELAY_SELECT_MAX     (2U)                    /* Max value of delay correction select */

/* SW Phase Correction Constant */
#define EM_SW_PHASE_CORRECTION_DEGREE_MIN           (-13.8f)                /* Min phase degree */
#define EM_SW_PHASE_CORRECTION_DEGREE_MAX           ( 13.8f)                /* Max phase degree */

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Variable Externs
***********************************************************************************************************************/

/***********************************************************************************************************************
Functions Prototypes
***********************************************************************************************************************/

#endif /* _EM_CONSTRAINT_H */
