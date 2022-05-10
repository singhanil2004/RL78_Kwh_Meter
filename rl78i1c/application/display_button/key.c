/******************************************************************************
* DISCLAIMER

* This software is supplied by Renesas Electronics Corporation and is only
* intended for use with Renesas products. No other uses are authorized.

* This software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.

* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES
* REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY,
* INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
* PARTICULAR PURPOSE AND NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY
* DISCLAIMED.

* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES
* FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS
* AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.

* Renesas reserves the right, without notice, to make changes to this
* software and to discontinue the availability of this software.
* By using this software, you agree to the additional terms and
* conditions found by accessing the following link:
* http://www.renesas.com/disclaimer
******************************************************************************/
/* Copyright (C) 2011 Renesas Electronics Corporation. All rights reserved.  */
/******************************************************************************
* File Name    : key.c
* Version      : 1.00
* Device(s)    : RL78/I1C
* Tool-Chain   : CubeSuite Version 1.5d
* H/W Platform : RL78/I1C Energy Meter Platform
* Description  : KEY processing source File
******************************************************************************
* History : DD.MM.YYYY Version Description
******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
/* Driver */
#include "r_cg_macrodriver.h"   /* CG Macro Driver */

/* Application */
#include "key.h"            /* KEY Interface Header File */
#include "em_display.h"     /* LCD Display Header File */
#include "inst_read.h"     /* user read Header File */
/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Macro definitions
******************************************************************************/

/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/
extern volatile uint8_t g_KeyScan_Flg;
/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/

/******************************************************************************
Private global variables and functions
******************************************************************************/
KEY_FLAG g_key_flag;

/******************************************************************************
* Function Name    : void KEY_PollingProcessing(void)
* Description      : KEY Polling Processing
* Arguments        : None
* Functions Called : None
* Return Value     : None
******************************************************************************/
void KEY_PollingProcessing(void)
{
    /* Detection on port here */
    static int8_t key_1_last = (!KEY_PRESSED);
    static int8_t key_2_last = (!KEY_PRESSED);
    static int8_t key_3_last = (!KEY_PRESSED);
    static uint8_t key_press_cnt = 0;
    //    static int8_t key_4_last = (!KEY_PRESSED);
    //    static int8_t key_5_last = (!KEY_PRESSED);
    //    static int8_t key_6_last = (!KEY_PRESSED);
    
    if (g_KeyScan_Flg)
    {
        g_KeyScan_Flg = 0;
        
        if (KEY_1 == (!KEY_PRESSED) &&
            key_1_last == KEY_PRESSED)
        {
            if (g_calibmode)
            {
                INST_AutoCalibrate();
            }
            else
            {
                g_key_flag.Bits.is_key_1_pressed = 1;
            }
        }
        
        key_1_last = KEY_1;
        
        if (KEY_2 == (!KEY_PRESSED) &&
            key_2_last == KEY_PRESSED)
        {
            if (g_calibmode)
            {
                INST_Print_Parameters();
            }
            
            g_key_flag.Bits.is_key_2_pressed = 1;
        }
        
        key_2_last = KEY_2;
        
        if (KEY_3 == (!KEY_PRESSED) &&
            key_3_last == KEY_PRESSED)
        {
            g_key_flag.Bits.is_key_3_pressed = 1;
            key_press_cnt = 0 ;
        }
        
        key_3_last = KEY_3;
        
        if (KEY_3 == (!KEY_PRESSED))
        {
            key_press_cnt++;
            
            if (key_press_cnt > 50)
            {
                key_press_cnt = 0;
                LCD_SwitchProgMode();
                g_key_flag.Bits.is_key_3_pressed = 0;
            }
        }
        
        /*
        if (KEY_4 == (!KEY_PRESSED) &&
            key_4_last == KEY_PRESSED)
        {
            g_key_flag.Bits.is_key_4_pressed = 1;
            INST_Buzzer_On(BUZZER_SHORT);
        }
        
        key_4_last = KEY_4;
        
        
        if (KEY_5 == (!KEY_PRESSED) &&
            key_5_last == KEY_PRESSED)
        {
            g_key_flag.Bits.is_key_up_pressed = 1;
        }
        
        key_5_last = KEY_5;
        
        if (KEY_6 == (!KEY_PRESSED) &&
            key_6_last == KEY_PRESSED)
        {
            g_key_flag.Bits.is_key_down_pressed = 1;
        }
        
        key_6_last = KEY_6;
        
        */
    }
}
