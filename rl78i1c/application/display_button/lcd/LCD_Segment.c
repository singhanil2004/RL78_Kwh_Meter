/***********************************************************************************************************************
* File Name    : JHD1286.c
* Version      : Applilet4 for RL78/I1C V1.00.01.04 [05 Aug 2016]
* Device(s)    : R5F10NMJ
* Tool-Chain   : CA78K0R
* Description  : This file implements device driver for SAU module.
* Creation Date: 11/8/2016
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include <stdio.h>
#include "LCD_Segment.h"
#include "wrp_mcu.h"
#include "r_cg_wdt.h"                   /* CG WDT Driver */
#include "user_config.h"
#include "inst_read.h"
/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#define NO_OF_DISPLAY_PARAM  10


/***********************************************************************************************************************
Imported global variables and functions (from other files)
***********************************************************************************************************************/

/***********************************************************************************************************************
Exported global variables and functions (to be accessed by other files)
***********************************************************************************************************************/
const uint16_t  digit_weight[5] = {1, 1000, 100, 10, 1};
const uint8_t look_up_display[26] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x98, 0xff, 0xbf, 0xC6, 0xA3, 0xAF, 0x87, 0x89, 0xC0, 0x8C, 0xAA, 0xE3, 0xAB, 0x88, 0xA1, 0xC7, 0x86};
//  0    1    2    3    4    5    6    7    8    9         -    C    o     r    t    H    i    P    m    u
uint8_t  display_array[8] = {8, 8, 8, 8, 8, 8, 8, 8};
uint8_t  display_dot[8] = {TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE};

uint8_t     lookup_led[NO_OF_DISPLAY_PARAM] = {0xFB, 0xFB, 0xFB, 0xdf, 0xdb, 0xdf, 0xdf, 0xdf, 0xff, 0xff};
uint8_t OhmUnit, OhmDecimal;
LED_FLAG g_led_flag;

