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
* File Name    : r_cg_port.c
* Version      : 
* Device(s)    : RL78/I1C
* Tool-Chain   : CCRL
* Description  : This file implements device driver for PORT module.
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
#include "r_cg_port.h"
//#include "r_cg_vbatt.h"
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_PORT_Create
* Description  : This function initializes the Port I/O for Global Reference Meter.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_PORT_Create(void)
{
    /*  3P4W PROTOTYPE METER PINS MAPPING
     *--------------------------------------------------------------------------------------------------------------------------------------------
     *              7               6               5               4               3               2               1               0
     *--------------------------------------------------------------------------------------------------------------------------------------------
     *  P0  MCU     TXD0            RXD0            PORT_IO         PORT_O          PORT_O          PORT_PW                         
     *      NAME    FEA_UART0_TXD   FEA_UART0_RXD   FEA_UART1_GPIO  FEA_UART1_TXD   FEA_UART1_RXD   VCC_5
     *--------------------------------------------------------------------------------------------------------------------------------------------
     *  P1  MCU     SEG11           SEG10           SEG9            SEG8            SEG7            SEG6            SEG5            SEG4
     *      NAME    SEG11           SEG10           SEG9            SEG8            SEG7            SEG6            SEG5            SEG4
     *--------------------------------------------------------------------------------------------------------------------------------------------
     *  P2  MCU                                     ANI5            ANI4            ANI3            ANI2/EXLVD      AVREFM          AVREFP
     *      NAME                                    ADC2            ADC1            ADC0            EXLVD           NC              NC
     *--------------------------------------------------------------------------------------------------------------------------------------------      
     *  P3  MCU     PORT            PORT_PW         PORT_O          PORT_O          PORT_PW         PORT_O          PORT_O          PORT_O
     *      NAME    NC              VCC-3           FEA_LED0        FEA_LED1        VCC-1           FEA_LED2        FEA_LED3        FEA_LED4
     *--------------------------------------------------------------------------------------------------------------------------------------------      
     *  P4  MCU                                                                     PORT_PW         PORT            PORT            E1
     *      NAME                                                                    VCC-6           NC              NC              TOOL0*
     *--------------------------------------------------------------------------------------------------------------------------------------------
     *  P5  MCU     PORT_PW         TXD2            RXD2            PORT            PORT_PW         PORT_O          PORT_IO         PORT
     *      NAME    VCC-2           FEA_IR_TXD      FEA_IR_RXD      NC              AUX_OUT         FEA_PWRSW0      FEA_UART0_GPIO  NC
     *--------------------------------------------------------------------------------------------------------------------------------------------
     *  P6  MCU                                                                                     PORT_O          SDAA0           SCLA0
     *      NAME                                                                                    FEA_IIC0_WP     FEA_IIC0_SDA    FEA_IIC0_SCL
     *--------------------------------------------------------------------------------------------------------------------------------------------
     *  P7  MCU     SEG23           SEG22           SEG21           SEG20           SEG19           SEG18           SEG17           SEG16
     *      NAME    SEG23           SEG22           SEG21           SEG20           SEG19           SEG18           SEG17           SEG16
     *--------------------------------------------------------------------------------------------------------------------------------------------
     *  P8  MCU                                     PORT_PW         PORT            SEG15           SEG14           SEG13           SEG12
     *      NAME                                    VCC-4           NC              SEG15           SEG14           SEG13           SEG12   
     *--------------------------------------------------------------------------------------------------------------------------------------------
     *  P12 MCU     CAPH            CAPL            PORT_I          XT2             XT1             NA              NA
     *      NAME    CAPH            CAPL            FEA_HALL_SENS   XT2             XT1             NA              NA
     *--------------------------------------------------------------------------------------------------------------------------------------------      
     *  P13 MCU     INTP0
     *      NAME    FEA_SW2
     *--------------------------------------------------------------------------------------------------------------------------------------------
     *  P15 MCU                                                                                     RTCIC2          RTCIC1          RTCOUT
     *      NAME                                                                                    FEA_SW1         FEA_CASE        MCU_1HZ 
     *--------------------------------------------------------------------------------------------------------------------------------------------
    */
//    /* Setting for not used segments EVB */
//    PFSEG2 = _80_PFSEG23_SEG | _40_PFSEG22_SEG | _20_PFSEG21_SEG | _10_PFSEG20_SEG |
//             _08_PFSEG19_SEG | _04_PFSEG18_SEG | _02_PFSEG17_SEG | _01_PFSEG16_SEG; 
//    PFSEG3 = _00_PFSEG31_PORT | _00_PFSEG30_PORT | _00_PFSEG29_PORT | _00_PFSEG28_PORT |
//             _00_PFSEG27_PORT | _00_PFSEG26_PORT | _00_PFSEG25_PORT | _00_PFSEG24_PORT; 
//    PFSEG4 = _00_PFSEG39_PORT | _00_PFSEG38_PORT | _00_PFSEG37_PORT | _00_PFSEG36_PORT |
//             _00_PFSEG35_PORT | _00_PFSEG34_PORT | _00_PFSEG33_PORT | _00_PFSEG32_PORT;
//    PFSEG5 = _00_PFSEG41_PORT | _00_PFSEG40_PORT;

//    /* Set port value */
//    P0 = _80_Pn7_OUTPUT_1 | _40_Pn6_OUTPUT_1 | _20_Pn5_OUTPUT_1 | _10_Pn4_OUTPUT_1 | 
//         _08_Pn3_OUTPUT_1 | _04_Pn2_OUTPUT_1;
//    P3 = _00_Pn7_OUTPUT_0 | _40_Pn6_OUTPUT_1 | _20_Pn5_OUTPUT_1 | _10_Pn4_OUTPUT_1 | 
//         _08_Pn3_OUTPUT_1 | _04_Pn2_OUTPUT_1 | _02_Pn1_OUTPUT_1 | _01_Pn0_OUTPUT_1;
//    P4 = _08_Pn3_OUTPUT_1 | _04_Pn2_OUTPUT_1 | _02_Pn1_OUTPUT_1;
////    P5 = _80_Pn7_OUTPUT_1 | _40_Pn6_OUTPUT_1 | _20_Pn5_OUTPUT_1 | _00_Pn4_OUTPUT_0 | 
////         _08_Pn3_OUTPUT_1 | _04_Pn2_OUTPUT_1 | _02_Pn1_OUTPUT_1 | _00_Pn0_OUTPUT_0;
//    P6 = _04_Pn2_OUTPUT_1;
////    P8 = _20_Pn5_OUTPUT_1;
    
//    /* Set port input/output mode */
//    ADPC = _00_ADPC_DI_OFF;
    
//    ISCLCD = _02_ISCVL3_INPUT_EFFECTIVE | _00_ISCCAP_INPUT_INEFFECTIVE;
    
//    PM0 = _00_PMn7_MODE_OUTPUT | _40_PMn6_MODE_INPUT |_00_PMn5_MODE_OUTPUT | _00_PMn4_MODE_OUTPUT |
//          _08_PMn3_MODE_INPUT | _00_PMn2_MODE_OUTPUT;
//    PM1 = _00_PMn7_MODE_OUTPUT | _00_PMn6_MODE_OUTPUT |_00_PMn5_MODE_OUTPUT | _00_PMn4_MODE_OUTPUT |
//          _00_PMn3_MODE_OUTPUT | _00_PMn2_MODE_OUTPUT | _00_PMn1_MODE_OUTPUT | _00_PMn0_MODE_OUTPUT;
//    PM3 = _00_PMn7_MODE_OUTPUT | _00_PMn6_MODE_OUTPUT |_00_PMn5_MODE_OUTPUT | _00_PMn4_MODE_OUTPUT |
//          _00_PMn3_MODE_OUTPUT | _00_PMn2_MODE_OUTPUT | _00_PMn1_MODE_OUTPUT | _00_PMn0_MODE_OUTPUT;
//    PM4 = _00_PMn3_MODE_OUTPUT | _00_PMn2_MODE_OUTPUT | _00_PMn1_MODE_OUTPUT | _01_PMn0_MODE_INPUT;
////    PM5 = _00_PMn7_MODE_OUTPUT | _00_PMn6_MODE_OUTPUT |_20_PMn5_MODE_INPUT | _10_PMn4_MODE_INPUT |
////          _00_PMn3_MODE_OUTPUT | _00_PMn2_MODE_OUTPUT | _00_PMn1_MODE_OUTPUT | _00_PMn0_MODE_OUTPUT;
//    PM6 = _08_PMn3_NOT_USE | _00_PMn2_MODE_OUTPUT | _00_PMn1_MODE_OUTPUT | _00_PMn0_MODE_OUTPUT;
//    PM7 = 0xFF;//_00_PMn7_MODE_OUTPUT | _00_PMn6_MODE_OUTPUT |_00_PMn5_MODE_OUTPUT | _00_PMn4_MODE_OUTPUT |
//        //  _//00_PMn3_MODE_OUTPUT | _00_PMn2_MODE_OUTPUT | _00_PMn1_MODE_OUTPUT | _00_PMn0_MODE_OUTPUT;
////    PM8 = _00_PMn5_MODE_OUTPUT | _00_PMn4_MODE_OUTPUT | 
////          _00_PMn3_MODE_OUTPUT | _00_PMn2_MODE_OUTPUT | _00_PMn1_MODE_OUTPUT | _00_PMn0_MODE_OUTPUT;
//    PM12 = _20_PMn5_MODE_INPUT;
////    PM15 = _04_PMn2_MODE_INPUT | _02_PMn1_MODE_INPUT;
    
//    /* Set port pull-up */
////    PU15 = _04_PUn2_PULLUP_ON | _00_PUn1_PULLUP_OFF | _00_PUn0_PULLUP_OFF;
    
//    P0 |= 0x0C;		//kWh Pin P0.2 & kVArh Pin P0.3,
//    PM0 &= ~0x0C;


    PFSEG0 = _00_PFSEG07_PORT | _00_PFSEG06_PORT | _00_PFSEG05_PORT | _00_PFSEG04_PORT;
    PFSEG1 = _00_PFSEG11_PORT | _00_PFSEG10_PORT | _00_PFSEG09_PORT | _00_PFSEG08_PORT | _F0_PFSEG1_DEFAULT_VALUE;
    PFSEG2 = _00_PFSEG20_PORT | _00_PFSEG19_PORT | _00_PFSEG18_PORT | _00_PFSEG17_PORT | _00_PFSEG16_PORT | 
             _E0_PFSEG2_DEFAULT_VALUE;
    PFSEG3 =  _00_PFSEG25_PORT | _00_PFSEG24_PORT | _F0_PFSEG3_DEFAULT_VALUE;
    
    ISCLCD = _02_VL3_BUFFER_VALID | _01_CAPLH_BUFFER_VALID;
    P0 = _00_Pn7_OUTPUT_0;
    P1 = _00_Pn7_OUTPUT_0 | _00_Pn6_OUTPUT_0 | _00_Pn5_OUTPUT_0 | _00_Pn4_OUTPUT_0 | _00_Pn3_OUTPUT_0 | 
         _00_Pn2_OUTPUT_0 | _00_Pn1_OUTPUT_0 | _00_Pn0_OUTPUT_0;
    P4 = _00_Pn3_OUTPUT_0;
    P7 = _00_Pn4_OUTPUT_0 | _00_Pn3_OUTPUT_0 | _00_Pn2_OUTPUT_0 | _00_Pn1_OUTPUT_0 | _00_Pn0_OUTPUT_0;
    P12 = _00_Pn7_OUTPUT_0 | _00_Pn6_OUTPUT_0 | _00_Pn5_OUTPUT_0;
    PU4 = _01_PUn0_PULLUP_ON;
    POM0 = _00_POMn5_NCH_OFF;
    POM1 = _00_POMn7_NCH_OFF | _00_POMn6_NCH_OFF | _00_POMn5_NCH_OFF | _00_POMn4_NCH_OFF | _00_POMn3_NCH_OFF |
           _00_POMn2_NCH_OFF;
    PM0 = _1F_PM0_DEFAULT_VALUE | _00_PMn7_MODE_OUTPUT | _40_PMn6_MODE_INPUT | _20_PMn5_MODE_INPUT;
    PM1 = _00_PMn7_MODE_OUTPUT | _00_PMn6_MODE_OUTPUT | _00_PMn5_MODE_OUTPUT | _00_PMn4_MODE_OUTPUT | 
          _00_PMn3_MODE_OUTPUT | _00_PMn2_MODE_OUTPUT | _00_PMn1_MODE_OUTPUT | _00_PMn0_MODE_OUTPUT;
    PM4 = _F6_PM4_DEFAULT_VALUE | _00_PMn3_MODE_OUTPUT;
    PM6 = _F8_PM6_DEFAULT_VALUE | _04_PMn2_MODE_INPUT;
    PM7 = _E0_PM7_DEFAULT_VALUE | _00_PMn4_MODE_OUTPUT | _00_PMn3_MODE_OUTPUT | _00_PMn2_MODE_OUTPUT | 
          _00_PMn1_MODE_OUTPUT | _00_PMn0_MODE_OUTPUT;
    PM12 = _1F_PM12_DEFAULT_VALUE | _00_PMn7_MODE_OUTPUT | _00_PMn6_MODE_OUTPUT | _00_PMn5_MODE_OUTPUT;
}

