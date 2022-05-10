/**********************************************************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
 * other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
 * applicable laws, including copyright laws.
 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
 * THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
 * EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
 * SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO
 * THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
 * this software. By using this software, you agree to the additional terms and conditions found by accessing the
 * following link:
 * http://www.renesas.com/disclaimer
 *
 * Copyright (C) 2019 Renesas Electronics Corporation. All rights reserved.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * File Name    : inst_read.c
 * Version      : 1.0
 * Description  : Perform periodic snapshot of system parameters and shared globally
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * History : DD.MM.YYYY Version  Description
 *         :
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
/* Driver */
#include "r_cg_macrodriver.h"   /* MD Macro Driver */
#include "r_cg_wdt.h"           /* MD WDT Driver */
#include "r_cg_dsadc.h"
#include "r_cg_sau.h"

/* Code Standard */
#include "typedef.h"            /* GSCE Standard Typedef */
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

/* EM */
#include "em_measurement.h"
#include "em_operation.h"
#include "r_calib.h"

/* Application */
#include "inst_read.h"
#include "config_storage.h"
#include "user_config.h"
#include "wrp_mcu.h"

/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/
#define INST_VRMS_DECIMAL_POINT                         100
#define INST_IRMS_DECIMAL_POINT                         1000
#define INST_POWER_FACTOR_DECIMAL_POINT                 1000
#define INST_ENERGY_DECIMAL_POINT                       200.0f
#define INST_FREQUENCY_DECIMAL_POINT                    100
#define INST_POWER_DECIMAL_POINT                        1.0f
#define INST_VOLTAGE_THD_DECIMAL_POINT                  100
#define INST_CURRENT_THD_DECIMAL_POINT                  100
/**********************************************************************************************************************
 Local Typedef definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Exported global variables
 *********************************************************************************************************************/
EM_INST_READ_PARAMS g_inst_read_params;
EM_ENERGY_COUNTER MainsEnergies;
EM_ENERGY_COUNTER PeriodicEnergies;
uint8_t IsDG = 0;
uint8_t g_calibmode = 0, g_modbusmode = 0;
uint8_t g_uart2_send_end;
uint8_t calib_result = 0;
uint8_t pulse_on_duration;
uint16_t pulse_period;
uint16_t pulse_weight_wh;
volatile int32_t pulse_energy;
static float32_t s_PowerMulFactor;
static float32_t s_EnergyMulFactor;
static float32_t s_CurrentMulFactor;
/* debug direct power output will override the inst direct power output */
#ifdef __DEBUG
volatile float32_t g_debug_direct_power_output_value = -1.0f;
#endif
float32_t g_inst_read_direct_power_output_value = -1.0f;

/**********************************************************************************************************************
 Private (static) variables and functions
 *********************************************************************************************************************/
static int16_t s_EngergySaveCounter;

/**********************************************************************************************************************
* Function Name: void INST_READ_RTC_InterruptCallBack(void)
* Description  : 0.5s tick for reading system snapshot
*              : NOTE: please put this under 0.5s RTC
* Arguments    : None
* Return Value : None
 *********************************************************************************************************************/
