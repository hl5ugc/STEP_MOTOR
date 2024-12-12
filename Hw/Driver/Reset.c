/*
 * [ PROJECT   ]  stm32g431_led
 * [ License   ]  SAMJIN ELECTRONICS.,Co.Ltd
 * [ Author    ]  Copyright 2024 By HAG-SEONG KANG
 * [ E-MAIL    ]  hl5ugc@nate.com (82)10- 3841-9706
 * [ C  P  U   ]
 * [ Compller  ]  
 * [ Filename  ]  Reset.c
 * [ Version   ]  1.0
 * [ Created   ]  2024[12:05:32 PM ]
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
 * Hagseong Kang  Apr 25, 2024    First release of this file
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
#include "Reset.h"
#include "rtc.h"
#ifdef _USE_HW_RESET
// ---------------------------------------------------------------------------
//  Define Macros .
// ---------------------------------------------------------------------------
//
#define RESET_REG_PARAM       0U
#define RESET_REG_COUNT       1U
// ---------------------------------------------------------------------------
//  Define Constants.
// ---------------------------------------------------------------------------
//
 
// ---------------------------------------------------------------------------
//  Define Private variables.
// ---------------------------------------------------------------------------
//
static uint32_t resetCount = 0x00U;
static uint32_t run_timeout_count = 0x00U ;
// ---------------------------------------------------------------------------
// Define Private function prototype.
// ---------------------------------------------------------------------------
//


static void resetToRunBoot(void);
// ---------------------------------------------------------------------------
// Define Public function definitions. 
// ---------------------------------------------------------------------------
//
bool resetInit(void)
{
  bool bRet    = true ;
  bool isDebug = false ;

  // if debug connection
  if(CoreDebug->DHCSR & CoreDebug_DHCSR_C_DEBUGEN_Msk)
  {
    isDebug = true ;
  }
  // Reset Pin Press
  if(RCC->CSR & (1<<26U) && isDebug == false)
  {
    rtcBackupRegWrite(RESET_REG_COUNT, rtcBackupRegRead(RESET_REG_COUNT)+1);
    delay(500);
    resetCount = rtcBackupRegRead(RESET_REG_COUNT);
  }

  rtcBackupRegWrite(RESET_REG_COUNT, 0U);


  return (bRet);
}

uint32_t resetGetCount(void)
{
  return resetCount;
}
void resetToBoot(uint32_t timeout)
{
  if(timeout == 0U)
  {
    resetToRunBoot();
  }
  else
  {
    run_timeout_count = timeout ;
  }
}
void resetToSysBoot(void)
{
  void (*SysMemBootJump)(void);
  volatile uint32_t addr = 0x1FFF0000U ;

  HAL_RCC_DeInit();

  SysTick->CTRL = 0U ;
  SysTick->LOAD = 0U ;
  SysTick->VAL  = 0U ;

  for(int i = 0 ; i<8; i++)
  {
    NVIC->ICER[i] = 0xFFFFFFFFU ;
    NVIC->ICPR[i] = 0xFFFFFFFFU ;

    __DSB();
    __ISB();
  }

  SysMemBootJump = (void (*)(void))(*((uint32_t *)(addr + 4)));
  //  Set Main Stack Pointer
  __set_MSP(*(uint32_t *)addr);
  //
  SysMemBootJump();
}
// ---------------------------------------------------------------------------
// Define private function definitions.  
// ---------------------------------------------------------------------------
//
void resetToRunBoot(void)
{
  uint32_t u32Reg = 0x00U ;

  u32Reg = rtcBackupRegRead(RESET_REG_PARAM);

  u32Reg |= (1<<0U);
  rtcBackupRegWrite(RESET_REG_PARAM, u32Reg);

  NVIC_SystemReset();
}
// ---------------------------------------------------------------------------
//  Define Callbacks definitions.
// ---------------------------------------------------------------------------
//
void resetISR(void)
{
  if (run_timeout_count > 0)
  {
    run_timeout_count--;

    if (run_timeout_count == 0)
    {
      resetToRunBoot();
    }
  }
}
 
#endif /*Reset.c End Of File !! Well Done */
