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
* File Name    : r_cg_cgc_user.c
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
#include "r_cg_tau.h"
//#include "r_cg_sau.h"
#include "r_cg_wdt.h"
//#include "r_cg_iica.h"
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/***********************************************************************************************************************
* Function Name: R_CGC_FclkUseFsub
* Description  : Use fSUB as fCLK and supply to CPU/peripherals
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_CGC_FclkUseFsub(void)
{
    uint16_t w_count;
    
    /* Select fSUB as fCLK */
    SELLOSC = 0U;   /* sub clock (fSX) */

    CSS = 1U;       /* subsystem clock (fSUB) */
    
    /* Stop High-speed on-chip clock */
    HIOSTOP = 1U;
}

/***********************************************************************************************************************
* Function Name: R_CGC_FclkUseFil
* Description  : Use fSUB as fCLK and supply to CPU/peripherals
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_CGC_FclkUseFil(void)
{
    uint16_t w_count;
    
    /* Select fIL as fCLK */
    SELLOSC = 1U;   /* sub clock (fIL) */
    /* Change the waiting time according to the system */
    for (w_count = 0U; w_count <= CGC_FILWAITTIME; w_count++)
    {
       NOP();
    }
    CSS = 1U; 
    
    /* Stop High-speed on-chip clock */
    HIOSTOP = 1U;
}

/***********************************************************************************************************************
* Function Name: R_CGC_FclkUseFih
* Description  : Use fIH as fCLK and supply to CPU/peripherals
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_CGC_FclkUseFih(void)
{
    volatile uint16_t w_count;
    
    /* Start fIH */
    HIOSTOP = 0U;
    
    /* Wait stabilization time */
    for (w_count = 0U; w_count <= CGC_FIHWAITTIME; w_count++)
    {
        NOP();
    }
    
    /* Select fMAIN as CPU/peripherals clock */
    CSS = 0U;
}

/******************************************************************************
* Function Name : R_CGC_OperateAt24MHz
* Interface     : void R_CGC_OperateAt24MHz(void)
* Description   : 
* Arguments     : None:
* Function Calls: 
* Return Value  : None
******************************************************************************/
void R_CGC_OperateAt24MHz(void)
{
    HOCODIV = 0x00U;        /*  MCU operate at 24Mhz */
    TPS0 = _0000_TAU_CKM0_fCLK_0 | _0070_TAU_CKM1_fCLK_7 | _0000_TAU_CKM2_fCLK_1 | _0000_TAU_CKM3_fCLK_8;
}

/******************************************************************************
* Function Name : R_CGC_OperateAt1M5Hz
* Interface     : void R_CGC_OperateAt1M5Hz(void)
* Description   : 
* Arguments     : None:
* Function Calls: 
* Return Value  : None
******************************************************************************/
void R_CGC_OperateAt1M5Hz(void)
{
    HOCODIV = 0x04U;        /*  MCU operate at 1.5Mhz */
    /* No need to change the SAU setting, no uart used under this frequency */
    /* No need to change the Timer setting, no timer used under this frequency */

}

/* End user code. Do not edit comment generated here */
