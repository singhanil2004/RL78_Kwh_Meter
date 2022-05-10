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
* File Name    : em_display.c
* Version      : 1.00
* Device(s)    : RL78/I1C
* Tool-Chain   : CA78K0R
* H/W Platform : RL78/I1C Energy Meter Platform
* Description  : EM Display Application Layer APIs
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include "stdio.h"
/* Driver layer */
#include "r_cg_macrodriver.h"
#include "r_cg_userdefine.h"
#include "typedef.h"            /* GCSE Standard definitions */
#include "r_cg_wdt.h"

/* Application layer */
#include "platform.h"
#include "em_display.h"         /* EM Display Application Layer */
//#include "powermgmt.h"
#include "user_config.h"

/* Display tamper condition */
#include "format.h"
#include "eeprom.h"
#include "em_core.h"
#include "LCD_Segment.h"
#include "inst_read.h"
#include "key.h"
#include "r_calib.h"
#include "config_storage.h"
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
volatile uint8_t g_KeyScan_Flg;
volatile uint8_t g_Display_Refresh_Flg;
/***********************************************************************************************************************
Private global variables and functions
***********************************************************************************************************************/
static void EM_DisplayNormalMode(void);
static void EM_DisplayProgMode(void);
static void EM_DisplaySelfTest(void);
static void EM_LCD_DisplayEBEnergy(void);
static void EM_LCD_DisplayDGEnergy(void);
static void EM_LCD_DisplayActPower(void);
static void EM_LCD_DisplayAppPower(void);
static void EM_LCD_DisplayPF(void);
static void EM_LCD_DisplayRunHour(void);
static void EM_LCD_Password(void);
static void EM_LCD_DisplayCtr(void);
static void EM_LCD_DisplayCtrNormal(void);
static void EM_LCD_DisplayModbusId(void);
static uint32_t abs(int32_t input);
static uint8_t BCD2DEC(uint8_t BCD_number);

