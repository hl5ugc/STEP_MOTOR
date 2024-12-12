/*
 * [ PROJECT   ]  stm32g431_SM_UART
 * [ License   ]  SAMJIN ELECTRONICS.,Co.Ltd
 * [ Author    ]  Copyright 2024 By HAG-SEONG KANG
 * [ E-MAIL    ]  hl5ugc@nate.com (82)10- 3841-9706
 * [ C  P  U   ]
 * [ Compller  ]  
 * [ Filename  ]  DataConvert.c
 * [ Version   ]  1.0
 * [ Created   ]  2024[9:53:34 AM ]
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
#include "DataConvert.h"

// ---------------------------------------------------------------------------
//  Define Macros .
// ---------------------------------------------------------------------------
//
 
// ---------------------------------------------------------------------------
//  Define TypeDef & Constants.
// ---------------------------------------------------------------------------
//
typedef struct
{
  uint8_t   u8Data1 ;   // 리틀 엔디안 Little-Endian
  uint8_t   u8Data2 ;
} cvt_u16_u8_t;
typedef struct
{
  uint8_t   u8Data1 ;
  uint8_t   u8Data2 ;
  uint8_t   u8Data3 ;
  uint8_t   u8Data4 ;
} cvt_u32_u8_t;
//
typedef union
{
    uint16_t        u16Val ;
    cvt_u16_u8_t    u8Val ;
} cvt_u16_t ;
//
typedef union
{
    uint32_t        u32Val ;
    cvt_u32_u8_t    u8Val ;
} cvt_u32_t ;
// ---------------------------------------------------------------------------
//  Define Private variables.
// ---------------------------------------------------------------------------
//
 
// ---------------------------------------------------------------------------
// Define Private function prototype.
// ---------------------------------------------------------------------------
//
static uint8_t cvtHexBin(uint8_t u8AscHex);
static uint8_t hex2bin(const uint8_t  * pBuff);
static uint8_t asc2bin(const uint8_t  * pBuff);
// ---------------------------------------------------------------------------
// Define Callback function prototype.
// ---------------------------------------------------------------------------
//

// ---------------------------------------------------------------------------
// Define Public function definitions. 
// ---------------------------------------------------------------------------
//
/*
 * @brief Byte data convert to 2digit ASCII Decimal
 *
 */
void cvtByte2DASC(uint8_t u8Data,uint8_t * const pBuff)
{
    uint8_t u8Cal = 0x00U ;
    //
    if(u8Data > 99U) { u8Data =  u8Data % 100U ; }
    //
    u8Cal =  u8Data / 10U ;
    pBuff[0x00U] = u8Cal + 0x30U ;
    //
    u8Cal =  u8Data % 10U ;
    pBuff[0x01U] = u8Cal + 0x30U ;
}
/*
 * @brief
 *
 */
void cvtByte3DASC(uint8_t u8Data,uint8_t * const pBuff)
{
    uint8_t u8Cal = 0x00U ;
    uint8_t u8Cal2 = 0x00U ;
    //
    u8Cal =  u8Data / 100 ;
    pBuff[0x00U] = u8Cal + 0x30U ;
    //
    u8Cal =  u8Data % 100 ;
    u8Cal2 = u8Cal / 10 ;
    pBuff[0x01U] = u8Cal2 + 0x30U ;
    //
    u8Cal2 =  u8Cal % 10 ;
    pBuff[0x02U] = u8Cal2 + 0x30U ;
}
/*
 * @brief
 *
 */
void cvtWord3DASC(uint16_t u16Data,uint8_t * const pBuff)
{
    uint8_t u8Cal = 0x00U ;
    uint8_t u8Cal2 = 0x00U ;
    //
    if(u16Data > 999) { u16Data =  u16Data % 1000 ;  }
    //
    u8Cal =  u16Data / 100 ;
    pBuff[0x00U]  = u8Cal + 0x30U ;
    //
    u8Cal =  u16Data % 100 ;
    u8Cal2 = u8Cal / 10 ;
    pBuff[0x01U]  = u8Cal2 + 0x30U ;
    //
    u8Cal2 =  u8Cal % 10 ;
    pBuff[0x02U]  = u8Cal2 + 0x30U ;
}
/*
 * @brief
 *
 */
