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
* File Name    : r_cg_elc.c
* Version      : 
* Device(s)    : RL78/I1C
* Tool-Chain   : CCRL
* Description  : This file implements device driver for ELC module.
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
#include "r_cg_elc.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_ELC_Create
* Description  : This function initializes the ELC module.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_ELC_Create(void)
{
    ELSELR00 = _00_ELC_EVENT_LINK_OFF;
    ELSELR01 = _00_ELC_EVENT_LINK_OFF;
    ELSELR02 = _00_ELC_EVENT_LINK_OFF;
    ELSELR03 = _00_ELC_EVENT_LINK_OFF;
    ELSELR04 = _00_ELC_EVENT_LINK_OFF;
    ELSELR05 = _00_ELC_EVENT_LINK_OFF;
    ELSELR06 = _00_ELC_EVENT_LINK_OFF;
    ELSELR07 = _00_ELC_EVENT_LINK_OFF;
    ELSELR08 = _00_ELC_EVENT_LINK_OFF;
    ELSELR09 = _00_ELC_EVENT_LINK_OFF;
    ELSELR10 = _00_ELC_EVENT_LINK_OFF;
    ELSELR11 = _00_ELC_EVENT_LINK_OFF;
    ELSELR12 = _00_ELC_EVENT_LINK_OFF;
    ELSELR13 = _00_ELC_EVENT_LINK_OFF;
    ELSELR14 = _00_ELC_EVENT_LINK_OFF;
    ELSELR15 = _00_ELC_EVENT_LINK_OFF;
    ELSELR16 = _00_ELC_EVENT_LINK_OFF;
    ELSELR17 = _00_ELC_EVENT_LINK_OFF;
    ELSELR18 = _00_ELC_EVENT_LINK_OFF;
    ELSELR19 = _00_ELC_EVENT_LINK_OFF;
    ELSELR20 = _00_ELC_EVENT_LINK_OFF;
    ELSELR21 = _00_ELC_EVENT_LINK_OFF;
}

/***********************************************************************************************************************
* Function Name: R_ELC_Stop
* Description  : This function stops the ELC event resources.
* Arguments    : src -
*                    event resources to be stopped (ELSELRn)
* Return Value : None
***********************************************************************************************************************/
void R_ELC_Stop(uint32_t src)
{
    volatile uint32_t   w_count;
    uint8_t           * sfr_addr;

    sfr_addr = (uint8_t *)&ELSELR00;
    
    for (w_count = 0U; w_count < ELC_TRIGGER_SRC_COUNT; w_count++) 
    {
        if (((src >> w_count) & 0x1U) == 0x1U)
        {
            *sfr_addr = _00_ELC_EVENT_LINK_OFF;
        }
        
        sfr_addr++;
    }
}

/* Start user code for adding. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_ELC_Set
* Description  : This function set the ELC event link.
* Arguments    : src -
*                    event resources to be stoped (ELSELRn)
*              : dest - 
*                    event destination
* Return Value : None
***********************************************************************************************************************/
void R_ELC_Set(elc_src_t src, uint8_t dest)
{
    uint8_t * sfr_addr = (uint8_t *)(&ELSELR00 + src);
    *sfr_addr = dest;
}

/***********************************************************************************************************************
* Function Name: R_ELC_Clear
* Description  : This function clear the ELC event link.
* Arguments    : src -
*                    event resources to be stoped (ELSELRn)
* Return Value : None
***********************************************************************************************************************/
void R_ELC_Clear(elc_src_t src)
{
    uint8_t  *sfr_addr = (uint8_t *)(&ELSELR00 + src);
    *sfr_addr = _00_ELC_EVENT_LINK_OFF;
}

/* End user code. Do not edit comment generated here */
