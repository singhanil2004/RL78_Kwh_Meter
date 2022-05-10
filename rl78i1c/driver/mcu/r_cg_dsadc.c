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
* File Name    : r_cg_dsadc.c
* Version      : 
* Device(s)    : RL78/I1C
* Tool-Chain   : CCRL
* Description  : This file implements device driver for DSADC module.
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
#include "r_cg_dsadc.h"
/* Start user code for include. Do not edit comment generated here */
#include "wrp_mcu.h"
#include "wrp_em_sw_config.h"
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
static uint16_t g_reg_phase0;
static uint16_t g_reg_phase1;
static uint16_t g_reg_phase2;
static uint16_t g_reg_phase3;
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_DSADC_Create
* Description  : This function initializes the DSAD converter.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_DSADC_Create(void)
{
		CRMK = 0U;
		HOCOFC = 0x40;
		if((HOCOFC&0x01)==0)
			HOCOFC = 0x41;							//Continous Operation Mode with Interrupt Enabled
		NOP();
    DSADCK = _00_DSAD_OPERATION_CLOCK_FIH;      /* OCO clock selected */
    DSADCEN = 1U;   /* Supply clock to ADC */
    DSAMK = 1U;     /* Disalbe INTSAD interrupt */
    DSAIF = 0U;     /* Clear INTSAD interrupt flag */
    
    /* Checkpoint */
    DSAZMK0 = 1U;      /* disable INTDSADZC0 interrupt */
    DSAZIF0 = 0U;       /* clear INTDSADZC0 interrupt flag */
    DSAZMK1 = 1U;      /* disable INTDSADZC1 interrupt */
    DSAZIF1 = 0U;       /* clear INTDSADZC1 interrupt flag */   
    
    /* Set INTDSAD high priority */
    DSAPR0 = 0U;
    DSAPR1 = 0U;

    /* Set INTDSADZC0 low priority */
    DSAZPR00 = 1U;
    DSAZPR01 = 1U;
    
    /* Set INTDSADZC0 low priority */
    DSAZPR10 = 1U;
    DSAZPR11 = 1U;  
    
    /* Setup for ZC */
    DSADICR = 0U;
    DSADICR = _01_DSAD_ZC0_INPUT_CH1 | _00_DSAD_ZC0_DECTECTION_MODE_PULSE | _00_DSAD_ZC0_EDGE_SELECT_NONE |
              _00_DSAD_ZC1_INPUT_CH3 | _00_DSAD_ZC1_DECTECTION_MODE_PULSE | _00_DSAD_ZC0_EDGE_SELECT_NONE;  
    
    /* Sampling rate at 3906Hz, 24 bits resolution */
    DSADMR = _0000_DSAD_SAMPLING_FREQUENCY_0 | _0000_DSAD_RESOLUTION_24BIT; 
    DSADMR |= _0F0F_DSAD_ENABLE_ALLCH;

    /* High pass filter */
    #if 0
    R_DSADC_SetHighPassFilterOn(DSADCHANNEL3);     /* Turn on HPF on channel 3 - Neutral channel */
    R_DSADC_SetHighPassFilterOff(DSADCHANNEL2);
    R_DSADC_SetHighPassFilterOff(DSADCHANNEL1);
    R_DSADC_SetHighPassFilterOff(DSADCHANNEL0);
    #else
    R_DSADC_SetHighPassFilterOn(DSADCHANNEL3);
    R_DSADC_SetHighPassFilterOn(DSADCHANNEL2);
    R_DSADC_SetHighPassFilterOn(DSADCHANNEL1);
    R_DSADC_SetHighPassFilterOn(DSADCHANNEL0);
    R_DSADC_Set_CutOffFrequency(EM_SW_PROPERTY_ADC_HPF_CUTOFF_FREQUENCY);
    #endif /* 0 or 1 */

    /* Gain setting */
    DSADGCR0 = _00_DSAD_CH0_PGAGAIN_1 | _00_DSAD_CH1_PGAGAIN_1;
    DSADGCR1 = _00_DSAD_CH2_PGAGAIN_1 | _00_DSAD_CH3_PGAGAIN_1;
}

