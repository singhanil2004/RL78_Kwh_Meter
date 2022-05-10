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
#include "LCD_JH1286.h"
#include "wrp_mcu.h"
#include "r_cg_wdt.h"                   /* CG WDT Driver */
#include "user_config.h"
/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#define UP_ARROW    '^'
#define DOWN_ARROW  '`'
#define BOX     127

//LCD PIN Definitions

#define LCD_DATA         P1
#define LCD_E_PORT       P7
#define LCD_CS1_PORT     P7
#define LCD_CS2_PORT     P7
#define LCD_RS_PORT      P7
#define LCD_RST_PORT     P7

#define LCD_RS       0
#define LCD_E        1
#define LCD_RST      2
#define LCD_CS1      3
#define LCD_CS2      4

#define pgm_read_byte  *
/***********************************************************************************************************************
Imported global variables and functions (from other files)
***********************************************************************************************************************/

/***********************************************************************************************************************
Exported global variables and functions (to be accessed by other files)
***********************************************************************************************************************/

const uint8_t lcd_font_1x1[]  = { \
                                  0x00, 0x00, 0x00, 0x00, 0x00, \
                                  0x00, 0x00, 0x5F, 0x00, 0x00, \
                                  0x00, 0x07, 0x00, 0x07, 0x00, \
                                  0x00, 0x06, 0x09, 0x09, 0x06,   \
                                  0x00, 0x30, 0x4A, 0x4D, 0x39,   \
                                  0x23, 0x13, 0x08, 0x64, 0x62, \
                                  0x36, 0x49, 0x55, 0x22, 0x50, \
                                  0x00, 0x05, 0x03, 0x00, 0x00, \
                                  0x00, 0x1C, 0x22, 0x41, 0x00, \
                                  0x00, 0x41, 0x22, 0x1C, 0x00, \
                                  0x08, 0x2A, 0x1C, 0x2A, 0x08, \
                                  0x08, 0x08, 0x3E, 0x08, 0x08, \
                                  0x00, 0x50, 0x30, 0x00, 0x00, \
                                  0x08, 0x08, 0x08, 0x08, 0x08, \
                                  0x00, 0x30, 0x30, 0x00, 0x00, \
                                  0x20, 0x10, 0x08, 0x04, 0x02, \
                                  0x3E, 0x51, 0x49, 0x45, 0x3E, \
                                  0x00, 0x42, 0x7F, 0x40, 0x00, \
                                  0x42, 0x61, 0x51, 0x49, 0x46, \
                                  0x21, 0x41, 0x45, 0x4B, 0x31, \
                                  0x18, 0x14, 0x12, 0x7F, 0x10, \
                                  0x27, 0x45, 0x45, 0x45, 0x39, \
                                  0x3C, 0x4A, 0x49, 0x49, 0x30, \
                                  0x01, 0x71, 0x09, 0x05, 0x03, \
                                  0x36, 0x49, 0x49, 0x49, 0x36, \
                                  0x06, 0x49, 0x49, 0x29, 0x1E, \
                                  0x00, 0x36, 0x36, 0x00, 0x00, \
                                  0x00, 0x56, 0x36, 0x00, 0x00, \
                                  0x00, 0x08, 0x14, 0x22, 0x41, \
                                  0x14, 0x14, 0x14, 0x14, 0x14, \
                                  0x41, 0x22, 0x14, 0x08, 0x00, \
                                  0x02, 0x01, 0x51, 0x09, 0x06, \
                                  0x32, 0x49, 0x79, 0x41, 0x3E, \
                                  0x7E, 0x11, 0x11, 0x11, 0x7E, \
                                  0x7F, 0x49, 0x49, 0x49, 0x36, \
                                  0x3E, 0x41, 0x41, 0x41, 0x22, \
                                  0x7F, 0x41, 0x41, 0x22, 0x1C, \
                                  0x7F, 0x49, 0x49, 0x49, 0x41, \
                                  0x7F, 0x09, 0x09, 0x01, 0x01, \
                                  0x3E, 0x41, 0x41, 0x51, 0x32, \
                                  0x7F, 0x08, 0x08, 0x08, 0x7F, \
                                  0x00, 0x41, 0x7F, 0x41, 0x00, \
                                  0x20, 0x40, 0x41, 0x3F, 0x01, \
                                  0x7F, 0x08, 0x14, 0x22, 0x41, \
                                  0x7F, 0x40, 0x40, 0x40, 0x40, \
                                  0x7F, 0x02, 0x04, 0x02, 0x7F, \
                                  0x7F, 0x04, 0x08, 0x10, 0x7F, \
                                  0x3E, 0x41, 0x41, 0x41, 0x3E, \
                                  0x7F, 0x09, 0x09, 0x09, 0x06, \
                                  0x3E, 0x41, 0x51, 0x21, 0x5E, \
                                  0x7F, 0x09, 0x19, 0x29, 0x46, \
                                  0x46, 0x49, 0x49, 0x49, 0x31, \
                                  0x01, 0x01, 0x7F, 0x01, 0x01, \
                                  0x3F, 0x40, 0x40, 0x40, 0x3F, \
                                  0x1F, 0x20, 0x40, 0x20, 0x1F, \
                                  0x7F, 0x20, 0x18, 0x20, 0x7F, \
                                  0x63, 0x14, 0x08, 0x14, 0x63, \
                                  0x03, 0x04, 0x78, 0x04, 0x03, \
                                  0x61, 0x51, 0x49, 0x45, 0x43, \
                                  0x00, 0x00, 0x7F, 0x41, 0x41, \
                                  0x02, 0x04, 0x08, 0x10, 0x20, \
                                  0x41, 0x41, 0x7F, 0x00, 0x00, \
                                  0x04, 0x02, 0x7F, 0x02, 0x04,   \
                                  0x40, 0x40, 0x40, 0x40, 0x40, \
                                  0x10, 0x20, 0x7F, 0x20, 0x10,   \
                                  0x20, 0x54, 0x54, 0x54, 0x78, \
                                  0x7F, 0x48, 0x44, 0x44, 0x38, \
                                  0x38, 0x44, 0x44, 0x44, 0x20, \
                                  0x38, 0x44, 0x44, 0x48, 0x7F, \
                                  0x38, 0x54, 0x54, 0x54, 0x18, \
                                  0x08, 0x7E, 0x09, 0x01, 0x02, \
                                  0x08, 0x14, 0x54, 0x54, 0x3C, \
                                  0x7F, 0x08, 0x04, 0x04, 0x78, \
                                  0x00, 0x44, 0x7D, 0x40, 0x00, \
                                  0x20, 0x40, 0x44, 0x3D, 0x00, \
                                  0x00, 0x7F, 0x10, 0x28, 0x44, \
                                  0x00, 0x41, 0x7F, 0x40, 0x00, \
                                  0x7C, 0x04, 0x18, 0x04, 0x78, \
                                  0x7C, 0x08, 0x04, 0x04, 0x78, \
                                  0x38, 0x44, 0x44, 0x44, 0x38, \
                                  0x7C, 0x14, 0x14, 0x14, 0x08, \
                                  0x08, 0x14, 0x14, 0x18, 0x7C, \
                                  0x7C, 0x08, 0x04, 0x04, 0x08, \
                                  0x48, 0x54, 0x54, 0x54, 0x20, \
                                  0x04, 0x3F, 0x44, 0x40, 0x20, \
                                  0x3C, 0x40, 0x40, 0x20, 0x7C, \
                                  0x1C, 0x20, 0x40, 0x20, 0x1C, \
                                  0x3C, 0x40, 0x30, 0x40, 0x3C, \
                                  0x44, 0x28, 0x10, 0x28, 0x44, \
                                  0x0C, 0x50, 0x50, 0x50, 0x3C, \
                                  0x44, 0x64, 0x54, 0x4C, 0x44, \
                                  0x00, 0x08, 0x36, 0x41, 0x00, \
                                  0x08, 0x08, 0x2A, 0x1C, 0x08,   \
                                  0x00, 0x41, 0x36, 0x08, 0x00, \
                                  0xFE, 0xFE, 0xFE, 0xFE, 0xFE, \
                                  0xFF, 0xFF, 0xFF, 0xFF, 0xFF
                                };

