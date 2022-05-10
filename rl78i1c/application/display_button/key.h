/******************************************************************************
  Copyright (C) 2011 Renesas Electronics Corporation, All Rights Reserved.
*******************************************************************************
* File Name    : key.h
* Version      : 1.00
* Description  : KEY Interface Header file
******************************************************************************
* History : DD.MM.YYYY Version Description
******************************************************************************/

#ifndef _KEY_H
#define _KEY_H

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/

/******************************************************************************
Typedef definitions
******************************************************************************/
/* Key flag */
typedef union
{
    uint8_t Byte;
    struct tagEventFlag
    {
        uint8_t is_key_1_pressed  : 1;   /* Key 1 flag */
        uint8_t is_key_2_pressed  : 1;   /* Key 2 flag */
        uint8_t is_key_3_pressed  : 1;   /* Key 3 flag */
        uint8_t is_key_3_Longpressed  : 1;   /* Key 3 flag */
        uint8_t is_key_4_pressed  : 1;   /* Key 4 flag */
        uint8_t is_key_up_pressed  : 1;   /* Key 5 flag */
        uint8_t is_key_down_pressed  : 1;   /* Key 5 flag */
        uint8_t reserved          : 1;   /* (NO USE) Reserved */
    } Bits;
} KEY_FLAG;

/******************************************************************************
Macro definitions
******************************************************************************/
#ifndef KEY_PRESSED
#define KEY_PRESSED     0
#endif

#define     KEY_UP      (BIT_SELECT(P13,7))

#define     KEY_1      (BIT_SELECT(P13,7))
#define     KEY_2      (BIT_SELECT(P12,1))
#define     KEY_3      (BIT_SELECT(P12,2))
#define     KEY_4      (BIT_SELECT(P13,7))
#define     KEY_5      (BIT_SELECT(P12,6))
#define     KEY_6      (BIT_SELECT(P12,7))
/******************************************************************************
Variable Externs
******************************************************************************/
extern KEY_FLAG g_key_flag;
/******************************************************************************
Functions Prototypes
******************************************************************************/
/* Key Polling Processing */
void KEY_PollingProcessing(void);

#endif /* _KEY_H */

