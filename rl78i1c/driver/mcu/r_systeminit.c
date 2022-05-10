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
* File Name    : r_systeminit.c
* Version      : 
* Device(s)    : RL78/I1C
* Tool-Chain   : CCRL
* Description  : This file implements system initializing function.
* Creation Date: 
***********************************************************************************************************************/

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_cgc.h"
#include "r_cg_port.h"
#include "r_cg_dsadc.h"
#include "r_cg_tau.h"
#include "r_cg_wdt.h"
#include "r_cg_sau.h"           /* Serial Driver */
#include "r_cg_it.h"           /* Interval timer */
#include "r_cg_rtc.h"
//#include "r_cg_intp.h"
#include "r_cg_macl.h"
#include "r_cg_adc.h"
#include "r_cg_dtc.h"
#include "r_cg_elc.h"
//#include "r_cg_vbatt.h"
#include "r_cg_lvd.h"
/* Start user code for include. Do not edit comment generated here */
#include "wrp_mcu.h"
#include "startup.h"
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
#ifdef __ICCRL78__

/* Set option bytes : 7E3BA2 */
#pragma location = "OPTBYTE"
__root const uint8_t opbyte0 = 0x7EU;
#pragma location = "OPTBYTE"
__root const uint8_t opbyte1 = 0x33U;
#pragma location = "OPTBYTE"
__root const uint8_t opbyte2 = 0xA2U;

/* Set OCD option bytes : 84 */
#pragma location = "OPTBYTE"
__root const uint8_t opbyte3 = 0x84U;

/* Set security ID */
#pragma location = "SECUID"
__root const uint8_t secuid[10] = 
    {0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U};

/* Set Debug Monitor Area value 
 *  This reserved area is not need if using IAR Embedded Workbench debug tool
 *  But if using rl78-elf-gdb debug tool and not reserved this area
 *      An error will happen, and the program will not be downloaded
 *      Therefore, the whole flash (including dataflash) must be erased before programming
 *  
*/
#pragma location = "DEBUGMON1"
__root const uint8_t debugmon1[2] = 
    {0xFFU, 0xFFU};

#pragma location = "DEBUGMON2"
__root const uint8_t debugmon2[10] =
    {0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU};

#endif /* __ICCRL78__ */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_Systeminit
* Description  : This function initializes every macro.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_Systeminit(void)
{
    PIOR0 = 0x00U;
    MCU_Delay(50000);   /* Delay 50ms */
    R_PORT_Create();
	R_LVD_Create();
	R_LVD_Start_VRTC();
	MCU_Delay(5000);
	while(LVDVRTCF == 1)
	{
		MCU_Delay(5000);
	}
    R_CGC_Create();
    //R_RTC_Create();
    
    // R_SAU0_Create();
    R_SAU1_Create();
    R_TAU0_Create();
	
    R_DSADC_Create();
//    R_INTC_Create();
    R_WDT_Create();
  //  R_LVD_Create();
  //  R_INTC_Create();
    R_MACL_Create();
    R_ADC_Create();
    R_DTC_Create();
    R_ELC_Create();
 ///   R_VBATT_Create();
    R_IT_Create();
    CRC0CTL = 0x80U;
    IAWCTL = 0x80U;
}


/***********************************************************************************************************************
* Function Name: hdwinit
* Description  : This function initializes hardware setting.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
#ifdef __CCRL__
void hdwinit(void)
{
    DI();
    R_Systeminit();
    EI();
}
#endif /* __CCRL__ */

#ifdef __ICCRL78__
int __low_level_init(void)
{
    DI();
    R_Systeminit();
    EI();
    /* Must return 1 to continue initialization of data, else it will jump to main directly */
    return (1);
}
#endif /* __ICCRL78__ */

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