//DEG    #
//DEL    $
//UP     ^
//DOWN   `
//RIGHT  |

//0x14, 0x7F, 0x14, 0x7F, 0x14, \   "#"
//0x24, 0x2A, 0x7F, 0x2A, 0x12, \   "$"
//0x00, 0x00, 0x7F, 0x00, 0x00, \   "|"
//0x08, 0x04, 0x08, 0x10, 0x08, \   "~"
//0x04, 0x02, 0x01, 0x02, 0x04, \   "^"
//0x00, 0x01, 0x02, 0x04, 0x00, \   "`"

//0x60, 0x50, 0x48, 0x50, 0x60. \   "delta"
//0x00, 0x30, 0x4A, 0x4D, 0x39, \   "del"

#define FONT_m      10 + 0x30   //:
#define FONT_ohm    11 + 0x30   //;
#define FONT_micro  12 + 0x30   //<

const uint8_t lcd_font_2x2[]  = { \
                                  /* 000: '0' */ \
                                  0xfc, 0xfe, 0x07, 0x03, 0x83, 0x43, 0x23, 0x17, 0xfe, 0xfc, \
                                  0x0f, 0x1f, 0x3a, 0x31, 0x30, 0x30, 0x30, 0x38, 0x1f, 0x0f, \
                                  /* 020: '1' */ \
                                  0x00, 0x00, 0x0c, 0x0e, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00,  \
                                  0x00, 0x00, 0x30, 0x30, 0x3f, 0x3f, 0x30, 0x30, 0x00, 0x00,  \
                                  /* 040: '2' */ \
                                  0x06, 0x07, 0x03, 0x03, 0x03, 0x83, 0xc3, 0xe7, 0x7e, 0x3c, \
                                  0x30, 0x38, 0x3c, 0x3e, 0x37, 0x33, 0x31, 0x30, 0x30, 0x30, \
                                  /* 060: '3' */ \
                                  0x06, 0x07, 0x03, 0x63, 0x63, 0x63, 0x63, 0xf7, 0xde, 0x8c,  \
                                  0x0c, 0x1c, 0x38, 0x30, 0x30, 0x30, 0x30, 0x38, 0x1f, 0x0f,  \
                                  /* 080: '4' */ \
                                  0xc0, 0xe0, 0x70, 0x38, 0x1c, 0x0e, 0xff, 0xff, 0x00, 0x00, \
                                  0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x3f, 0x3f, 0x03, 0x03, \
                                  /* 100: '5' */ \
                                  0x3f, 0x3f, 0x33, 0x33, 0x33, 0x33, 0x33, 0x73, 0xe3, 0xc3,  \
                                  0x0c, 0x1c, 0x38, 0x30, 0x30, 0x30, 0x30, 0x38, 0x1f, 0x0f,  \
                                  /* 120: '6' */ \
                                  0xfc, 0xfe, 0xc7, 0x63, 0x63, 0x63, 0x63, 0xe3, 0xc7, 0x86, \
                                  0x0f, 0x1f, 0x38, 0x30, 0x30, 0x30, 0x30, 0x38, 0x1f, 0x0f, \
                                  /* 140: '7' */ \
                                  0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0xc3, 0xf3, 0x3f, 0x0f,  \
                                  0x00, 0x00, 0x00, 0x30, 0x3c, 0x0f, 0x03, 0x00, 0x00, 0x00,  \
                                  /* 160: '8' */ \
                                  0x8c, 0xde, 0xf7, 0x63, 0x63, 0x63, 0x63, 0xf7, 0xde, 0x8c, \
                                  0x0f, 0x1f, 0x38, 0x30, 0x30, 0x30, 0x30, 0x38, 0x1f, 0x0f, \
                                  /* 180: '9' */ \
                                  0x3c, 0x7e, 0xe7, 0xc3, 0xc3, 0xc3, 0xc3, 0x67, 0xfe, 0xfc,  \
                                  0x0c, 0x1c, 0x38, 0x30, 0x30, 0x30, 0x30, 0x38, 0x1f, 0x0f,  \
                                  /* 200: 'm' */ \
                                  0xc0, 0xc0, 0xc0, 0xc0, 0x80, 0x80, 0xc0, 0xc0, 0x80, 0x00, \
                                  0x3f, 0x3f, 0x00, 0x00, 0x3f, 0x3f, 0x00, 0x00, 0x3f, 0x3f, \
                                  /* 220: ohm. ASCII - 234 */ \
                                  0x80, 0xc0, 0x60, 0x70, 0x30, 0x30, 0x70, 0x60, 0xc0, 0x80,  \
                                  0x37, 0x3f, 0x38, 0x30, 0x00, 0x00, 0x30, 0x38, 0x3f, 0x37,  \
                                  /* 240: 'µ'. ASCII - 230 */ \
                                  0xe0, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0xe0, 0x00, \
                                  0x3f, 0x3f, 0x0c, 0x18, 0x18, 0x18, 0x0c, 0x0f, 0x0f, 0x08, \
                                  /* 260: ' '. Repeat twice */ \
                                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  \
                                  /* 270: '.'. Use space for upper blank */ \
                                  0x00, 0x00, 0x00, 0x00, 0x38, 0x38, 0x38, 0x00, 0x00, 0x00, \
                                  /* 280: Degree. Use space for lower blank. ASCII - 167 */ \
                                  0x0c, 0x1e, 0x33, 0x33, 0x1e, 0x0c, 0x00, 0x00, 0x00, 0x00, \
                                  /* 290: '-'. Use space for lower blank */ \
                                  0x00, 0x00, 0x00, 0x00, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0x00
                                };



