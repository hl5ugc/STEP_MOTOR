/*
 * [ PROJECT   ]  stm32g431_fw
 * [ License   ]  SAMJIN ELECTRONICS.,Co.Ltd
 * [ Author    ]  Copyright 2024 By HAG-SEONG KANG
 * [ E-MAIL    ]  hl5ugc@nate.com (82)10- 3841-9706
 * [ C  P  U   ]
 * [ Compller  ]  
 * [ Filename  ]  CDC.c
 * [ Version   ]  1.0
 * [ Created   ]  2024[3:09:43 PM ]
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
#include "CDC.h"

#ifdef _USE_HW_CDC
#include "usb\usb_cdc\usbd_cdc_if.h"
// ---------------------------------------------------------------------------
//  Define Macros .
// ---------------------------------------------------------------------------
//

// ---------------------------------------------------------------------------
//  Define Constants.
// ---------------------------------------------------------------------------
//
static bool is_init = false;
// ---------------------------------------------------------------------------
//  Define Private variables.
// ---------------------------------------------------------------------------
//

// ---------------------------------------------------------------------------
// Define private function definitions.
// ---------------------------------------------------------------------------
//


// ---------------------------------------------------------------------------
// Define Public function definitions.
// ---------------------------------------------------------------------------
//
bool cdcInit(void)
{
  bool bRet = true;

  bRet = cdcIfInit();

  is_init = bRet;

  return bRet;
}

bool cdcIsInit(void)
{
  return is_init;
}

bool cdcIsConnect(void)
{
  return cdcIfIsConnected();
}

uint32_t cdcAvailable(void)
{
  return cdcIfAvailable();
}

uint8_t cdcRead(void)
{
  return cdcIfRead();
}

uint32_t cdcWrite(uint8_t *p_data, uint32_t length)
{
  return cdcIfWrite(p_data, length);
}

uint32_t cdcGetBaud(void)
{
  return cdcIfGetBaud();
}

uint8_t cdcGetType(void)
{
  return cdcIfGetType();
}


#endif /* CDC.c End Of File !! Well Done */