/***********************************************************************************************************************
* Function Name: R_DSADC_Start
* Description  : This function starts the DSAD converter.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_DSADC_Start(void)
{
    uint8_t wait;
    
    #if 1
    DSAMK = 1U;  /* disable INTDSAD interrupt */
    DSAIF = 0U;  /* clear INTDSAD interrupt flag */ 
    #endif /* 0 or 1 */
    
    DSADMR &= ~(_0F0F_DSAD_ENABLE_ALLCH);   /* Stop conversion */
    DSADMR |= _0F0F_DSAD_ENABLE_ALLCH;  /* Start conversion ADC channel 01-- */
    
    /* Wait for setup time */
    wait = _80_DSAD_WAIT_SETUP_TIME;
    while (1)
    {
        if (DSAIF == 1)
        {
            DSAIF = 0U;  /* Clear INTSAD interrupt flag */
            MCU_Delay(20);
            BIT_SELECT( DTCEN1, 5 ) = 1;
            wait--;
        }
        if (wait == 0)
        {
            break;
        }
    }
    
    #if 1
    DSAMK = 0U;  /* Enable INTSAD interrupt */
    #endif /* 0 or 1 */
}

/***********************************************************************************************************************
* Function Name: R_DSADC_ZC_Start
* Description  : This function starts the DSAD Zero Crossing Detection.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_DSADC_ZC_Start(void)
{
    /* Checkpoint */
    DSAZMK0 = 1U;      /* disable INTDSADZC0 interrupt */
    DSAZIF0 = 0U;       /* clear INTDSADZC0 interrupt flag */
    DSAZMK1 = 1U;      /* disable INTDSADZC1 interrupt */
    DSAZIF1 = 0U;       /* clear INTDSADZC1 interrupt flag */
    
    /* Clear the flag of level detection mode */
    DSADICLR |= _01_DSAD_ZC0_FLAG_CLEAR | _10_DSAD_ZC1_FLAG_CLEAR;
    DSAZMK0 = 0U;      /* Enable the INTDSADZC0 interrupt */
    DSAZMK1 = 0U;      /* Enable the INTDSADZC1 interrupt */
}

/***********************************************************************************************************************
* Function Name: R_DSADC_ZC_Stop
* Description  : This function stop the DSAD Zero Crossing Detection.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_DSADC_ZC_Stop(void)
{
    /* Checkpoint */
    DSAZMK0 = 0U;      /* disable INTDSADZC0 interrupt */
    DSAZIF0 = 0U;       /* clear INTDSADZC0 interrupt flag */
    DSAZMK1 = 0U;      /* disable INTDSADZC1 interrupt */
    DSAZIF1 = 0U;       /* clear INTDSADZC1 interrupt flag */   
}

