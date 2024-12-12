/*
 * [ PROJECT   ]  stm32g431_SM_UART
 * [ License   ]  SAMJIN ELECTRONICS.,Co.Ltd
 * [ Author    ]  Copyright 2024 By HAG-SEONG KANG
 * [ E-MAIL    ]  hl5ugc@nate.com (82)10- 3841-9706
 * [ C  P  U   ]
 * [ Compller  ]  
 * [ Filename  ]  SM_Driver.c
 * [ Version   ]  1.0
 * [ Created   ]  2024[11:33:57 AM ]
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



/* Define Includes */
#include "SM_Driver.h"
#include "74hc595.h"
#include "hwTimer.h"
// ---------------------------------------------------------------------------
//  Define Macros .
// ---------------------------------------------------------------------------
//
 
// ---------------------------------------------------------------------------
//  Define TypeDef & Constants.
// ---------------------------------------------------------------------------
//
 
// ---------------------------------------------------------------------------
//  Define Private variables.
// ---------------------------------------------------------------------------
//
static uint16_t FcurrentStep = 0x00U ;
static uint16_t HcurrentStep = 0x00U ;
static uint16_t McurrentStep = 0x00U ;
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
void SM_Drive_Off(void)
{
  FcurrentStep = 0x00U ;
  McurrentStep = 0x00U ;
  FcurrentStep = 0x00U ;
  GPIOA->ODR &=  0xFFF0U ;
}
// EN1 EN PWM A L   Bit 0
// EN2 EN PWM A R   Bit 1
// EN3 EN PWM B L   Bit 2
// EN4 EN PWM B R   Bit 3
void HalfStep_Run(uint8_t dir ,uint8_t inDelay, uint16_t steps)
{
  uint16_t i = 0x00U ;

  for(i=0U; i<steps; i++)
  {
    HalfStep(dir);
    tim6_delay(inDelay);
  }
  //
  HcurrentStep = 0x00U ;
  GPIOA->ODR &=  0xFFF0U ;
}
void HalfStep(uint8_t dir)
{
  static uint16_t currentStep = 0x00U ;


  switch(currentStep)
  {
    case 0x07U:   // 7:  EN1 Enable , EN2 Disable , EN3 Disable , EN4 Enable
      SM_DAC_TBL_AMPOUT(8U,0U);
      GPIOA->ODR &=  0xFFF9U ;
      GPIOA->ODR |=  0x0009U ;
      break ;
    case 0x00U:
      SM_DAC_TBL_AMPOUT(8U,8U);
      // 0: EN1 Enable , EN2 Disable , EN3 Disable , EN4 Disable
      GPIOA->ODR &=  0xFFF1U ;
      GPIOA->ODR |=  0x0001U ;
      break ;
    case 0x01U:   // 1: EN1 Enable , EN2 Disable , EN3 Enable , EN4 Disable
      SM_DAC_TBL_AMPOUT(8U,0U);
      GPIOA->ODR &=  0xFFF5U ;
      GPIOA->ODR |=  0x0005U ;
      break ;
    case 0x02U:   // 2:  EN1 Disable , EN2 Disable , EN3 Enable , EN4 Disable
      SM_DAC_TBL_AMPOUT(0U,0U);
      GPIOA->ODR &=  0xFFF4U ;
      GPIOA->ODR |=  0x0004U ;
      break ;
    case 0x03U:   // 3:  EN1 Disable , EN2 Enable , EN3 Enable , EN4 Disable
      SM_DAC_TBL_AMPOUT(8U,0U);
      GPIOA->ODR &=  0xFFF6U ;
      GPIOA->ODR |=  0x0006U ;
      break ;
    case 0x04U:   // 4: EN1 Disable , EN2 Enable , EN3 Disable , EN4 Disable
      SM_DAC_TBL_AMPOUT(8U,8U);
      GPIOA->ODR &=  0xFFF2U ;
      GPIOA->ODR |=  0x0002U ;
      break ;
    case 0x05U:   // 5: EN1 Disable , EN2 Enable , EN3 Disable , EN4 Enable
      SM_DAC_TBL_AMPOUT(8U,0U);
      GPIOA->ODR &=  0xFFFAU ;
      GPIOA->ODR |=  0x000AU ; ;
      break ;
    case 0x06U:   // 6:  EN1 Disable , EN2 Disable , EN3 Disable , EN4 Enable
      SM_DAC_TBL_AMPOUT(0U,0U);
      GPIOA->ODR &=  0xFFF8U ;
      GPIOA->ODR |=  0x0008U ;
      break ;
    default:
      break ;
  }
  //
  if(dir == 0x00U)
  {
    currentStep = (currentStep + 1U) % 8U ;
  }
  else
  {
    if(currentStep == 0x00U)
    {
      currentStep = 7U ;
    }
    else
    {
      currentStep = (currentStep - 1U) % 8U ;
    }
  }
}
/**
 *
 */
