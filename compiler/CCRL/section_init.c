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
* File Name    : section_init.c
* Version      : 
* Device(s)    : RL78/I1C
* Tool-Chain   : CCRL
* Description  : Section initialization routine
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

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/
typedef struct 
{
    char NEAR_PTR *ram_sectop;        /*Start address of section                  */
    char NEAR_PTR *ram_secend;        /*End address + 1 of section                */
} bsec_t;

typedef struct 
{
    char FAR_PTR *rom_sectop;         /*Start address of copy source section      */
    char FAR_PTR *rom_secend;         /*End address + 1 of copy source section    */
    char NEAR_PTR *ram_sectop;        /*Start address of copy destination section */
} dsec_t;

/***********************************************************************************************************************
Imported global variables and functions (from other files)
***********************************************************************************************************************/

/***********************************************************************************************************************
Exported global variables and functions (to be accessed by other files)
***********************************************************************************************************************/

/***********************************************************************************************************************
Private global variables and functions
***********************************************************************************************************************/

/* Not use the bsec_table: since all RAM area is init by cstart before */
#if 0
const bsec_t bsec_table[] = 
{
    {
        (char NEAR_PTR *)__sectop(".bss"),
        (char NEAR_PTR *)__secend(".bss")
    },
    {
        (char NEAR_PTR *)__sectop(".sbss"),
        (char NEAR_PTR *)__secend(".sbss")
    },
    {
        (char NEAR_PTR *)__sectop("ADC_Data_n"),
        (char NEAR_PTR *)__secend("ADC_Data_n")
    },
    #ifdef __DEBUG    
    {
        (char NEAR_PTR *)__sectop("EMMetroBss_n"),
        (char NEAR_PTR *)__secend("EMMetroBss_n")
    },
    {
        (char NEAR_PTR *)__sectop("WrpAdcBss_n"),
        (char NEAR_PTR *)__secend("WrpAdcBss_n")
    }
    #endif /* __DEBUG */
};
#define BSEC_MAX                        (sizeof(bsec_table) / sizeof(bsec_t))   /*Number of BSS sections to be initialized to 0*/
#endif /* 0 or 1 */

const dsec_t dsec_table[] = 
{
    {
        __sectop(".data"),
        __secend(".data"),
        (char NEAR_PTR *)__sectop(".dataR")
    },
    {
        __sectop(".sdata"),
        __secend(".sdata"),
        (char NEAR_PTR *)__sectop(".sdataR")
    },
    #ifdef __DEBUG 
    {
        __sectop("EMMetroData_n"),
        __secend("EMMetroData_n"),
        (char NEAR_PTR *)__sectop("EMMetroDataR_n")
    },
    {
        __sectop("WrpAdcData_n"),
        __secend("WrpAdcData_n"),
        (char NEAR_PTR *)__sectop("WrpAdcDataR_n")
    },
    #endif /* __DEBUG */
};

#define DSEC_MAX                        (sizeof(dsec_table) / sizeof(dsec_t))   /*Number of DATA sections to be copied*/

/***********************************************************************************************************************
* Function Name: void section_init(void)
* Description  : Initialize memory sections:
*              : . ROMization
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void section_init(void)
{
    unsigned int i;
    char FAR_PTR *rom_p;
    char NEAR_PTR *ram_p;
        
    /* Data Section Init */
    for (i = 0; i < DSEC_MAX; i++) 
    {
        rom_p = dsec_table[i].rom_sectop;
        ram_p = dsec_table[i].ram_sectop;
        for ( ; rom_p != dsec_table[i].rom_secend; rom_p++, ram_p++) 
        {
            *ram_p = *rom_p;
        }
    }
}
