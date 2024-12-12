/*
 * [ PROJECT   ]  stm32g431_SM_UART
 * [ License   ]  SAMJIN ELECTRONICS.,Co.Ltd
 * [ Author    ]  Copyright 2024 By HAG-SEONG KANG
 * [ E-MAIL    ]  hl5ugc@nate.com (82)10- 3841-9706
 * [ C  P  U   ]
 * [ Compller  ]  
 * [ Filename  ]  74hc595.c
 * [ Version   ]  1.0
 * [ Created   ]  2024[11:44:21 AM ]
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
#include "74hc595.h"
#ifdef _USE_HW_74HC595
// ---------------------------------------------------------------------------
//  Define Macros .
// ---------------------------------------------------------------------------
//
#define EN_PWM_A_L_Pin        GPIO_PIN_0
#define EN_PWM_A_L_GPIO_Port  GPIOA
#define EN_PWM_A_R_Pin        GPIO_PIN_1
#define EN_PWM_A_R_GPIO_Port  GPIOA
#define EN_PWM_B_L_Pin        GPIO_PIN_2
#define EN_PWM_B_L_GPIO_Port  GPIOA
#define EN_PWM_B_R_Pin        GPIO_PIN_3
#define EN_PWM_B_R_GPIO_Port  GPIOA
#define DAC_SCK_Pin           GPIO_PIN_5
#define DAC_SCK_GPIO_Port     GPIOA
#define DAC_SYNC_Pin          GPIO_PIN_6
#define DAC_SYNC_GPIO_Port    GPIOA
#define DAC_MOSI_Pin          GPIO_PIN_7
#define DAC_MOSI_GPIO_Port    GPIOA
#define Led_Blue_Pin          GPIO_PIN_6
#define Led_Blue_GPIO_Port    GPIOC
#define STEP_RUN_Pin          GPIO_PIN_4
#define STEP_RUN_GPIO_Port    GPIOB
#define STEP_DIR_Pin          GPIO_PIN_5
#define STEP_DIR_GPIO_Port    GPIOB
#define STEP_CLK_Pin          GPIO_PIN_6
#define STEP_CLK_GPIO_Port    GPIOB
#define STEP_CLK_EXTI_IRQn    EXTI9_5_IRQn
// ---------------------------------------------------------------------------
//  Define TypeDef & Constants.
// ---------------------------------------------------------------------------
//
 
// ---------------------------------------------------------------------------
//  Define Private variables.
// ---------------------------------------------------------------------------
//
const uint16_t CosTbl[16] = {10000U,9808U,9239U,8315U,7071U,5556U,3827U,1951U,
                               0U,1951U,3827U,5556U,7071U,8315U,9239U,9808U} ;
const uint16_t SinTbl[16] = {   0U,1951U,3827U,5556U,7071U,8315U,9239U,9808U,
                            10000U,9808U,9239U,8315U,7071U,5556U,3827U,1951U};
static uint8_t DacTbl_A[16] ;
static uint8_t DacTbl_B[16] ;
// ---------------------------------------------------------------------------
// Define Private function prototype.
// ---------------------------------------------------------------------------
//
static void DAC_DataOut(uint16_t inVal);
static void SM_GPIO_Init(void);
// ---------------------------------------------------------------------------
// Define Callback function prototype.
// ---------------------------------------------------------------------------
//

// ---------------------------------------------------------------------------
// Define Public function definitions. 
// ---------------------------------------------------------------------------
//
bool HC595Init(void)
{
  SM_GPIO_Init();
  DacTbl_Set(100U) ;
  //
  return (true);
}

void DacTbl_Set(uint16_t amp)
{
  uint8_t    i  = 0U ;
  uint8_t u8Cal = 0U ;
  uint32_t cal  = 0U ;
  float   fCal  = 0.425f ;

  // 300V  => 255  , 100 =
  fCal = fCal * amp ;
  u8Cal = fCal ;

  for(i=0U; i<16U; i++)
  {
    cal =  (uint32_t)SinTbl[i]  * u8Cal ;
    DacTbl_A[i] = (uint8_t)(cal / 10000U)  ;

    cal =  (uint32_t)CosTbl[i]  * u8Cal ;
    DacTbl_B[i] = (uint8_t)(cal / 10000U) ;
  }

}
/**
 *
 */