/***********************************************************************************************************************
* Function Name: R_DSADC_StartWithoutGainAndPhase
* Description  : This function starts 4 ADC channels without gain and phase setting
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_DSADC_StartWithoutGainAndPhase(uint16_t channel_control)
{
    uint8_t wait;
    
    DSAMK = 1U;  /* Disable INTAD interrupt */
    DSAIF = 0U;  /* Clear INTSAD interrupt flag */  
    DSADMR &= ~_0F0F_DSAD_ENABLE_ALLCH; /* Stop conversion */
    
    /* Gain setting */
    DSADGCR0 = 0x00;
    DSADGCR1 = 0x00;
    
    /* Phase setting */
    DSADPHCR0 = 0x0000;
    DSADPHCR1 = 0x0000;
    DSADPHCR2 = 0x0000;
    DSADPHCR3 = 0x0000;
    
    /* High pass filter */

    #if 0
    if ((channel_control & 0x0008) != 0)
    {
        R_DSADC_SetHighPassFilterOn(DSADCHANNEL3);     /* Turn on HPF on channel 3 - Neutral channel */
    }
    R_DSADC_SetHighPassFilterOff(DSADCHANNEL2);     /* Turn off HPF on channel 2 */
    R_DSADC_SetHighPassFilterOff(DSADCHANNEL1);     /* Turn off HPF on channel 1 */
    R_DSADC_SetHighPassFilterOff(DSADCHANNEL0);     /* Turn off HPF on channel 0 */
    R_DSADC_Set_CutOffFrequency(HPF_4857_HZ);       /* Set cut off frequency for HPF */
    #else   
    if ((channel_control & 0x0008) != 0)
    {
        R_DSADC_SetHighPassFilterOn(DSADCHANNEL3);      /* Turn on HPF on channel 3 */
    }
    R_DSADC_SetHighPassFilterOn(DSADCHANNEL2);      /* Turn on HPF on channel 2 */
    R_DSADC_SetHighPassFilterOn(DSADCHANNEL1);      /* Turn on HPF on channel 1 */
    R_DSADC_SetHighPassFilterOn(DSADCHANNEL0);      /* Turn on HPF on channel 0 */
    R_DSADC_Set_CutOffFrequency(EM_SW_PROPERTY_ADC_HPF_CUTOFF_FREQUENCY);       /* Set cut off frequency for HPF */
    #endif /* 0 or 1 */

    DSADMR |= channel_control;      /* Start conversion ADC channel 0-3 */
    
    /* Wait for setup time */
    wait = _80_DSAD_WAIT_SETUP_TIME;
    while (1)
    {
        if(DSAIF == 1)
        {
            DSAIF = 0U;  /* Clear INTSAD interrupt flag */
            wait--;
        }
        if(wait == 0)
        {
            break;
        }
    }
    
    #if 1
    DSAMK = 0U;  /* Enable INTSAD interrupt */
    #endif /* 0 or 1 */
}

/***********************************************************************************************************************
* Function Name: R_ADC_StartChannel0Only
* Description  : This function starts the AD converter (CH0 only).
* Caution      : Only call this API on NM mode
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_DSADC_StartChannel0Only(void)
{
    DSAMK = 1U;  /* Disable INTAD interrupt */
    DSAIF = 0U;  /* Clear INTSAD interrupt flag */
    DSADPHCR0 = 0x0000;     /* Reset phase adjustment for channel 0 */
    DSADMR &= ~_0F0F_DSAD_ENABLE_ALLCH;     /* others off */
    DSADMR |= _0101_DSAD_ENABLE_CH0;        /* Start conversion ADC channel 0 only */
    
    #if 1
    DSAMK = 0U;  /* Enable INTSAD interrupt */
    #endif /* 0 or 1 */
}

/***********************************************************************************************************************
* Function Name: R_ADC_StartChannel0Only
* Description  : This function starts the AD converter (CH1 only).
* Caution      : Only call this API on NM mode
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_DSADC_StartChannel1Only(void)
{
    DSAMK = 1U;  /* Disable INTAD interrupt */
    DSAIF = 0U;  /* Clear INTSAD interrupt flag */
    DSADPHCR1 = 0x0000;     /* Reset phase adjustment for channel 0 */
    DSADMR &= ~_0F0F_DSAD_ENABLE_ALLCH;     /* others off */
    DSADMR |= _0202_DSAD_ENABLE_CH1;        /* Start conversion ADC channel 0 only */
    
    #if 1
    DSAMK = 0U;  /* Enable INTSAD interrupt */
    #endif /* 0 or 1 */
}

/***********************************************************************************************************************
* Function Name: R_ADC_StartChannel2Only
* Description  : This function starts the AD converter (CH2 only).
* Caution      : Only call this API on NM mode
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_DSADC_StartChannel2Only(void)
{
    DSAMK = 1U;  /* Disable INTAD interrupt */
    DSAIF = 0U;  /* Clear INTSAD interrupt flag */  
    DSADPHCR1 = 0x0000;                     /* Reset phase adjustment for channel 2 */
    DSADMR &= ~_0F0F_DSAD_ENABLE_ALLCH;     /* others off */
    DSADMR |= _0404_DSAD_ENABLE_CH2;        /* Start conversion ADC channel 2 only */
    
    #if 1
    DSAMK = 0U;  /* Enable INTSAD interrupt */
    #endif /* 0 or 1 */
}

