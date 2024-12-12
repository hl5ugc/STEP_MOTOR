/*
 * [ PROJECT   ]  stm32g431_SM_UART
 * [ License   ]  SAMJIN ELECTRONICS.,Co.Ltd
 * [ Author    ]  Copyright 2024 By HAG-SEONG KANG
 * [ E-MAIL    ]  hl5ugc@nate.com (82)10- 3841-9706
 * [ C  P  U   ]
 * [ Compller  ]  
 * [ Filename  ]  74hc595.h
 * [ Version   ]  1.0
 * [ Created   ]  2024[11:44:05 AM ]
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
#ifndef HW_DRIVER_74HC595_H_
#define HW_DRIVER_74HC595_H_

#ifdef __cplusplus
extern "C" {
#endif
/* Includes */
#include "hw_def.h"
#ifdef _USE_HW_74HC595

 
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
bool HC595Init(void);
void DacTbl_Set(uint16_t amp);
void DAC_AMPOUT(uint8_t AmpA,uint8_t AmpB);
void SM_DAC_AMPOUT(uint8_t inStep);
void SM_DAC_MAX_AMPOUT(void);
void SM_DAC_HALF_AMPOUT(void);
void SM_DAC_TBL_AMPOUT(uint8_t aAmp,uint8_t bAmp);
//

void SM_DAC_AMPOUT_Test(void);
#endif 
#ifdef __cplusplus
}
#endif
#endif /* 74hc595.h End Of File !! Well Done */
