/*
 * [ PROJECT   ]   
 * [ License   ]  SAMJIN ELECTRONICS.,Co.Ltd
 * [ Author    ]  Copyright 2023 By HAG-SEONG KANG
 * [ E-MAIL    ]  hl5ugc@nate.com (82)10- 3841-9706
 * [ C  P  U   ]
 * [ Compller  ]  
 * [ Filename  ]  hw_def.h
 * [ Version   ]  1.0
 * [ Created   ]  2023[11:47:55 AM ]
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
 * Hagseong Kang  Aug 7, 2023    First release of this file
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
#ifndef HW_HW_DEF_H_
#define HW_HW_DEF_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes */
#include "def.h"
#include "bsp.h"
// ---------------------------------------------------------------------------
// Define Global define
// --------------------------------------------------------------------------- 
// 
#define _USE_HW_HWTIMER

#define _USE_HW_SWTIMER               1
#define      HW_SWTIMER_MAX_CH        4

#define _USE_HW_LED
#define      HW_LED_MAX_CH            1

#define _USE_HW_DMA

#define _USE_HW_RTC
#define _USE_HW_RESET
#define _USE_HW_CDC
//#define _USE_HW_USB
//#define      HW_USE_CDC               1
#define _USE_HW_UART
#define      HW_UART_MAX_CH           1   // 1 : UART

#define _USE_HW_74HC595

#ifdef __cplusplus
}
#endif
#endif /* hw_def.h End Of File !! Well Done */