/* Start user code for adding. Do not edit comment generated here */
/***********************************************************************************************************************
* Function Name: R_PORT_StopCreate
* Description  : This function turn off unnesccesary port pin before stopping to conserve power
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_PORT_StopCreate(void)
{
    /*  Turn off un necessary port before sleeping 
    *   Only keep below items:
    *       1. AUX_OUT (for Switch and Case Open)
    *       2. 
    *   Magnetics not keep due to high current consumption:
    */
    PFSEG0 = 0xF0;
    PFSEG1 = 0xFF;
    PFSEG2 = 0xFF;
    PFSEG3 = 0x00;
    PFSEG4 = 0x00;
    PFSEG5 = 0x00;
    
    P0 = 0x00;
    P1 = 0x00;
    P2 = 0x00;
    P3 = 0x00;
    P4 = 0x00;
//    P5 = 0x08;
    P6 = 0x00;
    P7 = 0x00;
//    P8 = 0x00; 
    P12 = 0x00;
    
    PU4 = 0x01;
//    PU15 = 0x04;
    
    ADPC = 0x00;
    ISCLCD = 0x02;
    
    PM0 = 0x00;
    PM1 = 0x00;
    PM2 = 0x00;
    PM3 = 0x00;
    PM4 = 0x01;
//    PM5 = 0x00;
    PM6 = 0x00;
    PM7 = 0xff;
//    PM8 = 0x00;
    PM12 = 0x1E;
//    PM15 = 0x06;
}
/* End user code. Do not edit comment generated here */
