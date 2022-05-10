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
* File Name    : r_lcd_config.c
* Version      : 1.00
* Device(s)    : RL78/I1C
* Tool-Chain   : CA78K0R
* H/W Platform : RL78/I1C Energy Meter Platform
* Description  : Declare const array for LCD configuration
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include "r_lcd_config.h"
#include "r_drv_lcd_ext.h"

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Imported global variables and functions (from other files)
***********************************************************************************************************************/

/***********************************************************************************************************************
Exported global variables and functions (to be accessed by other files)
***********************************************************************************************************************/
/*
 * Data mapping:
 * This data must be changed when there is any difference on PIN
 * (COM and SEGMENT) connection setting between LCD and MCU
 */
const LCD_NUM_MAP g_LCD_NumMap[LCD_NUM_DIGIT] =
{
    /* Pos   Address Number Type */
    {1,    LCD_RAM_START_ADDRESS + 18 ,  NUM_TYPE_A},
    {2,    LCD_RAM_START_ADDRESS + 17 ,  NUM_TYPE_A},
    {3,    LCD_RAM_START_ADDRESS + 16 ,  NUM_TYPE_A},
    {4,    LCD_RAM_START_ADDRESS + 11 ,  NUM_TYPE_A},
    {5,    LCD_RAM_START_ADDRESS + 10 ,  NUM_TYPE_A},
    {6,    LCD_RAM_START_ADDRESS + 9,    NUM_TYPE_A},
    {7,    LCD_RAM_START_ADDRESS + 8,    NUM_TYPE_A},
    {8,    LCD_RAM_START_ADDRESS + 7,    NUM_TYPE_A},
    {9,    LCD_RAM_START_ADDRESS + 6,    NUM_TYPE_A},
};

const LCD_NUM_TYPE_VALUE g_LCD_NumType[LCD_NUM_DIGIT_CHAR] =
{
    /*For 8COM,12SEG*/
    {0xEE},//{ 0xD7 }    , /* '0' - 0*/
    {0x48},//{ 0x06 }    , /* '1' - 1*/
    {0xD6},//{ 0xE3 }    , /* '2' - 2*/
    {0xDA},//{ 0xA7 }    , /* '3' - 3*/
    {0x78},//{ 0x36 }    , /* '4' - 4*/
    {0xBA},//{ 0xB5 }    , /* '5' - 5*/
    {0xBE},//{ 0xF5 }    , /* '6' - 6*/
    {0xC8},//{ 0x07 }    , /* '7' - 7*/
    {0xFE},//{ 0xF7 }    , /* '8' - 8*/
    {0xFA},//{ 0xB7 }    , /* '9' - 9*/
    {0xFC},//{ 0x77 }    , /* 'A' - 10*/
    {0x3E},//{ 0xF4 }    , /* 'b' - 11*/
    {0xA6},//{ 0xD1 }    , /* 'C' - 12*/
    {0x5E},//{ 0xE6 }    , /* 'd' - 13*/
    {0xB6},//{ 0xF1 }    , /* 'E' - 14*/
    {0xB4},//{ 0x71 }    , /* 'F' - 15*/
    {0x10},//{ 0x20 }    , /* '-' - 16*/
    {0xF4},//{ 0x73 }    , /* 'P' - 17*/
    {0x26},//{ 0xD0 }    , /* 'L' - 18*/
    {0xEC},//{ 0x57 }    , /* 'N' - 19*/
    {0x01},//{ 0x08 }    , /* na - 20*/
    {0x6E},//{ 0xD6 }    , /* 'U' - 21*/
};

/* Mapping all special sign of LCD to LCDRAM */
const LCD_SIGN_MAP g_LCD_SignMap[LCD_NUM_SIGN] =
{
    /* Number No.   Address   Bit No. */
    {S_OK   , LCD_RAM_START_ADDRESS + 19, 3}, /* 00 */
    {S_DATE , LCD_RAM_START_ADDRESS + 19, 4}, /* 01 */
    {S_TIME , LCD_RAM_START_ADDRESS + 19, 5}, /* 02 */
    {S_EARTH, LCD_RAM_START_ADDRESS + 19, 6}, /* 03 */
    {S_REV  , LCD_RAM_START_ADDRESS + 19, 7}, /* 04 */
    {S_MN   , LCD_RAM_START_ADDRESS + 8, 0}, /* 05 */
    {S_MAG  , LCD_RAM_START_ADDRESS + 7, 0}, /* 06 */
    {S_BILL , LCD_RAM_START_ADDRESS + 6, 0}, /* 07 */
    {S_PF   , LCD_RAM_START_ADDRESS + 5, 0}, /* 08 */
    {S_T10  , LCD_RAM_START_ADDRESS + 5, 7}, /* 09 */
    {S_T11  , LCD_RAM_START_ADDRESS + 5 , 6}, /* 10 */
    {S_T12  , LCD_RAM_START_ADDRESS + 5 , 5}, /* 11 */
    {S_T13  , LCD_RAM_START_ADDRESS + 5 , 4}, /* 12 */
    {S_R    , LCD_RAM_START_ADDRESS + 5 , 3}, /* 13 */
    {S_Y    , LCD_RAM_START_ADDRESS + 5 , 2}, /* 14 */
    {S_B    , LCD_RAM_START_ADDRESS + 5 , 1}, /* 15 */
    {S_T8   , LCD_RAM_START_ADDRESS + 4 , 7}, /* 16 */
    {S_T7   , LCD_RAM_START_ADDRESS + 4 , 6}, /* 17 */
    {S_T6   , LCD_RAM_START_ADDRESS + 4 , 5}, /* 18 */
    {S_T5   , LCD_RAM_START_ADDRESS + 4 , 4}, /* 19 */
    {S_T4   , LCD_RAM_START_ADDRESS + 4 , 3}, /* 20 */
    {S_T3   , LCD_RAM_START_ADDRESS + 4 , 2}, /* 21 */
    {S_T2   , LCD_RAM_START_ADDRESS + 4 , 1}, /* 22 */
    {S_T1   , LCD_RAM_START_ADDRESS + 4 , 0}, /* 23 */
    {S_D6   , LCD_RAM_START_ADDRESS + 9 , 0}, /* 24 */
    {S_D5   , LCD_RAM_START_ADDRESS + 10, 0}, /* 25 */
    {S_D4   , LCD_RAM_START_ADDRESS + 18, 0}, /* 26 */
    {S_D3   , LCD_RAM_START_ADDRESS + 11, 0}, /* 27 */
    {S_D2   , LCD_RAM_START_ADDRESS + 16, 0}, /* 28 */
    {S_D1   , LCD_RAM_START_ADDRESS + 17, 0}, /* 29 */
    {S_T9   , LCD_RAM_START_ADDRESS + 19, 0}, /* 30 */
    {S_TOD  , LCD_RAM_START_ADDRESS + 19, 1}, /* 31 */
    {S_MD   , LCD_RAM_START_ADDRESS + 19, 2}, /* 32 */
};

const LCD_DECIMAL_INFO g_DecInfo  = {6, S_D5};
const LCD_DECIMAL_INFO g_DecInfo1 = {5, S_D4};

/***********************************************************************************************************************
Private global variables and functions
***********************************************************************************************************************/