static uint16_t     g_em_display_count = LCD_DELAY_TIME;
static uint8_t      g_is_display_changed_next = 1;
static int8_t       g_DispCounter = 0;  /* first position is voltage */
static uint8_t      g_auto_roll = 1;
static uint8_t      g_prog_mode = 0;
static int8_t       g_blink_digit = 0;
static int8_t       s_count = 0;
uint8_t             g_eng_decimal;
static uint16_t     s_password = 0;
typedef void (* __far fp_display_t)(void);
static const uint16_t  digit_weight[5] = {1, 10, 100, 1000, 1};
static const uint16_t  decimal_weight[5] = {1000, 100, 10, 1, 1};
/*
* Display list, scroll over at end
* Append or move the display API inside this list for arrangement
* of LCD display
*/
const fp_display_t fp_display[] =
{
    EM_DisplaySelfTest,
    EM_LCD_DisplayEBEnergy,
#ifdef DUAL_SOURCE
    EM_LCD_DisplayDGEnergy,
#endif
    EM_LCD_DisplayActPower,
    EM_LCD_DisplayAppPower,
    EM_LCD_DisplayPF,
    EM_LCD_DisplayRunHour,
    NULL,
    EM_LCD_DisplayCtrNormal,
    NULL,
};
const fp_display_t fp_prog_display[] =
{
    EM_LCD_Password,
    EM_LCD_DisplayCtr,
    EM_LCD_DisplayModbusId,
    NULL,
};
/***********************************************************************************************************************
* Function Name: void EM_DisplaySequence(void)
* Description  :
* Arguments    :
* Output       :
* Return Value :
***********************************************************************************************************************/
void EM_DisplaySequence(void)
{
    if (g_prog_mode)
    {
        EM_DisplayProgMode();
    }
    else
    {
        EM_DisplayNormalMode();
    }
}
/***********************************************************************************************************************
* Function Name: void EM_DisplayNormalMode(void)
* Description  : This function display Normal Parameter
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
void EM_DisplayNormalMode(void)
{
    if (g_Display_Refresh_Flg)
    {
        g_Display_Refresh_Flg = 0;
        s_count++;
        
        if (s_count < 2)
        {
            return;
        }
        
        s_count = 0;
        INST_READ_RTC_InterruptCallBack();
        
        if (g_auto_roll)
        {
            g_em_display_count++;   /* Counter used for displaying measured value */
            
            if (g_em_display_count > LCD_DELAY_TIME)
            {
                LCD_ChangeNext();
                g_auto_roll = 0;
            }
        }
        else if (g_em_display_count)
        {
            g_em_display_count--;
            
            if (g_em_display_count == 0)
            {
                g_DispCounter = 1;
            }
        }
        
        /* Display the parameter */
        if (fp_display[g_DispCounter] != NULL)
        {
            (fp_display[g_DispCounter])();
        }
        
        R_WDT_Restart();
    }
    
    if (g_key_flag.Byte)
    {
        if (g_key_flag.Bits.is_key_2_pressed == 1)
        {
            if (g_auto_roll == 0)
            {
                LCD_ChangeNext();
                g_em_display_count = LCD_DELAY_TIME;
            }
        }
        else if (g_key_flag.Bits.is_key_3_pressed == 1)
        {
#ifdef DUAL_SOURCE
            g_DispCounter = 7;
#else
            g_DispCounter = 6;
#endif
            
            LCD_ChangeNext();
            g_em_display_count = 3;
        }
        
        g_key_flag.Byte = 0;
    }
    
    R_WDT_Restart();
}
/***********************************************************************************************************************
* Function Name: void EM_DisplayProgMode(void)
* Description  : This function display Normal Parameter
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
void EM_DisplayProgMode(void)
{
    if (g_Display_Refresh_Flg)
    {
        g_Display_Refresh_Flg = 0;
        
        /* Display the parameter */
        if (fp_prog_display[g_DispCounter] != NULL)
        {
            (fp_prog_display[g_DispCounter])();
        }
        
        R_WDT_Restart();
        
        if (g_em_display_count)
        {
            g_em_display_count--;
            
            if (g_em_display_count == 0) // if not exit manual from prog
            {
                g_DispCounter = 1;
                g_prog_mode = 0;
                CONFIG_Restore(CONFIG_ITEM_USER);
            }
        }
    }
    
    if (g_key_flag.Byte)
    {
        g_em_display_count = LCD_DELAY_TIME * 2; // restart time out
        
        if (g_key_flag.Bits.is_key_3_pressed == 1)
        {
            g_key_flag.Bits.is_key_3_pressed = 0;
            
            if (g_DispCounter == 0)
            {
                if (s_password != 1234)
                {
                    s_password = 0;
                    g_DispCounter = -1;
                }
            }
            
            g_DispCounter++;   /* Counter used for displaying measured value */
            
            if (g_DispCounter == 2) // Modbus ID
            {
                if (g_modbusmode == 0)
                {
                    g_DispCounter++; //Skip Modbus ID Display
                }
            }
            
            if (fp_prog_display[g_DispCounter] == NULL)
            {
                g_DispCounter = 1;
            }
            
            if (g_DispCounter < 2)
            {
                g_blink_digit = 3;
            }
            else
            {
                g_blink_digit = 2;
            }
            
            (fp_prog_display[g_DispCounter])();
        }
        
        if (g_key_flag.Bits.is_key_2_pressed == 1)
        {
            g_key_flag.Bits.is_key_2_pressed = 0;
            g_blink_digit--;
            
            if (g_blink_digit < 0)
            {
                if (g_DispCounter < 2)
                {
                    g_blink_digit = 3;
                }
                else
                {
                    g_blink_digit = 2;
                }
            }
        }
        
        g_Display_Refresh_Flg = 1;
    }
}
/***********************************************************************************************************************
* Function Name: void LCD_RTC_InterruptCallback(void)
* Description  : This function is called by RTC to increase the counter which
*              : is used to control the displayed time of variable
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
void EM_RTC_DisplayInterruptCallback(void)
{
    //g_em_display_count++;   /* Counter used for displaying measured value */
}

