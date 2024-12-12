/*
 * [ PROJECT   ]  5_stm32g431_UART
 * [ License   ]  SAMJIN ELECTRONICS.,Co.Ltd
 * [ Author    ]  Copyright 2024 By HAG-SEONG KANG
 * [ E-MAIL    ]  hl5ugc@nate.com (82)10- 3841-9706
 * [ C  P  U   ]
 * [ Compller  ]  
 * [ Filename  ]  Uart.c
 * [ Version   ]  1.0
 * [ Created   ]  2024[2:19:24 PM ]
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
 * Hagseong Kang  May 16, 2024    First release of this file
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
#include "Uart.h"
#include "dma.h"
#include "cdc.h"
#include "qbuffer.h"
#include "led.h"

#ifdef _USE_HW_UART

// ---------------------------------------------------------------------------
//  Define Macros .
// ---------------------------------------------------------------------------
//
#define UART_RX_BUF_LENGTH      256U
#define UART_TX_BUF_LENGTH      256U
// ---------------------------------------------------------------------------
//  Define TypeDef & Constants.
// ---------------------------------------------------------------------------
//
typedef struct
{
  bool      isOpen ;
  uint32_t  baud ;

  uint8_t   rxBuf[UART_RX_BUF_LENGTH] ;
  uint8_t   txBuf[UART_TX_BUF_LENGTH] ;
  qbuffer_t rxQbuff ;
  qbuffer_t txQbuff ;

  UART_HandleTypeDef  *pHuart;
  DMA_HandleTypeDef   *pHdma_rx ;
  DMA_HandleTypeDef   *pHdma_tx ;

  uint32_t rx_cnt ;
  uint32_t tx_cnt ;

} uart_tbl_t;

typedef struct
{
  const char          *pMsg;
  USART_TypeDef       *pUart;
  UART_HandleTypeDef  *pHuart;
  DMA_HandleTypeDef   *pHdma_rx;
  DMA_HandleTypeDef   *pHdma_tx;
  bool                 is_rs485;

} uart_hw_t;

// ---------------------------------------------------------------------------
//  Define Private variables.
// ---------------------------------------------------------------------------
//
UART_HandleTypeDef huart1;
DMA_HandleTypeDef  hdma_usart1_rx;
DMA_HandleTypeDef  hdma_usart1_tx;

const static uart_hw_t uart_hw_tbl[UART_MAX_CH] =
{
  {"USART1 SWD   ", USART1, &huart1, &hdma_usart1_rx, &hdma_usart1_tx, false},
};

static bool isInit = false  ;
static uart_tbl_t uart_tbl[UART_MAX_CH];

volatile static bool txDMAOK = true ;
static uint8_t   txDMABuff[UART_RX_BUF_LENGTH];

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
bool uartInit(void)
{
  bool bRet = true ;

  for(int i=0; i<UART_MAX_CH; i++)
  {
    uart_tbl[i].isOpen = false ;
    uart_tbl[i].baud   = 57600UL ;
    uart_tbl[i].rx_cnt = 0UL ;
    uart_tbl[i].tx_cnt = 0UL ;
  }

  isInit = true ;

  return (bRet);
}
bool uartDeInit(void)
{
  return true;
}

bool uartIsInit(void)
{
  return isInit;
}
//
bool uartOpen(uint8_t ch,uint32_t baud)
{
  bool bRet = false ;
  HAL_StatusTypeDef hRet = HAL_ERROR ;

  if(ch < UART_MAX_CH)
  {
    if((uart_tbl[ch].isOpen == false) || (uart_tbl[ch].baud != baud))
    {
      switch(ch)
      {
        case UART1:
          uart_tbl[ch].baud  = baud ;
          uart_tbl[ch].pHuart   = uart_hw_tbl[ch].pHuart ;
          uart_tbl[ch].pHdma_rx = uart_hw_tbl[ch].pHdma_rx ;
          uart_tbl[ch].pHdma_tx = uart_hw_tbl[ch].pHdma_tx ;

          uart_tbl[ch].pHuart->Instance             = uart_hw_tbl[ch].pUart ;
          uart_tbl[ch].pHuart->Init.BaudRate        = baud ;
          uart_tbl[ch].pHuart->Init.WordLength      = UART_WORDLENGTH_8B;
          uart_tbl[ch].pHuart->Init.StopBits        = UART_STOPBITS_1;
          uart_tbl[ch].pHuart->Init.Parity          = UART_PARITY_NONE;
          uart_tbl[ch].pHuart->Init.Mode            = UART_MODE_TX_RX;
          uart_tbl[ch].pHuart->Init.HwFlowCtl       = UART_HWCONTROL_NONE;
          uart_tbl[ch].pHuart->Init.OverSampling    = UART_OVERSAMPLING_16;
          uart_tbl[ch].pHuart->Init.OneBitSampling  = UART_ONE_BIT_SAMPLE_DISABLE;
          uart_tbl[ch].pHuart->Init.ClockPrescaler  = UART_PRESCALER_DIV1;
          uart_tbl[ch].pHuart->AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;

          qbufferCreate(&uart_tbl[ch].rxQbuff, &uart_tbl[ch].rxBuf[0] , UART_RX_BUF_LENGTH);
          qbufferCreate(&uart_tbl[ch].txQbuff, &uart_tbl[ch].txBuf[0] , UART_TX_BUF_LENGTH);

          __HAL_RCC_DMAMUX1_CLK_ENABLE();
          __HAL_RCC_DMA1_CLK_ENABLE();
          HAL_NVIC_SetPriority(DMA1_Channel2_IRQn, 0, 0);
          HAL_NVIC_EnableIRQ(DMA1_Channel2_IRQn);

          HAL_UART_DeInit(uart_tbl[ch].pHuart);
          hRet = HAL_UART_Init(uart_tbl[ch].pHuart);
          if(hRet == HAL_OK)
          {
            bRet = true ;
            uart_tbl[ch].isOpen = true ;

            if(HAL_UART_Receive_DMA(uart_tbl[ch].pHuart, &uart_tbl[ch].rxBuf[0] , UART_RX_BUF_LENGTH) != HAL_OK)
            {
              bRet = false;
            }

            uart_tbl[ch].rxQbuff.in  = (uart_tbl[ch].rxQbuff.len -  ((DMA_Channel_TypeDef *)uart_tbl[ch].pHdma_rx->Instance)->CNDTR);
            uart_tbl[ch].rxQbuff.out = uart_tbl[ch].rxQbuff.in;

            txDMAOK = true ;
          }
          break ;

        case UART2:
          uart_tbl[ch].baud    = baud;
          uart_tbl[ch].isOpen  =  true;
          bRet= true;
          break ;
      }
    }
  }
  return (bRet);
}
bool uartClose(uint8_t ch)
{
  bool bRet = false ;

  if(ch < UART_MAX_CH)
  {
    uart_tbl[ch].isOpen  =  false;
  }

  return (bRet);
}
uint32_t uartWrite(uint8_t ch,const uint8_t *tData)
{
  uint32_t u32Ret = 0x00UL ;
  //HAL_StatusTypeDef halStatus = HAL_ERROR ;

  switch(ch)
  {
    case UART1:
      // HAL_UART_Transmit(&huart1, (const uint8_t *)tData, 1, 1000U);
      // HAL_UART_Transmit_IT(&huart1, (const uint8_t *)tData, 1);

      break ;
    case UART2:
      //u32Ret = cdcWrite(&tData, 1);

      break ;
  }
  return (u32Ret);
}
uint32_t uartWriteBuff(uint8_t ch,const uint8_t *pBuff, uint32_t len)
{
  uint32_t u32Ret = 0x00UL ;

  switch(ch)
  {
    case UART1:

      while(len > qbufferWriteAvailable(&uart_tbl[ch].txQbuff))
      {
        delay(1);
      }
      //
      qbufferWrite(&uart_tbl[ch].txQbuff,(uint8_t *)pBuff, len);

      if(txDMAOK == true)
      {
        u32Ret = qbufferAvailable(&uart_tbl[ch].txQbuff) ;
        qbufferRead(&uart_tbl[ch].txQbuff, txDMABuff, u32Ret);
        HAL_UART_Transmit_DMA(uart_tbl[ch].pHuart, txDMABuff, u32Ret);
        txDMAOK = false ;
      }


      break ;
    case UART2:
      //u32Ret = cdcWrite((uint8_t *)pBuff, len);
      break ;
  }
  return (u32Ret);
}
/**
 *
 */
