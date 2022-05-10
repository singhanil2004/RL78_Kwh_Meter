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
* File Name    : r_cg_macl.h
* Version      : 
* Device(s)    : RL78/I1C
* Tool-Chain   : CCRL
* Description  : This file implements device driver for MACL module.
* Creation Date: 
***********************************************************************************************************************/
#ifndef MACL_H
#define MACL_H

/***********************************************************************************************************************
Macro definitions (Register bit)
***********************************************************************************************************************/
/*
    Multiplier-Accumulator Control Register (MULC)
*/
/* Control of MACL accumulator mode (MACMODE) */
#define _00_MACL_MUL_MODE                       (0x00U) /* Multiplier only mode */
#define _80_MACL_MAC_MODE                       (0x80U) /* Multiply-Accumulate mode */

/* Control of MACL sign mode (MACSM) */
#define _00_MACL_UNSIGNED_MODE                  (0x00U) /* Unsigned operation */
#define _40_MACL_SIGNED_MODE                    (0x40U) /* Signed operation */

/* Control of MACL fixed-point mode (MULFRAC) */
#define _00_MACL_FIXED_POINT_MODE               (0x00U) /* Fixed-point operation */
#define _10_MACL_FRACTION_MODE                  (0x10U) /* Fraction operation */

/* MAC operation overflow status (MACOF) */
#define _00_MACL_MAC_RESULT_NORMAL              (0x00U) /* MAC operation is normal*/
#define _04_MACL_MAC_RESULT_OVERFLOW            (0x04U) /* MAC operation is overflow */

/* MAC result sign (MACSF) */
#define _00_MACL_MAC_RESULT_POSITIVE            (0x00U) /* MAC result is positive */
#define _02_MACL_MAC_RESULT_NEGATIVE            (0x02U) /* MAC result is negative */

/* Multiplication operating status (MULST) */
#define _00_MACL_MUL_COMPLETE                   (0x00U) /* Multiplication is idle */
#define _01_MACL_MUL_OPERATING                  (0x01U) /* Multiplication is in progress */


/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/


/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Global functions
***********************************************************************************************************************/
void R_MACL_Create(void);
//void R_MACL_Start(void);
void R_MACL_EnableInterrupt(void);
void R_MACL_DisableInterrupt(void);

/* Start user code for function. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#endif /* MACL_H */