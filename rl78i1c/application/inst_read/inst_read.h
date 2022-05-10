/******************************************************************************
  Copyright (C) 2011 Renesas Electronics Corporation, All Rights Reserved.
*******************************************************************************
* File Name    : dataflash.h
* Version      : 1.00
* Description  : Data Flash Application Layer APIs
******************************************************************************
* History : DD.MM.YYYY Version Description
******************************************************************************/

#ifndef _INST_READ_H
#define _INST_READ_H

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "typedef.h"        /* GSCE Standard Typedef */
/* EM */
#include "em_constraint.h"
#include "em_type.h"
/******************************************************************************
Typedef definitions
******************************************************************************/
typedef struct tagEMInstantRead
{
    int32_t   vrms[EM_CALC_NUM_OF_LINE];
    int32_t   vrmsp_p[EM_CALC_NUM_OF_LINE];
    int32_t   irms[EM_CALC_NUM_OF_LINE];
    int32_t   freq[EM_CALC_NUM_OF_LINE];
    int32_t   power_factor[EM_CALC_NUM_OF_LINE];
    EM_PF_SIGN  power_factor_sign[EM_CALC_NUM_OF_LINE];
    int32_t   active_power[EM_CALC_NUM_OF_LINE];
    int32_t   apparent_power[EM_CALC_NUM_OF_LINE];
    int32_t   reactive_power[EM_CALC_NUM_OF_LINE];
    int32_t   vector_sum_irms;
    int32_t   phase_angle[EM_NUM_OF_PHASE];
    int32_t   current_phase_angle[EM_CALC_NUM_OF_LINE];
    int32_t   current_neutral_angle[EM_CALC_NUM_OF_LINE];
    int32_t   fundamental_power[EM_CALC_NUM_OF_LINE];
    int32_t   fundamental_vrms[EM_CALC_NUM_OF_LINE];
    int32_t   fundamental_irms[EM_CALC_NUM_OF_LINE];
    int32_t   voltage_thd[EM_CALC_NUM_OF_LINE];
    int32_t   current_thd[EM_CALC_NUM_OF_LINE];
    int32_t   power_thd[EM_CALC_NUM_OF_LINE];
    
} EM_INST_READ_PARAMS;

/******************************************************************************
Macro definitions
******************************************************************************/
#define DG_SENCE                    (BIT_SELECT(P0,6))
#define CALIB_MODE                  (BIT_SELECT(P6,2))
#define MODBUS_MODE                 (BIT_SELECT(P0,5))
#define SO_PULSE4_ON_STATEMENT          {SET_BIT( P0, 7, 0 );}         /* Pulse 4 LED On */
#define SO_PULSE4_OFF_STATEMENT         {SET_BIT( P0, 7, 1 );}         /* Pulse 4 LED Off */
#define PERIODIC_SAVE               0
#define PWR_FAIL_SAVE               1
/******************************************************************************
Variable Externs
******************************************************************************/
extern EM_INST_READ_PARAMS g_inst_read_params;
extern EM_ENERGY_COUNTER MainsEnergies;
extern EM_ENERGY_COUNTER PeriodicEnergies;
extern uint8_t g_calibmode, g_modbusmode;
extern uint8_t g_eng_decimal;
/******************************************************************************
Functions Prototypes
******************************************************************************/
void INST_OutPulseInterruptCallback(void);
void INST_READ_RTC_InterruptCallBack(void);
void INST_UpdateEnergiesAtStartup(void);
void INST_UpdateMultiplyfactor(void);
void INST_ResetEnergies(void);
void INST_SaveEnergies(uint8_t SaveType);
void INST_AutoCalibrate(void);
void INST_Print_Parameters(void);
void PWRFail_PollingProcessing(void);


#endif /* _INST_READ_H */
