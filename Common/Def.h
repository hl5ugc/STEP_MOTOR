/*
 * [ PROJECT   ]  stm32g431_fw
 * [ License   ]  SAMJIN ELECTRONICS.,Co.Ltd
 * [ Author    ]  Copyright 2024 By HAG-SEONG KANG
 * [ E-MAIL    ]  hl5ugc@nate.com (82)10- 3841-9706
 * [ C  P  U   ]
 * [ Compller  ]  
 * [ Filename  ]  Def.h
 * [ Version   ]  1.0
 * [ Created   ]  2024[6:11:15 PM ]
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


/* Mutiple includes protection */
#ifndef SRC_COMMON_DEF_H_
#define SRC_COMMON_DEF_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes */
 
#include <stdio.h>
#include <stdint.h>   // uint32_t
#include <stdbool.h>  //
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

#define _DEF_POS0             0U
#define _DEF_POS1             1U
#define _DEF_POS2             2U
#define _DEF_POS3             3U
#define _DEF_POS4             4U
#define _DEF_POS5             5U
#define _DEF_POS6             6U
#define _DEF_POS7             7U
#define _DEF_POS8             8U
#define _DEF_POS9             9U
#define _DEF_POS10            10U
#define _DEF_POS11            11U
#define _DEF_POS12            12U
#define _DEF_POS13            13U
#define _DEF_POS14            14U
#define _DEF_POS15            15U


#define _DEF_BIT0             0x0001U
#define _DEF_BIT1             0x0002U
#define _DEF_BIT2             0x0004U
#define _DEF_BIT3             0x0008U
#define _DEF_BIT4             0x0010U
#define _DEF_BIT5             0x0020U
#define _DEF_BIT6             0x0040U
#define _DEF_BIT7             0x0080U
#define _DEF_BIT8             0x0100U
#define _DEF_BIT9             0x0200U
#define _DEF_BIT10            0x0400U
#define _DEF_BIT11            0x0800U
#define _DEF_BIT12            0x1000U
#define _DEF_BIT13            0x2000U
#define _DEF_BIT14            0x4000U
#define _DEF_BIT15            0x8000U

#define _DEF_LED1             0
#define _DEF_LED2             1
#define _DEF_LED3             2
#define _DEF_LED4             3

#define _DEF_UART1            0
#define _DEF_UART2            1
#define _DEF_UART3            2
#define _DEF_UART4            3

#define _DEF_BUTTON1          0
#define _DEF_BUTTON2          1
#define _DEF_BUTTON3          2
#define _DEF_BUTTON4          3

#define _DEF_SPI1             0
#define _DEF_SPI2             1
#define _DEF_SPI3             2
#define _DEF_SPI4             3

#define _DEF_I2C1             0
#define _DEF_I2C2             1
#define _DEF_I2C3             2
#define _DEF_I2C4             3

#define _DEF_I2S1             0
#define _DEF_I2S2             1
#define _DEF_I2S3             2
#define _DEF_I2S4             3

#define _DEF_PWM1             0
#define _DEF_PWM2             1
#define _DEF_PWM3             2
#define _DEF_PWM4             3

#define _DEF_LOW              0
#define _DEF_HIGH             1

#define _DEF_INPUT            0
#define _DEF_INPUT_PULLUP     1
#define _DEF_INPUT_PULLDOWN   2
#define _DEF_OUTPUT           3
#define _DEF_OUTPUT_PULLUP    4
#define _DEF_OUTPUT_PULLDOWN  5

#define _DEF_CAN1             0
#define _DEF_CAN2             1
#define _DEF_CAN3             2
#define _DEF_CAN4             3

#define _DEF_CAN_100K         0
#define _DEF_CAN_125K         1
#define _DEF_CAN_250K         2
#define _DEF_CAN_500K         3
#define _DEF_CAN_1000K        4

#define _DEF_CAN_STD          0
#define _DEF_CAN_EXT          1

#define _DEF_DXL1             0
#define _DEF_DXL2             1
#define _DEF_DXL3             2
#define _DEF_DXL4             3

#define COUNTOF(__BUFFER__)   (sizeof(__BUFFER__) / sizeof(*(__BUFFER__)))

#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))

#ifndef max
#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif


#ifndef map
#define map(value, in_min, in_max, out_min, out_max) ((value - in_min) * (out_max - out_min) / (in_max - in_min) + out_min)
#endif


#define MAGIC_NUMBER              0x5555AAAA
#define FLASH_MAGIC_NUMBER        0x5555AAAA

#define VERSION_MAGIC_NUMBER      0x56455220    // "VER "
#define TAG_MAGIC_NUMBER          0x54414720    // "TAG "

typedef struct
{
  uint8_t version[32];
  uint8_t name[32];
  char mfr_str[32] ;
  char pname_str[30] ;
  char email_str[30];
  char tel_str[30] ;
  uint32_t mfr_code ;
} firm_version_t;


typedef struct
{
  uint32_t magic_number;

  //-- fw info
  //
  uint32_t addr_tag;
  uint32_t addr_fw;
  uint32_t size_tag;


  //-- tag info
  //
  uint32_t tag_flash_type;
  uint32_t tag_flash_start;
  uint32_t tag_flash_end;
  uint32_t tag_flash_length;
  uint32_t tag_flash_crc;
  uint32_t tag_length;
  uint8_t  tag_date_str[32];
  uint8_t  tag_time_str[32];

} firm_tag_t;
#ifdef __cplusplus
}
#endif
#endif /* Def.h End Of File !! Well Done */
