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
* File Name    : r_cg_dtc.c
* Version      : 
* Device(s)    : RL78/I1C
* Tool-Chain   : CCRL
* Description  : This file implements device driver for DTC module.
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
#include "r_cg_dtc.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* 
 * Memory section changed by linker options 
*/
#ifdef __CCRL__
/* TAU01 Data */
#pragma section const T01_Trigger
const uint8_t __near g_dtc_tau01_trigger = 0x02;

/* ADC Data */
#pragma section bss ADC_Data
volatile uint16_t __near g_adc_data[8];
/* End user code. Do not edit comment generated here */

/* DTC Vector and control data */
#pragma section bss DTC_Vect
volatile uint8_t __near dtc_vectortable[40U];

#pragma section bss DTCD0
volatile st_dtc_data __near dtc_controldata_0;

#pragma section bss DTCD1
volatile st_dtc_data __near dtc_controldata_1;

#elif (defined __ICCRL78__)

#pragma constseg="T01_Trigger"
const uint8_t g_dtc_tau01_trigger = 0x02;

/* ADC Data */
#pragma default_variable_attributes=@"ADC_Data"
volatile uint16_t g_adc_data[8];

/* DTC Vector and control data */
#pragma default_variable_attributes=__no_init @"DTC_Vect"
volatile uint8_t dtc_vectortable[40U];

#pragma default_variable_attributes=__no_init @"DTCD0"
volatile st_dtc_data dtc_controldata_0;

#pragma default_variable_attributes=__no_init @"DTCD1"
volatile st_dtc_data dtc_controldata_1;

#pragma default_variable_attributes =
#else
const uint8_t __near g_dtc_tau01_trigger = 0x02;
volatile uint16_t __near g_adc_data[8];
volatile uint8_t dtc_vectortable[40U];
volatile st_dtc_data dtc_controldata_0;
volatile st_dtc_data dtc_controldata_1;

#endif 

/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_DTC_Create
* Description  : This function initializes the DTC module.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_DTC_Create(void)
{
    /* Enable input clock supply */
    DTCEN = 1U;
    /* Disable all DTC channels operation */
    DTCEN0 = 0x00U;
    DTCEN1 = 0x00U;
    DTCEN2 = 0x00U;
    DTCEN3 = 0x00U;
    DTCEN4 = 0x00U;
    /* Set base address */
    DTCBAR = 0xE7U; //clkwek

    /* Set DTCD0 */
    dtc_vectortable[10U] = 0x40U;
    dtc_controldata_0.dtccr = _01_DTC_TRANSFER_MODE_REPEAT | _00_DTC_DATA_SIZE_8BITS | _02_DTC_REPEAT_AREA_SOURCE | 
                              _00_DTC_DEST_ADDR_FIXED | _20_DTC_REPEAT_INT_ENABLE | _00_DTC_CHAIN_TRANSFER_DISABLE;
    dtc_controldata_0.dtbls = _01_DTCD0_TRANSFER_BLOCKSIZE;
    dtc_controldata_0.dtcct = _01_DTCD0_TRANSFER_BYTE;
    dtc_controldata_0.dtrld = _01_DTCD0_TRANSFER_BYTE;

    dtc_controldata_0.dtsar = (uint16_t)&g_dtc_tau01_trigger;
    dtc_controldata_0.dtdar = (uint16_t)&TS0L;

    /* Set DTCD1 */
    dtc_vectortable[11U] = 0x48U;
    dtc_controldata_1.dtccr = _01_DTC_TRANSFER_MODE_REPEAT | _40_DTC_DATA_SIZE_16BITS | _00_DTC_SOURCE_ADDR_FIXED | 
                              _00_DTC_REPEAT_AREA_DEST | _00_DTC_REPEAT_INT_DISABLE |  _00_DTC_CHAIN_TRANSFER_DISABLE;
    dtc_controldata_1.dtbls = _01_DTCD1_TRANSFER_BLOCKSIZE;
    dtc_controldata_1.dtcct = _08_DTCD1_TRANSFER_BYTE;
    dtc_controldata_1.dtrld = _08_DTCD1_TRANSFER_BYTE;

    dtc_controldata_1.dtsar = _FF1E_DTCD0_SRC_ADDRESS;
    dtc_controldata_1.dtdar = (uint16_t)&g_adc_data;
}

/***********************************************************************************************************************
* Function Name: R_DTCD0_Start
* Description  : This function enables DTCD0 module operation.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_DTCD0_Start(void)
{
    BIT_SELECT( DTCEN1, 5 ) = 1;
}

/***********************************************************************************************************************
* Function Name: R_DTCD0_Stop
* Description  : This function disables DTCD0 module operation.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_DTCD0_Stop(void)
{
    BIT_SELECT( DTCEN1, 5 ) = 0;
    dtc_controldata_0.dtccr = _01_DTC_TRANSFER_MODE_REPEAT | _00_DTC_DATA_SIZE_8BITS | _02_DTC_REPEAT_AREA_SOURCE | 
                              _00_DTC_DEST_ADDR_FIXED | _20_DTC_REPEAT_INT_ENABLE | _00_DTC_CHAIN_TRANSFER_DISABLE;
    dtc_controldata_0.dtbls = _01_DTCD0_TRANSFER_BLOCKSIZE;
    dtc_controldata_0.dtcct = _01_DTCD0_TRANSFER_BYTE;
    dtc_controldata_0.dtrld = _01_DTCD0_TRANSFER_BYTE;

    dtc_controldata_0.dtsar = (uint16_t)&g_dtc_tau01_trigger;
    dtc_controldata_0.dtdar = (uint16_t)&TS0L;
}


/***********************************************************************************************************************
* Function Name: R_DTCD1_Start
* Description  : This function enables DTCD1 module operation.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_DTCD1_Start(void)
{
    BIT_SELECT( DTCEN1, 4 ) = 1;
}

/***********************************************************************************************************************
* Function Name: R_DTCD1_Stop
* Description  : This function disables DTCD1 module operation.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_DTCD1_Stop(void)
{
    BIT_SELECT( DTCEN1, 4 ) = 0;
    dtc_controldata_1.dtccr = _01_DTC_TRANSFER_MODE_REPEAT | _40_DTC_DATA_SIZE_16BITS | _00_DTC_SOURCE_ADDR_FIXED | 
                              _00_DTC_REPEAT_AREA_DEST | _00_DTC_REPEAT_INT_DISABLE |  _00_DTC_CHAIN_TRANSFER_DISABLE;
    dtc_controldata_1.dtbls = _01_DTCD1_TRANSFER_BLOCKSIZE;
    dtc_controldata_1.dtcct = _08_DTCD1_TRANSFER_BYTE;
    dtc_controldata_1.dtrld = _08_DTCD1_TRANSFER_BYTE;

    dtc_controldata_1.dtsar = _FF1E_DTCD0_SRC_ADDRESS;
    dtc_controldata_1.dtdar = (uint16_t)&g_adc_data;
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
