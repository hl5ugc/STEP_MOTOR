/*
 * [ PROJECT   ]  stm32g431_SM_UART
 * [ License   ]  SAMJIN ELECTRONICS.,Co.Ltd
 * [ Author    ]  Copyright 2024 By HAG-SEONG KANG
 * [ E-MAIL    ]  hl5ugc@nate.com (82)10- 3841-9706
 * [ C  P  U   ]
 * [ Compller  ]  
 * [ Filename  ]  SM_PROTOCOL.c
 * [ Version   ]  1.0
 * [ Created   ]  2024[5:28:41 PM ]
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
 * Hagseong Kang  Dec 11, 2024    First release of this file
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
#include "SM_PROTOCOL.h"
#include "qBuffer.h"
#include "DataConvert.h"

#ifdef _USE_HW_UART
#include "uart.h"

#include "swTimer.h"
// ---------------------------------------------------------------------------
//  Define Macros .
// ---------------------------------------------------------------------------
//
#define SM_RX_BUFF_SIZE     30U
//
#define SM_STX              '@'
#define SM_ETX              '$'
#define SM_GET_DATA         '?'
#define SM_MODE             'M'
#define SM_FULL_STEP        'F'
#define SM_FULL_STEP_M      'M'
#define SM_HALF_STEP        'H'
#define SM_CURRENT_SET      'A'
#define SM_MAX_CURRENT      'M'
#define SM_HOLD_CURRENT     'H'
#define SM_UART_SET         'C'
#define SM_STEP_DIR         'D'
#define SM_STEPS            'S'
#define SM_STEP_WAIT        'W'
#define SM_STEP_RUN         'R'
// ---------------------------------------------------------------------------
//  Define TypeDef & Constants.
// ---------------------------------------------------------------------------
//
//typedef struct
//{
//  uint8_t  mode ;
//  uint16_t max_amp ;
//  uint16_t hold_amp ;
//  uint8_t  dir  ;
//  uint16_t steps ;
//  uint8_t  wait ;
//  uint8_t  runstop ;
//
//} STEMPMOTOR_SET_t;

// ---------------------------------------------------------------------------
//  Define Private variables.
// ---------------------------------------------------------------------------
//
qbuffer_t   SM_RX_RingBuff;
uint8_t     SM_RX_Buff[SM_RX_BUFF_SIZE];
uint8_t     swtimer_ch ;

static volatile STEMPMOTOR_SET_t SM_Set_Data ;

// ---------------------------------------------------------------------------
// Define Private function prototype.
// ---------------------------------------------------------------------------
//
static volatile uint8_t RxPasingON = 0U ;
// ---------------------------------------------------------------------------
// Define Callback function prototype.
// ---------------------------------------------------------------------------
//
void SM_RxCheck(void *arg);
void SM_RxParsing(void *arg);
// ---------------------------------------------------------------------------
// Define Public function definitions. 
// ---------------------------------------------------------------------------
//
bool SM_Protocol_Init(void)
{
  bool bRet = true ;

  qbufferCreate(&SM_RX_RingBuff, SM_RX_Buff, sizeof(SM_RX_Buff));

  if(swTimerGetCh(&swtimer_ch) == true)
  {
    swtimerSet(swtimer_ch, SM_RxCheck,NULL , LOOP_TIME, 1U);

    swtimerStart(swtimer_ch);
  }
  if(swTimerGetCh(&swtimer_ch) == true)
  {
    swtimerSet(swtimer_ch, SM_RxParsing,NULL , LOOP_TIME, 2U);

    swtimerStart(swtimer_ch);
  }

  SM_Set_Data.mode      = 'F' ;
  SM_Set_Data.max_amp   = 100U ;
  SM_Set_Data.hold_amp  = 50U ;
  SM_Set_Data.dir       = 0U ;    // CW
  SM_Set_Data.steps     = 200U ;
  SM_Set_Data.wait      = 2U ;
  SM_Set_Data.runstop   = 0U ;

  return (bRet);
}
//
STEMPMOTOR_SET_t SM_PTC_Get(void)
{

  return (SM_Set_Data);
}
void SM_PTC_Set_Stop(void)
{
  SM_Set_Data.runstop = 0x00U ;
}
// ---------------------------------------------------------------------------
// Define private function definitions.  
// ---------------------------------------------------------------------------
//
 
// ---------------------------------------------------------------------------
//  Define Callbacks definitions.
// ---------------------------------------------------------------------------
//



void SM_RxParsing(void *arg)
{
  uint8_t SM_RX_DATA[20];
  uint8_t len = 0U ;
  if(RxPasingON == 1U)
  {
    RxPasingON = 0U ;
    len = qbufferAvailable(&SM_RX_RingBuff) ;
    if(len < 20U)
    {
      qbufferRead(&SM_RX_RingBuff, SM_RX_DATA, len) ;

      if(SM_RX_DATA[0] == SM_STX)
      {
        switch(SM_RX_DATA[1])
        {
          case SM_MODE :        // STEP MOTOR MODE
            switch(SM_RX_DATA[2])
            {
              case SM_FULL_STEP :
                SM_Set_Data.mode = SM_FULL_STEP ;
                break ;
              case SM_FULL_STEP_M :
                SM_Set_Data.mode = SM_FULL_STEP_M ;
                break ;
              case SM_HALF_STEP :
                SM_Set_Data.mode = SM_HALF_STEP ;
                break ;
            }
            //uartPrintf(UART1, "SM Mode      = %c \n",SM_Set_Data.mode);
            break ;

          case SM_CURRENT_SET : // CURRENT SETTING
            switch(SM_RX_DATA[2])
            {
              case SM_MAX_CURRENT :
                SM_Set_Data.max_amp = cvt3ASCBin((const uint8_t *)&SM_RX_DATA[3]) ;
                //uartPrintf(UART1, "SM Max Amp   = %d \n",SM_Set_Data.max_amp);
                break ;
              case SM_HOLD_CURRENT :
                SM_Set_Data.hold_amp = cvt3ASCBin((const uint8_t *)&SM_RX_DATA[3]) ;
                //uartPrintf(UART1, "SM Hold Amp  = %d \n",SM_Set_Data.hold_amp);
                break ;
            }
            break ;

          case SM_UART_SET :    // UART STEP CONTROL
            switch(SM_RX_DATA[2])
            {
              case SM_STEP_DIR :
                SM_Set_Data.dir = SM_RX_DATA[3] & 0x0FU ;
                //uartPrintf(UART1, "SM DIR       = %d \n",SM_Set_Data.dir);
                break ;
              case SM_STEPS :
                SM_Set_Data.steps = cvt4ASCBin((const uint8_t *)&SM_RX_DATA[3]) ;
                //uartPrintf(UART1, "SM Steps Amp = %d \n",SM_Set_Data.steps);
                break ;
              case SM_STEP_WAIT :
                SM_Set_Data.wait = cvt2ASCBin((const uint8_t *)&SM_RX_DATA[3]) ;
                //uartPrintf(UART1, "SM Wait      = %d \n",SM_Set_Data.wait);
                break ;
              case SM_STEP_RUN :
                SM_Set_Data.runstop = SM_RX_DATA[3] & 0x0FU ;
                //uartPrintf(UART1, "SM Run/Stop  = %d \n",SM_Set_Data.runstop);
                break ;
            }
            break ;
        case SM_GET_DATA: // Send Setting Data
             uartPrintf(UART1, "Mode = %c\n",SM_Set_Data.mode);
             uartPrintf(UART1, "Max Amp = %d\n",SM_Set_Data.max_amp);
             uartPrintf(UART1, "Hold Amp = %d\n",SM_Set_Data.hold_amp);
             uartPrintf(UART1, "DIR = %d\n",SM_Set_Data.dir);
             uartPrintf(UART1, "Steps Amp = %d\n",SM_Set_Data.steps);
             uartPrintf(UART1, "Wait = %d\n",SM_Set_Data.wait);
            break;
        }
      }
      //uartWriteBuff(UART1, (const uint8_t *)&SM_RX_DATA[0], len);
    }
  }
}
void SM_RxCheck(void *arg)
{
  uint8_t rxData = 0U ;

  if(uartAvail(UART1) > 0U)
  {
    rxData = uartRead(UART1);

    if(rxData == SM_STX)
    {
      qbufferFlush(&SM_RX_RingBuff);
      RxPasingON = 0x00U ;
    }
    else if(rxData == SM_ETX)
    {
//      rxData = qbufferAvailable(&SM_RX_RingBuff);
//      uartPrintf(UART1, "RX Data = %X\n",rxData);
      RxPasingON = 0x01U ;
    }

    qbufferWrite(&SM_RX_RingBuff,&rxData,1U);

  }



}
// ---------------------------------------------------------------------------
//  Define CLI definitions.
// ---------------------------------------------------------------------------
//
#endif /* SM_PROTOCOL.c End Of File !! Well Done */
