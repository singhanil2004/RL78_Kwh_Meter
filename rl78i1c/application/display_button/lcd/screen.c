/***********************************************************************************************************************
* File Name    : r_lcd_config.c
* Version      : 1.00
* Device(s)    : RL78/I1C
* Tool-Chain   : CA78K0R
* H/W Platform : RL78/I1C Energy Meter Platform
* Description  : Declare const array for LCD configuration
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
/* Code Standard */
#include "typedef.h"            /* GSCE Standard Typedef */
#include "user_config.h"
/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Imported global variables and functions (from other files)
***********************************************************************************************************************/

/***********************************************************************************************************************
Exported global variables and functions (to be accessed by other files)
***********************************************************************************************************************/
const uint8_t E_screen0A[]     = {"Firmware Upgrade by"};
const uint8_t E_screen0B[]     = {"Bootloader"};
const uint8_t E_screen0C[]     = {"Initialising"};
const uint8_t E_screen0D[]     = {"Contact"};
const uint8_t E_screen0E[]     = {"info@"};

const uint8_t E_screen1A[]     = {" SELF TEST "};
const uint8_t E_screen1B[]     = {"1. HV Channel: PASS"};
const uint8_t E_screen1C[]     = {"2. LV Channel: PASS"};
const uint8_t E_screen1D[]     = {"3. Current Ch: PASS"};
const uint8_t E_screen1E[]     = {"4. Keypad    : PASS"};
const uint8_t E_screen1F[]     = {"4. Keypad    : FAIL"};
const uint8_t E_screen1G[]     = {" realtimesystems.in "};


const uint8_t E_screen2A[]     = {" TRANSFORMER "};
const uint8_t E_screen2B[]     = {" RATIO METER "};
const uint8_t E_screen2C[]     = {" REAL TIME SYSTEMS "};

const uint8_t E_screen2N[]     = {" IMPORTANT "};
const uint8_t E_screen2O[]     = {"Current Terminal [-I]"};
const uint8_t E_screen2P[]     = {"must be connected to"};
const uint8_t E_screen2Q[]     = {"the GROUNDED end of"};
const uint8_t E_screen2R[]     = {"the DUT (if any end"};
const uint8_t E_screen2S[]     = {"of DUT is connected"};
const uint8_t E_screen2T[]     = {"to ground / earth)"};

const uint8_t E_screen3A[]     = {" MAIN MENU "};
const uint8_t E_screen3B[]     = {"1. Ratio Measurement"};
const uint8_t E_screen3C[]     = {"2. Tap-Changer Ratio"};
const uint8_t E_screen3D[]     = {"2. Setup"};
const uint8_t E_screen3E[]   =   {"4. Configure"};

const uint8_t E_screen4A[]     = {" RATIO MEASUREMENT "};
const uint8_t E_screen4B[]     = {"Current Range:"};
const uint8_t E_screen4C[]   = {"Press [START] to test"};

const uint8_t E_screen5A[]   = {" CONTACT RESISTANCE "};
const uint8_t E_screen5B[]   = {"R ="};
const uint8_t E_screen5C[]   = {"I ="};
const uint8_t E_screen5D[]   = {"AutoStop: Press a key"};
const uint8_t E_screen5E[]   = {" Results "};
const uint8_t E_screen5F[]   = {"Press [OK] to save"};
const uint8_t E_screen5G[]   = {"File Saved"};
const uint8_t E_screen5H[]   = {"File NOT Saved"};

const uint8_t E_screen6A[]   = {"Select File"};
const uint8_t E_screen6B[]   = {"File No.:"};
const uint8_t E_screen6C[]   = {"Filename:"};
const uint8_t E_screen6D[]   = {"Remark:"};

const uint8_t E_screen8A[]   = {" Delete File "};
const uint8_t E_screen8B[]   = {"File Deleted"};
const uint8_t E_screen8C[]   = {"[OK] to del all files"};
const uint8_t E_screen8D[]   = {"All Files Deleted"};

const uint8_t E_screen9A[]   = {" Send Files to PC "};
const uint8_t E_screen9B[]   = {"Connect RS232 cable"};
const uint8_t E_screen9C[]   = {"and then run"};
const uint8_t E_screen9D[]   = {"'RTS Communicator'"};

const uint8_t E_screen10A[]  = {" CONFIGURE "};
const uint8_t E_screen10B[]    = {"Password:"};
const uint8_t E_screen10C[]  = {"1. Debug Menu"};
const uint8_t E_screen10D[]  = {"2. Set Time"};
const uint8_t E_screen10E[]  = {"3. License File"};
const uint8_t E_screen10F[]  = {"4. Hardware Setup"};

const uint8_t E_screen11A[]    = {" CHANGE PASSWORD "};
const uint8_t E_screen11B[]    = {"New Password:"};
const uint8_t E_screen11C[]    = {"Retype      : "};

const uint8_t E_screen12A[]  = {" SET DATE "};
const uint8_t E_screen12B[]  = {"System Date"};
const uint8_t E_screen12C[]  = {"New Date"};

const uint8_t E_screen13A[]  = {" SET TIME "};
const uint8_t E_screen13B[]  = {"System Time"};
const uint8_t E_screen13C[]  = {"New Time"};

const uint8_t E_screen14A[]  = {"    INST-DEBUG-AVG "};
const uint8_t E_screen14B[]  = {" PASSWORD "};
const uint8_t E_screen14C[]  = {"Sh= "};
const uint8_t E_screen14D[]  = {"Ch= "};
const uint8_t E_screen14E[]  = {"[GAIN]  2:Attn"};
const uint8_t E_screen14F[]  = {"1:Gain  [ATTN]"};

const uint8_t E_screen15A[]  = {" FILE MANAGER "};
const uint8_t E_screen15B[]    = {"1. Print File"};
const uint8_t E_screen15C[]    = {"1. View File"};
const uint8_t E_screen15D[]    = {"2. Delete File"};
const uint8_t E_screen15E[]    = {"3. Send Files to PC"};

const uint8_t E_screen16A[]  = {" LICENSE FILE "};
const uint8_t E_screen16B[]    = {"1. Accuracy  :"};
const uint8_t E_screen16C[]    = {"2. Setup Time:"};

const uint8_t E_screenA[]        = {" RESULTS "};
const uint8_t E_screenB[]        = {"   HV: No Signal     "};
const uint8_t E_screenC[]        = {"   I-Ext: Over Range "};
const uint8_t E_screenD[]        = {"Checking Short Ckt   "};
const uint8_t E_screenE[]        = {"Checking HV-LV REV   "};
const uint8_t E_screenF[]        = {"LV Sig. HI/HV-LV REV"};
const uint8_t E_screenG[]        = {"---------------------"};


/**************************************/
