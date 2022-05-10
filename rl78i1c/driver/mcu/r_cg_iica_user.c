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
* File Name    : r_cg_iica_user.c
* Version      : 
* Device(s)    : RL78/I1C
* Tool-Chain   : CCRL
* Description  : This file implements device driver for IICA module.
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
#include "r_cg_iica.h"
/* Start user code for include. Do not edit comment generated here */

/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
extern volatile uint8_t   g_iica0_master_status_flag;  /* iica0 master flag */ 
extern volatile uint8_t   g_iica0_slave_status_flag;   /* iica0 slave flag */
extern volatile uint8_t * gp_iica0_rx_address;         /* iica0 receive buffer address */
extern volatile uint16_t  g_iica0_rx_cnt;              /* iica0 receive data length */
extern volatile uint16_t  g_iica0_rx_len;              /* iica0 receive data count */
extern volatile uint8_t * gp_iica0_tx_address;         /* iica0 send buffer address */
extern volatile uint16_t  g_iica0_tx_cnt;              /* iica0 send data count */
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: r_iica0_interrupt
* Description  : None
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
#ifdef __CCRL__
#pragma interrupt r_iica0_interrupt(vect=INTIICA0)
#endif /* __CCRL__ */
#ifdef __ICCRL78__
#pragma vector = INTIICA0_vect
#endif /* __ICCRL78__ */
__INTERRUPT static void r_iica0_interrupt(void)
{
    if ((IICS0 & _80_IICA_STATUS_MASTER) == 0x80U)
    {
        iica0_masterhandler();
    }
}
/***********************************************************************************************************************
* Function Name: iica0_masterhandler
* Description  : This function is IICA0 master handler.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void iica0_masterhandler(void)
{
    /* Control for communication */
    if ((0U == IICBSY0) && (g_iica0_tx_cnt != 0U))
    {
        r_iica0_callback_master_error(MD_SPT);
    }
    /* Control for sended address */
    else
    {
        if ((g_iica0_master_status_flag & _80_IICA_ADDRESS_COMPLETE) == 0U)
        {
            if (1U == ACKD0)
            {
                g_iica0_master_status_flag |= _80_IICA_ADDRESS_COMPLETE;

                if (1U == TRC0)
                {
                    WTIM0 = 1U;

                    if (g_iica0_tx_cnt > 0U)
                    {
                        IICA0 = *gp_iica0_tx_address;
                        gp_iica0_tx_address++;
                        g_iica0_tx_cnt--;
                    }
                    else
                    {
                        r_iica0_callback_master_sendend();
                    }
                }
                else
                {
                    ACKE0 = 1U;
                    WTIM0 = 0U;
                    WREL0 = 1U;
                }
            }
            else
            {
                r_iica0_callback_master_error(MD_NACK);
            }
        }
        else
        {
            /* Master send control */
            if (1U == TRC0)
            {
                if ((0U == ACKD0) && (g_iica0_tx_cnt != 0U))
                {
                    r_iica0_callback_master_error(MD_NACK);
                }
                else
                {
                    if (g_iica0_tx_cnt > 0U)
                    {
                        IICA0 = *gp_iica0_tx_address;
                        gp_iica0_tx_address++;
                        g_iica0_tx_cnt--;
                    }
                    else
                    {
                        r_iica0_callback_master_sendend();
                    }
                }
            }
            /* Master receive control */
            else
            {
                if (g_iica0_rx_cnt < g_iica0_rx_len)
                {
                    *gp_iica0_rx_address = IICA0;
                    gp_iica0_rx_address++;
                    g_iica0_rx_cnt++;

                    if (g_iica0_rx_cnt == g_iica0_rx_len)
                    {
                        ACKE0 = 0U;
                        WREL0 = 1U;
                        WTIM0 = 1U;
                    }
                    else
                    {
                        WREL0 = 1U;
                    }
                }
                else
                {
                    r_iica0_callback_master_receiveend();
                }
            }
        }
    }
}
/***********************************************************************************************************************
* Function Name: r_iica0_callback_master_error
* Description  : This function is a callback function when IICA0 master error occurs.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_iica0_callback_master_error(MD_STATUS flag)
{
    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}
/***********************************************************************************************************************
* Function Name: r_iica0_callback_master_receiveend
* Description  : This function is a callback function when IICA0 finishes master reception.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_iica0_callback_master_receiveend(void)
{
    SPT0 = 1U;
    /* Start user code. Do not edit comment generated here */
    
    /* End user code. Do not edit comment generated here */
}
/***********************************************************************************************************************
* Function Name: r_iica0_callback_master_sendend
* Description  : This function is a callback function when IICA0 finishes master transmission.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_iica0_callback_master_sendend(void)
{
    SPT0 = 1U;
    /* Start user code. Do not edit comment generated here */
    
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_IICA0_SetTransClockAt1M5Hz
* Description  : Set transfert clock = 100000 for IIC at fCLK = 1.5MHz
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_IICA0_SetTransClockAt1M5Hz(void)
{
    SMC0 = 0U;                                      /* operates in standard mode */
    IICWL0 = _08_IICA0_IICWL_VALUE;
    IICWH0 = _08_IICA0_IICWH_VALUE;
    DFC0 = 0U; /* digital filter off */
    IICCTL01 &= (uint8_t)~_01_IICA_fCLK_HALF;
}

/***********************************************************************************************************************
* Function Name: R_IICA0_SetTransClockAt6MHz
* Description  : Set transfert clock = 400000 for IIC at fCLK = 6MHz
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_IICA0_SetTransClockAt6MHz(void)
{
    SMC0 = 1U;                                      /* operates in fast mode */
    IICWL0 = _08_IICA0_IICWL_VALUE;
    IICWH0 = _08_IICA0_IICWH_VALUE;
    DFC0 = 1U; /* digital filter off */
    IICCTL01 &= (uint8_t)~_01_IICA_fCLK_HALF;
}

/***********************************************************************************************************************
* Function Name: R_IICA0_SetTransClockAt12MHz
* Description  : Set transfert clock = 400000 for IIC at fCLK = 12MHz
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_IICA0_SetTransClockAt12MHz(void)
{
    SMC0 = 1U;                                      /* operates in fast mode */
    IICWL0 = _10_IICA0_IICWL_VALUE;
    IICWH0 = _0F_IICA0_IICWH_VALUE;
    DFC0 = 1U; /* digital filter off */
    IICCTL01 &= (uint8_t)~_01_IICA_fCLK_HALF;
}

/***********************************************************************************************************************
* Function Name: R_IICA0_SetTransClockAt24MHz
* Description  : Set transfert clock = 400000 for IIC at fCLK = 24MHz
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_IICA0_SetTransClockAt24MHz(void)
{
    /* Change from 100kHz to 1MHz operation also */
    SMC0 = 1U;
    IICWL0 = _07_IICA0_IICWL_VALUE;
    IICWH0 = _06_IICA0_IICWH_VALUE;
    DFC0 = 0U; /* digital filter off */
    IICCTL01 |= _01_IICA_fCLK_HALF;
}
/* End user code. Do not edit comment generated here */
