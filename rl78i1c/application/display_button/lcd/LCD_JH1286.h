/***********************************************************************************************************************
* File Name    : _LCD_JH1286_H.h
* Version      : 1.00
* Device(s)    : RL78/I1C
* Tool-Chain   : CCRL
* H/W Platform : RL78/I1C Energy Meter Platform
* Description  : Command Header file
***********************************************************************************************************************/

#ifndef _LCD_JH1286_H
#define _LCD_JH1286_H

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include "typedef.h"            /* GSCE Standard Typedef */
#include "screen.h"
/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#define MAX_COLUMN              21
#define MAX_ROW                 8

#define RES_U1                  0
#define RES_M001                1
#define RES_M01                 2

#define TRUE        0xff
#define FALSE       0x00

/***********************************************************************************************************************
Variable Externs
***********************************************************************************************************************/

/***********************************************************************************************************************
Functions Prototypes
*****************************************************************************/
void lcd_initialise(void);
void lcd_text_pos(uint8_t x, uint8_t y);
void lcd_send_command(uint8_t varByte, uint8_t lcd_chip1, uint8_t lcd_chip2);
void lcd_send_data(uint8_t varByte);
void lcd_print_string_2x2(uint8_t x, uint8_t y, uint8_t* p);
void lcd_print_string_xy_1x1(uint8_t x, uint8_t y, char* p);
void lcd_print_string_1x1(char* p);
void lcd_print_string_rev_1x1(int8_t* p);
void lcd_print_string_rev_bottom_1x1(char* p);
void lcd_print_underline_1x1(uint8_t count);
void lcd_statusbar(char* text1);
void lcd_hot_keys(uint8_t row, char* text1, char* text2, char* text3, char* text4);
void lcd_clear_line(uint8_t x);
void lcd_print_statusbar_eeprom(const uint8_t* E_ptr);
void lcd_print_title_eeprom(uint8_t x, uint8_t y, const uint8_t* E_ptr);
void lcd_print_string_eeprom(uint8_t x, uint8_t y, const uint8_t* E_ptr);
void lcd_clear_page(void);
void float2string(float var_float, uint8_t* text, uint8_t half_digit);
#endif /* _LCD_JH1286_H */

