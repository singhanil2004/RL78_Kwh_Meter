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
* File Name    : ct_compensation.c
* Version      : 1.0
* Device(s)    : RL78/I1C
* Tool-Chain   : CCRL
* H/W Platform : RL78/I1C Energy Meter Platform
* Description  : <Description of file>
************************************************************************************************************************
* History      : 18.11.2016 Initial version
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include "typedef.h"        /* GSCE Typedef */

/* MW/Core */
#include "em_core.h"        /* EM Core APIs */
#include "platform.h"

#include "ct_compensation.h"
/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#define COMPENSATE_INTERPOLATION_ENABLE (1)
/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Imported global variables and functions (from other files)
***********************************************************************************************************************/

/***********************************************************************************************************************
Exported global variables and functions (to be accessed by other files)
***********************************************************************************************************************/

/* Imported from ct_profile.c */
extern const ct_phase_shift_t g_ct_phase_shift_phase_r[];
extern const ct_phase_shift_t g_ct_phase_shift_phase_y[];
extern const ct_phase_shift_t g_ct_phase_shift_phase_b[];

extern const uint16_t g_ct_phase_shift_size_phase_r;
extern const uint16_t g_ct_phase_shift_size_phase_y;
extern const uint16_t g_ct_phase_shift_size_phase_b;

/***********************************************************************************************************************
* Function Name    : uint16_t EM_GetPhaseAngleCompensation(EM_LINE line)
* Description      : Do compensate the theta error base on the CT datasheet
* Arguments        : EM_LINE line to get CT profile
* Functions Called : EM_ADC_SetPhaseCorrection_Phase_R
*                  : EM_ADC_SetPhaseCorrection_Phase_Y
*                  : EM_ADC_SetPhaseCorrection_Phase_B
* Return Value     : float32_t : the theta error to be compensated
*                  :
***********************************************************************************************************************/
float32_t EM_CT_GetPhaseAngleCompensation(EM_LINE line, float32_t current)
{
#ifdef ENABLE_CT_COMPENSATION
    float32_t               theta = 0;
    uint16_t                i;
    
    const ct_phase_shift_t*  ct_profile1 = NULL;  /* Near point 1, used to linear interpolate */
    const ct_phase_shift_t*  ct_profile2 = NULL;  /* Near point 2, used to linear interpolate */
    const ct_phase_shift_t*  ct_profile  = NULL;
    
    uint16_t ct_profile_size;
    
    switch (line)
    {
        case LINE_PHASE_R:
            ct_profile = g_ct_phase_shift_phase_r;
            ct_profile_size = g_ct_phase_shift_size_phase_r;
            break;
            
        case LINE_PHASE_Y:
            ct_profile = g_ct_phase_shift_phase_y;
            ct_profile_size = g_ct_phase_shift_size_phase_y;
            break;
            
        case LINE_PHASE_B:
            ct_profile = g_ct_phase_shift_phase_b;
            ct_profile_size = g_ct_phase_shift_size_phase_b;
            break;
            
        default:
            /* Support compensation for phase R-Y-B only */
            return (0);
    }
    
    if (ct_profile_size == 0)
    {
        return (0);
    }
    
    /* Scan positive branch until meet the current (A) larger than measured current (A) */
    if (ct_profile[0].current >= current)
    {
        return (-ct_profile[0].theta);
    }
    else if (ct_profile[ct_profile_size - 1].current < current)
    {
        return (-ct_profile[ct_profile_size - 1].theta);
    }
    else
    {
        for (i = 1; i < ct_profile_size; i++)
        {
            if (ct_profile[i].current >= current)
            {
                /* Get CT Phase Angle Compensate 1 value */
                ct_profile1 = &ct_profile[i - 1];
                
                /* Get CT Phase Angle Compensate 2 value */
                ct_profile2 = &ct_profile[i];
                
                break;
            }
        }
        
        /* Apply linear interpolation formula */
        if (ct_profile1 != NULL && ct_profile2 != NULL)
        {
            theta = ct_profile1->theta + (ct_profile2->theta - ct_profile1->theta) * (current - ct_profile1->current) / (ct_profile2->current - ct_profile1->current);
        }
        else
        {
            theta = 0;
        }
    }
    
    return (-theta);
#else
    return (0);
#endif /* ENABLE_CT_COMPENSATION */
}


/***********************************************************************************************************************
Private global variables and functions
***********************************************************************************************************************/
