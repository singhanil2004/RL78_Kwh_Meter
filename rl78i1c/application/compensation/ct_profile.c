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
* Copyright (C) 2013, 2016 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : ct_profile.c
* Version      : 1.0
* Device(s)    : RL78/I1C
* Tool-Chain   : CCRL
* H/W Platform : 3P4W RL78/I1C Energy Meter Platform
* Description  : Custom profile for CT Angle Compensate.
************************************************************************************************************************
* History      : 21.11.2016 Initial version
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include "typedef.h"        /* GSCE Typedef */
/* MW/Core */
#include "em_core.h"        /* EM Core APIs */

#include "ct_compensation.h"
/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#define CT_OTHER            (0)     /* The empty profile, please add the parameter later */
#define CT_ASM_060L         (1)
#define CT_AST_080L3        (2)
#define CT_ASM_120LS        (3)

#define CT_SENSOR_USED      (CT_AST_080L3)
/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Imported global variables and functions (from other files)
***********************************************************************************************************************/

/***********************************************************************************************************************
Exported global variables and functions (to be accessed by other files)
***********************************************************************************************************************/
#if(CT_SENSOR_USED == CT_OTHER)
const ct_phase_shift_t g_ct_phase_shift_phase_r[] =
{
    /* {  (A)   , (degree)} */
    {   1    , 0.000   },
    {  10    , 0.000   },
};
const ct_phase_shift_t g_ct_phase_shift_phase_y[] =
{
    /* {  (A)   , (degree)} */
    {   1    , 0.000   },
    {  10    , 0.000   },
};
const ct_phase_shift_t g_ct_phase_shift_phase_b[] =
{
    /* {  (A)   , (degree)} */
    {   1    , 0.000   },
    {  10    , 0.000   },
};
#elif(CT_SENSOR_USED == CT_ASM_060L)
const ct_phase_shift_t g_ct_phase_shift_phase_r[] =
{
    /* {  (A)   , (degree)} */
    {   1    , 0.380   },
    {  10    , 0.180   },
};
const ct_phase_shift_t g_ct_phase_shift_phase_y[] =
{
    /* {  (A)   , (degree)} */
    {   1    , 0.300   },
    {  10    , 0.180   },
};
const ct_phase_shift_t g_ct_phase_shift_phase_b[] =
{
    /* {  (A)   , (degree)} */
    {   1    , 0.300   },
    {  10    , 0.180   },
};
#elif(CT_SENSOR_USED == CT_AST_080L3)
const ct_phase_shift_t g_ct_phase_shift_phase_r[] =
{
    /* {  (A)   , (degree)} */
    {   1    , 0.095   },
    {  10    , 0.075   },
    {  100   , 0.040   },
};
const ct_phase_shift_t g_ct_phase_shift_phase_y[] =
{
    /* {  (A)   , (degree)} */
    {   1    , 0.095   },
    {  10    , 0.075   },
    {  100   , 0.040   },
};
const ct_phase_shift_t g_ct_phase_shift_phase_b[] =
{
    /* {  (A)   , (degree)} */
    {   1    , 0.095   },
    {  10    , 0.075   },
    {  100   , 0.040   },
};

#elif(CT_SENSOR_USED == CT_ASM_120LS)
const ct_phase_shift_t g_ct_phase_shift_phase_r[] =
{
    /* {  (A)   , (degree)} */
    {   9    , 0.250   },
    { 100    , 0.150   },
};
const ct_phase_shift_t g_ct_phase_shift_phase_y[] =
{
    /* {  (A)   , (degree)} */
    {   9    , 0.250   },
    { 100    , 0.150   },
};
const ct_phase_shift_t g_ct_phase_shift_phase_b[] =
{
    /* {  (A)   , (degree)} */
    {   9    , 0.250   },
    { 100    , 0.150   },
};
#else
const ct_phase_shift_t g_ct_phase_shift_phase_r[] =
{
    /* {  (A)   , (degree)} */
    {   1    , 0.000   },
    {  10    , 0.000   },
};
const ct_phase_shift_t g_ct_phase_shift_phase_y[] =
{
    /* {  (A)   , (degree)} */
    {   1    , 0.000   },
    {  10    , 0.000   },
};
const ct_phase_shift_t g_ct_phase_shift_phase_b[] =
{
    /* {  (A)   , (degree)} */
    {   1    , 0.000   },
    {  10    , 0.000   },
};
#endif

const uint16_t g_ct_phase_shift_size_phase_r = sizeof(g_ct_phase_shift_phase_r) / sizeof(ct_phase_shift_t);
const uint16_t g_ct_phase_shift_size_phase_y = sizeof(g_ct_phase_shift_phase_y) / sizeof(ct_phase_shift_t);
const uint16_t g_ct_phase_shift_size_phase_b = sizeof(g_ct_phase_shift_phase_b) / sizeof(ct_phase_shift_t);


/***********************************************************************************************************************
Private global variables and functions
***********************************************************************************************************************/
