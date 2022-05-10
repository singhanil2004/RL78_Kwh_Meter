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
* File Name    : r_lvd.c
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
#include "wrp_mcu.h"
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
    /* Set INTLVI low priority */
    LVIPR1 = 1U;
    LVIPR0 = 1U;
    /* Set INTLVDVDD low priority */
    LVDVDPR1 = 1U;
    LVDVDPR0 = 1U;
    /* Set INTLVDVBAT low priority */
    LVDVBPR1 = 1U;
    LVDVBPR0 = 1U;
    /* Set INTLVDVRTC low priority */
    LVDVRPR1 = 1U;
    LVDVRPR0 = 1U;
    /* Set INTLVDEXLVD low priority */
    LVDEXPR1 = 1U;
    LVDEXPR0 = 1U;
    
    /* Set up detection threshold */
    LVDVDD = _03_VLVDVDD_LEVEL3;
    LVDVBAT = _04_VLVDVBAT_LEVEL4;
    LVDVRTC = _00_VLVDVRTC_LEVEL0;
    /* LVDEXLVD only one threshold, no setting for threshold*/
    
    LVDVDDEN = 1U;
    LVDVBATEN = 1U;
    LVDVRTCEN = 1U;
    LVDEXLVDEN = 1U;
    
    /* Delay 300us as spec */
    MCU_Delay(300);
    
    /* Clear the flag */
    LVDVDIF = 0U;
    LVDVBIF = 0U;
    LVDVRIF = 0U;
    LVDEXIF = 0U;
    
	/* Enable interrupt */
    LVDVDMK = 0U;
    LVDEXMK = 0U;
        
    /* Set EXLVD pin */
    PM2 |= 0x04U;
}

/***********************************************************************************************************************
* Function Name: R_LVD_InterruptMode_Start
* Description  : This function enables the voltage detector interrupt.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_LVD_InterruptMode_Start(void)
{
    LVIIF = 0U;     /* clear INTLVI interrupt flag */
    LVIMK = 0U;     /* enable INTLVI interrupt */
    
}
/***********************************************************************************************************************
* Function Name: R_LVD_Create
* Description  : This function initializes the voltage detector.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_LVDVDD_Create(void)
{
    LVDVDMK = 1U;     /* disable INTLVI interrupt */
    LVDVDIF = 0U;     /* clear INTLVI interrupt flag */
    
    LVDVDDEN = 0U;
    /* When developing, set according to voltage source using:
        When switch to backup battery, the VDD also has smaller value voltage
    */
    //LVDVDD = _03_VLVDVDD_LEVEL3;  /* Rising Edge 3.16 V, Falling Edge 3.09 V */
    LVDVDD = _01_VLVDVDD_LEVEL1;  /* Rising Edge 2.75 V, Falling Edge 2.68 V */
    
    /* Set INTLVI low priority */
    LVDVDPR1 = 1U;
    LVDVDPR0 = 1U;
}

/***********************************************************************************************************************
* Function Name: R_LVD_InterruptMode_Start
* Description  : This function enables the voltage detector interrupt.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_LVDVDD_Start(void)
{
    LVDVDDEN = 1U;
    
    LVDVDIF = 0U;     /* clear INTLVI interrupt flag */
    LVDVDMK = 0U;     /* enable INTLVI interrupt */
}

/***********************************************************************************************************************
* Function Name: R_LVD_InterruptMode_Start
* Description  : This function enables the voltage detector interrupt.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_LVDVDD_Stop(void)
{
    LVDVDMK = 1U;     /* disable INTLVI interrupt */
    LVDVDIF = 0U;     /* clear INTLVI interrupt flag */
        
    LVDVDDEN = 0U;
}