void cvtWord4DASC(uint16_t u16Data,uint8_t * const pBuff)
{
    uint8_t  u8Cal = 0x00U ;
    uint16_t u16Cal = 0x00U ;
    //
    if(u16Data > 999) { u16Data =  u16Data % 10000U ;  }
    //
    u8Cal =  u16Data / 1000U ;
    pBuff[0x00U] = u8Cal + 0x30U ;
    //
    u16Cal =  u16Data % 1000U ;
    u8Cal = u16Cal / 100U ;
    pBuff[0x01U]  = u8Cal + 0x30U ;
    //
    u16Cal =  u16Cal % 100U  ;
    u8Cal =  u16Cal / 10U ;
    pBuff[0x02U]  = u8Cal + 0x30U ;
    //
    u8Cal =  u16Cal % 10U ;
    pBuff[0x03U]  = u8Cal + 0x30U ;
}
/*
 * @brief
 *
 */
void cvtWord5DASC(uint16_t u16Data,uint8_t * const pBuff)
{
    uint8_t  u8Cal = 0x00U ;
    uint16_t u16Cal = 0x00U ;
    //
    u8Cal  =  u16Data / 10000U ;
    pBuff[0x00U] = u8Cal + 0x30U ;
    //
    u16Cal =  u16Data % 10000U ;
    u8Cal  = u16Cal / 1000U ;
    pBuff[0x01U] = u8Cal + 0x30U ;
    //
    u16Cal =  u16Cal % 1000U ;
    u8Cal  =  u16Cal / 100U ;
    pBuff[0x02U] = u8Cal + 0x30U ;
    //
    u16Cal =  u16Cal % 100U ;
    u8Cal  = u16Cal / 10U ;
    pBuff[0x03U] = u8Cal + 0x30U ;
    //
    u8Cal =  u16Cal % 10U ;
    pBuff[0x04U] = u8Cal + 0x30U ;
}
/*
 * @brief
 *
 */
void cvtByte2HEX(uint8_t u8Data,uint8_t * const pBuff)
{
    uint8_t u8Cal = 0x00U ;
    //
    u8Cal =  (u8Data >> 4) & 0x0FU ;
    if(u8Cal <= 9U)  { pBuff[0x00U] = u8Cal + 0x30U ; }
    else             { pBuff[0x00U] = u8Cal + 0x37U ; }
    //
    u8Cal =  (u8Data  & 0x0FU);
    if(u8Cal <= 9U)  { pBuff[0x01U] = u8Cal + 0x30U ; }
    else             { pBuff[0x01U] = u8Cal + 0x37U ; }
}
/*
 * @brief
 *
 */
void cvtWord4HEX(uint16_t u16Data,uint8_t * const pBuff)
{
    uint8_t u8Cal = 0x00U ;
    //
    u8Cal =  (u16Data >> 12U) & 0x0FU ;
    if(u8Cal <= 9U)  { pBuff[0x00U] = u8Cal + 0x30U ; }
    else             { pBuff[0x00U] = u8Cal + 0x37U ; }
    //
    u8Cal =  (u16Data >> 8U) & 0x0FU ;
    if(u8Cal <= 9U)  { pBuff[0x01U] = u8Cal + 0x30U ; }
    else             { pBuff[0x01U] = u8Cal + 0x37U ; }
    //
    u8Cal =  (u16Data >> 4U) & 0x0FU ;
    if(u8Cal <= 9U)  { pBuff[0x02U] = u8Cal + 0x30U ; }
    else             { pBuff[0x02U] = u8Cal + 0x37U ; }
    //
    u8Cal =  (u16Data  & 0x0FU);
    if(u8Cal <= 9U)  { pBuff[0x03U] = u8Cal + 0x30U ; }
    else             { pBuff[0x03U] = u8Cal + 0x37U ; }
}
/*
 * @brief
 *
 */