void INST_READ_RTC_InterruptCallBack(void)
{
    EM_LINE line;
    uint32_t delta_energy;
    float tmp, tmp1;
    static float s_last_energy = 0;
    
    /* Update the parameter */
    for (line = LINE_PHASE_R; line <= LINE_TOTAL; line++)
    {
        /* VRMS */
        g_inst_read_params.vrms[line] = (int32_t)(EM_GetVoltageRMS(line) * INST_VRMS_DECIMAL_POINT);
        
        /* IRMS */
        g_inst_read_params.irms[line] = (int32_t)(EM_GetCurrentRMS(line) * s_CurrentMulFactor);
        
        /* Line Frequency */
        g_inst_read_params.freq[line] = (int32_t)(EM_GetLineFrequency(line) * INST_FREQUENCY_DECIMAL_POINT);
        
        /* Power factor */
        g_inst_read_params.power_factor[line] = (int32_t)(EM_GetPowerFactor(line) * INST_POWER_FACTOR_DECIMAL_POINT);
        g_inst_read_params.power_factor_sign[line] = (int32_t)EM_GetPowerFactorSign(line);
        
        if (g_inst_read_params.power_factor_sign[line] == PF_SIGN_LEAD_C)
        {
            g_inst_read_params.power_factor[line] = -g_inst_read_params.power_factor[line];
        }
        
        /* Active power */
        g_inst_read_params.active_power[line] = (int32_t)(EM_GetActivePower(line) * s_PowerMulFactor);
        
        /* Reactive power */
        g_inst_read_params.reactive_power[line] = (int32_t)(EM_GetReactivePower(line) * s_PowerMulFactor);
        
        /* Apparent power */
        g_inst_read_params.apparent_power[line] = (int32_t)(EM_GetApparentPower(line) * s_PowerMulFactor);
        
        
        /* Fundamental VRMS */
        //g_inst_read_params.fundamental_vrms[line] = (int32_t)(EM_GetFundamentalVoltageRMS(line) * INST_VRMS_DECIMAL_POINT);
        
        /* Fundamental IRMS */
        //g_inst_read_params.fundamental_irms[line] = (int32_t)(EM_GetFundamentalCurrentRMS(line) * s_EnergyMulFactor);
        
        /* Fundamental active power */
        //g_inst_read_params.fundamental_power[line] = (int32_t)(EM_GetFundamentalActivePower(line) * s_PowerMulFactor);
        
        /* Voltage THD */
        g_inst_read_params.voltage_thd[line] = (int32_t)(EM_GetVoltageTHD(line) * INST_VOLTAGE_THD_DECIMAL_POINT);
        
        /* Current THD */
        g_inst_read_params.current_thd[line] = (int32_t)(EM_GetCurrentTHD(line) * INST_VOLTAGE_THD_DECIMAL_POINT);
        
        /* Power THD */
        //g_inst_read_params.power_thd[line] = (int32_t)(EM_GetActivePowerTHD(line) * INST_VOLTAGE_THD_DECIMAL_POINT);
        
        /* Current phase to phase angle */
        g_inst_read_params.current_phase_angle[line] = EM_GetCurrentPhaseAngle(line, (EM_LINE)((line + 1) % LINE_NEUTRAL));
        
        /* Current neutral to phase angle */
        g_inst_read_params.current_neutral_angle[line] = EM_GetCurrentPhaseAngle(LINE_NEUTRAL, line);
        
    }
    
    /* Voltage phase to phase angle */
    g_inst_read_params.phase_angle[LINE_PHASE_R] = EM_GetPhaseAngleRtoY();
    g_inst_read_params.phase_angle[LINE_PHASE_Y] = EM_GetPhaseAngleYtoB();
    g_inst_read_params.phase_angle[LINE_PHASE_B] = EM_GetPhaseAngleBtoR();
    
    
    /* Vector sum current, 3 decimal */
    //g_inst_read_params.vector_sum_irms = (int32_t)(EM_GetPhaseCurrentSumVector() * INST_IRMS_DECIMAL_POINT);
    
    /*Get Energy */
    tmp = (int32_t)(EM_GetActiveEnergyTotal(LINE_TOTAL) * 1000);
    delta_energy = (tmp - s_last_energy) * s_EnergyMulFactor;
    s_last_energy = tmp;
    
#ifdef DUAL_SOURCE
    IsDG = DG_SENCE;
    
    if (IsDG)
    {
        MainsEnergies.active_exp += delta_energy;
    }
    else
    {
        MainsEnergies.active_imp += delta_energy;
    }
    
#else
    pulse_energy += delta_energy;
    
    if (pulse_energy > 0)
    {
        pulse_period = (248 * pulse_weight_wh) / pulse_energy;
    }
    else
    {
        pulse_period = 0xFFFF;
    }
    
    MainsEnergies.active_imp += delta_energy;
#endif
    
    /* Set the power */
    // EM_PULSE0_SetPower(g_inst_read_params.active_power[LINE_PHASE_R] ,  INST_ACTIVE_POWER_DECIMAL_POINT );
    // EM_PULSE1_SetPower(g_inst_read_params.active_power[LINE_PHASE_Y] ,  INST_ACTIVE_POWER_DECIMAL_POINT );
    // EM_PULSE2_SetPower(g_inst_read_params.active_power[LINE_PHASE_B] ,  INST_ACTIVE_POWER_DECIMAL_POINT );
    EM_PULSE3_SetPower(EM_GetActivePower(LINE_TOTAL));
    EM_PULSE4_SetPower(g_inst_read_params.reactive_power[LINE_TOTAL]);
    
    /* Update pulse one time in case specified power */
    
    //#ifdef __DEBUG
    
    //    /* Debug fixed power output will override  */
    //    if (g_debug_direct_power_output_value > 0.0f)
    //    {
    //        EM_PULSE3_SetPower(g_debug_direct_power_output_value);
    //        EM_PULSE4_SetPower(0);
    //    }
    //    else
    //    {
    //#endif
    
    //        if (g_inst_read_direct_power_output_value > 0.0f)
    //        {
    //            /* Fixed power output request from other applications */
    //            //EM_PULSE3_SetPower(g_inst_read_direct_power_output_value);
    //            //EM_PULSE4_SetPower(g_inst_read_direct_power_output_value);
    //        }
    //        else
    //        {
    //            EM_PULSE3_SetPower(g_inst_read_params.active_power[LINE_TOTAL]);
    //            EM_PULSE4_SetPower(g_inst_read_params.reactive_power[LINE_TOTAL]);
    //        }
    
    //#ifdef __DEBUG
    //    }
    
    //#endif
    
    /* Reset counting, start updating system parameters as below */
    s_EngergySaveCounter++;
    MainsEnergies.run_hour++;
    
    if ((s_EngergySaveCounter > 600) == 0)
    {
        INST_SaveEnergies(PERIODIC_SAVE);
    }
}