static uint8_t pos_y;                               //y = 0 to 127

//#define   lcd_clear_page for(uint8_t i=1; i<=MAX_ROW; ++i) lcd_clear_line(i);

/********************************/
void lcd_clear_line(uint8_t x)
{
    uint8_t i;
    
    lcd_text_pos(x, 1);
    
    for (i = 0; i < 128; ++i)
    {
        lcd_send_data(0x00);
    }
    
    lcd_text_pos(1, 1);
}

/********************************/
void lcd_statusbar(char* text1)
{
    int8_t text[22];
    
    lcd_text_pos(8, 1);
    sprintf((char*)text, "%-21s", text1);
    lcd_print_string_rev_1x1(text);
    lcd_text_pos(1, 1);
}

/********************************/
void lcd_hot_keys(uint8_t row, char* text1, char* text2, char* text3, char* text4)
{
    int8_t text[6];
    
    lcd_text_pos(row, 1);
    lcd_send_data(0x0);
    sprintf((char*)text, "%-5s", text1);
    lcd_print_string_rev_1x1(text);
    lcd_send_data(0x0);
    lcd_send_data(0x0);
    sprintf((char*)text, "%-5s", text2);
    lcd_print_string_rev_1x1(text);
    lcd_send_data(0x0);
    lcd_send_data(0x0);
    sprintf((char*)text, "%-5s", text3);
    lcd_print_string_rev_1x1(text);
    lcd_send_data(0x0);
    lcd_send_data(0x0);
    sprintf((char*)text, "%-5s", text4);
    lcd_print_string_rev_1x1(text);
    lcd_text_pos(1, 1);
}

