/*
 * [ PROJECT   ]  stm32g431_fw
 * [ License   ]  SAMJIN ELECTRONICS.,Co.Ltd
 * [ Author    ]  Copyright 2024 By HAG-SEONG KANG
 * [ E-MAIL    ]  hl5ugc@nate.com (82)10- 3841-9706
 * [ C  P  U   ]
 * [ Compller  ]  
 * [ Filename  ]  swTimer.h
 * [ Version   ]  1.0
 * [ Created   ]  2024[11:35:55 AM ]
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
#ifndef COMMON_HW_INCLUDE_SWTIMER_H_
#define COMMON_HW_INCLUDE_SWTIMER_H_

#ifdef __cplusplus
extern "C" {
#endif
/* Includes */
#include "hw_def.h"
#ifdef _USE_HW_SWTIMER

 
// ---------------------------------------------------------------------------
// Define Global define
// --------------------------------------------------------------------------- 
// 
#ifndef ON
#define ON              0x01U
#endif
#ifndef OFF
#define OFF             0x00U
#endif


#define SWTIMER_MAX_CH  HW_SWTIMER_MAX_CH

// ---------------------------------------------------------------------------
// Define typedef
// --------------------------------------------------------------------------- 
// 
typedef int16_t         swtimer_handle_t ;
typedef enum
{
  NORMAL    = 0x00U ,
  ONE_TIME  = 0x01U ,
  LOOP_TIME = 0x02U ,
} SwTimerMode_e;
// ---------------------------------------------------------------------------
// Define  Global Function prototypes.
// --------------------------------------------------------------------------- 
//
bool swtimerInit(void);
bool swTimerGetCh(uint8_t *p_ch);

void swtimerSet (uint8_t ch,
                 void (*func)(void *arg),
                 void *func_arg ,
                 SwTimerMode_e    mode,
                 uint8_t tims_ms);

void swtimerStart(swtimer_handle_t timNum);
void swtimerReset(swtimer_handle_t timNum);
void swtimerISR(void);

swtimer_handle_t swtimerGetHandle(void);
uint32_t swtimerGetTimer(void);

#endif 
#ifdef __cplusplus
}
#endif
#endif /* swTimer.h End Of File !! Well Done */