/******************************************************************************
* Function Name: void EM_DisplaySelfTest(void)
* Description  : Display unit code on LCD screen (now, it is fixed RE0001)
* Arguments    : none
* Output       : none
* Return Value : none
******************************************************************************/
static void EM_DisplaySelfTest(void)
{
    display_array[0] = CODE_u;
    display_array[1] = CODE_E;
    display_array[2] = CODE_r;
    display_array[3] = 1;
    display_array[4] = 0;
    display_array[5] = 0;
    display_dot[0] = FALSE;
    display_dot[1] = FALSE;
    display_dot[2] = FALSE;
    display_dot[3] = TRUE;
    display_dot[4] = FALSE;
    display_dot[5] = FALSE;
}
/******************************************************************************
* Function Name: void EM_DisplaySelfTest(void)
* Description  : Display unit code on LCD screen (now, it is fixed RE0001)
* Arguments    : none
* Output       : none
* Return Value : none
******************************************************************************/
void EM_DisplayCalibStatus(uint32_t CalStatus)
{
    if (CalStatus > 4)
    {
        g_DispCounter = 1;
        display_array[0] = CODE_BLANK;
        display_array[1] = CODE_d;
        display_array[2] = CODE_o;
        display_array[3] = CODE_n;
        display_array[4] = CODE_E;
        display_array[5] = CODE_BLANK;
    }
    else
    {
        display_array[0] = CODE_C;
        display_array[1] = CODE_A;
        display_array[2] = CODE_L;
        display_array[3] = CODE_DASH;
        display_array[4] = CalStatus;
        display_array[5] = CODE_BLANK;
        display_dot[0] = FALSE;
        display_dot[1] = FALSE;
        display_dot[2] = FALSE;
        display_dot[3] = FALSE;
        display_dot[4] = FALSE;
        display_dot[5] = FALSE;
    }
    
}
/******************************************************************************
* Function Name: void EM_LCD_DisplayEBEnergy(void)
* Description  : Display unit code on LCD screen
* Arguments    : none
* Output       : none
* Return Value : none
******************************************************************************/
static void EM_LCD_DisplayEBEnergy(void)
{
    uint32_t data;
    data = (uint32_t)(MainsEnergies.active_imp / decimal_weight[g_eng_decimal]);
	//data = (uint32_t)(g_inst_read_params.vrms[0]*10 / decimal_weight[g_eng_decimal]);
    g_led_flag.Byte = 0;
    
    if (LVDVRTCF == 0)
    {
        g_led_flag.Bits.KWh_DG = 1;
    }
    else
    {
        g_led_flag.Bits.KWh_DG = 0;
    }
    
    g_led_flag.Bits.KWh_EB = 1;
    //g_led_flag.Bits.Unit_K = 1;
    //g_led_flag.Bits.Unit_W = 1;
    display_6Digit(data, g_eng_decimal);
}
/******************************************************************************
* Function Name: void EM_LCD_DisplayDGEnergy(void)
* Description  : Display unit code on LCD screen
* Arguments    : none
* Output       : none
* Return Value : none
******************************************************************************/
static void EM_LCD_DisplayDGEnergy(void)
{
    uint32_t data = (uint32_t)(MainsEnergies.active_exp / decimal_weight[g_eng_decimal]);
    g_led_flag.Byte = 0;
    g_led_flag.Bits.KWh_DG = 1;
    //g_led_flag.Bits.Unit_K = 1;
    //g_led_flag.Bits.Unit_W = 1;
    display_6Digit(data, g_eng_decimal);
}
/******************************************************************************
* Function Name: void EM_LCD_DisplayDGEnergy(void)
* Description  : Display unit code on LCD screen
* Arguments    : none
* Output       : none
* Return Value : none
******************************************************************************/
static void EM_LCD_DisplayActPower(void)
{
    uint32_t data;
    g_led_flag.Byte = 0;
    data = abs(g_inst_read_params.active_power[LINE_TOTAL] / decimal_weight[2]);
    //g_led_flag.Bits.Unit_K = 1;
    g_led_flag.Bits.Unit_W = 1;
    display_6Digit(data, 2);
}
/******************************************************************************
* Function Name: void EM_LCD_DisplayAppPower(void)
* Description  : Display unit code on LCD screen
* Arguments    : none
* Output       : none
* Return Value : none
******************************************************************************/
static void EM_LCD_DisplayAppPower(void)
{
    uint32_t data;
    g_led_flag.Byte = 0;
    data = abs(g_inst_read_params.apparent_power[LINE_TOTAL] / decimal_weight[2]);
    //g_led_flag.Bits.Unit_K = 1;
    g_led_flag.Bits.Unit_VA = 1;
    display_6Digit(data, 2);
}
/******************************************************************************
* Function Name: void EM_LCD_DisplayPF(void)
* Description  : Display unit code on LCD screen
* Arguments    : none
* Output       : none
* Return Value : none
******************************************************************************/
static void EM_LCD_DisplayPF(void)
{
    uint32_t data = abs(g_inst_read_params.power_factor[LINE_TOTAL]);
    g_led_flag.Byte = 0;
    g_led_flag.Bits.Unit_PF = 1;
    display_PF(data, 3);
}
/******************************************************************************
* Function Name: void EM_LCD_DisplayRunHour(void)
* Description  : Display unit code on LCD screen
* Arguments    : none
* Output       : none
* Return Value : none
******************************************************************************/
static void EM_LCD_DisplayRunHour(void)
{
    uint32_t data;;
    data = (MainsEnergies.run_hour / 3600) * 100 + ((MainsEnergies.run_hour / 60) % 60);
    g_led_flag.Byte = 0;
    g_led_flag.Bits.Unit_H = 1;
    display_6Digit(data, 2);
}
/******************************************************************************
* Function Name: void EM_LCD_DisplayCtr(void)
* Description  : Display unit code on LCD screen
* Arguments    : none
* Output       : none
* Return Value : none
******************************************************************************/
static void EM_LCD_DisplayCtr(void)
{
    uint32_t  data;
    
    if (g_key_flag.Bits.is_key_1_pressed == 1)
    {
        g_key_flag.Bits.is_key_1_pressed = 0;
        data = (user_config.CTR / digit_weight[g_blink_digit]);
        user_config.CTR += (((data + 1) % 10) - (data % 10)) * digit_weight[g_blink_digit];
        
        if (user_config.CTR < 5)
        {
            user_config.CTR = 5;
        }
    }
    
    display_CTR(user_config.CTR, g_blink_digit, 0);
}
/******************************************************************************
* Function Name: void EM_LCD_DisplayCtrNormal(void)
* Description  : Display unit code on LCD screen
* Arguments    : none
* Output       : none
* Return Value : none
******************************************************************************/
static void EM_LCD_DisplayCtrNormal(void)
{
    display_CTR(user_config.CTR, 0xff, 0);
}
/******************************************************************************
* Function Name: void EM_LCD_Password(void)
* Description  : Display unit code on LCD screen
* Arguments    : none
* Output       : none
* Return Value : none
******************************************************************************/
static void EM_LCD_Password(void)
{
    uint32_t  data;
    
    if (g_key_flag.Bits.is_key_1_pressed == 1)
    {
        g_key_flag.Bits.is_key_1_pressed = 0;
        data = (s_password / digit_weight[g_blink_digit]);
        s_password += (((data + 1) % 10) - (data % 10)) * digit_weight[g_blink_digit];
    }
    
    display_CTR(s_password, g_blink_digit, 1u);
}
/******************************************************************************
* Function Name: void EM_LCD_DisplayModbusId(void)
* Description  : Display unit code on LCD screen
* Arguments    : none
* Output       : none
* Return Value : none
******************************************************************************/
static void EM_LCD_DisplayModbusId(void)
{
    uint32_t  data;
    
    if (g_key_flag.Bits.is_key_1_pressed == 1)
    {
        g_key_flag.Bits.is_key_1_pressed = 0;
        data = (user_config.ModbusID / digit_weight[g_blink_digit]);
        user_config.ModbusID += (((data + 1) % 10) - (data % 10)) * digit_weight[g_blink_digit];
        
        if (user_config.ModbusID > 250)
        {
            user_config.ModbusID = 18;
        }
    }
    
    display_Modbus_ID(user_config.ModbusID, g_blink_digit);
}
/******************************************************************************
* Function Name: void EM_SwitchAutoRoll(void)
* Description  : Toogle between using auto roll and not use auto roll
* Arguments    : none
* Output       : [g_auto_roll]: flag to determine if using auto roll or not
* Return Value :
******************************************************************************/
void LCD_SwitchAutoRoll(void)
{
    /* Toogle auto scroll flag */
    g_auto_roll ^= 0x1;
    /* Reset display counter */
    g_em_display_count = 0;
}
/******************************************************************************
* Function Name: void LCD_SwitchProgMode(void)
* Description  : Toogle between using Programming Mode
* Arguments    : none
* Output       : None
* Return Value :
******************************************************************************/
void LCD_SwitchProgMode(void)
{
    /* Toogle Prog mode */
    g_prog_mode ^= 0x1;
    
    if (g_prog_mode == 0) /*Exit from Prog Mode then Save Config */
    {
        if (CONFIG_Backup(CONFIG_ITEM_USER) != EM_OK)
        {
        }
        
        INST_UpdateMultiplyfactor();
        g_DispCounter = 1;
    }
    else
    {
        g_DispCounter = 0;
        s_password = 0;
    }
    
    g_blink_digit = 3;
    g_em_display_count = LCD_DELAY_TIME * 2; // restart time out
    display_dot[0] = FALSE;
    display_dot[1] = FALSE;
    display_dot[2] = FALSE;
    display_dot[3] = FALSE;
    display_dot[4] = FALSE;
    display_dot[5] = FALSE;
    display_array[6] = 0xFF;
}

