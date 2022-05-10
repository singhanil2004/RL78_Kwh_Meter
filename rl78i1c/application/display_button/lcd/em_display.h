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
* File Name    : em_display.h
* Version      : 1.00
* Device(s)    : RL78/I1C
* Tool-Chain   : CA78K0R
* H/W Platform : RL78/I1C Energy Meter Platform
* Description  : EM Display Application Layer APIs
***********************************************************************************************************************/

#ifndef _EM_DISPLAY_H
#define _EM_DISPLAY_H

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/

/* Wrapper layer */
#include "r_drv_lcd_ext.h"

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/
typedef struct tagTimeDataInfo
{
    uint8_t sec;
    uint8_t min;
    uint8_t hour;
} TIME_DATA_INFO;

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#define LCD_FIRST_DISPLAY_POS   1       /* default is paramerter 0 */
#define LCD_DELAY_TIME          60      /* Create delay time 5s */


#define  SELF_TEST_SCREEN       0
#define  VERSION_SCREEN         1
#define  MENU_SCREEN            2
#define  TEST_START_SCREEN      3
#define  TEST_RESULT_SCREEN     4
#define  SETUP_SCREEN           5
#define  DEBUG_SCREEN           6
#define  PASSWORD_SCREEN        7

#define  PRODUCT_VERSION_NO     100
/***********************************************************************************************************************
Variable Externs
***********************************************************************************************************************/
extern volatile uint8_t g_KeyScan_Flg;
extern volatile uint8_t g_Display_Refresh_Flg;
/***********************************************************************************************************************
Functions Prototypes
***********************************************************************************************************************/
/* Prototype APIs which are used to display EM information */
void EM_DisplaySequence(void);
void EM_RTC_DisplayInterruptCallback(void);
void EM_Timer_DisplayInterruptCallback(void);

/* APIs which are used to control LCD */
void LCD_SwitchAutoRoll(void);
void LCD_SwitchProgMode(void);
void LCD_ChangeNext(void);
void EM_LCD_DisplayPOR(void);

/* APIs which are used to display some extra information on LCD */
uint8_t LCD_DisplayTime(TIME_DATA_INFO time_info, uint8_t is_used_BCD);

/* APIs which are used to display number on LCD */
uint8_t LCD_DisplayIntWithPos(long lNum, int8_t position);
uint8_t LCD_DisplayFloat(float32_t fnum);
uint8_t LCD_DisplayFloat3Digit(float32_t fnum);
void EM_DisplayCalibStatus(uint32_t CalStatus);

#endif /* _EM_DISPLAY_H */