uint32_t uartAvail(uint8_t ch)
{
  uint32_t u32Ret = 0x00UL ;

  switch(ch)
  {
    case UART1: // 12.6.4 DMA channel x number of data to transfer register (DMA_CNDTRx)
      uart_tbl[ch].rxQbuff.in = (uart_tbl[ch].rxQbuff.len - ((DMA_Channel_TypeDef *)uart_tbl[ch].pHdma_rx->Instance)->CNDTR);
      u32Ret = qbufferAvailable(&uart_tbl[ch].rxQbuff);
      break ;

    case UART2:
        #ifdef _USE_HW_USB
        //ret = cdcAvailable();
        #endif
      break ;
  }

  return (u32Ret);
}

bool uartFlush(uint8_t ch)
{
  uint32_t pre_time;

  pre_time = millis();
  while(uartAvail(ch))
  {
    if (millis()-pre_time >= 10)
    {
      break;
    }
    uartRead(ch);
  }

  return true;
}
/**
 *
 */
uint8_t uartRead(uint8_t ch)
{
  uint8_t u8Ret = 0x00U ;

  switch(ch)
  {
    case UART1:
      qbufferRead(&uart_tbl[ch].rxQbuff, &u8Ret, 1U);
      break ;

    case UART2:
      #ifdef _USE_HW_USB
      u8Ret = cdcRead();
      #endif
      break ;
  }
  return (u8Ret);
}
/**
 *
 */