/*===========================================================================================================
* Title : display_6Digit
* Details :
===========================================================================================================*/
void display_6Digit(uint32_t data, uint8_t decimal)
{
    display_array[0]    = data / 100000;
    data                = data % 100000;
    display_array[1]    = data / 10000;
    data                = data % 10000;
    display_array[2]    = data / 1000;
    data                = data % 1000;
    display_array[3]    = data / 100;
    data                = data % 100;
    display_array[4]    = data / 10;
    data                = data % 10;
    display_array[5]    = data;
    display_dot[0] = FALSE;
    display_dot[1] = FALSE;
    display_dot[2] = FALSE;
    display_dot[3] = FALSE;
    display_dot[4] = FALSE;
    display_dot[5] = FALSE;
    
    if (decimal != 0)
    {
        display_dot[5 - decimal]  = TRUE;
    }
    
    display_array[6] = ~g_led_flag.Byte;
}
/*===========================================================================================================
* Title : display_6Digit
* Details :
===========================================================================================================*/
void display_PF(uint32_t data, uint8_t decimal)
{
    if (g_inst_read_params.power_factor_sign[LINE_TOTAL] == PF_SIGN_LEAD_C)
    {
        display_array[0] = CODE_C;
    }
    else if (g_inst_read_params.power_factor_sign[LINE_TOTAL] == PF_SIGN_LAG_L)
    {
        display_array[0] = CODE_L;
    }
    else
    {
        display_array[0] = CODE_BLANK;
    }
    
    display_array[1]    = CODE_BLANK;
    
    display_array[2]    = data / 1000;
    data                = data % 1000;
    display_array[3]    = data / 100;
    data                = data % 100;
    display_array[4]    = data / 10;
    data                = data % 10;
    display_array[5]    = data;
    display_dot[0] = FALSE;
    display_dot[1] = FALSE;
    display_dot[2] = FALSE;
    display_dot[3] = FALSE;
    display_dot[4] = FALSE;
    display_dot[5] = FALSE;
    
    if (decimal != 0)
    {
        display_dot[5 - decimal]  = TRUE;
    }
    
    display_array[6] = ~g_led_flag.Byte;
}
/*===========================================================================================================
* Title : display_CTR
* Details :
===========================================================================================================*/
void display_CTR(uint32_t data, uint8_t blinkdigit, uint8_t ispassword)
{
    static uint8_t blink = 0;
    display_array[0] = CODE_C;
    
    if (ispassword)
    {
        display_array[1] = CODE_d;
    }
    else
    {
        display_array[1] = CODE_t;
    }
    
    display_array[2]    = data / 1000;
    data                = data % 1000;
    display_array[3]    = data / 100;
    data                = data % 100;
    display_array[4]    = data / 10;
    data                = data % 10;
    display_array[5]    = data;
    
    if (blinkdigit == 0xff)
    {
        blink = 0;
    }
    
    if (blink)
    {
        display_array[5 - blinkdigit]    = CODE_BLANK;
    }
    
    blink = ~ blink;
}
/*===========================================================================================================
* Title : display_Modbus_ID
* Details :
===========================================================================================================*/
void display_Modbus_ID(uint32_t data, uint8_t blinkdigit)
{
    static uint8_t blink = 0;
    display_array[0] = 1;
    display_array[1] = CODE_d;
    display_array[2] = CODE_DASH;
    
    display_array[3]    = data / 100;
    data                = data % 100;
    display_array[4]    = data / 10;
    data                = data % 10;
    display_array[5]    = data;
    
    if (blink)
    {
        display_array[5 - blinkdigit]    = CODE_BLANK;
    }
    
    blink = ~ blink;
}
/*===========================================================================================================
* Title : Task
* Details :
===========================================================================================================*/
void float2string(float var_float, uint32_t* data, uint8_t half_digit)
{
    //    uint8_t ch, ch1;
    //    uint8_t Resolution;
    //    uint8_t text[10];
    
    //    //g_led_flag.Byte[1] = 0;
    
    //    if (var_float < 1e-3)
    //    {
    //        var_float *= 1e6;
    //        *data = var_float;
    //        OhmUnit =  RESISTANCE_UNIT_uOHM;
    //        //g_led_flag.Bits.u_LED = 1;
    //    }
    
    //    if (var_float < 1)
    //    {
    //        var_float *= 1000;
    //        *data = var_float;
    //        OhmUnit =  RESISTANCE_UNIT_mOHM;
    //        //g_led_flag.Bits.u_LED = 1;
    //    }
    //    else if (var_float < 1000.0)
    //    {
    //        var_float *= 1;
    //        *data = var_float;
    //        OhmUnit =  RESISTANCE_UNIT_OHM;
    //        //g_led_flag.Bits.m_LED = 1;
    //    }
    //    else
    //    {
    //        var_float /= 1000;
    //        *data = var_float;
    //        OhmUnit =  RESISTANCE_UNIT_KOHM;
    //        //g_led_flag.Bits.K_LED = 1;
    //    }
    
    //    if (half_digit == TRUE)
    //    {
    //        if (var_float >= 2000)
    //        {
    //            var_float += 0.5;
    //            *data = var_float;
    //            OhmDecimal = 0;
    //            sprintf((char*)text, "%5d", (int)var_float);
    //            //ERROR
    //        }
    //        else if (var_float >= 200)
    //        {
    //            var_float += 0.5;
    //            *data = var_float;
    //            OhmDecimal = 0;
    //            sprintf((char*)text, "%3d", (int)var_float);
    //        }
    //        else if ((var_float >= 20) || (OhmUnit ==  RESISTANCE_UNIT_uOHM))
    //        {
    //            var_float += 0.05;
    //            *data = var_float * 10;
    //            OhmDecimal = 1;
    //            sprintf((char*)text, "%3d.%01d", (int)var_float, (int)(var_float * 10) % 10);
    //        }
    //        else if (var_float >= 2)
    //        {
    //            var_float += 0.005;
    //            *data = var_float * 100;
    //            OhmDecimal = 2;
    //            sprintf((char*)text, "%2d.%02d", (int)var_float, (int)(var_float * 100) % 100);
    //        }
    //        else
    //        {
    //            var_float += 0.0005;
    //            *data = var_float * 1000;
    //            OhmDecimal = 3;
    //            sprintf((char*)text, "%1d.%03d", (int)var_float, (int)(var_float * 1000) % 1000);
    //        }
    //    }
    //    else
    //    {
    //        if (var_float >= 1000)
    //        {
    //            var_float += 0.5;
    //            *data = var_float;
    //            OhmDecimal = 0;
    //            //sprintf((char*)text, "%4d%1c", (int)var_float, ch1);
    //        }
    //        else if (var_float >= 100)
    //        {
    //            var_float += 0.05;
    //            *data = var_float * 10;
    //            OhmDecimal = 1;
    //            //sprintf((char*)text, "%3d.%01d%1c", (int)var_float, (int)(var_float * 10) % 10, ch1);
    //        }
    //        else if (var_float >= 10)
    //        {
    //            var_float += 0.005;
    //            *data = var_float * 100;
    //            OhmDecimal = 2;
    //            //sprintf((char*)text, "%2d.%02d%1c", (int)var_float, (int)(var_float * 100) % 100, ch1);
    //        }
    //        else
    //        {
    //            var_float += 0.0005;
    //            *data = var_float * 1000;
    //            OhmDecimal = 3;
    //            //sprintf((char*)text, "%1d.%03d%1c", (int)var_float, (int)(var_float * 1000) % 1000, ch1);
    //        }
    //    }
}