/***********************************************************************************************************************
* Function Name: R_ADC_StartChannel3Only
* Description  : This function starts the AD converter (CH3 only).
* Caution      : Only call this API on NM mode
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_DSADC_StartChannel3Only(void)
{
    DSAMK = 1U;  /* Disable INTAD interrupt */
    DSAIF = 0U;  /* Clear INTSAD interrupt flag */  
    DSADPHCR3 = 0x0000;                     /* Reset phase adjustment for channel 2 */
    DSADMR &= ~_0F0F_DSAD_ENABLE_ALLCH;     /* others off */
    DSADMR |= _0808_DSAD_ENABLE_CH3;        /* Start conversion ADC channel 2 only */
    
    #if 1
    DSAMK = 0U;  /* Enable INTSAD interrupt */
    #endif /* 0 or 1 */
}

/***********************************************************************************************************************
* Function Name: R_ADC_StartCurrentChannels
* Description  : This function starts the AD converter (2 current channel, CH0 & CH2).
* Caution      : Only call this API on NM mode
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_DSADC_StartCurrentChannels(void)
{
    DSAMK = 1U;                                                     /* Disable INTAD interrupt */
    DSAIF = 0U;                                                     /* Clear INTSAD interrupt flag */   
    DSADMR |= (_0101_DSAD_ENABLE_CH0 | _0404_DSAD_ENABLE_CH2);      /* Start conversion ADC channel 0 and 2 only */
    DSADMR &= ~(_0202_DSAD_ENABLE_CH1 | _0808_DSAD_ENABLE_CH3);     /* others off */
    
    #if 1
    DSAMK = 0U;  /* Enable INTSAD interrupt */
    #endif /* 0 or 1 */
}

/***********************************************************************************************************************
* Function Name: R_ADC_BackupPhaseRegSetting
* Description  : This function backup register setting of DSADPHCR0 and DSADPHCR1
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_DSADC_BackupPhaseRegSetting(void)
{
    g_reg_phase0 = DSADPHCR0;
    g_reg_phase1 = DSADPHCR1;
    g_reg_phase2 = DSADPHCR2;
    g_reg_phase3 = DSADPHCR3;
}

/***********************************************************************************************************************
* Function Name: R_ADC_RestorePhaseRegSetting
* Description  : This function restore register setting of DSADPHCR0 and DSADPHCR1
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_DSADC_RestorePhaseRegSetting(void)
{
    DSADPHCR0 = g_reg_phase0;
    DSADPHCR1 = g_reg_phase1;
    DSADPHCR2 = g_reg_phase2;
    DSADPHCR3 = g_reg_phase3;
}

/***********************************************************************************************************************
* Function Name: R_ADC_Stop
* Description  : This function stops the AD converter.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_DSADC_Stop(void)
{
    DSADMR &= ~_0F0F_DSAD_ENABLE_ALLCH;  /* Stop conversion */
    DSAMK = 1U;  /* Disable INTAD interrupt */
    DSAIF = 0U;  /* Clear INTAD interrupt flag */
}

/***********************************************************************************************************************
* Function Name: R_DSADC_SetHighPassFilterOff
* Description  : This function is used to turn off HPF function for specified channel
* Arguments    : channel -
*                    the channel use HPF
* Return Value : MD_OK
*                    set HPF successfully
*                MD_ARGERROR
                     Input argument is invalid
***********************************************************************************************************************/
MD_STATUS R_DSADC_SetHighPassFilterOff(dsad_channel_t channel)
{
    MD_STATUS status = MD_OK;   
    switch(channel)
    {
        case DSADCHANNEL0:
            DSADHPFCR |= _01_DSAD_CH0_HIGHPASS_FILTER_DISABLE;  
            break;
        case DSADCHANNEL1:
            DSADHPFCR |= _02_DSAD_CH1_HIGHPASS_FILTER_DISABLE;
            break;
        case DSADCHANNEL2:
            DSADHPFCR |= _04_DSAD_CH2_HIGHPASS_FILTER_DISABLE;  
            break;
        case DSADCHANNEL3:
            DSADHPFCR |= _08_DSAD_CH3_HIGHPASS_FILTER_DISABLE;  
            break;              
        default:
            /* Channel is invalid */
            status = MD_ARGERROR;
            break;      
    }
    return status;
}

