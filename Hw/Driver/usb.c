/*
 * [ PROJECT   ]  stm32g431_fw
 * [ License   ]  SAMJIN ELECTRONICS.,Co.Ltd
 * [ Author    ]  Copyright 2024 By HAG-SEONG KANG
 * [ E-MAIL    ]  hl5ugc@nate.com (82)10- 3841-9706
 * [ C  P  U   ]
 * [ Compller  ]  
 * [ Filename  ]  usb.c
 * [ Version   ]  1.0
 * [ Created   ]  2024[3:10:06 PM ]
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
#include "usb.h"
#include "cdc.h"

#ifdef _USE_HW_USB
#include "usbd_core.h"

#if HW_USE_CDC == 1
#include "usbd_cdc.h"
#include "usbd_cdc_if.h"
#endif


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
static bool is_init = false;
static UsbMode is_usb_mode = USB_NON_MODE;
USBD_HandleTypeDef hUsbDeviceFS;

extern USBD_DescriptorsTypeDef CDC_Desc;
extern USBD_DescriptorsTypeDef MSC_Desc;
extern PCD_HandleTypeDef hpcd_USB_FS;

// ---------------------------------------------------------------------------
// Define Private function prototype.
// ---------------------------------------------------------------------------
//
 
// ---------------------------------------------------------------------------
// Define Public function definitions. 
// ---------------------------------------------------------------------------
//
bool usbInit(void)
{
  bool bRet = true ;

  return (bRet);
}


void usbDeInit(void)
{
  if(is_init == true)
  {
    USBD_DeInit(&hUsbDeviceFS);
  }
}


bool usbIsOpen(void)
{
  return cdcIsConnect();
}


bool usbIsConnect(void)
{
  bool bRet = false ;

  if(hUsbDeviceFS.pClassData != NULL)
  {
    if(hUsbDeviceFS.dev_state == USBD_STATE_CONFIGURED)
    {
      if(hUsbDeviceFS.dev_config != 0U)
      {
        bRet = true ;
      }
    }
  }

  return (bRet);
}

UsbMode usbGetMode(void)
{
  return (is_usb_mode);
}


bool usbBegin(UsbMode usb_mode)
{
  bool bRet = false ;

  #if HW_USE_CDC == 1
  if(usb_mode == USB_CDC_MODE)
  {
    if(USBD_Init(&hUsbDeviceFS, &CDC_Desc, DEVICE_FS) == USBD_OK)
    {
      if(USBD_RegisterClass(&hUsbDeviceFS, &USBD_CDC) == USBD_OK)
      {
        if(USBD_CDC_RegisterInterface(&hUsbDeviceFS, &USBD_Interface_fops_FS) == USBD_OK)
        {
          if(USBD_Start(&hUsbDeviceFS) == USBD_OK)
          {
            cdcInit();
            is_usb_mode = USB_CDC_MODE ;
            bRet = true ;
          }
        }
      }
    }
  }
  #endif

  is_init = bRet ;

  return (bRet);
}

// ---------------------------------------------------------------------------
// Define private function definitions.  
// ---------------------------------------------------------------------------
//
 
// ---------------------------------------------------------------------------
//  Define Callbacks definitions.
// ---------------------------------------------------------------------------
//
void USB_LP_IRQHandler(void)
{
  HAL_PCD_IRQHandler(&hpcd_USB_FS);
}
#endif /*usb.c End Of File !! Well Done */
