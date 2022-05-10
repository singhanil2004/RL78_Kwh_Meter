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
* File Name    : main.c
* Version      : 1.00
* Device(s)    : RL78/I1C
* Tool-Chain   : CCRL
* H/W Platform : RL78/I1C Energy Meter Platform
* Description  : Main processing
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
/* Driver */
#include "r_cg_macrodriver.h"           /* CG Macro Driver */
#include "r_cg_cgc.h"
#include "r_cg_wdt.h"                   /* CG WDT Driver */
/* Wrapper/User */
#include "wrp_em_sw_config.h"
#include "wrp_em_adc_data.h"
/* Application */
#include "startup.h"                    /* Startup Header File */
#include "inst_read.h"
#include "modbus.h"
#include "key.h"
#include "em_display.h"

/*
 * CS+ CCRL reload trigger:
 * This is used to reload changes from middlewware
 * when there's no change on application layer
 * There's no dummy_reload.h file, this just make
 * a "not found" file in dependency list of main.c
 * Hence, trigger re-compile everytime build project
*/
#ifdef DUMMY_FOR_RELOAD
#include "dummy_reload.h"
#endif /* DUMMY_FOR_RELOAD */
/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#include "typedef.h"

/***********************************************************************************************************************
Imported global variables and functions (from other files)
***********************************************************************************************************************/

/***********************************************************************************************************************
Exported global variables and functions (to be accessed by other files)
***********************************************************************************************************************/

/***********************************************************************************************************************
Private global variables and functions
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: void main(void)
* Description  : Main processing
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
void main(void)
{
    uint16_t            startup_count = 0;
	
    R_CGC_OperateAt24MHz();
    
    /* Start the meter with relavent peripherals and EM Metrology*/
    while (startup() != STARTUP_OK)
    {
        startup_count++;
        R_WDT_Restart();
    }
    
    /* Start-up OK */
    while (1)
    {
        MODBUS_Process();
        KEY_PollingProcessing();
        EM_DisplaySequence();
        PWRFail_PollingProcessing();
        R_WDT_Restart();
	/* Sampling chain Polling Processing */
        //EM_ADC_SAMP_PollingProcessing();
    }
}
