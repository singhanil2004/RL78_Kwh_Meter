/***********************************************************************************************************************
* File Name    : _LCD_JH1286_H.h
* Version      : 1.00
* Device(s)    : RL78/I1C
* Tool-Chain   : CCRL
* H/W Platform : RL78/I1C Energy Meter Platform
* Description  : Command Header file
***********************************************************************************************************************/

#ifndef _LCD_SEGMENT_H
#define _LCD_SEGMENT_H

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include "typedef.h"            /* GSCE Standard Typedef */

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/
/* Key flag */
typedef union
{
    uint8_t Byte;
    struct tIconFlag
    {
        uint8_t Unit_W  : 1;
        uint8_t Unit_VA : 1;
        uint8_t Unit_PF : 1;
        uint8_t Unit_K  : 1;
        uint8_t Unit_M  : 1;
        uint8_t Unit_H  : 1;
        uint8_t KWh_EB  : 1;
        uint8_t KWh_DG  : 1;
    } Bits;
} LED_FLAG;
/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#define CODE_BLANK              10
#define CODE_DASH               11
#define CODE_C                  12
#define CODE_o                  13
#define CODE_r                  14
#define CODE_t                  15
#define CODE_H                  16
#define CODE_i                  17
#define CODE_P                  18
#define CODE_m                  19
#define CODE_u                  20
#define CODE_n                  21
#define CODE_A                  22
#define CODE_d                  23
#define CODE_L                  24
#define CODE_E                  25

//LCD PIN Definitions
#define LCD_DATA_PORT         P1
#define LCD_Q1_Q5_PORT        P7
#define LCD_Q6_PORT        P12
#define LCD_Q7_PORT           P12

#define LCD_Q1      0
#define LCD_Q2      1
#define LCD_Q3      2
#define LCD_Q4      3
#define LCD_Q5      4
#define LCD_Q6      6
#define LCD_Q7      7

/***********************************************************************************************************************
Variable Externs
***********************************************************************************************************************/
extern const uint8_t look_up_display[26];
extern uint8_t display_array[8];
extern uint8_t display_dot[8];
extern uint8_t OhmUnit, OhmDecimal;
extern LED_FLAG g_led_flag;
/***********************************************************************************************************************
Functions Prototypes
*****************************************************************************/
void float2string(float var_float, uint32_t* data, uint8_t half_digit);
void display_6Digit(uint32_t data, uint8_t decimal);
void display_PF(uint32_t data, uint8_t decimal);
void display_CTR(uint32_t data, uint8_t blinkdigit, uint8_t ispassword);
void display_Modbus_ID(uint32_t data, uint8_t blinkdigit);

#endif /* _LCD_SEGMENT_H */