void DAC_AMPOUT(uint8_t AmpA,uint8_t AmpB)
{
  uint16_t u16Cal = 0x00U ;
  u16Cal =((uint16_t)AmpA << 8U) + AmpB ;
  DAC_DataOut(u16Cal) ;
}
void SM_DAC_MAX_AMPOUT(void)
{
  DAC_AMPOUT(DacTbl_A[8U],DacTbl_B[0U]) ;
}
void SM_DAC_HALF_AMPOUT(void)
{
  DAC_AMPOUT(DacTbl_A[4U],DacTbl_B[4U]) ;
}
void SM_DAC_TBL_AMPOUT(uint8_t aAmp,uint8_t bAmp)
{
  DAC_AMPOUT(DacTbl_A[aAmp],DacTbl_B[aAmp]) ;
}
void SM_DAC_AMPOUT(uint8_t inStep)
{
  DAC_AMPOUT(DacTbl_A[inStep%16U],DacTbl_B[inStep%16U]) ;
}
void SM_DAC_AMPOUT_Test(void)
{
  uint8_t i = 0U ;

  for(i=0U; i<255; i++)
  {
    DAC_AMPOUT(i , i) ;
    delay(1);
  }
}
// ---------------------------------------------------------------------------
// Define private function definitions.  
// ---------------------------------------------------------------------------
//
static void DAC_DataOut(uint16_t inVal)
{
  uint16_t i = 0x00U ;

  HAL_GPIO_WritePin(DAC_SCK_GPIO_Port, DAC_SCK_Pin, 0x00U) ;
  HAL_GPIO_WritePin(DAC_SYNC_GPIO_Port, DAC_SYNC_Pin, 0x01U) ;
  //
  for(i=0U; i<16U; i++)
  {

    if((inVal & 0x8000U) == 0x00U)
    {
      HAL_GPIO_WritePin(DAC_MOSI_GPIO_Port, DAC_MOSI_Pin, 0x00U) ;
    }
    else
    {
      HAL_GPIO_WritePin(DAC_MOSI_GPIO_Port, DAC_MOSI_Pin, 0x01U) ;
    }

    HAL_GPIO_WritePin(DAC_SCK_GPIO_Port, DAC_SCK_Pin, 0x01U) ;
    //
    inVal = (inVal) << 1U ;
    //
    HAL_GPIO_WritePin(DAC_SCK_GPIO_Port, DAC_SCK_Pin, 0x00U) ;

  }
  //
  HAL_GPIO_WritePin(DAC_SYNC_GPIO_Port, DAC_SYNC_Pin, 0x00U) ;
  //
  HAL_GPIO_WritePin(DAC_SYNC_GPIO_Port, DAC_SYNC_Pin, 0x01U) ;
  HAL_GPIO_WritePin(DAC_MOSI_GPIO_Port, DAC_MOSI_Pin, 0x00U) ;
}

static void SM_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, EN_PWM_A_L_Pin|EN_PWM_A_R_Pin|EN_PWM_B_L_Pin|EN_PWM_B_R_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, DAC_SCK_Pin|DAC_SYNC_Pin|DAC_MOSI_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(Led_Blue_GPIO_Port, Led_Blue_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : EN_PWM_A_L_Pin EN_PWM_A_R_Pin EN_PWM_B_L_Pin EN_PWM_B_R_Pin */
  GPIO_InitStruct.Pin = EN_PWM_A_L_Pin|EN_PWM_A_R_Pin|EN_PWM_B_L_Pin|EN_PWM_B_R_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : DAC_SCK_Pin DAC_SYNC_Pin DAC_MOSI_Pin */
  GPIO_InitStruct.Pin = DAC_SCK_Pin|DAC_SYNC_Pin|DAC_MOSI_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : Led_Blue_Pin */
  GPIO_InitStruct.Pin = Led_Blue_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(Led_Blue_GPIO_Port, &GPIO_InitStruct);
////////////////////////////////////////////////////////////////
  /*Configure GPIO pin Output Level */

  /*Configure GPIO pins : STEP_RUN_Pin STEP_DIR_Pin */
  GPIO_InitStruct.Pin = STEP_RUN_Pin|STEP_DIR_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : STEP_CLK_Pin */
  GPIO_InitStruct.Pin = STEP_CLK_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(STEP_CLK_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

}
// ---------------------------------------------------------------------------
//  Define Callbacks definitions.
// ---------------------------------------------------------------------------
//
 
// ---------------------------------------------------------------------------
//  Define CLI definitions.
// ---------------------------------------------------------------------------
//
#endif /* 74hc595.c End Of File !! Well Done */
