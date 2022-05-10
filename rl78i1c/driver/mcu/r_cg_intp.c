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
* File Name    : r_cg_intp.c
* Version      : 
* Device(s)    : RL78/I1C
* Tool-Chain   : CCRL
* Description  : This file implements device driver for INTP module.
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
#include "r_cg_intp.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_INTC_Create
* Description  : This function initializes INTP module.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_INTC_Create(void)
{
    PMK0 = 1U;    /* disable INTP0 operation */
    PIF0 = 0U;    /* clear INTP0 interrupt flag */
    PMK1 = 1U;    /* disable INTP1 operation */
    PIF1 = 0U;    /* clear INTP1 interrupt flag */
    PMK2 = 1U;    /* disable INTP2 operation */
    PIF2 = 0U;    /* clear INTP2 interrupt flag */
    PMK3 = 1U;    /* disable INTP3 operation */
    PIF3 = 0U;    /* clear INTP3 interrupt flag */
    PMK4 = 1U;    /* disable INTP4 operation */
    PIF4 = 0U;    /* clear INTP4 interrupt flag */
    PMK5 = 1U;    /* disable INTP5 operation */
    PIF5 = 0U;    /* clear INTP5 interrupt flag */
    PMK6 = 1U;    /* disable INTP6 operation */
    PIF6 = 0U;    /* clear INTP6 interrupt flag */
    PMK7 = 1U;    /* disable INTP7 operation */
    PIF7 = 0U;    /* clear INTP7 interrupt flag */
//    RTCIMK0 = 1U;   /* disable INTRTCIC0 interrupt */
//    RTCIIF0 = 0U;   /* clear INTRTCIC0 interrupt flag */
//    RTCIMK1 = 1U;   /* disable INTRTCIC1 interrupt */
//    RTCIIF1 = 0U;   /* clear INTRTCIC1 interrupt flag */
//    RTCIMK2 = 1U;   /* disable INTRTCIC2 interrupt */
//    RTCIIF2 = 0U;   /* clear INTRTCIC2 interrupt flag */
    /* Set INTP0 low priority */
    PPR10 = 1U;
    PPR00 = 1U;
    /* Set INTP1 low priority */
    PPR11 = 1U;
    PPR01 = 1U;
    /* Set INTRTCIC1 low priority */
//    RTCIPR11 = 1U;
//    RTCIPR01 = 1U;
    /* Set INTRTCIC2 low priority */
//    RTCIPR12 = 1U;
//    RTCIPR02 = 1U;
    
    EGP0 = _02_INTP1_EDGE_RISING_SEL;
    EGN0 = _01_INTP0_EDGE_FALLING_SEL;
    
    /* Set RTCICn falling edge detection into input port */
//    EGP1 = _00_INTRTCIC2_EDGE_FALLING_DISABLE | _00_INTRTCIC1_EDGE_FALLING_DISABLE;
//    EGN1 = _40_INTRTCIC2_EDGE_FALLING_BOTH | _20_INTRTCIC1_EDGE_FALLING_BOTH;
    
//  /* Set INTP5 pin */
//    PM0 |= 0x04U;

    /* Set RTCIC1 pin */
//    PM15 |= 0x02U;
    /* Set RTCIC2 pin */
//    PM15 |= 0x04U;
}

/***********************************************************************************************************************
* Function Name: R_INTC0_Start
* Description  : This function clears INTP0 interrupt flag and enables interrupt.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_INTC0_Start(void)
{
    PIF0 = 0U;    /* clear INTP0 interrupt flag */
    PMK0 = 0U;    /* enable INTP0 interrupt */
}
/***********************************************************************************************************************
* Function Name: R_INTC0_Stop
* Description  : This function disables INTP0 interrupt and clears interrupt flag.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_INTC0_Stop(void)
{
    PMK0 = 1U;    /* disable INTP0 interrupt */
    PIF0 = 0U;    /* clear INTP0 interrupt flag */
}
/***********************************************************************************************************************
* Function Name: R_INTC1_Start
* Description  : This function clears INTP1 interrupt flag and enables interrupt.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_INTC1_Start(void)
{
    PIF1 = 0U;    /* clear INTP1 interrupt flag */
    PMK1 = 0U;    /* enable INTP1 interrupt */
}
/***********************************************************************************************************************
* Function Name: R_INTC1_Stop
* Description  : This function disables INTP1 interrupt and clears interrupt flag.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_INTC1_Stop(void)
{
    PMK1 = 1U;    /* disable INTP1 interrupt */
    PIF1 = 0U;    /* clear INTP1 interrupt flag */
}

/***********************************************************************************************************************
* Function Name: R_INTRTCIC1_Start
* Description  : This function clears INTRTCIC1 interrupt flag and enables interrupt.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_INTRTCIC1_Start(void)
{
//    RTCIIF1 = 0U;   /* clear INTRTCIC1 interrupt flag */
//    RTCIMK1 = 0U;   /* enable INTRTCIC1 interrupt */
}
/***********************************************************************************************************************
* Function Name: R_INTRTCIC1_Stop
* Description  : This function disables INTRTCIC1 interrupt and clears interrupt flag.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_INTRTCIC1_Stop(void)
{
//    RTCIMK1 = 1U;   /* disable INTRTCIC1 interrupt */
//    RTCIIF1 = 0U;   /* clear INTRTCIC1 interrupt flag */
}
/***********************************************************************************************************************
* Function Name: R_INTRTCIC2_Start
* Description  : This function clears INTRTCIC2 interrupt flag and enables interrupt.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_INTRTCIC2_Start(void)
{
//    RTCIIF2 = 0U;   /* clear INTRTCIC2 interrupt flag */
//    RTCIMK2 = 0U;   /* enable INTRTCIC2 interrupt */
}
/***********************************************************************************************************************
* Function Name: R_INTRTCIC2_Stop
* Description  : This function disables INTRTCIC2 interrupt and clears interrupt flag.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_INTRTCIC2_Stop(void)
{
//    RTCIMK2 = 1U;   /* disable INTRTCIC2 interrupt */
//    RTCIIF2 = 0U;   /* clear INTRTCIC2 interrupt flag */
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
