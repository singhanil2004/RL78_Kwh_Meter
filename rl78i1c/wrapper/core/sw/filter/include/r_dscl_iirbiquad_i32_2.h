/******************************************************************************* 
* DISCLAIMER 
* This software is supplied by Renesas Electronics Corporation and is only  
* intended for use with Renesas products. No other uses are authorized. This  
* software is owned by Renesas Electronics Corporation and is protected under 
* all applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING 
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT 
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE  
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. 
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS  
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE  
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR 
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE 
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES. 
* Renesas reserves the right, without notice, to make changes to this software 
* and to discontinue the availability of this software. By using this software, 
* you agree to the additional terms and conditions found by accessing the  
* following link: 
* http://www.renesas.com/disclaimer 
* 
* Copyright (C) 2016 Renesas Electronics Corporation. All rights reserved.     
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_dscl_iirbiquad.h
* Version      : 0.1
* Description  : INTERNAL header file for lattice fir filter
***********************************************************************************************************************/
/***********************************************************************************************************************
* History      : YYYY.MM.DD  Version  Description 
*              : 2016.07.21  0.10     First draft
***********************************************************************************************************************/

#ifndef R_DSCL_IIRBIQUAD32_2_H
#define R_DSCL_IIRBIQUAD32_2_H

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "typedef.h"

/***********************************************************************************************************************
Macro definitions 
***********************************************************************************************************************/
#define R_DSCL_IIRBIQUAD2_MAX_NUM_OF_PORT                        (3U)
#define R_DSCL_IIRBIQUAD2_MAX_NUM_OF_STAGES                      (3U)

/***********************************************************************************************************************
Typedef definitions 
***********************************************************************************************************************/

/***********************************************************************************************************************
Variable Externs
***********************************************************************************************************************/


/***********************************************************************************************************************
Function prototypes
***********************************************************************************************************************/
void R_DSCL_IIRBiquad_i32i32_Reset_2(uint8_t port);
uint8_t R_DSCL_IIRBiquad_i32i32_c_nr_2(NEAR_PTR int32_t input, NEAR_PTR int32_t * output, uint16_t port);

#endif /* R_DSCL_IIRBIQUAD32_2_H */
/* End of file */
