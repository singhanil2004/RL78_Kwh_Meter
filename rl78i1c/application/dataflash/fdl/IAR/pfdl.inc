;*******************************************************************************
; Library       : Flash Data Library T04 (PicoFDL)
;
; File Name     : pfdl.inc
; Device(s)     : RL78
; Version       : V1.05 (for IAR V2)
; Description   : Assembler language API definition of the Flash Data Library
;******************************************************************************
; DISCLAIMER
; This software is supplied by Renesas Electronics Corporation and is only
; intended for use with Renesas products. No other uses are authorized. This
; software is owned by Renesas Electronics Corporation and is protected under
; all applicable laws, including copyright laws.
; THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
; THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
; LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
; AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
; TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
; ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
; FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
; ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
; BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
; Renesas reserves the right, without notice, to make changes to this software
; and to discontinue the availability of this software. By using this software,
; you agree to the additional terms and conditions found by accessing the
; following link:
; http://www.renesas.com/disclaimer
;
; Copyright (C) 2015 Renesas Electronics Corporation. All rights reserved.
;*******************************************************************************



#ifdef __PFDL_INC_INCLUDED

#else

#define __PFDL_INC_INCLUDED




;==============================================================================================
; constant definitions
;==============================================================================================


; PFDL command code definitions
; -----------------------------------------------------
PFDL_CMD_READ_BYTES                     EQU       00H
PFDL_CMD_IVERIFY_BYTES                  EQU       06H
PFDL_CMD_BLANKCHECK_BYTES               EQU       08H
PFDL_CMD_WRITE_BYTES                    EQU       04H
PFDL_CMD_ERASE_BLOCK                    EQU       03H


; status code definitions returned by the PFDL functions

; operation related status code
PFDL_OK                                 EQU       000H
PFDL_IDLE                               EQU       030H
PFDL_BUSY                               EQU       0FFH
; flash access related status code
PFDL_ERR_PROTECTION                     EQU       010H
PFDL_ERR_ERASE                          EQU       01AH
PFDL_ERR_MARGIN                         EQU       01BH
PFDL_ERR_WRITE                          EQU       01CH
PFDL_ERR_PARAMETER                      EQU       005H



; ---------------------------------------------------------------------------------------------------------------
;  Block type:   standard function
; ---------------------------------------------------------------------------------------------------------------
; Purpose:      Unconditional activation of the Data Flash Library.
;               Based on the descriptor data:
;               - the flash firmware will be initialized for data-flash access only
;               - the internal timing and low-voltage capability will be configured according to the descriptor
;               After successful initialization the flash clock is ON and the PFDL is ready to use.
;
;               CAUTION:
;               Due to the code size minimization no plausibility checks are done by the PicoFDL.
;               Neither configuration, frequency range nor data flash size will be checked by the library.
;
;  Input:        AX = address of the PFDL descriptor variable
;  Output:       -
;  Destroyed:    X, HL, C
;  Return:       A = PFDL status
; ---------------------------------------------------------------------------------------------------------------
 EXTERN PFDL_Open


; ---------------------------------------------------------------------------------------------------------------
;  Block type:   standard function
; ---------------------------------------------------------------------------------------------------------------
;  Purpose:      Disable data flash access unconditionally.
;                If any command is just executed, PFDL_Close will stop it immediately.
;                After return the flash clock is switched OFF.
;  Input:        -
;  Output:       -
;  Destroyed:    C
;  Return:       -
; ---------------------------------------------------------------------------------------------------------------
 EXTERN PFDL_Close


; ---------------------------------------------------------------------------------------------------------------
;  Block type:   standard function
; ---------------------------------------------------------------------------------------------------------------
;  Purpose:      Initiating execution of the PFDL request related to the data flash.
;                The corresponding request variable has to be parametrized before.
;
;                request_pstr->index_u16        :   byte-index or block-number within PFDL-pool
;                request_pstr->data_pu08        :   start address of the RAM data that should be read/written
;                request_pstr->bytecount_u16    :   number of bytes has to be read/written
;                request_pstr->command_enu      :   command code
;
;                CAUTION:
;                Due to the code size minimization no plausibility checks are done by the PFDL.
;
;  Input:        AX = pointer to PFDL request variable
;  Output:       -
;  Destroyed:    X, BC, DE, HL
;  Return:       A = status of the request
; ---------------------------------------------------------------------------------------------------------------
 EXTERN PFDL_Execute


; ---------------------------------------------------------------------------------------------------------------
;  Block type:   standard function
; ---------------------------------------------------------------------------------------------------------------
;  Purpose:      Continuation of command execution and status update of requests being under execution.
;  Input:        -
;  Output:       -
;  Destroyed:    C
;  Return:       A = PFDL status
; ---------------------------------------------------------------------------------------------------------------
 EXTERN PFDL_Handler



; ---------------------------------------------------------------------------------------------------------------
;  Block type:   standard function
; ---------------------------------------------------------------------------------------------------------------
;  Purpose:      Returns the start address of the library version string
;
;  Input:        -
;  Output:       -
;  Destroyed:    -
;  Return:       A  = HIGH-byte of starting address of the zero-terminated version string
;                DE = LOW-word of that address
; ---------------------------------------------------------------------------------------------------------------
 EXTERN PFDL_GetVersionString



#endif