void FullStep_M8_Run(uint8_t dir,uint8_t inDelay, uint16_t steps)
{
  uint16_t i = 0 ;

  for(i=0; i<steps; i++)
  {
    FullStep_M8(dir);
    //delay(inDelay);
    tim6_delay(inDelay);
  }
  //
  McurrentStep = 0x00U ;
  GPIOA->ODR &=  0xFFF0U ;
}
/**
 *
 */
// full step 8 * 4  = 32
void FullStep_M8(uint8_t dir)
{
  static uint16_t currentStep = 0x00U ;

  SM_DAC_AMPOUT(currentStep);

  switch(currentStep/8U)
  {
    case 0x00U : // EN4 enable, EN3 disable , EN2 disable, EN1 enable
      GPIOA->ODR &=  0xFFF9U ;
      GPIOA->ODR |=  0x0009U ;
      break ;

    case 0x01U : // EN4 disable, EN3 enable , EN2 disable, EN1 enable
      GPIOA->ODR &=  0xFFF5U ;
      GPIOA->ODR |=  0x0005U ;
      break ;
    case 0x02U : // EN4 disable, EN3 enable , EN2 enable, EN1 disable
      GPIOA->ODR &=  0xFFF6U ;
      GPIOA->ODR |=  0x0006U ;
      break ;
    case 0x03U : // EN4 enable, EN3 disable , EN2 enable, EN1 disable
      GPIOA->ODR &=  0xFFFAU ;
      GPIOA->ODR |=  0x000AU ;
      break ;
  }

  if(dir == 0x00U)
  {
    currentStep = (currentStep + 1U) % 32U ;
  }
  else
  {
    if(currentStep == 0x00U)
    {
      currentStep = 31U ;
    }
    else
    {
      currentStep = (currentStep - 1U) % 32U ;
    }
  }
}
/**
 *
 */
void FullStep_Run(uint8_t dir,uint8_t inDelay, uint16_t steps)
{
  uint16_t i = 0 ;

  for(i=0; i<steps; i++)
  {
    FullStep(dir);
    tim6_delay(inDelay*8U);
  }
  //
  FcurrentStep = 0x00U ;
  GPIOA->ODR  &=  0xFFF0U ;
}
/**
 *
 */
void FullStep(uint8_t dir)
{
  static uint16_t currentStep = 0x00U ;

  SM_DAC_MAX_AMPOUT();

  switch(currentStep)
  {
    case 0x00U : // EN4 enable, EN3 disable , EN2 disable, EN1 enable
      GPIOA->ODR &=  0xFFF9U ;
      GPIOA->ODR |=  0x0009U ;
      break ;

    case 0x01U : // EN4 disable, EN3 enable , EN2 disable, EN1 enable
      GPIOA->ODR &=  0xFFF5U ;
      GPIOA->ODR |=  0x0005U ;
      break ;
    case 0x02U : // EN4 disable, EN3 enable , EN2 enable, EN1 disable
      GPIOA->ODR &=  0xFFF6U ;
      GPIOA->ODR |=  0x0006U ;
      break ;
    case 0x03U : // EN4 enable, EN3 disable , EN2 enable, EN1 disable
      GPIOA->ODR &=  0xFFFAU ;
      GPIOA->ODR |=  0x000AU ;
      break ;
  }

  if(dir == 0x00U)
  {
    currentStep = (currentStep + 1U) % 4U ;
  }
  else
  {
    if(currentStep == 0x00U)
    {
      currentStep = 3U ;
    }
    else
    {
      currentStep = (currentStep - 1U) % 4U ;
    }
  }
}
// ---------------------------------------------------------------------------
// Define private function definitions.
// ---------------------------------------------------------------------------
//

// ---------------------------------------------------------------------------
//  Define Callbacks definitions.
// ---------------------------------------------------------------------------
//
 
// ---------------------------------------------------------------------------
//  Define CLI definitions.
// ---------------------------------------------------------------------------
//
/* SM_Driver.c End Of File !! Well Done */