/********************************/
void lcd_print_string_2x2(uint8_t x, uint8_t y, uint8_t* p)
{
    uint8_t i;
    
    while (*p != 0)
    {
        lcd_text_pos(x - 1, y);
        lcd_send_data(0x00);
        lcd_send_data(0x00);
        
        if (*p == ' ')
        {
            for (i = 0; i < 10; ++i)
            {
                lcd_send_data(pgm_read_byte(lcd_font_2x2 + 260 + i));
            }
        }
        else if (*p == '.')
        {
            for (i = 0; i < 10; ++i)
            {
                lcd_send_data(pgm_read_byte(lcd_font_2x2 + 260 + i));
            }
        }
        else if (*p == 'º')                            // º (Degree, ASCII 167)
        {
            for (i = 0; i < 10; ++i)
            {
                lcd_send_data(pgm_read_byte(lcd_font_2x2 + 280 + i));
            }
        }
        else if (*p == '-')
        {
            for (i = 0; i < 10; ++i)
            {
                lcd_send_data(pgm_read_byte(lcd_font_2x2 + 290 + i));
            }
        }
        else
        {
            for (i = 0; i < 10; ++i)
            {
                lcd_send_data(pgm_read_byte(lcd_font_2x2 + (20 * (*p - 0x30)) + i));
            }
        }
        
        lcd_text_pos(x, y);
        lcd_send_data(0x00);
        lcd_send_data(0x00);
        
        if (*p == ' ')
        {
            for (i = 0; i < 10; ++i)
            {
                lcd_send_data(pgm_read_byte(lcd_font_2x2 + 260 + i));
            }
            
            y += 2;
        }
        else if (*p == '.')
        {
            for (i = 0; i < 10; ++i)
            {
                lcd_send_data(pgm_read_byte(lcd_font_2x2 + 270 + i));
            }
            
            y += 2;
        }
        else if (*p == 'º')                            // º (Degree, ASCII 167)
        {
            for (i = 0; i < 10; ++i)
            {
                lcd_send_data(pgm_read_byte(lcd_font_2x2 + 260 + i));
            }
            
            y += 2;
        }
        else if (*p == '-')
        {
            for (i = 0; i < 10; ++i)
            {
                lcd_send_data(pgm_read_byte(lcd_font_2x2 + 260 + i));
            }
            
            y += 2;
        }
        else
        {
            for (i = 0; i < 10; ++i)
            {
                lcd_send_data(pgm_read_byte(lcd_font_2x2 + (20 * (*p - 0x30)) + 10 + i));
            }
            
            y += 2;
        }
        
        ++p;
    }
}