/***********************************************************************************************************************
* Function Name: R_LVD_Create
* Description  : This function initializes the voltage detector.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_LVDVBAT_Create(void)
{
    LVDVBMK = 1U;     /* disable INTLVI interrupt */
    LVDVBIF = 0U;     /* clear INTLVI interrupt flag */
    
    LVDVBATEN = 0U;
    LVDVBAT |= _03_VLVDVBAT_LEVEL3; /* Rising Edge 2.43 V, Falling Edge 2.37 V */ 
    
    /* Set INTLVI low priority */
    LVDVBPR1 = 1U;
    LVDVBPR0 = 1U;
}

/***********************************************************************************************************************
* Function Name: R_LVD_InterruptMode_Start
* Description  : This function enables the voltage detector interrupt.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_LVDVBAT_Start(void)
{
    LVDVBATEN = 1U;
    
    LVDVBIF = 0U;     /* clear INTLVI interrupt flag */
    LVDVBMK = 0U;     /* enable INTLVI interrupt */
}
/***********************************************************************************************************************
* Function Name: R_LVD_InterruptMode_Start
* Description  : This function enables the voltage detector interrupt.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_LVDVBAT_Stop(void)
{
    LVDVBMK = 1U;     /* disable INTLVI interrupt */
    LVDVBIF = 0U;     /* clear INTLVI interrupt flag */
    
    LVDVBATEN = 0U;
}

/***********************************************************************************************************************
* Function Name: R_LVD_Create
* Description  : This function initializes the voltage detector.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_LVDVRTC_Create(void)
{
    LVDVRMK = 1U;     /* disable INTLVI interrupt */
    LVDVRIF = 0U;     /* clear INTLVI interrupt flag */
    
    LVDVRTCEN = 0U;
    LVDVRTC |= _00_VLVDVRTC_LEVEL0 ;  /* Rising Edge 2.63 V, Falling Edge 2.57V */
   
    
    /* Set INTLVI low priority */
    LVDVRPR1 = 1U;
    LVDVRPR0 = 1U;    
}

/***********************************************************************************************************************
* Function Name: R_LVD_InterruptMode_Start
* Description  : This function enables the voltage detector interrupt.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_LVDVRTC_Start(void)
{
    LVDVRTCEN = 1U;
    
    LVDVRIF = 0U;     /* clear INTLVI interrupt flag */
    LVDVRMK = 0U;     /* enable INTLVI interrupt */
}

/***********************************************************************************************************************
* Function Name: R_LVD_InterruptMode_Start
* Description  : This function enables the voltage detector interrupt.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_LVDVRTC_Stop(void)
{
    LVDVRMK = 1U;     /* disable INTLVI interrupt */
    LVDVRIF = 0U;     /* clear INTLVI interrupt flag */
    
    LVDVRTCEN = 0U;
}

/***********************************************************************************************************************
* Function Name: R_LVD_Create
* Description  : This function initializes the voltage detector.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_LVDEXLVD_Create(void)
{
    LVDEXMK = 1U;     /* disable INTLVI interrupt */
    LVDEXIF = 0U;     /* clear INTLVI interrupt flag */
    
    LVDEXLVDEN = 0U;
    
    /* Set INTLVI low priority */
    LVDEXPR1 = 1U;
    LVDEXPR0 = 1U;
}

/***********************************************************************************************************************
* Function Name: R_LVD_InterruptMode_Start
* Description  : This function enables the voltage detector interrupt.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_LVDEXLVD_Start(void)
{
    LVDEXLVDEN = 1U;
    
    LVDEXIF = 0U;     /* clear INTLVI interrupt flag */
    LVDEXMK = 0U;     /* enable INTLVI interrupt */
}


/***********************************************************************************************************************
* Function Name: R_LVD_InterruptMode_Start
* Description  : This function enables the voltage detector interrupt.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_LVDEXLVD_Stop(void)
{
    LVDEXMK = 1U;     /* disable INTLVI interrupt */
    LVDEXIF = 0U;     /* clear INTLVI interrupt flag */
    
    LVDEXLVDEN = 0U;
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
