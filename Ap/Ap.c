/*
 * [ PROJECT   ]  stm32g431_fw
 * [ License   ]  SAMJIN ELECTRONICS.,Co.Ltd
 * [ Author    ]  Copyright 2024 By HAG-SEONG KANG
 * [ E-MAIL    ]  hl5ugc@nate.com (82)10- 3841-9706
 * [ C  P  U   ]
 * [ Compller  ]  
 * [ Filename  ]  Ap.c
 * [ Version   ]  1.0
 * [ Created   ]  2024[6:09:33 PM ]
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
 * Hagseong Kang  Apr 24, 2024    First release of this file
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
#include "Ap.h"
#include "SM_PROTOCOL.h"

// ---------------------------------------------------------------------------
//  Define Macros .
// ---------------------------------------------------------------------------
//
 
// ---------------------------------------------------------------------------
//  Define Constants.
// ---------------------------------------------------------------------------
//
 
// ---------------------------------------------------------------------------
//  Define Private variables.
// ---------------------------------------------------------------------------
//
static STEMPMOTOR_SET_t SM_Set_Data ;
static STEMPMOTOR_SET_t SM_Set_Data_old ;
static uint32_t Exti6_Count  ;
// ---------------------------------------------------------------------------
// Define Private function prototype.
// ---------------------------------------------------------------------------
//
static void SM_RUN_Main(void);
// ---------------------------------------------------------------------------
// Define Public function definitions.
// ---------------------------------------------------------------------------
//
void apInit(void)
{
  uartOpen(UART1,38400UL);

  SM_Protocol_Init();
}

/**
 *
 */
void apMain(void)
{
  uint32_t preTime  = 0UL ;
  uint32_t preTime2 = 0UL ;
  uint32_t i = 0UL;

  UNUSED(preTime2);
  UNUSED(i);

  timerStart();
  //

  //
  preTime = millis();
  SM_Set_Data = SM_PTC_Get();
  SM_Set_Data_old = SM_Set_Data ;

  while(1)
  {
    if((millis()-preTime) >= 500UL)
    {
      preTime = millis();
      ledToggle(_DEF_LED1);
      if((GPIOB->IDR & _DEF_BIT4) != 0x00U)
      {
        SM_Drive_Off();
        DAC_AMPOUT(0x00,0x00U);
      }
    }
    if(SM_Set_Data.hold_amp != SM_Set_Data.max_amp)
    {
      SM_Set_Data_old = SM_Set_Data ;
      DacTbl_Set(SM_Set_Data.max_amp);
    }
    SM_RUN_Main();


  }
}
// ---------------------------------------------------------------------------
// Define private function definitions.  
// ---------------------------------------------------------------------------
//
static void SM_RUN_Main(void)
{
  SM_Set_Data = SM_PTC_Get();

  if(SM_Set_Data.runstop == 1U)
  {
    uartPrintf(UART1, "SM Mode      = %c \n",SM_Set_Data.mode);
    uartPrintf(UART1, "SM Max Amp   = %d \n",SM_Set_Data.max_amp);
    uartPrintf(UART1, "SM Hold Amp  = %d \n",SM_Set_Data.hold_amp);
    uartPrintf(UART1, "SM DIR       = %d \n",SM_Set_Data.dir);
    uartPrintf(UART1, "SM Steps Amp = %d \n",SM_Set_Data.steps);
    uartPrintf(UART1, "SM Wait      = %d \n",SM_Set_Data.wait);
    uartPrintf(UART1, "SM Run/Stop  = %d \n",SM_Set_Data.runstop);

    if(SM_Set_Data.mode == 'F')
    {
      DacTbl_Set(SM_Set_Data.max_amp);
      SM_PTC_Set_Stop();
      uartPrintf(UART1, "Full Step Motor Run \n");
      FullStep_Run(SM_Set_Data.dir,SM_Set_Data.wait,SM_Set_Data.steps);
      uartPrintf(UART1, "Full Step Motor Stop \n");

    }
    else if(SM_Set_Data.mode == 'M')
    {
      DacTbl_Set(SM_Set_Data.max_amp);
      SM_PTC_Set_Stop();
      uartPrintf(UART1, "Micro  8 Step Motor Run \n");
      FullStep_M8_Run(SM_Set_Data.dir,SM_Set_Data.wait,SM_Set_Data.steps);
      uartPrintf(UART1, "Micro  8 Step Motor Stop \n");

    }
    else if(SM_Set_Data.mode == 'H')
    {
     DacTbl_Set(SM_Set_Data.max_amp);
     SM_PTC_Set_Stop();

     uartPrintf(UART1, "Half Step Motor Run \n");
     HalfStep_Run(SM_Set_Data.dir,SM_Set_Data.wait,SM_Set_Data.steps);
     uartPrintf(UART1, "Half Step Motor Stop \n");

    }
    else
    {
      SM_PTC_Set_Stop();
      uartPrintf(UART1, "Mode Error \n");
    }
  }

}
// ---------------------------------------------------------------------------
//  Define Callbacks definitions.
// ---------------------------------------------------------------------------
//
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  uint8_t sm_dir = 0x00U ;
  Exti6_Count++ ;
  if(GPIO_Pin == GPIO_PIN_6)
  {
    if((GPIOB->IDR & _DEF_BIT4) == 0x00U)
    {
      if((GPIOB->IDR & _DEF_BIT5) == 0x00U) { sm_dir = 0x00U ; }
      else  { sm_dir = 0x01U ; }
      //
      if(SM_Set_Data.mode == 'F')
      {
        FullStep(sm_dir);
      }
      else if(SM_Set_Data.mode == 'M')
      {
        FullStep_M8(sm_dir);
      }
      else if(SM_Set_Data.mode == 'H')
      {
       HalfStep(sm_dir);
      }

    }

  }
}
 
/*Ap.c End Of File !! Well Done */
