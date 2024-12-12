/*
 * [ PROJECT   ]  stm32g431_fw
 * [ License   ]  SAMJIN ELECTRONICS.,Co.Ltd
 * [ Author    ]  Copyright 2024 By HAG-SEONG KANG
 * [ E-MAIL    ]  hl5ugc@nate.com (82)10- 3841-9706
 * [ C  P  U   ]
 * [ Compller  ]  
 * [ Filename  ]  swTimer.c
 * [ Version   ]  1.0
 * [ Created   ]  2024[11:38:36 AM ]
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



/* Define Includes */
#include "swTimer.h"
#ifdef _USE_HW_SWTIMER
// ---------------------------------------------------------------------------
//  Define Macros .
// ---------------------------------------------------------------------------
//
 
// ---------------------------------------------------------------------------
//  Define TypeDef & Constants.
// ---------------------------------------------------------------------------
//
typedef struct
{

  uint8_t  Timer_En;             // Timer enable signal
  SwTimerMode_e  Timer_Mode;     // Timer Mode
  uint32_t Timer_Ctn;            // Current Timer Value
  uint32_t Timer_Init;           // Count value when timer is initialized
  void (*TimFnct)(void *);       // Function to run when timer count expires
  void *TimFnctArg;              // Arguments to pass as a function
} swtimer_t;
// ---------------------------------------------------------------------------
//  Define Private variables.
// ---------------------------------------------------------------------------
//
extern TIM_HandleTypeDef htim7;

static volatile uint32_t sw_timer_counter      = 0x0U;
static volatile uint16_t sw_timer_handle_index = 0x0U;
static swtimer_t  swtimer_tbl[SWTIMER_MAX_CH];          // Timer Array Declaration
// ---------------------------------------------------------------------------
// Define Private function prototype.
// ---------------------------------------------------------------------------
//

// ---------------------------------------------------------------------------
// Define Callback function prototype.
// ---------------------------------------------------------------------------
//

// ---------------------------------------------------------------------------
// Define Public function definitions. 
// ---------------------------------------------------------------------------
//
bool swtimerInit(void)
{
  uint8_t u8I = 0x00U ;
  static uint8_t execute = 0x00U ;

  if(execute == 0x00U)
  {
    for(u8I=0x00U; u8I<SWTIMER_MAX_CH ; u8I++)
    {
      swtimer_tbl[u8I].Timer_En   = OFF ;
      swtimer_tbl[u8I].Timer_Ctn  = 0x00U ;
      swtimer_tbl[u8I].Timer_Init = 0x00U ;
      swtimer_tbl[u8I].TimFnct    = NULL ;
    }
  }
  execute = 0x01U ;

  return true ;
}
/**
 *
 */

void swtimerSet (uint8_t ch,
                 void (*func)(void *arg),
                 void *func_arg ,
                 SwTimerMode_e    mode,
                 uint8_t tims_ms)
{
  if(ch < SWTIMER_MAX_CH)
  {
    swtimer_tbl[ch].Timer_Init = tims_ms ;
    swtimer_tbl[ch].Timer_Ctn  = tims_ms ;
    swtimer_tbl[ch].Timer_Mode = mode ;  // Timer Mode Set
 
    swtimer_tbl[ch].TimFnct    = func ;     // Run a function
    swtimer_tbl[ch].TimFnctArg = func_arg ;
  }
}
void swtimerStart(swtimer_handle_t timNum)
{
  if(timNum < SWTIMER_MAX_CH)
  {
    swtimer_tbl[timNum].Timer_Ctn  = swtimer_tbl[timNum].Timer_Init ;
    swtimer_tbl[timNum].Timer_En   = ON ;
  }
}
void swtimerReset(swtimer_handle_t timNum)
{
  if(timNum < SWTIMER_MAX_CH)
  {
    swtimer_tbl[timNum].Timer_Ctn  = swtimer_tbl[timNum].Timer_Init ;
    swtimer_tbl[timNum].Timer_En   = OFF ;
  }
}
/**
 * Get Next Timer handle Index data if index is -1 then Not Use Swtimer.
 */
swtimer_handle_t swtimerGetHandle(void)
{
  swtimer_handle_t timIndex = sw_timer_handle_index;

  if(timIndex < SWTIMER_MAX_CH)
  {
    sw_timer_handle_index++ ;
    timIndex = sw_timer_handle_index ;
  }
  else
  {
    timIndex = -1 ;
  }

  return (timIndex);
}
//
bool swTimerGetCh(uint8_t *p_ch)
{
  bool bRet = false ;

  swtimer_handle_t timIndex = sw_timer_handle_index;

  if(sw_timer_handle_index < SWTIMER_MAX_CH)
  {
    *p_ch = sw_timer_handle_index ;
    sw_timer_handle_index++ ;
    bRet = true ;
  }

  return (bRet);
}
uint32_t swtimerGetTimer(void)
{
  return sw_timer_counter;
}
// ---------------------------------------------------------------------------
// Define private function definitions.  
// ---------------------------------------------------------------------------
//

// ---------------------------------------------------------------------------
//  Define Callbacks definitions.
// ---------------------------------------------------------------------------
//

void swtimerISR(void)
{
  uint8_t u8I = 0x00U ;

  sw_timer_counter++ ;

  for(u8I=0x00U; ((u8I<SWTIMER_MAX_CH) && (u8I<sw_timer_handle_index)); u8I++) // By the number of timers
  {
    if(swtimer_tbl[u8I].Timer_En == ON)                           // Is the timer activated
    {
      swtimer_tbl[u8I].Timer_Ctn-- ;                              // Decrease timer value

      if(swtimer_tbl[u8I].Timer_Ctn == 0x00U)                     // Timer Overfloor
      {
        if(swtimer_tbl[u8I].Timer_Mode == ONE_TIME)               // If you run it once
        {
          swtimer_tbl[u8I].Timer_En = OFF ;                       // Turn off the timer.
        }
        swtimer_tbl[u8I].Timer_Ctn = swtimer_tbl[u8I].Timer_Init;   // Timer initialization for Restart
        (*swtimer_tbl[u8I].TimFnct)(swtimer_tbl[u8I].TimFnctArg);   // Run a function
      }
    }
  }
}
// ---------------------------------------------------------------------------
//  Define CLI definitions.
// ---------------------------------------------------------------------------
//
#endif /* swTimer.c End Of File !! Well Done */
