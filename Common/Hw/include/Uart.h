/*
 * [ PROJECT   ]  5_stm32g431_UART
 * [ License   ]  SAMJIN ELECTRONICS.,Co.Ltd
 * [ Author    ]  Copyright 2024 By HAG-SEONG KANG
 * [ E-MAIL    ]  hl5ugc@nate.com (82)10- 3841-9706
 * [ C  P  U   ]
 * [ Compller  ]  
 * [ Filename  ]  Uart.h
 * [ Version   ]  1.0
 * [ Created   ]  2024[2:19:04 PM ]
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
 * Hagseong Kang  May 16, 2024    First release of this file
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
#ifndef COMMON_HW_INCLUDE_UART_H_
#define COMMON_HW_INCLUDE_UART_H_

#ifdef __cplusplus
extern "C" {
#endif
/* Includes */
#include "hw_def.h"
#ifdef _USE_HW_UART

 
// ---------------------------------------------------------------------------
// Define Global define
// --------------------------------------------------------------------------- 
// 
#define UART_MAX_CH       HW_UART_MAX_CH
// ---------------------------------------------------------------------------
// Define typedef
// --------------------------------------------------------------------------- 
// 

#define   UART1       0x00U   // uart1
#define   UART2       0x01U   //

extern UART_HandleTypeDef huart1;
// ---------------------------------------------------------------------------
// Define  Global Function prototypes.
// --------------------------------------------------------------------------- 
//
bool uartInit(void);
bool uartDeinit(void);
bool uartIsInit(void);
bool uartOpen(uint8_t ch,uint32_t baud);
bool uartClose(uint8_t ch);

uint32_t uartAvail(uint8_t ch);
bool     uartFlush(uint8_t ch);

uint32_t uartWrite(uint8_t ch,const uint8_t *tData);
uint32_t uartWriteBuff(uint8_t ch,const uint8_t *pBuff, uint32_t len);
uint8_t  uartRead(uint8_t ch);
uint32_t uartReadBuff(uint8_t ch,uint8_t * const pBuff, uint32_t len);
uint32_t uartPrintf(uint8_t ch, const char *fmt, ...);
uint32_t uartGetBaud(uint8_t ch);


#endif
#ifdef __cplusplus
}
#endif
#endif /* Uart.h End Of File !! Well Done */
