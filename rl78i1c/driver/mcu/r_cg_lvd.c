/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
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
* Copyright (C) 2015, 2020 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_cg_lvd.c
* Version      : Code Generator for RL78/I1C V1.01.06.01 [25 Nov 2020]
* Device(s)    : R5F10NLG
* Tool-Chain   : CCRL
* Description  : This file implements device driver for LVD module.
* Creation Date: 26-08-2021
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_lvd.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_LVD_Create
* Description  : This function initializes the voltage detector.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_LVD_Create(void)
{
    LVIMK = 1U;     /* disable INTLVI interrupt */
    LVIIF = 0U;     /* clear INTLVI interrupt flag */
    LVDVDMK = 1U;   /* disable INTLVDVDD interrupt */
    LVDVDIF = 0U;   /* clear INTLVDVDD interrupt flag */
    LVDVBMK = 1U;   /* disable INTLVDVBAT interrupt */
    LVDVBIF = 0U;   /* clear INTLVDVBAT interrupt flag */
    LVDVRMK = 1U;   /* disable INTLVDVRTC interrupt */
    LVDVRIF = 0U;   /* clear INTLVDVRTC interrupt flag */
    LVDEXMK = 1U;   /* disable INTLVDEXLVD interrupt */
    LVDEXIF = 0U;   /* clear INTLVDEXLVD interrupt flag */
    /* Set INTLVDVRTC high priority */
    LVDVRPR1 = 0U;
    LVDVRPR0 = 0U;
}






/***********************************************************************************************************************
* Function Name: R_LVD_Start_VRTC
* Description  : This function enables the voltage detector VRTC.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_LVD_Start_VRTC(void)
{
    //LVDVRIF = 0U;   /* clear INTLVDVRTC interrupt flag */
    //LVDVRMK = 0U;   /* enable INTLVDVRTC interrupt */
    LVDVRTC = _80_LVD_VRTC_DELECT_ENABLE | _01_LVD_VBAT_VOLTAGE_237;
}

/***********************************************************************************************************************
* Function Name: R_LVD_Stop_VRTC
* Description  : This function disables the voltage detector VRTC.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_LVD_Stop_VRTC(void)
{
    LVDVRMK = 1U;   /* disable INTLVDVRTC interrupt */
    LVDVRIF = 0U;   /* clear INTLVDVRTC interrupt flag */
    LVDVRTCEN = 0U; /* disables detection */
}



/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