/******************************************************************************
* Function Name: void EM_ChangeNext(void)
* Description  : Switch to the next display value on EM
* Arguments    : none
* Output       : [g_display_pos] determine the order of displayed information
*              : [g_em_display_count] the counter for displaying
* Return Value : none
******************************************************************************/
void LCD_ChangeNext(void)
{
    g_DispCounter++;
    g_is_display_changed_next = 1;
    
    if (fp_display[g_DispCounter] == NULL)
    {
        g_DispCounter = 1;
    }
    
    display_dot[0] = FALSE;
    display_dot[1] = FALSE;
    display_dot[2] = FALSE;
    display_dot[3] = FALSE;
    display_dot[4] = FALSE;
    display_dot[5] = FALSE;
    g_led_flag.Byte = 0;
    (fp_display[g_DispCounter])();
}

/* Display some extra information */

/*****************************************************************************************
* function      : uint8_t BCD2DEC(uint8_t BCD_number)
* description   : This function is used to convert the BCD number into decimal number
* input         : [decimal_number] : the normal number in decimal type
* output        : none
* return        : the number in BCD type
*******************************************************************************************/
static uint8_t BCD2DEC(uint8_t BCD_number)
{
    volatile uint8_t rts;
    
    rts = (uint8_t)(BCD_number & 0x0f);
    BCD_number = (uint8_t)(((BCD_number & 0xf0) >> 4) * 10);
    rts = (uint8_t)(rts + BCD_number);
    return rts;
}