/********************************/
void lcd_print_string_xy_1x1(uint8_t x, uint8_t y, char* p)
{
    uint8_t i;
    
    lcd_send_command(0x40, TRUE, TRUE);                                 //y position = 0
    lcd_send_command(0xb8 | ((x - 1) & 0b00000111), TRUE, TRUE);        //set x page
    y = ((y - 1) * 6) & 0b01111111;
    
    if (y < 64)                                                         //set y position
    {
        lcd_send_command(0x40 | y, TRUE, FALSE);
    }
    else
    {
        lcd_send_command(0x40 | (y - 64), FALSE, TRUE);
    }
    
    pos_y = y;
    
    while (*p != 0)
    {
        lcd_send_data(0x00);
        
        for (i = 0; i < 5; ++i)
        {
            lcd_send_data(pgm_read_byte(lcd_font_1x1 + (5 * (*p - 32)) + i));
        }
        
        ++p;
    }
}

/********************************/
void lcd_print_string_1x1(char* p)
{
    uint8_t i;
    
    while (*p != 0)
    {
        lcd_send_data(0x00);
        
        for (i = 0; i < 5; ++i)
        {
            lcd_send_data(pgm_read_byte(lcd_font_1x1 + (5 * (*p - 32)) + i));
        }
        
        ++p;
    }
}

