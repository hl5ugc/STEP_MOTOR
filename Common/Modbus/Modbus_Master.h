/*
 * [ PROJECT   ]  5_stm32g431_UART
 * [ License   ]  SAMJIN ELECTRONICS.,Co.Ltd
 * [ Author    ]  Copyright 2024 By HAG-SEONG KANG
 * [ E-MAIL    ]  hl5ugc@nate.com (82)10- 3841-9706
 * [ C  P  U   ]
 * [ Compller  ]  
 * [ Filename  ]  Modbus_Master.h
 * [ Version   ]  1.0
 * [ Created   ]  2024[12:18:08 PM ]
 * --------------------------------------------------------------------------
 * Revision History :
 * ------------------------------------------------------------------
 *
 *
 *  *.The use of this source code shall be deemed to have been 
 *    tacitly agreed by the user.
 *  *.This source code is freely available for personal learning 
 *    and research and development.
 *  *.In the case of secondary authoring or redistribution using this source, 
 *    it is essential The company name of SAMJIN ELECTRONICS must be specified.
 *  *.Do not sell or for-profit this source code.
 *	*.This clause and the original author mark are prohibited from being 
 *	  modified or deleted. 
 *  
 *------------------------------------------------------------------
 * --------------------------------------------------------------------------
 * Author         Date       Comments on this revision
 * --------------------------------------------------------------------------
 * Hagseong Kang  May 30, 2024    First release of this file
 * --------------------------------------------------------------------------
 * Additional Notes:
 * **************************************************************************
 */
 /**
 * @brief 
 * @param  
 * @return  
 */


/* Mutiple includes protection */
#ifndef COMMON_MODBUS_MODBUS_MASTER_H_
#define COMMON_MODBUS_MODBUS_MASTER_H_

#ifdef __cplusplus
extern "C" {
#endif
/* Includes */
#include "def.h"

#include "ModbusCRC.h"
#include "word.h"         // core
// ---------------------------------------------------------------------------
// Define Global define
// --------------------------------------------------------------------------- 
// 
#define MODBUS_PORT     1U
#define MODBUS_BPS      38400UL
// ---------------------------------------------------------------------------
// Define typedef
// --------------------------------------------------------------------------- 
// 
// ---------------------------------------------------------------------------
// Define  Global Function prototypes.
// --------------------------------------------------------------------------- 
//
void    ModbusMaster_begin(void);
void    ModbusMaster_beginTransmission(uint16_t u16Address);
uint8_t ModbusMaster_requestFrom(uint16_t address, uint16_t quantity);
void    ModbusMaster_sendBit(uint8_t data);
void    ModbusMaster_send16(uint16_t data);
void    ModbusMaster_send32(uint32_t data);
void    ModbusMaster_send8(uint8_t data);
uint8_t ModbusMaster_available(void);
uint16_t ModbusMaster_receive(void);
uint16_t ModbusMaster_getResponseBuffer(uint8_t u8Index);
void    ModbusMaster_clearResponseBuffer(void);
uint8_t ModbusMaster_setTransmitBuffer(uint8_t u8Index, uint16_t u16Value);
void    ModbusMaster_clearTransmitBuffer(void);
uint8_t ModbusMaster_readCoils(uint8_t SlaveID,uint16_t u16ReadAddress, uint16_t u16BitQty);
uint8_t ModbusMaster_readDiscreteInputs(uint8_t SlaveID,uint16_t u16ReadAddress,uint16_t u16BitQty);
uint8_t ModbusMaster_readHoldingRegisters(uint8_t SlaveID,uint16_t u16ReadAddress,uint16_t u16ReadQty);
uint8_t ModbusMaster_readInputRegisters(uint8_t SlaveID,uint16_t u16ReadAddress,uint8_t u16ReadQty);
uint8_t ModbusMaster_writeSingleCoil(uint8_t SlaveID,uint16_t u16WriteAddress, uint8_t u8State);
uint8_t ModbusMaster_writeSingleRegister(uint8_t SlaveID,uint16_t u16WriteAddress,uint16_t u16WriteValue);
uint8_t ModbusMaster_writeMultipleCoils(uint8_t SlaveID,uint16_t u16WriteAddress,uint16_t u16BitQty);
uint8_t ModbusMaster_writeMultipleRegisters(uint8_t SlaveID,uint16_t u16WriteAddress,uint16_t u16WriteQty);
uint8_t ModbusMaster_maskWriteRegister(uint8_t SlaveID,uint16_t u16WriteAddress,uint16_t u16AndMask, uint16_t u16OrMask);
uint8_t ModbusMaster_readWriteMultipleRegisters(uint8_t SlaveID,uint16_t u16ReadAddress,uint16_t u16ReadQty, uint16_t u16WriteAddress, uint16_t u16WriteQty);

#ifdef __cplusplus
}
#endif
#endif /* Modbus_Master.h End Of File !! Well Done */
