/*===========================================================================================================
* Project: yyyy
* File: modbus.c
* Description: Template to create a C file
* Copyright: Embedded 2018
* Author: Firmware Team
===========================================================================================================*/

/*===========================================================================================================
* Standard included files
===========================================================================================================*/
#include "r_cg_macrodriver.h"           /* CG Macro Driver */
#include "r_cg_sau.h"
#include "inst_read.h"
/*===========================================================================================================
* Project included files
===========================================================================================================*/
#include "modbus.h"
#include "user_config.h"
/*===========================================================================================================
* Local define
===========================================================================================================*/

/*===========================================================================================================
* Local types
===========================================================================================================*/

/*===========================================================================================================
* Local data
===========================================================================================================*/
#if(MODBUS_ENABLE > 0)
uint8_t MODHead, MODTail;
uint8_t ModBuffer[150];
volatile uint8_t ModTimer;

/*===========================================================================================================
* Constant local data
===========================================================================================================*/
const uint16_t  cCTRSecondary = 5;
const uint16_t  cVersion = 1234;
const uint16_t  cSerialNo = 45674;
const uint8_t   cModelNo[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
const uint8_t   cDummyZero[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

tModTable ModTable[MOD_TABLE_SIZE] =
{
    /*Modbus Address            NoOfByres                   DataPtr*/
    {0x0000,                            2, (uint8_t*)& user_config.ModbusID},
    {0x0001,                            2, (uint8_t*)& user_config.CTR},
    {0x0002,                            2, (uint8_t*)& cCTRSecondary},
    {0x0003,                            10, (uint8_t*)& cModelNo},
    {0x0008,                            2, (uint8_t*)& cVersion},
    {0x0009,                            4, (uint8_t*)& cSerialNo},
    {0x000B,                            2, (uint8_t*)& g_inst_read_params.vrms[LINE_PHASE_R]},
    {0x000C,                            2, (uint8_t*)& g_inst_read_params.vrms[LINE_PHASE_Y]},
    {0x000D,                            2, (uint8_t*)& g_inst_read_params.vrms[LINE_PHASE_B]},
    {0x000E,                            2, (uint8_t*)& g_inst_read_params.vrms[LINE_TOTAL]},
    {0x000F,                            2, (uint8_t*)& g_inst_read_params.vrmsp_p[LINE_PHASE_R]},
    {0x0010,                            2, (uint8_t*)& g_inst_read_params.vrmsp_p[LINE_PHASE_Y]},
    {0x0011,                            2, (uint8_t*)& g_inst_read_params.vrmsp_p[LINE_PHASE_B]},
    {0x0012,                            2, (uint8_t*)& g_inst_read_params.freq[LINE_PHASE_R]},
    {0x0013,                            2, (uint8_t*)& g_inst_read_params.power_factor[LINE_PHASE_R]},
    {0x0014,                            2, (uint8_t*)& g_inst_read_params.power_factor[LINE_PHASE_Y]},
    {0x0015,                            2, (uint8_t*)& g_inst_read_params.power_factor[LINE_PHASE_B]},
    {0x0016,                            2, (uint8_t*)& g_inst_read_params.power_factor[LINE_TOTAL]},
    {0x0017,                            4, (uint8_t*)& g_inst_read_params.irms[LINE_PHASE_R]},
    {0x0019,                            4, (uint8_t*)& g_inst_read_params.irms[LINE_PHASE_Y]},
    {0x001B,                            4, (uint8_t*)& g_inst_read_params.irms[LINE_PHASE_B]},
    {0x001D,                            4, (uint8_t*)& g_inst_read_params.irms[LINE_TOTAL]},
    {0x001F,                            4, (uint8_t*)& g_inst_read_params.active_power[LINE_PHASE_R]},
    {0x0021,                            4, (uint8_t*)& g_inst_read_params.active_power[LINE_PHASE_Y]},
    {0x0023,                            4, (uint8_t*)& g_inst_read_params.active_power[LINE_PHASE_B]},
    {0x0025,                            4, (uint8_t*)& g_inst_read_params.active_power[LINE_TOTAL]},
    {0x0027,                            4, (uint8_t*)& g_inst_read_params.apparent_power[LINE_PHASE_R]},
    {0x0029,                            4, (uint8_t*)& g_inst_read_params.apparent_power[LINE_PHASE_Y]},
    {0x002B,                            4, (uint8_t*)& g_inst_read_params.apparent_power[LINE_PHASE_B]},
    {0x002D,                            4, (uint8_t*)& g_inst_read_params.apparent_power[LINE_TOTAL]},
    {0x002F,                            4, (uint8_t*)& g_inst_read_params.reactive_power[LINE_PHASE_R]},                 //Reactive Power
    {0x0031,                            4, (uint8_t*)& g_inst_read_params.reactive_power[LINE_PHASE_R]},                 //Reactive Power
    {0x0033,                            4, (uint8_t*)& g_inst_read_params.reactive_power[LINE_PHASE_R]},                 //Reactive Power
    {0x0035,                            4, (uint8_t*)& g_inst_read_params.reactive_power[LINE_PHASE_R]},                 //Reactive Power
    {0x0037,                            4, (uint8_t*)& MainsEnergies.active_imp},                 //Energies
    {0x0039,                            8, (uint8_t*)& MainsEnergies.active_exp},                 //Energies
    {0x003D,                            4, (uint8_t*)& MainsEnergies.run_hour},
    {0x003F,                            4, (uint8_t*)& MainsEnergies.run_hour},
    {0x0041,                            4, (uint8_t*)& cDummyZero},
};
/*
MODBUS Protocol
LSB is sent first and MSB is sent last for all numeric registers (i.e. except string reply for Model No.)
Default Device Address - 18

------------+---------------+-------------------+-------------------------------+-------
Function    | Reg. Address  | Size              | Register                      |Factor
------------+---------------+-------------------+-------------------------------+-------
0x03        | 0x0000        | 2 Byte            | Device Address                |           ;Read 1 parameter at a time
0x03        | 0x0001        | 2 Byte            | CTR Primary                     |         ;Read 1 parameter at a time
0x03        | 0x0002        | 2 Byte            | CTR Secondary                 |           ;Read 1 parameter at a time
------------+---------------+-------------------+-------------------------------+-------
0x04        | 0x0003        | 10 char           | Model                         |
0x04        | 0x0008        | 2 Byte            | Version                       |
0x04        | 0x0009        | 4 Bytes           | Serial                        |
0x04        | 0x000B        | 2 Byte            | Phase Voltage - RN            | * 10
0x04        | 0x000C        | 2 Byte            | Phase Voltage - YN            | * 10
0x04        | 0x000D        | 2 Byte            | Phase Voltage - BN            | * 10
0x04        | 0x000E        | 2 Byte            | Phase Voltage - Average   | * 10
0x04        | 0x000F        | 2 Byte            | Line Voltage - RY             | * 10
0x04        | 0x0010        | 2 Byte            | Line Voltage - YB             | * 10
0x04        | 0x0011        | 2 Byte            | Line Voltage - BR             | * 10
0x04        | 0x0012        | 2 Byte            | Frequency                             | * 100
0x04        | 0x0013        | 2 Byte            | Power Factor - R              | * 100
0x04        | 0x0014        | 2 Byte            | Power Factor - Y              | * 100
0x04        | 0x0015        | 2 Byte            | Power Factor - B              | * 100
0x04        | 0x0016        | 2 Byte            | Power Factor - Combined   | * 100
0x04        | 0x0017        | 4 Byte            | Current - R                           | * 1000
0x04        | 0x0019        | 4 Byte            | Current - Y                           | * 1000
0x04        | 0x001B        | 4 Byte            | Current - B                           | * 1000
0x04        | 0x001D        | 4 Byte            | Current - Average             | * 100
0x04        | 0x001F        | 4 Byte            | Active Power - R              | * 100
0x04        | 0x0021        | 4 Byte            | Active Power - Y              | * 100
0x04        | 0x0023        | 4 Byte            | Active Power - B              | * 100
0x04        | 0x0025        | 4 Byte            | Active Power - Combined   | * 100
0x04        | 0x0027        | 4 Byte            | Apparant Power - R            | * 100
0x04        | 0x0029        | 4 Byte            | Apparant Power - Y            | * 100
0x04        | 0x002B        | 4 Byte            | Apparant Power - B            | * 100
0x04        | 0x002D        | 4 Byte            | Apparant Power - Combined     | * 100
0x04        | 0x002F        | 4 Byte            | Reactive Power - R            | * 100
0x04        | 0x0031        | 4 Byte            | Reactive Power - Y            | * 100
0x04        | 0x0033        | 4 Byte            | Reactive Power - B            | * 100
0x04        | 0x0035        | 4 Byte            | Reactive Power - Combined     | * 100
0x04        | 0x0037        | 4 Byte            | Active Energy - Combined      | * 100
0x04        | 0x0039        | 4 Byte            | Apparant Energy - Combined    | * 100
0x04        | 0x003B        | 4 Byte            | Reactive Energy - Combined    | * 100
0x04        | 0x003D        | 4 Byte            | ON Hour                       |
0x04        | 0x003F        | 4 Byte            | RUN Hour                      |
------------+---------------+-------------------+-------------------------------+-------
0x06        | 0x0000        | 2 Byte            | Device Address (only LSB used)|
0x06        | 0x0002        | 2 Byte            | CTR Primary                   |

*/
/*===========================================================================================================
* Constant exported data
===========================================================================================================*/

/*===========================================================================================================
* Exported data
===========================================================================================================*/
tMODFlag MODFlag;
/*===========================================================================================================
* Local macros
===========================================================================================================*/
#define RS485_TX_ENABLE()				{SET_BIT( P12, 5, 1 );}
#define RS485_TX_DISABLE()				{SET_BIT( P12, 5, 0 );}
/*===========================================================================================================
* Local function prototypes
===========================================================================================================*/
static void MODSendData(uint8_t bytes);
/*===========================================================================================================
* Exported Fonctions declaration
===========================================================================================================*/

/*===========================================================================================================
* Title : Initialize
* Details :
===========================================================================================================*/

/*===========================================================================================================
Local fonctions declaration
===========================================================================================================*/

/*===========================================================================================================
* Title : Task
* Details :
===========================================================================================================*/
void MODBUS_Process(void)
{
    uint8_t FunCode;
    uint16_t CRC;
    uint16_t StartAdd;
    uint16_t NoOfWord;
    uint8_t zIdx;
    
    if ((g_modbusmode == 1) && (MODFlag.Process == 1))
    {
        MODFlag.Process = 0;
        
        CRC = ModBuffer[MODTail - 2u] + ModBuffer[MODTail - 1u] * 256;
        
        if (CRC == MOD_Crc_Calc((uint8_t*)&ModBuffer, MODTail - 2u))
        {
            uint8_t zError = NO_ERROR;
            
            if (ModBuffer[0] == user_config.ModbusID) // Check Slave Address
            {
                FunCode  = ModBuffer[1];
                StartAdd = ModBuffer[3]; //ModBuffer[3] + ModBuffer[2]*256;
                NoOfWord = ModBuffer[5]; //ModBuffer[5] + ModBuffer[4]*256;
                
                if ((StartAdd + NoOfWord) > 0x40)
                {
                    zError = ILLEGAL_DATA_ADDRESS_CODE;
                }
                else if (NoOfWord > 0x40)
                {
                    zError = ILLEGAL_DATA_VALUE_CODE;
                }
                else if ((FunCode == MOD_FUNC_CODE_3) || (FunCode == MOD_FUNC_CODE_4))
                {
                    uint8_t* zDataPtr = &ModBuffer[3];
                    uint8_t zTblIdx;
                    int8_t zByte;
                    
                    for (zTblIdx = 0; zTblIdx < MOD_TABLE_SIZE; zTblIdx++) // Find Modbus table index
                    {
                        if (ModTable[zTblIdx].Address == StartAdd)
                        {
                            break;
                        }
                    }
                    
                    for (zIdx = zTblIdx; zIdx < MOD_TABLE_SIZE; zIdx++) // Find Modbus table index
                    {
                        for (zByte = ModTable[zIdx].NoOfBytes - 1; zByte >= 0; zByte--)
                        {
                            *(zDataPtr++) = *(ModTable[zIdx].RegPtr + zByte);
                        }
                        
                        if (ModTable[zIdx + 1].Address == (StartAdd + NoOfWord))
                        {
                            break;
                        }
                        else if (ModTable[zIdx + 1].Address > (StartAdd + NoOfWord))
                        {
                            zError = ILLEGAL_RESPONCE_CODE;
                            break;
                        }
                    }
                    
                    ModBuffer[2] = NoOfWord * 2;
                    MODHead = ModBuffer[2] + 3;
                }
                else if (FunCode == MOD_FUNC_CODE_6)
                {
                    uint16_t zVal = ModBuffer[5] + ModBuffer[4] * 256; // get Value
                    
                    if (StartAdd == 0x00)
                    {
                        //user_config.ModbusID = zVal; //Device Address
                    }
                    else if (StartAdd == 0x01)
                    {
                        //user_config.CTR = zVal; //CTR
                    }
                    else
                    {
                        zError = ILLEGAL_DATA_ADDRESS_CODE;
                    }
                    
                    MODHead = 6;
                }
                else
                {
                    zError = ILLEGAL_FUNCTION_CODE;
                }
                
                if (zError > NO_ERROR)
                {
                    ModBuffer[1] = ModBuffer[1] | 0x80;
                    ModBuffer[2] = zError;
                    MODHead = 3;
                }
                
                MODSendData(MODHead);
            }
            else
            {
                MODTail = 0;
                MODHead = 0;
            }
        }
        else
        {
            MODTail = 0;
            MODHead = 0;
        }
    }
}
/*===========================================================================================================
* Title : EM_Timer_ModbusInterruptCallback
* Details : 1ms timer
===========================================================================================================*/
void EM_Timer_ModbusInterruptCallback(void)
{
    if (ModTimer)
    {
        if (ModTimer++ > 10)
        {
            ModTimer = 0;
            MODFlag.Process = 1;
        }
    }
}
/*===========================================================================================================
* Title : EM_RX_ModbusInterruptCallback
* Details : 1ms timer
===========================================================================================================*/
void EM_RX_ModbusInterruptCallback(uint8_t rxData)
{
    ModBuffer[MODTail++%140] = rxData;
    ModTimer = 1;
}
/*===========================================================================================================
* Title : EM_TX_EndInterruptCallback
* Details : 1ms timer
===========================================================================================================*/
void EM_TX_EndInterruptCallback(void)
{
    uint8_t zIdx;
    MODTail = 0;
    
    for (zIdx = 0; zIdx < 50; zIdx++); /*Wait Till Send Last Byte*/
    
    RS485_TX_DISABLE();
    
}
/*===========================================================================================================
* Title : MODSendData
* Details :
===========================================================================================================*/
void MODSendData(uint8_t bytes)
{
    uint16_t ZCheckSum;
    uint8_t* L_add;
	uint8_t zIdx;
    RS485_TX_ENABLE(); /* Enable RS485 Tx*/
	for (zIdx = 0; zIdx < 50; zIdx++); /*Wait Till Send Last Byte*/
    ZCheckSum = MOD_Crc_Calc((uint8_t*)&ModBuffer, bytes);
    L_add = (uint8_t*)&ZCheckSum;  /*Stores calculated checksum value into ModBuffer*/
    //ModBuffer[MODHead++] = *(L_add + 1);
    ModBuffer[MODHead++] = *L_add;
	ModBuffer[MODHead++] = *(L_add + 1);
    R_UART2_Send(ModBuffer, (uint16_t)MODHead);//USART_Data_Transmit(&ModBuffer[0] , MODHead );    /* Transmit ModBuffer data through USART.*/
    MODFlag.Wait_for_ACK = TRUE;
    
    MODFlag.ModBuffer_Transmit = TRUE;
    MODFlag.Process  = 0;
    MODTail = 0;
}

/*===========================================================================================================
* Title : MOD_Crc_Calc
* Details :variable count stores number of total  byte count present in frame except last 2 byte of CRC.
===========================================================================================================*/
uint16_t MOD_Crc_Calc(uint8_t* data, uint8_t count)
{
    uint16_t ui_temp1, ui_temp2;
    uint16_t crc;
    
    crc = 0xffffu ^ data[0];
    
    for (ui_temp1 = 1u; ui_temp1 <= count; ui_temp1++)
    {
        for (ui_temp2 = 0u; ui_temp2 <= 7u; ui_temp2++)
        {
            if ((crc & 0x01u) == 0X01u)
            {
                crc >>= 1u;
                crc = crc ^ 0xA001u;
            }
            else
            {
                crc >>= 1u;
            }
        }
        
        if (ui_temp1 != count)
        {
            crc = crc ^ data[ui_temp1];
        }
    }
    
    return crc; /* crc stores calculated CRC value. */
}
#endif //MODBUS_ENABLE