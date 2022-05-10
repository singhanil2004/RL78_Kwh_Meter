/*===========================================================================================================
* Project: yyy
* File: Version.h
* Description: Template to create a h file
* Copyright: Embedded 2018
* Author: Firmware Team
===========================================================================================================*/
#ifndef __MODBUS
#define __MODBUS
/*===========================================================================================================
* Standard included files
===========================================================================================================*/
#include "typedef.h"
/*===========================================================================================================
* Project included files
===========================================================================================================*/

/*===========================================================================================================
* Exported define
===========================================================================================================*/
#define MODBUS_ENABLE  1

#if(MODBUS_ENABLE > 0)
typedef struct
{
    uint8_t Process;
    uint8_t Wait_for_ACK;
    uint8_t ModBuffer_Transmit;
} tMODFlag;

typedef struct
{
    uint16_t Address;
    uint8_t  NoOfBytes;
    uint8_t* RegPtr;
} tModTable;

/*===========================================================================================================
* Exported types
===========================================================================================================*/
#define NO_ERROR                        0x00
#define ILLEGAL_FUNCTION_CODE           0x01
#define ILLEGAL_DATA_ADDRESS_CODE       0x02
#define ILLEGAL_DATA_VALUE_CODE         0x03
#define ILLEGAL_RESPONCE_CODE           0x04

#define MOD_FUNC_CODE_3                     3u
#define MOD_FUNC_CODE_4                     4u
#define MOD_FUNC_CODE_6                     6u

#define MOD_TABLE_SIZE                      40u

/*===========================================================================================================
* Exported Constant data
===========================================================================================================*/


/*===========================================================================================================
* Exported data
===========================================================================================================*/

extern tMODFlag MODFlag;
extern uint8_t MODHead, MODTail;
extern uint8_t ModBuffer[150];

/*===========================================================================================================
* Exported Fonctions declaration
===========================================================================================================*/
void MODBUS_Process(void);
void USART_Data_Transmit(uint8_t* pBuffer, uint8_t length);
uint16_t MOD_Crc_Calc(uint8_t* data, uint8_t count);
void EM_Timer_ModbusInterruptCallback(void);
void EM_RX_ModbusInterruptCallback(uint8_t rxData);
void EM_TX_EndInterruptCallback(void);
/*===========================================================================================================
* Exported macros
===========================================================================================================*/
#endif //MODBUS_ENABLE

#endif //__MODBUS