void  cvtByte7BASC(uint8_t u8Data,uint8_t * const pBuff)
{
    pBuff[0x00U] = (((u8Data >> 7U) & 0x01U) + '0') ;
    pBuff[0x01U] = (((u8Data >> 6U) & 0x01U) + '0') ;
    pBuff[0x02U] = (((u8Data >> 5U) & 0x01U) + '0') ;
    pBuff[0x03U] = (((u8Data >> 4U) & 0x01U) + '0') ;
    pBuff[0x04U] = (((u8Data >> 3U) & 0x01U) + '0') ;
    pBuff[0x05U] = (((u8Data >> 2U) & 0x01U) + '0') ;
    pBuff[0x06U] = (((u8Data >> 1U) & 0x01U) + '0') ;
    pBuff[0x07U] = ((u8Data         & 0x01U) + '0') ;
}

/**
 * @brief
 *
 * @param pBuff
 * @return uint8_t
 */
uint8_t hex2bin(const uint8_t * pBuff)
{
    uint8_t u8Cal = 0x00U ;
    uint8_t u8Caltemp = 0x00U ;

    u8Cal = pBuff[0x00U] ;
    if(u8Cal < 0x3AU) { u8Cal = u8Cal - 0x30U; }
    else              { u8Cal = u8Cal - 0x37U; }
    u8Caltemp = ((u8Cal << 4 )  & 0xF0U ) ;
    //
    u8Cal = pBuff[0x01U] ;
    if(u8Cal < 0x3AU) { u8Cal = u8Cal - 0x30U; }
    else              { u8Cal = u8Cal - 0x37U; }
    u8Caltemp = u8Caltemp +  u8Cal   ;
    //
    return u8Caltemp ;
}
/**
 * @brief   ASCII Decimal Data Convert To uint8_t Data
 *
 * @param   pBuff   ASCII Decimal
 * @return  uint8_t Converted uint8_t Data
 */
uint8_t asc2bin(const uint8_t * pBuff)
{
    uint8_t u8Ret = 0x00U ;
    uint8_t u8Cal = 0x00U ;

    u8Cal = pBuff[0x00U] ;
    if((u8Cal >= '0') && (u8Cal <= '9'))
    {
        u8Ret = (u8Cal - 0x30U) ;
    }

    return (u8Ret);
}
/**
 * @brief ASCII HEX Data Convert To Binary Data
 *
 * @param u8AscHex ASCII HEX Data
 * @return uint8_t Binary Data
 */
uint8_t cvtHexBin(uint8_t u8AscHex)
{
    uint8_t u8Ret = 0x00U ;

    if(u8AscHex < 0x3AU) { u8Ret = u8AscHex - 0x30U; }
    else if((u8AscHex >= 'A') && (u8AscHex <= 'F')) { u8Ret = u8AscHex - 0x37U; }
    else if((u8AscHex >= 'a') && (u8AscHex <= 'f')) { u8Ret = u8AscHex - 0x57U; }
    else {u8Ret = 0x00U; }
}
/*
 * @brief
 *
 */
void cvt2HEXBinP(const uint8_t * pBuff , uint8_t * const pData)
{
    uint8_t u8Cal = 0x00U ;
    uint8_t u8Len = 0x00U ;
    uint8_t u8Caltemp = 0x00U ;
    //
    u8Len = strlen(pBuff) ;

    if(u8Len == 2) {
        u8Cal = pBuff[0x00U] ;
        u8Caltemp = ((cvtHexBin(u8Cal) << 4U )  & 0xF0U ) ;
        //
        u8Cal = pBuff[0x01U] ;
        u8Caltemp = u8Caltemp + cvtHexBin(u8Cal)   ;
    //
    }
    else if(u8Len == 1)
    {
        u8Cal = pBuff[0x00U] ;
        u8Caltemp =  cvtHexBin(u8Cal)  & 0x0FU;
    }
    else { u8Caltemp = 0 ; }
    //
    pData[0x00U] =  u8Caltemp ;
}
/*
 * @brief 4Byte Ascii Hex Convert to uint16_t data
 *
 */
