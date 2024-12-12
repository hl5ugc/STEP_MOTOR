/*
 * [ PROJECT   ]  5_stm32g431_UART
 * [ License   ]  SAMJIN ELECTRONICS.,Co.Ltd
 * [ Author    ]  Copyright 2024 By HAG-SEONG KANG
 * [ E-MAIL    ]  hl5ugc@nate.com (82)10- 3841-9706
 * [ C  P  U   ]
 * [ Compller  ]  
 * [ Filename  ]  TransRecieveBuffControl.c
 * [ Version   ]  1.0
 * [ Created   ]  2024[12:33:28 PM ]
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



/* Define Includes */
#include "TransRecieveBuffControl.h"
#include "qBuffer.h"                  // core
#include "uart.h"                  // core
// ---------------------------------------------------------------------------
//  Define Macros .
// ---------------------------------------------------------------------------
//
 
// ---------------------------------------------------------------------------
//  Define TypeDef & Constants.
// ---------------------------------------------------------------------------
//
 
// ---------------------------------------------------------------------------
//  Define Private variables.
// ---------------------------------------------------------------------------
//
qbuffer_t   m_Modbus_Master_RX_RingBuff;
uint8_t     m_Modbus_Master_RX_Buff[200];
uint8_t     m_Modbus_Master_RX_Over = 0 ;
// ---------------------------------------------------------------------------
// Define Private function prototype.
// ---------------------------------------------------------------------------
//

// ---------------------------------------------------------------------------
// Define Callback function prototype.
// ---------------------------------------------------------------------------
//
void ModbusRxCheck(void);
// ---------------------------------------------------------------------------
// Define Public function definitions. 
// ---------------------------------------------------------------------------
//

/**
 *  Initialize the ringbuffer ring queue configuration for interrupt reception.
 *  The bytes received are managed by the m_Modbus_Master_RX_RingBuff structure
 *  pointer.
 */
bool Modbus_Master_RB_Init(void)
{
  bool bRet = true ;
  qbufferCreate(&m_Modbus_Master_RX_RingBuff,m_Modbus_Master_RX_Buff,sizeof(m_Modbus_Master_RX_Buff));

  //uartOpen(MODBUS_PORT,MODBUS_BPS) ;
  // callback void ModbusRxCheck(void);
  return (bRet);
}
uint8_t  Modbus_Master_Rece_Flush(void)
{
  // qbuffer  Flush
  qbufferFlush(&m_Modbus_Master_RX_RingBuff);
}
//
// uint32_t uartWriteBuff(uint8_t ch,const uint8_t *pBuff, uint32_t len)
//
uint8_t  Modbus_Master_Write(const uint8_t *buf,uint8_t length)
{
  //uartWriteBuff(buf,length);
}
//
//
//
uint8_t Modbus_Master_Rece_Available(void)
{
  uint8_t u8Ret = 0U;

  u8Ret = qbufferAvailable(&m_Modbus_Master_RX_RingBuff);

  return (u8Ret);
}
// ---------------------------------------------------------------------------
// Define private function definitions.  
// ---------------------------------------------------------------------------
//
 
// ---------------------------------------------------------------------------
//  Define Callbacks definitions.
// ---------------------------------------------------------------------------
//
void ModbusRxCheck(void)
{
  static uint8_t u8Time = 0U ;


 // uartAvail();


}
// ---------------------------------------------------------------------------
//  Define CLI definitions.
// ---------------------------------------------------------------------------
//
 /* TransRecieveBuffControl.c End Of File !! Well Done */