/***********************************************************************************************************************
* Function Name: R_DSADC_SetHighPassFilterOn
* Description  : This function is used to turn on HPF function for specified channel
* Arguments    : channel -
*                    the channel to turn on HPF
* Return Value : MD_OK
*                    turn on HPF successfully
*                MD_ARGERROR
                     Input argument is invalid
***********************************************************************************************************************/
MD_STATUS R_DSADC_SetHighPassFilterOn(dsad_channel_t channel)
{
    MD_STATUS status = MD_OK;       
    switch(channel)
    {
        case DSADCHANNEL0:
            DSADHPFCR &= (uint8_t)~_01_DSAD_CH0_HIGHPASS_FILTER_DISABLE;    
            break;
        case DSADCHANNEL1:
            DSADHPFCR &= (uint8_t)~_02_DSAD_CH1_HIGHPASS_FILTER_DISABLE;
            break;
        case DSADCHANNEL2:
            DSADHPFCR &= (uint8_t)~_04_DSAD_CH2_HIGHPASS_FILTER_DISABLE;    
            break;
        case DSADCHANNEL3:
            DSADHPFCR &= (uint8_t)~_08_DSAD_CH3_HIGHPASS_FILTER_DISABLE;    
            break;              
        default:
            /* Channel is invalid */
            status = MD_ARGERROR;
            break;      
    }
    return status;
}

/***********************************************************************************************************************
* Function Name: R_DSADC_Set_CutOffFrequency
* Description  : This function is used to set the cut off frequency of HPF
* Arguments    : frequency -
*                    the cut off frequency to be set for HPF
* Return Value : MD_OK
*                    set cut off frequency successfully
*                MD_ARGERROR
                     Input argument is invalid
***********************************************************************************************************************/
MD_STATUS R_DSADC_Set_CutOffFrequency(dsad_cutoff_t frequency)
{
    MD_STATUS status = MD_OK;
    
    if(HPF_0067HZ > frequency || frequency > HPF_4857_HZ)
    {
        status = MD_ARGERROR;
    }
    else 
    {
        DSADHPFCR &= 0x0F;
        DSADHPFCR |= (uint8_t)(frequency << 6);
    }
    return status;
}

/***********************************************************************************************************************
* Function Name: R_DSADC_AdjustPhaseDelay
* Description  : This function is used to set the cut off frequency of HPF
* Arguments    : channel -
*                    input channel to be selected
* Return Value : MD_OK
*                    phase adjust successfully
*                MD_ARGERROR
                     Input argument is invalid
***********************************************************************************************************************/
MD_STATUS R_DSADC_AdjustPhaseDelay(dsad_channel_t channel, uint16_t step)
{
    MD_STATUS status = MD_OK;

    if (step <= 1151)
    {   /* Step must be from 0 to 1151 */
        switch(channel)
        {
            case DSADCHANNEL0:
                DSADPHCR0 = step;    /* Set new step */
                break;
            case DSADCHANNEL1:
                DSADPHCR1 = step;    /* Set new step */
                break;
            case DSADCHANNEL2:
                DSADPHCR2 = step;    /* Set new step */
                break;
            case DSADCHANNEL3:
                DSADPHCR3 = step;    /* Set new step */
                break;          
            default:
                /* Channel is invalid */
                status = MD_ARGERROR;
                break;
        }       
    }
    else
    {
        /* Step is invalid */
        status = MD_ARGERROR;
    }
    
    return status;
}