uint32_t uartReadBuff(uint8_t ch,uint8_t * const pBuff, uint32_t len)
{
  uint32_t u32Ret = 0x00UL ;

  switch(ch)
  {
    case UART1:
      qbufferRead(&uart_tbl[ch].rxQbuff, (uint8_t * )pBuff, len);
      u32Ret =  len ;
      break ;

    case UART2:
      #ifdef _USE_HW_USB
      for(uint32_t i=0; i<len; i++)
      {
        pBuff[i] = cdcRead();
      }
      u32Ret =  len ;
      #endif
      break ;
  }
  //
  return (u32Ret);
}


//
uint32_t uartPrintf(uint8_t ch, const char *fmt, ...)
{
  uint32_t u32Ret = 0x00UL ;
  va_list arg ;
  char printBuff[256];

  va_start(arg,fmt);

  int len ;
  len = vsnprintf((char *)&printBuff[0],256,(const char *)fmt,arg);
  va_end(arg);

  if(len > 0)
  {
    u32Ret = uartWriteBuff(ch,(const uint8_t *)&printBuff[0],len);
  }

  return (u32Ret);
}
// ---------------------------------------------------------------------------
// Define private function definitions.  
// ---------------------------------------------------------------------------
//
void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspInit 0 */

  /* USER CODE END USART1_MspInit 0 */

  /** Initializes the peripherals clocks
  */
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
    PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
      Error_Handler();
    }

    /* USART1 clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();

    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART1 GPIO Configuration
    PC4     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_4;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART1 DMA Init */
    /* USART1_TX Init */
    hdma_usart1_tx.Instance = DMA1_Channel2;
    hdma_usart1_tx.Init.Request = DMA_REQUEST_USART1_TX;
    hdma_usart1_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_usart1_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart1_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart1_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart1_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart1_tx.Init.Mode = DMA_NORMAL;
    hdma_usart1_tx.Init.Priority = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma_usart1_tx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmatx,hdma_usart1_tx);

    /* USART1_RX Init */
    hdma_usart1_rx.Instance = DMA1_Channel1;
    hdma_usart1_rx.Init.Request = DMA_REQUEST_USART1_RX;
    hdma_usart1_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_usart1_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart1_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart1_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart1_rx.Init.Mode = DMA_CIRCULAR;
    hdma_usart1_rx.Init.Priority = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma_usart1_rx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmarx,hdma_usart1_rx);

    /* USART1 interrupt Init */
    HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspInit 1 */

  /* USER CODE END USART1_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspDeInit 0 */

  /* USER CODE END USART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();

    /**USART1 GPIO Configuration
    PC4     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_4);

    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_10);

    /* USART1 DMA DeInit */
    HAL_DMA_DeInit(uartHandle->hdmatx);
    HAL_DMA_DeInit(uartHandle->hdmarx);

    /* USART1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspDeInit 1 */

  /* USER CODE END USART1_MspDeInit 1 */
  }
}
// ---------------------------------------------------------------------------
//  Define Callbacks definitions.
// ---------------------------------------------------------------------------
//
void pushBuff(uint8_t data)
{
/*  if(data != 0x00U)
  {
    for(int i=0; i<2000; i++)
    {
      testBuff[i] = data ;
    }
  }
  //
  HAL_UART_Transmit_DMA(&huart1, testBuff, sizeof(testBuff));*/
}
/**
  * @brief  Tx Half Transfer completed callback.
  * @param  huart UART handle.
  * @retval None
  */
void HAL_UART_TxHalfCpltCallback(UART_HandleTypeDef *huart)
{
/*  for(int i=0; i<1000; i++)
  {
    testBuff[i] = testBuff[i] + 1U ;
  }*/
}
/**
  * @brief  Tx Transfer completed callback
  * @param  huart: UART handle.
  * @note   This example shows a simple way to report end of DMA Tx transfer, and
  *         you can add your own implementation.
  * @retval None
  */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{

  uint32_t u32Ret = 0UL ;

  if(huart->Instance == USART1)
  {
    u32Ret = qbufferAvailable(&uart_tbl[UART1].txQbuff) ;
    if(u32Ret == 0x00UL)
    {
      txDMAOK = true ;
    }
    else
    {
      qbufferRead(&uart_tbl[UART1].txQbuff, txDMABuff, u32Ret);
      HAL_UART_Transmit_DMA(uart_tbl[UART1].pHuart, txDMABuff, u32Ret);
      txDMAOK = false ;
    }
  }

}

/**
  * @brief  Rx Transfer completed callback
  * @param  huart: UART handle
  * @note   This example shows a simple way to report end of DMA Rx transfer, and
  *         you can add your own implementation.
  * @retval None
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{/*
  if(huart->Instance == USART1)
  {
    qbufferWrite(&rxQbuff[UART1],(uint8_t *)&rxData[UART1], 1U);

    HAL_UART_Receive_IT(&huart1, (uint8_t *)&rxData[UART1], 1U);
  }*/
}

/**
  * @brief  UART error callbacks
  * @param  huart: UART handle
  * @note   This example shows a simple way to report transfer error, and you can
  *         add your own implementation.
  * @retval None
  */
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
  /* Turn LED2 off: Transfer error in reception/transmission process */

}

// ---------------------------------------------------------------------------
//  Define CLI definitions.
// ---------------------------------------------------------------------------
//
#endif /* Uart.c End Of File !! Well Done */