/*****************************************************************************************
* function      : static float abs(float input)
* description   : This function returns abs value of [input]
* input         : [input] number which need to be get abs value
* output        : none
* return        : abs value of [input]
*******************************************************************************************/
static uint32_t abs(int32_t input)
{
    if (input < 0)
    {
        return (-input);
    }
    else
    {
        return (input);
    }
}
/*****************************************************************************************
* function      : EM_DisplayTimerCallback
* description   : This function call from 1 ms timer
* input         : none
* output        : none
* return        : none
*******************************************************************************************/
void EM_Timer_DisplayInterruptCallback(void)
{
    /* Start user code. Do not edit comment generated here */
    static uint8_t display_index;
    static uint16_t  msCnt = 0;
    
    
    /* Update Display */
    /*Select Digit*/
    LCD_Q1_Q5_PORT = 0xFF;
    //SET_BIT(LCD_Q6_PORT,LCD_Q6,1);
    LCD_Q6_PORT |= (1 << LCD_Q6);
    LCD_Q7_PORT |= (1 << LCD_Q7);
    
    if (display_index < 5)
    {
        LCD_Q1_Q5_PORT &= ~(1 << display_index);
    }
    else
    {
        //SET_BIT(LCD_Q6_PORT,LCD_Q6,0);
        LCD_Q6_PORT &= ~(1 << (display_index + 1));
    }
    
    if (display_index > 5)
    {
        LCD_DATA_PORT   =   display_array[display_index];
    }
    else
    {
        if (display_dot[display_index] == TRUE)
        {
            LCD_DATA_PORT   = (look_up_display[display_array[display_index]]) & 0x7F; // Data
        }
        else
        {
            LCD_DATA_PORT   =   look_up_display[display_array[display_index]]; // Data
        }
    }
    
    display_index++;
    
    if (display_index > 6)
    {
        display_index = 0;
    }
    
    msCnt++;
    
    if ((msCnt % 31) == 0)
    {
        g_KeyScan_Flg = 1;
#if 1
        
        if ((msCnt % 248) == 0)
        {
            if (R_CALIB_IsInCalibMode() == 1)
            {
                EM_CALIB_RTCConstantInterruptCallback();
            }
            
            g_Display_Refresh_Flg = 1;
            msCnt = 0;
        }
        
#else
        msCnt = 0;
#endif
        
    }
}


