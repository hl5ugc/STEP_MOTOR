/*
 * [ PROJECT   ]  stm32g431_SM_UART
 * [ License   ]  SAMJIN ELECTRONICS.,Co.Ltd
 * [ Author    ]  Copyright 2024 By HAG-SEONG KANG
 * [ E-MAIL    ]  hl5ugc@nate.com (82)10- 3841-9706
 * [ C  P  U   ]
 * [ Compller  ]  
 * [ Filename  ]  DataConvert.h
 * [ Version   ]  1.0
 * [ Created   ]  2024[9:53:15 AM ]
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
 * Hagseong Kang  Dec 12, 2024    First release of this file
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
#ifndef COMMON_CORE_DATACONVERT_H_
#define COMMON_CORE_DATACONVERT_H_

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

void cvtByte2DASC(uint8_t u8Data,uint8_t * const pBuff) ;
void cvtByte3DASC(uint8_t u8Data,uint8_t * const pBuff) ;
void cvtWord3DASC(uint16_t u16Data,uint8_t * const pBuff) ;
void cvtWord4DASC(uint16_t u16Data,uint8_t * const pBuff) ;
void cvtWord5DASC(uint16_t u16Data,uint8_t * const pBuff) ;
void cvtByte2HEX(uint8_t u8Data,uint8_t * const pBuff) ;
void cvtWord4HEX(uint16_t u16Data,uint8_t * const pBuff) ;
void cvtByte7BASC(uint8_t u8Data,uint8_t * const pBuff) ;
//
void cvt2HEXBinP(const uint8_t *pBuff , uint8_t  * const pData);
void cvt4HEXBinP(const uint8_t *pBuff , uint16_t * const pData);
void cvt6HEXBinP(const uint8_t *pBuff , uint32_t * const pData);
//
void cvt6ASCBin(const uint8_t *pBuff , uint16_t * const pData);
uint8_t  cvt2ASCBin(const uint8_t *pBuff);
uint16_t cvt3ASCBin(const uint8_t *pBuff);
uint16_t cvt4ASCBin(const uint8_t *pBuff);
uint16_t cvt5ASCBin(const uint8_t *pBuff);
#ifdef __cplusplus
}
#endif
#endif /* DataConvert.h End Of File !! Well Done */