/********************************/
void lcd_print_string_rev_1x1(int8_t* p)
{
    uint8_t i;
    
    while (*p != 0)
    {
        lcd_send_data(0xff);
        
        for (i = 0; i < 5; ++i)
        {
            lcd_send_data(~pgm_read_byte(lcd_font_1x1 + (5 * (*p - 32)) + i));
        }
        
        ++p;
    }
}

/********************************/
void lcd_print_string_rev_bottom_1x1(char* p)
{
    uint8_t i;
    
    while (*p != 0)
    {
        lcd_send_data(0xff);
        
        for (i = 0; i < 5; ++i)
        {
            lcd_send_data(~((pgm_read_byte(lcd_font_1x1 + (5 * (*p - 32)) + i)) << 1));
        }
        
        ++p;
    }
}

/********************************/
void lcd_print_underline_1x1(uint8_t count)
{
    uint8_t i;
    
    while (count-- > 0)
    {
        lcd_send_data(0x01);
        
        for (i = 0; i < 5; ++i)
        {
            lcd_send_data(~pgm_read_byte(lcd_font_1x1 + (5 * ('~' - 32)) + i));
        }
    }
}

/********************************/
void lcd_text_pos(uint8_t x, uint8_t y) //x = 1 to 8; y = 1 to 21
{
    --x;
    y = (y - 1) * 6;
    x &= 0b00000111;
    y &= 0b01111111;
    lcd_send_command(0x40, TRUE, TRUE);         //y position = 0
    lcd_send_command(0xb8 | x, TRUE, TRUE);     //set x page
    
    if (y < 64)                                 //set y position
    {
        lcd_send_command(0x40 | y, TRUE, FALSE);
    }
    else
    {
        lcd_send_command(0x40 | (y - 64), FALSE, TRUE);
    }
    
    pos_y = y;
}

/********************************/
void lcd_initialise(void)
{
    uint8_t i, j;
    
    LCD_RST_PORT &= ~(1 << LCD_RST);
    MCU_Delay(200);
    LCD_RST_PORT |= 1 << LCD_RST;
    MCU_Delay(200);
    
    lcd_send_command(0x40, TRUE, TRUE);     //y position = 0
    lcd_send_command(0xb8, TRUE, TRUE);     //x page = 0
    lcd_send_command(0xc0, TRUE, TRUE);     //display top line
    lcd_send_command(0x3f, TRUE, TRUE);     //display on
    
    for (i = 1; i <= 8; ++i)
    {
        lcd_text_pos(i, 1);
        
        for (j = 0; j < 128; ++j)
        {
            lcd_send_data(0);
        }
    }
    
    lcd_text_pos(1, 1);
}

/********************************/
void lcd_send_command(uint8_t varByte, uint8_t lcd_chip1, uint8_t lcd_chip2)
{
    LCD_DATA = varByte;
    LCD_E_PORT &= ~(1 << LCD_E);
    LCD_RS_PORT &= ~(1 << LCD_RS);
#ifdef LCD_RW_PORT
    LCD_RW_PORT &= ~(1 << LCD_RW);
#endif
    
    if (lcd_chip1 == TRUE)
    {
        LCD_CS1_PORT |= 1 << LCD_CS1;
    }
    
    if (lcd_chip2 == TRUE)
    {
        LCD_CS2_PORT |= 1 << LCD_CS2;
    }
    
    MCU_Delay(2);
    LCD_E_PORT |= 1 << LCD_E;
    MCU_Delay(2);
    LCD_E_PORT &= ~(1 << LCD_E);
    LCD_CS1_PORT &= ~(1 << LCD_CS1);
    LCD_CS2_PORT &= ~(1 << LCD_CS2);
}

