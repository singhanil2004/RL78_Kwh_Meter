/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only 
* intended for use with Renesas products. No other uses are authorized. This 
* software is owned by Renesas Electronics Corporation and is protected under 
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING 
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT 
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE 
* AND NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS 
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE 
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR 
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE 
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software 
* and to discontinue the availability of this software.  By using this software, 
* you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2012 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_lvd_user.c
* Version      : 
* Device(s)    : R5F10NPJ
* Tool-Chain   : CCRL
* Description  : This file implements device driver for LVD module.
* Creation Date: 5/7/2015
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
#include "r_cg_lvd.h"
/* Start user code for include. Do not edit comment generated here */
#include "platform.h"
/* End user code. Do not edit comment generated here */


/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */

/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: r_lvd_interrupt
* Description  : This function is INTLVI interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
#ifdef __CCRL__
#pragma interrupt r_lvd_interrupt(vect=INTLVI)
#endif /* __CCRL__ */
#ifdef __ICCRL78__
#pragma vector = INTLVI_vect
#endif /* __ICCRL78__ */
__INTERRUPT static void r_lvd_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */
    EI();
    
    //R_Wrapper_SignalDisplay();

    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_lvdvdd_interrupt
* Description  : This function is INTLVDVDD interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
#ifdef __CCRL__
#pragma interrupt r_lvdvdd_interrupt(vect=INTLVDVDD)
#endif /* __CCRL__ */
#ifdef __ICCRL78__
#pragma vector = INTLVDVDD_vect
#endif /* __ICCRL78__ */
__INTERRUPT static void r_lvdvdd_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */
    EI();
    
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_lvdvbat_interrupt
* Description  : This function is INTLVDVBAT interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
#ifdef __CCRL__
#pragma interrupt r_lvdvbat_interrupt(vect=INTLVDVBAT)
#endif /* __CCRL__ */
#ifdef __ICCRL78__
#pragma vector = INTLVDVBAT_vect
#endif /* __ICCRL78__ */
__INTERRUPT static void r_lvdvbat_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */
    EI();

    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_lvdvrtc_interrupt
* Description  : This function is INTLVDVRTC interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
#ifdef __CCRL__
#pragma interrupt r_lvdvrtc_interrupt(vect=INTLVDVRTC)
#endif /* __CCRL__ */
#ifdef __ICCRL78__
#pragma vector = INTLVDVRTC_vect
#endif /* __ICCRL78__ */
__INTERRUPT static void r_lvdvrtc_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */
    EI();
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_lvdexlvd_interrupt
* Description  : This function is INTLVDEXLVD interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
#ifdef __CCRL__
#pragma interrupt r_lvdexlvd_interrupt(vect=INTLVDEXLVD)
#endif /* __CCRL__ */
#ifdef __ICCRL78__
#pragma vector = INTLVDEXLVD_vect
#endif /* __ICCRL78__ */
__INTERRUPT static void r_lvdexlvd_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */
    EI();
    /* End user code. Do not edit comment generated here */
}
/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
