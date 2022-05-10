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
* File Name    : r_cg_lcd.c
* Version      : 
* Device(s)    : RL78/I1C
* Tool-Chain   : CCRL
* Description  : This file implements device driver for LCD module.
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
#include "r_cg_lcd.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_LCD_Create
* Description  : This function initializes the LCD module.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_LCD_Create(void)
{
    volatile uint32_t wt_count;
    
    OSMC |= 0x80;  /* Supply lock to LCD */
    LCDON = 0U;    /* disable LCD clock operation */
    LCDM1 |= _00_LCD_VOLTAGE_HIGH;
    LCDM0 = _00_LCD_DISPLAY_WAVEFORM_A | _0D_LCD_DISPLAY_MODE1;
    LCDM0 |= _40_LCD_VOLTAGE_MODE_INTERNAL;
    /* Set CAPL and CAPH pins */
    ISCLCD &= (uint8_t)~_01_LCD_CAPLH_BUFFER_VALID;
    /* Set VL3 */
    ISCLCD |= _02_LCD_VL3_BUFFER_VALID;
    
    LCDM1 |= _00_LCD_DISPLAY_PATTERN_A;
    LCDC0 = _00_LCD_SOURCE_CLOCK_FSUB | _05_LCD_CLOCK_FLCD_64;
    VLCD = _06_LCD_BOOST_VOLTAGE_110V;

    /* Change the waiting time according to the system */
    for (wt_count = 0U; wt_count <= LCD_REFVOLTAGE_WAITTIME; wt_count++)
    {
        NOP();
    }
}

/***********************************************************************************************************************
* Function Name: R_LCD_Start
* Description  : This function enables the LCD display.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_LCD_Start(void)
{
    LCDON = 1U;
}

/***********************************************************************************************************************
* Function Name: R_LCD_Stop
* Description  : This function disables the LCD display.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_LCD_Stop(void)
{
    LCDON = 0U;
}

/***********************************************************************************************************************
* Function Name: R_LCD_Set_VoltageOn
* Description  : This function enables voltage boost circuit or capacitor split circuit.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_LCD_Set_VoltageOn(void)
{
    volatile uint32_t wt_count;

    LCDM0 |= _40_LCD_VOLTAGE_MODE_INTERNAL;
    
    VLCON = 1U;     /* enables voltage boost and capacitor split operation */

    /* Change the waiting time according to the system */
    for(wt_count = 0U; wt_count <= LCD_VOLTAGE_WAITTIME; wt_count++)
    {
        NOP();
    }
    
    SCOC = 1U;      /* select common and segment pins output */
}

/***********************************************************************************************************************
* Function Name: R_LCD_Set_VoltageOff
* Description  : This function disables voltage boost circuit or capacitor split circuit.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_LCD_Set_VoltageOff(void)
{
    SCOC = 0U;
    VLCON = 0U;
    LCDM0 &= (uint8_t)~_C0_LCD_VOLTAGE_MODE_INITIALVALUE;
}

/* Start user code for adding. Do not edit comment generated here */
/***********************************************************************************************************************
* Function Name: R_LCD_PowerDown
* Description  : This function power down LCD module
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_LCD_PowerOff(void)
{
//    R_LCD_Stop();
    LCDON = 0U;     /* display off (all segment outputs are deselected) */
    
//    R_LCD_Set_VoltageOff();
    SCOC = 0U;      /* output ground level to segment/common pin */
    VLCON = 0U;     /* stops voltage boost and capacitor split operation */
    LCDM0 &= (uint8_t)~(_C0_LCD_VOLTAGE_MODE_INITIALVALUE);
}

/***********************************************************************************************************************
* Function Name: R_LCD_PowerOn
* Description  : This function power on LCD module
* Arguments    : uint8_t high_contrast: control display contrast
*              : Note: Set to high contrast will consume more current
* Return Value : None
***********************************************************************************************************************/
void R_LCD_PowerOn(uint8_t high_contrast)
{
    volatile uint32_t wt_count;
    
    /* LCD Start */
    LCDON = 1U;     /* display on */

    /* LCD Set voltage on */
    LCDM0 &= (uint8_t)~(_C0_LCD_VOLTAGE_MODE_INITIALVALUE);
    LCDM0 |= _40_LCD_VOLTAGE_MODE_INTERNAL;
    if (high_contrast)
    {
        LCDC0 = _05_LCD_CLOCK_FLCD_64;
        VLCD = _08_LCD_BOOST_VOLTAGE_120V;
    }
    else
    {
        LCDC0 = _06_LCD_CLOCK_FLCD_128;
        VLCD = _06_LCD_BOOST_VOLTAGE_110V;
    }
    
    VLCON = 1U;     /* enables voltage boost and capacitor split operation */

    /* Change the waiting time according to the system */
    for(wt_count = 0U; wt_count <= LCD_VOLTAGE_WAITTIME; wt_count++)
    {
        NOP();
    }
    
    SCOC = 1U;      /* select common and segment pins output */
}

/* End user code. Do not edit comment generated here */
