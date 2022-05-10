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
* File Name    : r_cg_cgc.c
* Version      : 
* Device(s)    : RL78/I1C
* Tool-Chain   : CCRL
* Description  : This file implements device driver for CGC module.
* Creation Date: 
***********************************************************************************************************************/

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_cgc.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_CGC_Create
* Description  : This function initializes the clock generator.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_CGC_Create(void)
{
volatile uint32_t w_count;
    
    /* Set fMX */
    CMC = _00_CGC_HISYS_PORT/*_40_CGC_HISYS_OSC */ | _10_CGC_SYSOSC_PERMITTED | _01_CGC_SYSOSC_OVER10M;
    MSTOP = 1U;     /* X1 oscillator/external clock stopped */
    
    /* Set fIM */
    MIOEN = 0U;     /* middle-speed on-chip oscillator stopped */
    
    /* Change the waiting time according to the system */
    for (w_count = 0U; w_count <= CGC_FIMWAITTIME; w_count++)
    {
        NOP();
    }
    
    /* Set fPLL */
    PCKC = _00_CGC_fIH_STOP;
    // DSCCTL = _00_CGC_PLL_STOP;
    
    /* Change the waiting time according to the system */
    for (w_count = 0U; w_count <= CGC_FPLLWAITTIME; w_count++)
    {
        NOP();
    }
    
    //  MCKC = _00_CGC_fOCO_SELECTED;
    
    /* Set fSX */
    OSMC = _80_CGC_CLK_STOP | _00_CGC_IT_CLK_fSX_CLK;
    VRTCEN = 1U;    /* enables input clock supply */
    
    if (RTCPORSR == 0)
    {
        SCMC = _10_CGC_SUB_OSC | _02_CGC_NORMAL_OSCILLATION;
        XTSTOP = 0U;    /* XT1 oscillator operating */
    }
    
    XT1SELDIS = 0U; /* stops clock supply */
    
    /* Set fSUB for CPU */
    SELLOSC = 0U;   /* 1: low speed on-chip (fIL) */
    /* Set fCLK */
    CSS = 0U;       /* main system clock (fMAIN) */
    /* Set fMAIN */
    MCM0 = 0U;      /* selects the main on-chip oscillator clock (fOCO) or PLL clock (fPLL) as the main system clock (fMAIN) */
    /* Set fMAIN Control */
    MCM1 = 0U;      /* high-speed on-chip oscillator clock */
	
	
	
//    volatile uint16_t w_count;

//    /* Can only set once */
//    CMC = _40_CGC_HISYS_OSC | _10_CGC_XT1_SELECT_ENA | _01_CGC_SYSOSC_OVER10M ;     
    
//    /* Supply clock to access RTC domain register */
//    VRTCEN = 1U;
    
//    if (RTCPORSR == 0)
//    {
//        SCMC = _10_CGC_SUB_OSC | _02_CGC_SUBMODE_NORMAL;
//        XTSTOP = 0U;    /* XT1 oscillator operating */
//    }
    
//    /* Waiting time for sub clock stabilization */
//    /* XT1 */
//    for (w_count = 0U; w_count <= CGC_SUBWAITTIME; w_count++)
//    {
//        NOP();
//    }
    
//    /* Enable XT1 clock for CPU */
//    XT1SELDIS = 0U;
    
//    /* Turn on fIH */
//    HIOSTOP = 0;

//    /* Select fMAIN as fCLK */
//    CSS = 0U;
    
//    /* Select fOCO as fMAIN */
//    MCM0 = 0;
    
//    /* Select fIH as fOCO */
//    MCM1 = 0;

//     volatile uint32_t w_count;

//    /* Set fMX */
//    CMC = 0;//_00_CGC_HISYS_PORT | _00_CGC_SYSOSC_PROHIBITED | _00_CGC_SYSOSC_UNDER10M;
//    MSTOP = 1U;     /* X1 oscillator/external clock stopped */

//    /* Set fIM */
//    MIOEN = 0U;     /* middle-speed on-chip oscillator stopped */

//    /* Change the waiting time according to the system */
//    for (w_count = 0U; w_count <= 420; w_count++)
//    {
//        NOP();
//    }


//    /* Set fSX */
//    XTSTOP = 1U;    /* XT1 oscillator stopped */

//    /* Set fSUB */
//    SELLOSC = 1U;   /* low-speed on-chip oscillator clock (fIL) */

//    /* Change the waiting time according to the system */
//    for (w_count = 0U; w_count <= 80; w_count++)
//    {
//        NOP();
//    }

//    OSMC = 0x10;
//    /* Set fMAIN */
//    MCM0 = 0U;      /* selects the main on-chip oscillator clock (fOCO) or PLL clock (fPLL) as the main system clock (fMAIN) */
//    /* Set fMAIN Control */
//    MCM1 = 0U;      /* high-speed on-chip oscillator clock */

}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
