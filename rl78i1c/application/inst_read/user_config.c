/******************************************************************************
* File Name    : user_config.c
* Version      : 1.00
* Device(s)    : RL78/I1C
* Tool-Chain   : CubeSuite Version 1.5d
* H/W Platform : RL78/I1C Energy Meter Platform
* Description  : LED processing source File
******************************************************************************
* History : DD.MM.YYYY Version Description
******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
/* Driver */
#include "r_cg_macrodriver.h"   /* CG Macro Driver */

/* MW/Core */
#include "em_core.h"        /* EM Core APIs */

/* Application */
#include "user_config.h"            /* LED Interface Header File */

/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Macro definitions
******************************************************************************/

/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/

/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/
USER_CONFIG user_config;
/******************************************************************************
Private global variables and functions
******************************************************************************/

/******************************************************************************
* Function Name    : void LED_LowVoltageOccurred(void)
* Description      : LED Callback for LVD Occurred
* Arguments        : None
* Functions Caluser_config : None
* Return Value     : None
******************************************************************************/
