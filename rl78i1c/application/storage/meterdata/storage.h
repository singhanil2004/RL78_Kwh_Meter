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
* File Name    : storage.h
* Version      : 1.00
* Device(s)    : RL78/I1C
* Tool-Chain   : CCRL
* H/W Platform : RL78/I1C Energy Meter Platform
* Description  : Storage Source File
***********************************************************************************************************************/

#ifndef _STORAGE_H
#define _STORAGE_H

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include "format.h"     /* Storage Format Header */

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
/* Error Status */
#define STORAGE_OK                  0       /* OK */
#define STORAGE_ERROR               1       /* Error */
#define STORAGE_ERROR_DATA_CORRUPT  2       /* Data Storage in EEPROM is corrupted */

/* Warning Status */
#define STORAGE_NOT_FORMATTED       3       /* Storage is NOT formatted */

/* Bit field selection for Backup/Restore */
#define STORAGE_ITEM_NONE           0x00    /* None selection */

#define STORAGE_ITEM_SYS_STATE      0x04    /* Select EM System State */
#define STORAGE_ITEM_RTC_TIME       0x08    /* Select RTC Time */
#define STORAGE_ITEM_ALL            (   STORAGE_ITEM_SYS_STATE  |\
                                        STORAGE_ITEM_RTC_TIME   )   /* Select all */

#define STORAGE_ITEM_ALL_NOT_RTC    ( STORAGE_ITEM_SYS_STATE  )   /* Select all, not RTC */

/***********************************************************************************************************************
Variable Externs
***********************************************************************************************************************/

/***********************************************************************************************************************
Functions Prototypes
***********************************************************************************************************************/
/* STORAGE Module Controller */
uint8_t STORAGE_Init(uint8_t is_checking);                              /* Storage Initialization */
uint8_t STORAGE_Format(void);                                           /* Storage Format Device */

/* STORAGE Module Write/Add/Backup/Restore */
/* Only call when the Storage device is formatted,
 * if not, error will occurred */
uint8_t STORAGE_Backup(uint8_t selection);                              /* Storage Backup */
uint8_t STORAGE_Restore(uint8_t selection);                             /* Storage Restore */
uint8_t STORAGE_ReadPlatformInfo(PLATFORM_INFO* p_platform_info);       /* Storage Read Platform Information */
uint8_t STORAGE_WritePlatformInfo(PLATFORM_INFO* p_platform_info);      /* Storage Write Platform Information */
uint8_t STORAGE_AddEnergyLog(ONE_MONTH_ENERGY_DATA_LOG* p_energy_log);  /* Storage Add Energy Log Record */
uint8_t STORAGE_AddTamperLog(ONE_TAMPER_DATA_LOG* p_tamper_log);        /* Storage Add Tamper Log record */
uint8_t STORAGE_GetEnergyLog(ONE_MONTH_ENERGY_DATA_LOG* p_energy_log);  /* Storage Get Tamper Log record */
uint8_t STORAGE_GetTamperLog(ONE_TAMPER_DATA_LOG* p_tamper_log);        /* Storage Get Tamper Log record */

/* Callback, need register to driver*/
void STORAGE_RtcCallback(void);                                         /* RTC Constand Interrupt Callback */


#endif /* _STORAGE_H */

