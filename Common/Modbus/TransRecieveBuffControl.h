/*
 * [ PROJECT   ]  5_stm32g431_UART
 * [ License   ]  SAMJIN ELECTRONICS.,Co.Ltd
 * [ Author    ]  Copyright 2024 By HAG-SEONG KANG
 * [ E-MAIL    ]  hl5ugc@nate.com (82)10- 3841-9706
 * [ C  P  U   ]
 * [ Compller  ]  
 * [ Filename  ]  TransRecieveBuffControl.h
 * [ Version   ]  1.0
 * [ Created   ]  2024[12:33:20 PM ]
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
#ifndef COMMON_MODBUS_TRANSRECIEVEBUFFCONTROL_H_
#define COMMON_MODBUS_TRANSRECIEVEBUFFCONTROL_H_

#ifdef __cplusplus
extern "C" {
#endif
/* Includes */
#include "def.h"


 
// ---------------------------------------------------------------------------
// Define Global define
// --------------------------------------------------------------------------- 
// 

// ---------------------------------------------------------------------------
// Define typedef
// --------------------------------------------------------------------------- 
// 
// ---------------------------------------------------------------------------
// Define  Global Function prototypes.
// --------------------------------------------------------------------------- 
//

bool     Modbus_Master_RB_Init(void);
uint8_t  Modbus_Master_Rece_Flush(void);
uint8_t  Modbus_Master_Rece_Available(void);
uint8_t  Modbus_Master_GetByte(uint8_t  *getbyte);
uint8_t  Modbus_Master_Rece_Handler(void);
uint8_t  Modbus_Master_Read(void);
uint8_t  Modbus_Master_Write(const uint8_t *buf,uint8_t length);
uint32_t Modbus_Master_Millis(void);

#ifdef __cplusplus
}
#endif
#endif /* TransRecieveBuffControl.h End Of File !! Well Done */