/***********************************************************************************************************************
* Function Name: 
* Description  : 
* Arguments    : 
* Return Value : 
***********************************************************************************************************************/
void R_DSADC_GetGainAndPhase(dsad_reg_setting_t *adc_gain_phase)
{
    /* Get curretn gain setting */
    adc_gain_phase->gain0 = DSADGCR0;
    adc_gain_phase->gain1 = DSADGCR1;
    /* Get phase angle setting */
    adc_gain_phase->phase0 = DSADPHCR0;
    adc_gain_phase->phase1 = DSADPHCR1;
    adc_gain_phase->phase2 = DSADPHCR2;
    adc_gain_phase->phase3 = DSADPHCR3; 
}

/***********************************************************************************************************************
* Function Name: 
* Description  : 
* Arguments    : 
* Return Value : 
***********************************************************************************************************************/
void R_DSADC_SetGainAndPhase(dsad_reg_setting_t adc_gain_phase)
{
    DSADGCR0    = adc_gain_phase.gain0;
    DSADGCR1    = adc_gain_phase.gain1;
    DSADPHCR0   = adc_gain_phase.phase0;
    DSADPHCR1   = adc_gain_phase.phase1;
    DSADPHCR2   = adc_gain_phase.phase2;
    DSADPHCR3   = adc_gain_phase.phase3;    
}

/***********************************************************************************************************************
* Function Name: 
* Description  : 
* Arguments    : 
* Return Value : 
***********************************************************************************************************************/
void R_DSADC_SetGain(dsad_reg_setting_t adc_gain_phase)
{
    DSADGCR0    = adc_gain_phase.gain0;
    DSADGCR1    = adc_gain_phase.gain1;
}

/***********************************************************************************************************************
* Function Name: 
* Description  : 
* Arguments    : 
* Return Value : 
***********************************************************************************************************************/
MD_STATUS R_DSADC_SetChannelGain(dsad_channel_t channel, dsad_gain_t gain)
{
    MD_STATUS   status = MD_OK;
    
    if (gain < GAIN_X1 || gain > GAIN_X32)
    {
        status = MD_ARGERROR;
    }
    else
    {
        switch(channel)
        {
            case DSADCHANNEL0:
                DSADGCR0 &= 0xF0;
                DSADGCR0 |= gain;
                break;
            case DSADCHANNEL1:
                DSADGCR0 &= 0x0F;
                DSADGCR0 |= (uint8_t)(gain << 4);
                break;
            case DSADCHANNEL2:
                DSADGCR1 &= 0xF0;
                DSADGCR1 |= gain;
                break;
            case DSADCHANNEL3:
                DSADGCR1 &= 0x0F;
                DSADGCR1 |= (uint8_t)(gain << 4);
                break;
            default:
                /* Channel is invalid */
                status = MD_ARGERROR;
                break;
        }
    }
    return status;
}


/***********************************************************************************************************************
* Function Name: 
* Description  : 
* Arguments    : 
* Return Value : 
***********************************************************************************************************************/
void R_DSADC_Get_Result(dsad_channel_t channel, int32_t * const buffer)
{
    switch(channel)
    {
        case DSADCHANNEL0:
            *((int16_t *)buffer) = DSADCR0;
            *((int8_t *)buffer+2) = DSADCR0H;
            *((int8_t *)buffer+3) = (int8_t)DSADCR0H >> 7;
            break;
        case DSADCHANNEL1:
            *((int16_t *)buffer) = DSADCR1;
            *((int8_t *)buffer+2) = (int8_t)DSADCR1H;
            *((int8_t *)buffer+3) = (int8_t)DSADCR1H >> 7;
            break;
        case DSADCHANNEL2:
            *((int16_t *)buffer) = DSADCR2;
            *((int8_t *)buffer+2) = (int8_t)DSADCR2H;
            *((int8_t *)buffer+3) = (int8_t)DSADCR2H >> 7;
            break;
        case DSADCHANNEL3:
            *((int16_t *)buffer) = DSADCR3;
            *((int8_t *)buffer+2) = (int8_t)DSADCR3H;
            *((int8_t *)buffer+3) = (int8_t)DSADCR3H >> 7;
            break;
        default:
            break;
    }
}