/**********************************************************************************************************************
* Function Name: void INST_OutPulseInterruptCallback(void)
* Description  :
* Arguments    : None
* Return Value : None
 *********************************************************************************************************************/
void INST_OutPulseInterruptCallback(void)
{
    static uint32_t pulse_timer = 0;
    pulse_timer++;
    
    if (pulse_on_duration)
    {
        pulse_on_duration--;
        
        if (pulse_on_duration == 0)
        {
            SO_PULSE4_OFF_STATEMENT;
        }
    }
    else
    {
        if (pulse_timer >= pulse_period)
        {
            if (pulse_energy > pulse_weight_wh)
            {
                pulse_on_duration = 10;
                SO_PULSE4_ON_STATEMENT;
                pulse_energy -= pulse_weight_wh;
            }
            
            pulse_timer = 0;
        }
    }
}
/**********************************************************************************************************************
* Function Name: void INST_UpdateEnergiesAtStartup(void)
* Description  :
* Arguments    : None
* Return Value : None
 *********************************************************************************************************************/
void INST_UpdateMultiplyfactor(void)
{
    s_PowerMulFactor = (INST_POWER_DECIMAL_POINT * user_config.CTR) / 5;;
    s_EnergyMulFactor = ((float)user_config.CTR / 5);
    s_CurrentMulFactor = ((float)user_config.CTR*1000 / 5);
    
    if (user_config.CTR < 11)
    {
        pulse_weight_wh = 10;
        g_eng_decimal = 2;
        //s_PowerMulFactor = (INST_POWER_DECIMAL_POINT * 100*user_config.CTR) / 5;;
    }
    else if (user_config.CTR < 101)
    {
        pulse_weight_wh = 100;
        g_eng_decimal = 1;
        //s_PowerMulFactor = (INST_POWER_DECIMAL_POINT * user_config.CTR) / 5;
    }
    else
    {
        pulse_weight_wh = 1000;
        g_eng_decimal = 0;
        s_PowerMulFactor = (INST_POWER_DECIMAL_POINT * user_config.CTR) / 5;;
        
    }
}
/**********************************************************************************************************************
* Function Name: void INST_UpdateEnergiesAtStartup(void)
* Description  :
* Arguments    : None
* Return Value : None
 *********************************************************************************************************************/
void INST_UpdateEnergiesAtStartup(void)
{
 if(PeriodicEnergies.active_imp > MainsEnergies.active_imp)
 {
	 MainsEnergies = PeriodicEnergies;
 }
#ifndef DUAL_SOURCE
    pulse_energy = MainsEnergies.active_imp % pulse_weight_wh;
    pulse_period = 0xFFFF;
#endif
    
    if (CALIB_MODE == 1)
    {
        g_calibmode = 1;
    }
    
    if (MODBUS_MODE == 0)
    {
        g_modbusmode = 1;
    }
    
    INST_UpdateMultiplyfactor();
}
/**********************************************************************************************************************
* Function Name: void INST_ResetEnergies(void)
* Description  :
* Arguments    : None
* Return Value : None
 *********************************************************************************************************************/
