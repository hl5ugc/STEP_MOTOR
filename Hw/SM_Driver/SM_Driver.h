/*
 * [ PROJECT   ]  stm32g431_SM_UART
 * [ License   ]  SAMJIN ELECTRONICS.,Co.Ltd
 * [ Author    ]  Copyright 2024 By HAG-SEONG KANG
 * [ E-MAIL    ]  hl5ugc@nate.com (82)10- 3841-9706
 * [ C  P  U   ]
 * [ Compller  ]  
 * [ Filename  ]  SM_Driver.h
 * [ Version   ]  1.0
 * [ Created   ]  2024[11:33:42 AM ]
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
#ifndef HW_SM_DRIVER_SM_DRIVER_H_
#define HW_SM_DRIVER_SM_DRIVER_H_

#ifdef __cplusplus
extern "C" {
#endif
/* Includes */
#include "hw_def.h"


 
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
void SM_Drive_Off(void);
void HalfStep(uint8_t dir);
void HalfStep_Run(uint8_t dir ,uint8_t inDelay, uint16_t steps);
void FullStep(uint8_t dir);
void FullStep_Run(uint8_t dir,uint8_t inDelay, uint16_t steps);
void FullStep_M8(uint8_t dir);
void FullStep_M8_Run(uint8_t dir,uint8_t inDelay, uint16_t steps);

#ifdef __cplusplus
}
#endif
#endif /* SM_Driver.h End Of File !! Well Done */