void  cvt4HEXBinP(const uint8_t * pBuff , uint16_t * const pData)
{
    uint8_t  u8Cal = 0x00U ;
    uint8_t  u8Len = 0x00U ;
    uint16_t u16Caltemp = 0x00U;
    //
    u8Len = strlen(pBuff) ;
 
    if(u8Len == 4)
    {
        u8Cal = pBuff[0x00U] ;
        u16Caltemp = (((uint16_t)cvtHexBin(u8Cal) << 12U)  & 0xF000U ) ;
        //
        u8Cal = pBuff[0x01U] ;
        u16Caltemp = u16Caltemp + (((uint16_t)cvtHexBin(u8Cal) << 8U)  & 0x0F00U ) ;
        //
        u8Cal = pBuff[0x02U] ;
        u16Caltemp = u16Caltemp + (((uint16_t)cvtHexBin(u8Cal) << 4U)  & 0x00F0U ) ;
        //
        u8Cal = pBuff[0x03U] ;
        u16Caltemp = u16Caltemp +  (uint16_t)cvtHexBin(u8Cal)   ;
        //
    }
    else if(u8Len == 3)
    {
        //
        u8Cal = pBuff[0x00U] ;
        u16Caltemp = (((uint16_t)cvtHexBin(u8Cal) << 8U)  & 0x0F00U ) ;
        //
        u8Cal = pBuff[0x01U] ;
        u16Caltemp = u16Caltemp + (((uint16_t)cvtHexBin(u8Cal) << 4U)  & 0x00F0U ) ;
        //
        u8Cal = pBuff[0x02U] ;
        u16Caltemp = u16Caltemp +  (uint16_t)cvtHexBin(u8Cal)   ;
        //
    }
    else if(u8Len == 2)
    {
        u8Cal = pBuff[0x00U] ;
        u16Caltemp = (((uint16_t)cvtHexBin(u8Cal) << 4U)  & 0x00F0U ) ;
        //
        u8Cal = pBuff[0x01U] ;
        u16Caltemp = u16Caltemp +  (uint16_t)cvtHexBin(u8Cal)   ;
        //
    }
    else if(u8Len == 1)
    {
        u8Cal = pBuff[0x00U] ;
        u16Caltemp = (uint16_t)cvtHexBin(u8Cal)   ;
        //
    }
    else { u16Caltemp = 0 ; }

    pData[0x00U] =  u16Caltemp ;

}
/*
 * @brief
 *
 */
void  cvt6HEXBinP(const uint8_t * pBuff , uint32_t * const pData)
{

    uint8_t  u8Len = 0x00U ;
    cvt_u32_t u32Caltemp  ;
    //
    u32Caltemp.u32Val =  0x00U ;
    //
    u8Len = strlen(pBuff) ;
    //
    if(u8Len == 8)
    {
        u32Caltemp.u8Val.u8Data4 = hex2bin(&pBuff[0x00U]);
        u32Caltemp.u8Val.u8Data3 = hex2bin(&pBuff[0x02U]);
        u32Caltemp.u8Val.u8Data2 = hex2bin(&pBuff[0x04U]);
        u32Caltemp.u8Val.u8Data1 = hex2bin(&pBuff[0x06U]);
    }
    else if(u8Len == 6)
    {
        u32Caltemp.u8Val.u8Data4 = 0 ;
        u32Caltemp.u8Val.u8Data3 = hex2bin(&pBuff[0x00U]);
        u32Caltemp.u8Val.u8Data2 = hex2bin(&pBuff[0x02U]);
        u32Caltemp.u8Val.u8Data1 = hex2bin(&pBuff[0x04U]);
    }
    else if(u8Len == 4)
    {
        u32Caltemp.u8Val.u8Data4 = 0 ;
        u32Caltemp.u8Val.u8Data3 = 0 ;
        u32Caltemp.u8Val.u8Data2 = hex2bin(&pBuff[0x00U]);
        u32Caltemp.u8Val.u8Data1 = hex2bin(&pBuff[0x02U]);
    }
    else if(u8Len == 2)
    {
        u32Caltemp.u8Val.u8Data4 = 0 ;
        u32Caltemp.u8Val.u8Data3 = 0 ;
        u32Caltemp.u8Val.u8Data2 = 0 ;
        u32Caltemp.u8Val.u8Data1 = hex2bin(&pBuff[0x00U]);
    }
    else { u32Caltemp.u32Val = 0 ; }
    //
    pData[0x00U] =  u32Caltemp.u32Val ;
}
/**
 * @brief ASCII Decimal Data Convert TO uint16_t Data
 *
 * @param pBuff ASCII Decimal Data String
 * @param pData Return uint16_t Data
 */