void INST_ResetEnergies(void)
{
    /* Reset Mains Energy */
    MainsEnergies.active_imp = 0;
    MainsEnergies.active_exp = 0;
    //    MainsEnergies.reactive_ind_imp = 0;
    //    MainsEnergies.reactive_cap_imp = 0;
    //    MainsEnergies.reactive_cap_imp = 0;
    //    MainsEnergies.reactive_cap_exp = 0;
    //    MainsEnergies.apparent_imp = 0;
    //    MainsEnergies.apparent_exp = 0;
    MainsEnergies.run_hour = 0;
    
    /* Reset Config */
    user_config.CTR = 5;
    user_config.ModbusID = 18;
    
    if (CONFIG_Backup(CONFIG_ITEM_USER) != EM_OK)
    {
    }
    
    s_EngergySaveCounter = 100;
    INST_SaveEnergies(PERIODIC_SAVE);
	s_EngergySaveCounter = 100;
    INST_SaveEnergies(PWR_FAIL_SAVE);
    INST_UpdateMultiplyfactor();
}
/**********************************************************************************************************************
* Function Name: void INST_ResetEnergies(void)
* Description  :
* Arguments    : None
* Return Value : None
 *********************************************************************************************************************/
void PWRFail_PollingProcessing(void)
{
    if (LVDVRTCF == 1) // Power fail occurs
    {
        INST_SaveEnergies(PWR_FAIL_SAVE);
		MCU_Delay(5000);
    }
}
/**********************************************************************************************************************
* Function Name: void INST_SaveEnergies(void)
* Description  :
* Arguments    : None
* Return Value : None
 *********************************************************************************************************************/
void INST_SaveEnergies(uint8_t SaveType)
{
    if (s_EngergySaveCounter > 15)
    {
        s_EngergySaveCounter = 0;
        if(SaveType == PWR_FAIL_SAVE)
		{
        if (CONFIG_Backup(CONFIG_ITEM_MAINS_ENG) != CONFIG_OK)
        {
        }
		}
		else
		{
			if (CONFIG_Backup(CONFIG_ITEM_PERIODIC_ENG) != CONFIG_OK)
        {
        }
		}
    }
}
/***********************************************************************************************************************
* Function Name : uart_printf
* Interface     : int uart_printf(const char __far *format, ...)
* Description   : This function print out a string with format to UART2
*               : Not overriding compiler default printf due to different near far specification
* Arguments     : const char * format:
* Return Value  : int
 **********************************************************************************************************************/
int uart_printf(const char __far* format, ...)
{
    uint8_t buffer[256];
    uint16_t length;
#ifdef __ICCRL78__
    va_list     arg = { NULL };
#else
    va_list     arg = NULL;
#endif /* __ICCRL78__ */
    
    /* Parse the argument list, print to buffer string */
    va_start(arg, format);
    
    /* Format the string */
    length = vsprintf((char*)buffer, (const char*)format, arg);
    
    /* Print out to UART2 TX: optical port */
    g_uart2_send_end = 0;
    R_UART2_Send(buffer, length);
    
    while (g_uart2_send_end == 0);
    
    va_end(arg);
    
    return length;
}
/***********************************************************************************************************************
 * Function Name: INST_AutoCalibrate
 * Description  : NA
 * Arguments    : void
 * Return Value : void
 **********************************************************************************************************************/
void INST_AutoCalibrate(void)
{
    uart_printf("\n\rStart autocalibration: \n\r");
    calib_result = COMMAND_InvokeCalibration();
    uart_printf("Autocalibration end\n\r");
    uart_printf("Calibration status: %d \n\r", calib_result);
    /* In case of break using E1, need to stop EM and start again to reset sampling chain */
    EM_Stop();
    EM_Start();
}
/***********************************************************************************************************************
 * Function Name: INST_Print_Parameters
 * Description  : NA
 * Arguments    : void
 * Return Value : void
 **********************************************************************************************************************/