/* Start user code for adding. Do not edit comment generated here */
/******************************************************************************
* Function Name    : dsad_gain_t R_DSADC_GetGainEnumValue(uint8_t gain)
* Description      : Get the enumuration gain value of ADC by a gain value
* Arguments        : None
* Functions Called : None
* Return Value     : dsad_gain_t, enum gain value
******************************************************************************/
dsad_gain_t R_DSADC_GetGainEnumValue(uint8_t gain)
{
    dsad_gain_t     gain_value;
    
    if (gain == 1)
    {
        gain_value = GAIN_X1;
    }
    else if (gain == 2)
    {
        gain_value = GAIN_X2;
    }
    else if (gain == 4)
    {
        gain_value = GAIN_X4;
    }
    else if (gain == 8)
    {
        gain_value = GAIN_X8;
    }
    else if (gain == 16)
    {
        gain_value = GAIN_X16;
    }
    else
    {
        gain_value = GAIN_X32;
    }
    
    return gain_value;
}


/******************************************************************************
* Function Name    : uint8_t R_DSADC_GetChannelGain(dsad_channel_t channel)
* Description      : Get the gain value of channel
* Arguments        : dsad_channel_t channel: specific dsad channel
* Functions Called : None
* Return Value     : uint8_t: channel gain
******************************************************************************/
uint8_t R_DSADC_GetChannelGain(dsad_channel_t channel)
{
    uint8_t gain;
    switch(channel)
    {
        case DSADCHANNEL0:
            gain = 1 << (DSADGCR0 & 0x0F);
            break;
        case DSADCHANNEL1:
            gain = 1 << ((DSADGCR0 & 0xF0) >> 4);
            break;
        case DSADCHANNEL2:
            gain = 1 << (DSADGCR1 & 0x0F);
            break;
        case DSADCHANNEL3:
            gain = 1 << ((DSADGCR1 & 0xF0) >> 4);
            break;
        default:
            gain = 0;
            break;
    }
    
    return gain;
}

/******************************************************************************
* Function Name    : uint16_t R_DSADC_GetChannelPhase(dsad_channel_t channel)
* Description      : Get the gain value of channel
* Arguments        : dsad_channel_t channel: specific dsad channel
* Functions Called : None
* Return Value     : uint16_t: channel phase setting
******************************************************************************/
uint16_t R_DSADC_GetChannelPhase(dsad_channel_t channel)
{
    uint16_t phase_setting;
    
    if ( channel <= DSADCHANNEL3 )
    {
        phase_setting = (uint16_t)*(&DSADPHCR0 + channel);
    }
    
    return phase_setting;
}

/******************************************************************************
* Function Name    : void R_DSADC_ClearGainSetting(dsad_reg_setting_t * p_setting, dsad_channel_t channel)
* Description      : Clear gain setting in setting to 0
* Arguments        : dsad_reg_setting_t * p_setting: setting register
*                  : dsad_channel_t channel: specific dsad channel
* Functions Called : None
* Return Value     : None
******************************************************************************/
void R_DSADC_ClearGainSetting(dsad_reg_setting_t * p_setting, dsad_channel_t channel)
{
    if (p_setting != NULL)
    {
        switch (channel)
        {
            case DSADCHANNEL0:
                p_setting->gain0 = p_setting->gain0 & 0xF0;
                break;
            case DSADCHANNEL1:
                p_setting->gain0 = p_setting->gain0 & 0x0F;
                break;
            case DSADCHANNEL2:
                p_setting->gain1 = p_setting->gain1 & 0xF0;
                break;
            case DSADCHANNEL3:
                p_setting->gain1 = p_setting->gain1 & 0x0F;
                break;
            default:
                /* Keep the same */
                break;
        }
    }
}
/* End user code. Do not edit comment generated here */
