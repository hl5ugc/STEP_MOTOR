/*
 * [ PROJECT   ]  stm32g431_fw
 * [ License   ]  SAMJIN ELECTRONICS.,Co.Ltd
 * [ Author    ]  Copyright 2024 By HAG-SEONG KANG
 * [ E-MAIL    ]  hl5ugc@nate.com (82)10- 3841-9706
 * [ C  P  U   ]
 * [ Compller  ]  
 * [ Filename  ]  hwTimer.h
 * [ Version   ]  1.0
 * [ Created   ]  2024[2:33:05 PM ]
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
 * Hagseong Kang  Apr 30, 2024    First release of this file
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
#ifndef COMMON_HW_INCLUDE_HWTIMER_H_
#define COMMON_HW_INCLUDE_HWTIMER_H_

#ifdef __cplusplus
extern "C" {
#endif
/* Includes */
#include "hw_def.h"
#ifdef _USE_HW_HWTIMER

 
// ---------------------------------------------------------------------------
// Define Global define
// --------------------------------------------------------------------------- 
// 

// ---------------------------------------------------------------------------
// Define typedef
// --------------------------------------------------------------------------- 
// 
extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim7;

// ---------------------------------------------------------------------------
// Define  Global Function prototypes.
// --------------------------------------------------------------------------- 
//
void HW_TIMER_Init(void);
void timerStart(void);
void tim6_delay(uint32_t delay);

void MX_TIM4_Init(void);
void MX_TIM6_Init(void);
void MX_TIM7_Init(void);

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);


#endif 
#ifdef __cplusplus
}
#endif
#endif /* hwTimer.h End Of File !! Well Done */