void INST_Print_Parameters(void)
{
    EM_INST_READ_PARAMS params;
    
    /* Pull global variables to local first */
    params = g_inst_read_params;
    
    /* Feed watch dog timer regularly during print out */
    R_WDT_Restart();
    uart_printf("\n\r");
    uart_printf("Printing out instantaneous parameters:");
    uart_printf("\n\r");
    
    R_WDT_Restart();
    uart_printf("PHASE_R\n\r");
    uart_printf("VRMS:               %# 10.3d (V)\n\r", params.vrms[LINE_PHASE_R]);
    uart_printf("IRMS:               %# 10.3d (A)\n\r", params.irms[LINE_PHASE_R]);
    uart_printf("Power factor:       %# 10.3d\n\r", params.power_factor[LINE_PHASE_R]);
    uart_printf("Active power:       %# 10.3d (Watt)\n\r", params.active_power[LINE_PHASE_R]);
    uart_printf("Reactive power:     %# 10.3d (Var)\n\r", params.reactive_power[LINE_PHASE_R]);
    uart_printf("Apparent power:     %# 10.3d (Va)\n\r", params.apparent_power[LINE_PHASE_R]);
    uart_printf("Fundamental power:  %# 10.3d (Watt)\n\r", params.fundamental_power[LINE_PHASE_R]);
    uart_printf("\n\r");
    
    R_WDT_Restart();
    uart_printf("PHASE_Y\n\r");
    uart_printf("VRMS:               %# 10.3d (V)\n\r", params.vrms[LINE_PHASE_Y]);
    uart_printf("IRMS:               %# 10.3d (A)\n\r", params.irms[LINE_PHASE_Y]);
    uart_printf("Power factor:       %# 10.3d\n\r", params.power_factor[LINE_PHASE_Y]);
    uart_printf("Active power:       %# 10.3d (Watt)\n\r", params.active_power[LINE_PHASE_Y]);
    uart_printf("Reactive power:     %# 10.3d (Var)\n\r", params.reactive_power[LINE_PHASE_Y]);
    uart_printf("Apparent power:     %# 10.3d (Va)\n\r", params.apparent_power[LINE_PHASE_Y]);
    uart_printf("Fundamental power:  %# 10.3d (Watt)\n\r", params.fundamental_power[LINE_PHASE_Y]);
    uart_printf("\n\r");
    
    R_WDT_Restart();
    uart_printf("PHASE_B\n\r");
    uart_printf("VRMS:               %# 10.3d (V)\n\r", params.vrms[LINE_PHASE_B]);
    uart_printf("IRMS:               %# 10.3d (A)\n\r", params.irms[LINE_PHASE_B]);
    uart_printf("Power factor:       %# 10.3d\n\r", params.power_factor[LINE_PHASE_B]);
    uart_printf("Active power:       %# 10.3d (Watt)\n\r", params.active_power[LINE_PHASE_B]);
    uart_printf("Reactive power:     %# 10.3d (Var)\n\r", params.reactive_power[LINE_PHASE_B]);
    uart_printf("Apparent power:     %# 10.3d (Va)\n\r", params.apparent_power[LINE_PHASE_B]);
    uart_printf("Fundamental power:  %# 10.3d (Watt)\n\r", params.fundamental_power[LINE_PHASE_B]);
    uart_printf("\n\r");
    
    R_WDT_Restart();
    uart_printf("NEUTRAL\n\r");
    uart_printf("IRMS:               %# 10.3d (A)\n\r", params.irms[LINE_NEUTRAL]);
    uart_printf("\n\r");
    
    R_WDT_Restart();
    uart_printf("TOTAL\n\r");
    uart_printf("VRMS:               %# 10.3d (V)\n\r", params.vrms[LINE_TOTAL]);
    uart_printf("IRMS:               %# 10.3d (A)\n\r", params.irms[LINE_TOTAL]);
    uart_printf("Vect_IRMS:          %# 10.3d (A)\n\r", params.vector_sum_irms);
    uart_printf("Power factor:       %# 10.3d\n\r", params.power_factor[LINE_TOTAL]);
    uart_printf("Active power:       %# 10.3d (Watt)\n\r", params.active_power[LINE_TOTAL]);
    uart_printf("Reactive power:     %# 10.3d (Var)\n\r", params.reactive_power[LINE_TOTAL]);
    uart_printf("Apparent power:     %# 10.3d (Va)\n\r", params.apparent_power[LINE_TOTAL]);
    uart_printf("Fundamental power:  %# 10.3d (Watt)\n\r", params.fundamental_power[LINE_TOTAL]);
    uart_printf("\n\r");
    
    R_WDT_Restart();
}

