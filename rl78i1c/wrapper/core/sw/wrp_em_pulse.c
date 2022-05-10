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
* File Name    : wrp_em_pulse.c
* Version      : 1.00
* Device(s)    : RL78/I1C
* Tool-Chain   : CCRL
* H/W Platform : RL78/I1C Energy Meter Platform
* Description  : Wrapper EM PULSE APIs
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include "compiler.h"
#include "wrp_em_sw_config.h"       /* Wrapper Shared Configuration */
#include "wrp_em_pulse.h"           /* EM PULSE Wrapper Layer */
#include "em_type.h"                /* EM Core Type Definitions */
#include "em_constraint.h"          /* EM Core Configuration Constraint */
/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Macro Definitions Checking
***********************************************************************************************************************/

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
* Function Name    : void EM_PULSE_Init(void)
* Description      : EM Pulse Initialization
* Arguments        : None
* Functions Called : EM_PULSE_DriverInit()
* Return Value     : None
***********************************************************************************************************************/
void EM_PULSE_Init(void)
{
    EM_PULSE_DriverInit();
}
 /***********************************************************************************************************************
* Function Name    : void EM_PULSE0_On(void)
* Description      : EM Pulse 0 LED Turned on
* Arguments        : None
* Functions Called : EM_PULSE0_ON_STATEMENT
* Return Value     : None
***********************************************************************************************************************/
void EM_PULSE0_On(void)
{
//    EM_PULSE0_ON_STATEMENT;
}

/***********************************************************************************************************************
* Function Name    : void EM_PULSE0_Off(void)
* Description      : EM Pulse 0 LED Turned off
* Arguments        : None
* Functions Called : EM_PULSE0_OFF_STATEMENT
* Return Value     : None
***********************************************************************************************************************/
void EM_PULSE0_Off(void)
{
//    EM_PULSE0_OFF_STATEMENT;
}

/***********************************************************************************************************************
* Function Name    : void EM_PULSE1_On(void)
* Description      : EM Pulse 1 LED Turned on
* Arguments        : None
* Functions Called : EM_PULSE1_ON_STATEMENT
* Return Value     : None
***********************************************************************************************************************/
void EM_PULSE1_On(void)
{
//    EM_PULSE1_ON_STATEMENT;
}

/***********************************************************************************************************************
* Function Name    : void EM_PULSE1_Off(void)
* Description      : EM Pulse 1 LED Turned off
* Arguments        : None
* Functions Called : EM_PULSE1_OFF_STATEMENT
* Return Value     : None
***********************************************************************************************************************/
void EM_PULSE1_Off(void)
{
//    EM_PULSE1_OFF_STATEMENT;
}

/***********************************************************************************************************************
* Function Name    : void EM_PULSE2_On(void)
* Description      : EM Pulse 2 LED Turned on
* Arguments        : None
* Functions Called : EM_PULSE2_ON_STATEMENT
* Return Value     : None
***********************************************************************************************************************/
void EM_PULSE2_On(void)
{
//    EM_PULSE2_ON_STATEMENT;
}

/***********************************************************************************************************************
* Function Name    : void EM_PULSE2_Off(void)
* Description      : EM Pulse 2 LED Turned off
* Arguments        : None
* Functions Called : EM_PULSE2_OFF_STATEMENT
* Return Value     : None
***********************************************************************************************************************/
void EM_PULSE2_Off(void)
{
//    EM_PULSE2_OFF_STATEMENT;
}

/******************************************************************************
* Function Name : EM_PULSE3_Start
* Interface     : void EM_PULSE3_Start(void)
* Description   : EM Pulse 3 Hi Res channel start
* Arguments     : None
* Return Value  : None
******************************************************************************/
void EM_PULSE3_Start(void)
{
    EM_PULSE3_DriverStart();
}

/******************************************************************************
* Function Name : EM_PULSE3_Stop
* Interface     : void EM_PULSE3_Stop(void)
* Description   : EM Pulse 3 Hi Res channel stop
* Arguments     : None
* Return Value  : None
******************************************************************************/
void EM_PULSE3_Stop(void)
{
    EM_PULSE3_DriverStop();
}

/******************************************************************************
* Function Name : EM_PULSE3_UpdateInterval
* Interface     : void EM_PULSE3_UpdateInterval(uint32_t us)
* Description   : EM Pulse 3 Hi Res channel update pulse interval
* Arguments     : uint32_t us: Micro-second of pulse duration
* Return Value  : None
******************************************************************************/
void EM_PULSE3_UpdateInterval(uint32_t us)
{
    EM_PULSE3_DriverUpdateInterval(us);
}

/******************************************************************************
* Function Name : EM_PULSE3_UpdatePulseHighWidth
* Interface     : void EM_PULSE3_UpdatePulseHighWidth(uint16_t ms)
* Description   : EM Pulse 3 Hi Res channel update pulse high width
* Arguments     : uint16_t ms: Mili-second of pulse high width
* Return Value  : None
******************************************************************************/
void EM_PULSE3_UpdatePulseHighWidth(uint16_t ms)
{
    EM_PULSE3_DriverUpdatePulseHighWidth(ms);
}

/***********************************************************************************************************************
* Function Name    : void EM_PULSE4_On(void)
* Description      : EM Pulse 4 LED Turned on
* Arguments        : None
* Functions Called : EM_PULSE4_ON_STATEMENT
* Return Value     : None
***********************************************************************************************************************/
void EM_PULSE4_On(void)
{
    EM_PULSE4_ON_STATEMENT;
}

/***********************************************************************************************************************
* Function Name    : void EM_PULSE4_Off(void)
* Description      : EM Pulse 4 LED Turned off
* Arguments        : None
* Functions Called : EM_PULSE4_OFF_STATEMENT
* Return Value     : None
***********************************************************************************************************************/
void EM_PULSE4_Off(void)
{
    EM_PULSE4_OFF_STATEMENT;
}
