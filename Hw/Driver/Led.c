/*
 * [ PROJECT   ]  stm32g431_led
 * [ License   ]  SAMJIN ELECTRONICS.,Co.Ltd
 * [ Author    ]  Copyright 2024 By HAG-SEONG KANG
 * [ E-MAIL    ]  hl5ugc@nate.com (82)10- 3841-9706
 * [ C  P  U   ]
 * [ Compller  ]  
 * [ Filename  ]  Led.c
 * [ Version   ]  1.0
 * [ Created   ]  2024[9:46:40 AM ]
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
#include "Led.h"

// ---------------------------------------------------------------------------
//  Define Macros .
// ---------------------------------------------------------------------------
//
 
// ---------------------------------------------------------------------------
//  Define Constants.
// ---------------------------------------------------------------------------
//
typedef struct
{
  GPIO_TypeDef    *port;
  uint16_t        pin ;
  GPIO_PinState   onState ;
  GPIO_PinState   offState ;
} led_tbl_t;
// ---------------------------------------------------------------------------
//  Define Private variables.
// ---------------------------------------------------------------------------
//
static const  led_tbl_t led_tbl[LED_MAX_CH] = {
    {GPIOC,GPIO_PIN_6, GPIO_PIN_SET,GPIO_PIN_RESET},
};
// ---------------------------------------------------------------------------
// Define Private function prototype.
// ---------------------------------------------------------------------------
//
 
// ---------------------------------------------------------------------------
// Define Public function definitions. 
// ---------------------------------------------------------------------------
//
bool ledInit(void)
{
  uint16_t i = 0x00U;

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();

  /*Configure GPIO pin  */
  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull  = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

  for(i=0; i<LED_MAX_CH; i++)
  {
    GPIO_InitStruct.Pin = led_tbl[i].pin ;
    HAL_GPIO_Init(led_tbl[i].port, &GPIO_InitStruct);

    ledOff(i);
  }
  //
  return (true);
}
void ledOn(uint8_t u8Ch)
{
  if(u8Ch<LED_MAX_CH)
  {
    HAL_GPIO_WritePin(led_tbl[u8Ch].port, led_tbl[u8Ch].pin, led_tbl[u8Ch].onState);
  }
}
void ledOff(uint8_t u8Ch)
{
  if(u8Ch<LED_MAX_CH)
  {
    HAL_GPIO_WritePin(led_tbl[u8Ch].port, led_tbl[u8Ch].pin, led_tbl[u8Ch].offState);
  }
}
void ledToggle(uint8_t u8Ch)
{
  if(u8Ch<LED_MAX_CH)
  {
    HAL_GPIO_TogglePin(led_tbl[u8Ch].port, led_tbl[u8Ch].pin);
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
 
 
/*Led.c End Of File !! Well Done */