/********************************/
void lcd_send_data(uint8_t varByte)
{
    if (pos_y > 127)
    {
        return;
    }
    
    LCD_DATA = varByte;
    LCD_E_PORT &= ~(1 << LCD_E);
    LCD_RS_PORT |= 1 << LCD_RS;
#ifdef LCD_RW_PORT
    LCD_RW_PORT &= ~(1 << LCD_RW);
#endif
    
    if (pos_y < 64)
    {
        LCD_CS1_PORT |= 1 << LCD_CS1;
    }
    else
    {
        LCD_CS2_PORT |= 1 << LCD_CS2;
    }
    
    MCU_Delay(2);
    LCD_E_PORT |= 1 << LCD_E;
    MCU_Delay(2);
    LCD_E_PORT &= ~(1 << LCD_E);
    LCD_CS1_PORT &= ~(1 << LCD_CS1);
    LCD_CS2_PORT &= ~(1 << LCD_CS2);
    ++pos_y;
    R_WDT_Restart();
}

void lcd_print_string_eeprom(uint8_t x, uint8_t y, const uint8_t* E_ptr)
{
    lcd_print_string_xy_1x1(x, y, (char*)E_ptr);
}
void lcd_print_title_eeprom(uint8_t x, uint8_t y, const uint8_t* E_ptr)
{
    lcd_text_pos(x, y);
    lcd_print_string_rev_bottom_1x1((char*)E_ptr);
    lcd_text_pos(x + 1, y);
    lcd_print_underline_1x1(strlen((char*)E_ptr));
}

void lcd_print_statusbar_eeprom(const uint8_t* E_ptr)
{
    lcd_statusbar((char*)E_ptr);
}

void lcd_clear_page(void)
{
    uint8_t i;
    
    for (i = 1; i <= MAX_ROW; ++i)
    {
        lcd_clear_line(i);
    }
}

void float2string(float var_float, uint8_t* text, uint8_t half_digit)
{
    uint8_t ch, ch1;
    uint8_t Resolution;
    
    //    if ((var_float < 1.0e-3) && (Resolution == RES_U1))
    //    {
    //        var_float *= 1e6;
    //        ch = FONT_micro;
    //        ch1 = 'u';
    //    }
    //    else if (var_float < 1.0)
    //    {
    //        var_float *= 1e3;
    //        ch = FONT_m;
    //        ch1 = 'm';
    //    }
    //    else
    //    {
    //  var_float *= 1e4;
    //        ch = ' ';
    //        ch1 = ' ';
    //    }
    
    if (half_digit == TRUE)
    {
        if (var_float >= 2000)
        {
            var_float += 0.5;
            sprintf((char*)text, "%5d", (int)var_float);
        }
        
        if (var_float >= 200)
        {
            var_float += 0.05;
            sprintf((char*)text, "%4d.%01d", (int)var_float, (int)(var_float * 10) % 10);
        }
        else if (var_float >= 20)
        {
            var_float += 0.005;
            sprintf((char*)text, "%3d.%02d", (int)var_float, (int)(var_float * 100) % 100);
        }
        else if (var_float >= 2)
        {
            var_float += 0.0005;
            sprintf((char*)text, "%2d.%03d", (int)var_float, (int)(var_float * 1000) % 1000);
        }
        else
        {
            var_float += 0.00005;
            sprintf((char*)text, "%1d.%04d", (int)var_float, (int)(var_float * 10000) % 10000);
        }
    }
    else
    {
        if (var_float >= 1000)
        {
            var_float += 0.5;
            sprintf((char*)text, "%4d%1c", (int)var_float, ch1);
        }
        else if (var_float >= 100)
        {
            var_float += 0.05;
            sprintf((char*)text, "%3d.%01d%1c", (int)var_float, (int)(var_float * 10) % 10, ch1);
        }
        else if (var_float >= 10)
        {
            var_float += 0.005;
            sprintf((char*)text, "%2d.%02d%1c", (int)var_float, (int)(var_float * 100) % 100, ch1);
        }
        else
        {
            var_float += 0.0005;
            sprintf((char*)text, "%1d.%03d%1c", (int)var_float, (int)(var_float * 1000) % 1000, ch1);
        }
    }
}
/********************************/
