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
* File Name    : r_lcd_config.h
* Version      : 1.00
* Device(s)    : RL78/I1C
* Tool-Chain   : CA78K0R
* H/W Platform : RL78/I1C Energy Meter Platform
* Description  : Declare const array for LCD configuration
***********************************************************************************************************************/

#ifndef _R_LCD_CONFIG_H
#define _R_LCD_CONFIG_H

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include "typedef.h"

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/
/*
 * LCD information definition
 * The data area which need to be changed when changing LCD
 */
#define     LCD_NUM_DIGIT_CHAR      22      /* number of displayable char for a digit (0..9, A..F, -) */
#define     LCD_NUM_DIGIT           9       /* number of digits in LCD panel */
#define     LCD_NUM_SIGN            33      /* number of sign in LCD panel */

#define     LCD_FIRST_POS_DIGIT     1       /* Specify the first position of digit on LCD screen */
#define     LCD_LAST_POS_DIGIT      7       /* Specify the last position of digit on LCD screen */

/* Some special characters can be displayed on LCD screen */
#define     LCD_MINUS_SIGN          16      /* To display minus on LCD digit */
#define     LCD_CLEAR_DIGIT         20      /* Value which is used to clear digit on LCD */
#define     LCD_CHAR_A              10      /* Display character A on LCD */
#define     LCD_CHAR_B              11      /* Display character b on LCD */
#define     LCD_CHAR_C              12      /* Display character C on LCD */
#define     LCD_CHAR_D              13      /* Display character d on LCD */
#define     LCD_CHAR_E              14      /* Display character E on LCD */
#define     LCD_CHAR_F              15      /* Display character F on LCD */
#define     LCD_CHAR_P              17      /* Display character P on LCD */
#define     LCD_CHAR_L              18      /* Display character L on LCD */
#define     LCD_CHAR_N              19      /* Display character N on LCD */
#define     LCD_CHAR_U              21      /* Display character U on LCD */

/* LCD number type */
typedef enum tagLCDNumType
{
    NUM_TYPE_A = 0,         /* Bit No.:  7 6 5 4 3 2 1 0
                             * Seg No.:  - C B A D E G F
                             */
} LCD_NUM_TYPE;

/* LCD special sign enummeration */
typedef enum tagLCDEMSpecSign
{
    S_OK = 0,
    S_DATE  ,
    S_TIME  ,
    S_EARTH ,
    S_REV   ,
    S_MN    ,
    S_MAG   ,
    S_BILL  ,
    S_PF    ,
    S_T10   ,
    S_T11   ,
    S_T12   ,
    S_T13   ,
    S_R     ,
    S_Y     ,
    S_B     ,
    S_T8    ,
    S_T7    ,
    S_T6    ,
    S_T5    ,
    S_T4    ,
    S_T3    ,
    S_T2    ,
    S_T1    ,
    S_D6    ,
    S_D5    ,
    S_D4    ,
    S_D3    ,
    S_D2    ,
    S_D1    ,
    S_T9    ,
    S_TOD   ,
    S_MD    ,
} LCD_EM_SPEC_SIGN;

/* LCD number and sign mapping on data RAM */
typedef struct tagLCDNumMap
{
    uint8_t         pos;        /* number no.   */
    uint32_t        addr;       /* ram address  */
    LCD_NUM_TYPE    type;       /* num map type in ram of this number */
} LCD_NUM_MAP;

typedef struct tagLCDNumTypeValue
{
    uint8_t     typeA;
} LCD_NUM_TYPE_VALUE;

typedef struct tagLCDSignMap
{
    LCD_EM_SPEC_SIGN    sign;       /* special sign */
    uint32_t            addr;       /* ram address  */
    uint8_t             pos;        /* bit number of this sign in pwRAM */
} LCD_SIGN_MAP;

/* Information for displaying data */
typedef struct tagLCDNumInfo
{
    uint32_t    addr;
    uint8_t     value;
} LCD_NUM_INFO;

typedef struct tagLCDSignInfo
{
    uint32_t    addr;
    uint8_t     pos;
} LCD_SIGN_INFO;

typedef struct tagLCDDecInfo
{
    uint8_t pos;
    LCD_EM_SPEC_SIGN sign;
} LCD_DECIMAL_INFO;

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Variable Externs
***********************************************************************************************************************/
extern const LCD_NUM_MAP        g_LCD_NumMap[];
extern const LCD_NUM_TYPE_VALUE g_LCD_NumType[];
extern const LCD_SIGN_MAP       g_LCD_SignMap[];
extern const LCD_DECIMAL_INFO   g_DecInfo;
extern const LCD_DECIMAL_INFO   g_DecInfo1;

/***********************************************************************************************************************
Functions Prototypes
***********************************************************************************************************************/

#endif /* _R_LCD_CONFIG_H */