void cvt6ASCBinP(const uint8_t *pBuff , uint16_t * const pData)
{
    uint16_t u16Cal = 0x00U ;
    uint8_t  u8Len  = 0x00U ;
    pData[0x00U]    = 0x00U ;
    //
    u8Len = strlen(pBuff) ;
    if((u8Len > 0U) && (u8Len <= 5U))
    {
        switch(u8Len)
        {
            case 5U: // 65535
                u16Cal  = (uint16_t)asc2bin(&pBuff[0x00U]) * 10000U ;
                u16Cal += (uint16_t)asc2bin(&pBuff[0x01U]) * 1000U ;
                u16Cal += (uint16_t)asc2bin(&pBuff[0x02U]) * 100U ;
                u16Cal += (uint16_t)asc2bin(&pBuff[0x03U]) * 10U ;
                u16Cal += (uint16_t)asc2bin(&pBuff[0x04U])  ;
                break ;
            case 4U: // 9999
                u16Cal  = (uint16_t)asc2bin(&pBuff[0x00U]) * 1000U ;
                u16Cal += (uint16_t)asc2bin(&pBuff[0x01U]) * 100U ;
                u16Cal += (uint16_t)asc2bin(&pBuff[0x02U]) * 10U ;
                u16Cal += (uint16_t)asc2bin(&pBuff[0x03U])  ;
                break ;
            case 3U: // 999
                u16Cal  = (uint16_t)asc2bin(&pBuff[0x00U]) * 100U ;
                u16Cal += (uint16_t)asc2bin(&pBuff[0x01U]) * 10U ;
                u16Cal += (uint16_t)asc2bin(&pBuff[0x02U])  ;
                break ;
            case 2U: // 99
                u16Cal  = (uint16_t)asc2bin(&pBuff[0x00U]) * 10U ;
                u16Cal += (uint16_t)asc2bin(&pBuff[0x01U])  ;
                break ;
            default:
                u16Cal  = (uint16_t)asc2bin(&pBuff[0x00U])  ;
                break ;
        }

        pData[0x00U] = u16Cal ;
    }
}

uint8_t cvt2ASCBin(const uint8_t *pBuff)
{
    uint8_t u8Cal = 0x00U ;
    //
    u8Cal  = asc2bin(&pBuff[0x00U]) * 10U ;
    u8Cal += asc2bin(&pBuff[0x01U])  ;

    return (u8Cal) ;
}
uint16_t cvt3ASCBin(const uint8_t *pBuff)
{
    uint16_t u16Cal = 0x00U ;
    //
    u16Cal  = (uint16_t)asc2bin(&pBuff[0x00U]) * 100U ;
    u16Cal += (uint16_t)asc2bin(&pBuff[0x01U]) * 10U ;
    u16Cal += (uint16_t)asc2bin(&pBuff[0x02U])  ;

    return (u16Cal) ;
}
uint16_t cvt4ASCBin(const uint8_t *pBuff)
{
    uint16_t u16Cal = 0x00U ;
    //
    u16Cal  = (uint16_t)asc2bin(&pBuff[0x00U]) * 1000U ;
    u16Cal += (uint16_t)asc2bin(&pBuff[0x01U]) * 100U ;
    u16Cal += (uint16_t)asc2bin(&pBuff[0x02U]) * 10U ;
    u16Cal += (uint16_t)asc2bin(&pBuff[0x03U])  ;
    //
    return (u16Cal) ;
}
uint16_t cvt5ASCBin(const uint8_t *pBuff)
{
    uint16_t u16Cal = 0x00U ;
    //
    u16Cal  = (uint16_t)asc2bin(&pBuff[0x00U]) * 10000U ;
    u16Cal  = (uint16_t)asc2bin(&pBuff[0x01U]) * 1000U ;
    u16Cal += (uint16_t)asc2bin(&pBuff[0x02U]) * 100U ;
    u16Cal += (uint16_t)asc2bin(&pBuff[0x03U]) * 10U ;
    u16Cal += (uint16_t)asc2bin(&pBuff[0x04U])  ;
    //
    return (u16Cal) ;
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

/* DataConvert.c End Of File !! Well Done */
