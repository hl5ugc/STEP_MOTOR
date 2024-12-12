/*
 * [ PROJECT   ]  5_stm32g431_UART
 * [ License   ]  SAMJIN ELECTRONICS.,Co.Ltd
 * [ Author    ]  Copyright 2024 By HAG-SEONG KANG
 * [ E-MAIL    ]  hl5ugc@nate.com (82)10- 3841-9706
 * [ C  P  U   ]
 * [ Compller  ]  
 * [ Filename  ]  Modbus_Master.c
 * [ Version   ]  1.0
 * [ Created   ]  2024[12:18:16 PM ]
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
 * https://github.com/LittleBigQi
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
#include "Modbus_Master.h"
#include "Uart.h"
// ---------------------------------------------------------------------------
//  Define Macros .
// ---------------------------------------------------------------------------
//
 
// ---------------------------------------------------------------------------
//  Define TypeDef & Constants.
// ---------------------------------------------------------------------------
//
#define ku8MBIllegalFunction             0x01U
#define ku8MBIllegalDataAddress          0x02U;
#define ku8MBIllegalDataValue            0x03U;
#define ku8MBSlaveDeviceFailure          0x04U;

#define ku8MBSuccess                     0x00U
#define ku8MBInvalidSlaveID              0xE0U
#define ku8MBInvalidFunction             0xE1U
#define ku8MBResponseTimedOut            0xE2U
#define ku8MBInvalidCRC                  0xE3U
// Modbus function codes for bit access
#define ku8MBReadCoils                   0x01U // < Modbus function 0x01 Read Coils
#define ku8MBReadDiscreteInputs          0x02U // < Modbus function 0x02 Read Discrete Inputs
#define ku8MBWriteSingleCoil             0x05U // < Modbus function 0x05 Write Single Coil
#define ku8MBWriteMultipleCoils          0x0FU // < Modbus function 0x0F Write Multiple Coils

// Modbus function codes for 16 bit access
#define ku8MBReadHoldingRegisters        0x03U // < Modbus function 0x03 Read Holding Registers
#define ku8MBReadInputRegisters          0x04U // < Modbus function 0x04 Read Input Registers
#define ku8MBWriteSingleRegister         0x06U // < Modbus function 0x06 Write Single Register
#define ku8MBWriteMultipleRegisters      0x10U // < Modbus function 0x10 Write Multiple Registers
#define ku8MBMaskWriteRegister           0x16U // < Modbus function 0x16 Mask Write Register
#define ku8MBReadWriteMultipleRegisters  0x17U // < Modbus function 0x17 Read Write Multiple Registers

// Modbus timeout [milliseconds]
#define ku16MBResponseTimeout           2000U ///< Modbus timeout [milliseconds]
// ---------------------------------------------------------------------------
//  Define Private variables.
// ---------------------------------------------------------------------------
//
 
#define ku8MaxBufferSize               64                     //   size of response/transmit buffers
uint8_t  _u8MBSlave;                                         //   Modbus slave (1..255) initialized in begin()

uint16_t _u16ReadAddress;                                    //  slave register from which to read
uint16_t _u16ReadQty;                                        //  quantity of words to read
uint16_t _u16RespBuffer[ku8MaxBufferSize];
uint16_t _u16WriteAddress;                                   //  slave register to which to write
uint16_t _u16WriteQty;                                       //  quantity of words to write
uint16_t _u16TransBuffer[ku8MaxBufferSize];
uint16_t* txBuffer; // from Wire.h -- need to clean this up Rx
uint8_t _u8TransmitBufferIndex;
uint16_t u16TransmitBufferLength;
uint16_t* rxBuffer; // from Wire.h -- need to clean this up Rx
uint8_t _u8ResponseBufferIndex;
uint8_t _u8ResponseBufferLength;
volatile uint8_t _u8MBFunction ;


// ---------------------------------------------------------------------------
// Define Private function prototype.
// ---------------------------------------------------------------------------
//
uint8_t ModbusMaster_ModbusMasterQuery(uint8_t u8MBFunction);
uint8_t ModbusMaster_ModbusMasterRespose(void);
// ---------------------------------------------------------------------------
// Define Callback function prototype.
// ---------------------------------------------------------------------------
//
// idle callback function; gets called during idle time between TX and RX
void (*_idle)();
// preTransmission callback function; gets called before writing a Modbus message
void (*_preTransmission)();
// postTransmission callback function; gets called after a Modbus message has been sent
void (*_postTransmission)();
// ---------------------------------------------------------------------------
// Define Public function definitions. 
// ---------------------------------------------------------------------------
//
void ModbusMaster_begin(void)
{
  _u8TransmitBufferIndex  = 0U ;
  u16TransmitBufferLength = 0U ;
  // Ring Buffer init
  //
  // Modubus driver ..... uartOpen(MODBUS_PORT,MODBUS_BPS);
  //
}
//
void ModbusMaster_beginTransmission(uint16_t u16Address)
{
  _u16WriteAddress = u16Address ;
  _u8TransmitBufferIndex  = 0U ;
  u16TransmitBufferLength = 0U ;
}
// eliminate this function in favor of using existing MB request functions
uint8_t ModbusMaster_requestFrom(uint16_t address, uint16_t quantity)
{
  uint8_t u8Read = 0U ;

  // clamp to buffer length
  if(quantity > ku8MaxBufferSize)
  {
    quantity = ku8MaxBufferSize ;
  }
  // set rx buffer iterator vars
  _u8ResponseBufferIndex = 0U ;
  _u8ResponseBufferLength = u8Read ;

  return (u8Read);
}

void  ModbusMaster_sendBit(uint8_t data)
{
  uint8_t txBitIndex = u16TransmitBufferLength % 16 ;
  if((u16TransmitBufferLength >> 4U) < ku8MaxBufferSize)
  {
    if(0 == txBitIndex)
    {
      _u16TransBuffer[_u8TransmitBufferIndex] = 0U ;
    }
    bitWrite(_u16TransBuffer[_u8TransmitBufferIndex],txBitIndex,data);
    u16TransmitBufferLength++ ;
    _u8TransmitBufferIndex = (u16TransmitBufferLength >> 4U);
  }
}
void ModbusMaster_send8(uint8_t data)
{
  ModbusMaster_send16((uint16_t)data);
}
void ModbusMaster_send16(uint16_t data)
{
  if(_u8TransmitBufferIndex < ku8MaxBufferSize)
  {
    _u16TransBuffer[_u8TransmitBufferIndex++] = data ;
    u16TransmitBufferLength = (_u8TransmitBufferIndex << 4U) ;
  }
}
void ModbusMaster_send32(uint32_t data)
{
  ModbusMaster_send16(lowWord(data));
  ModbusMaster_send16(highWord(data));
}

uint8_t ModbusMaster_available(void)
{
  return (_u8ResponseBufferLength - _u8ResponseBufferIndex) ;
}

uint16_t ModbusMaster_receive(void)
{
  uint16_t u16Ret = 0xFFFFU ;

  if(_u8ResponseBufferIndex < _u8ResponseBufferLength)
  {
    u16Ret = _u16RespBuffer[_u8ResponseBufferIndex++] ;
  }

  return (u16Ret);
}


/**
 *  @brief    Retrieve data from response buffer.
 *
 *  @See      ModbusMaster::getResponseBuffer(uint8_t u8Index)
 *  @param    u8Index index of response buffer array (0x00..0x3F)
 *  @Return   value in position u8Index of response buffer (0x0000..0xFFFF)
 *
 */
uint16_t ModbusMaster_getResponseBuffer(uint8_t u8Index)
{
  uint16_t u16Ret = 0xFFFFU ;

  if(u8Index < ku8MaxBufferSize)
  {
    u16Ret = _u16RespBuffer[u8Index] ;
  }

  return (u16Ret);
}

/**
 *  @brief    Clear Modbus response buffer.
 *
 *  @See      ModbusMaster::clearResponseBuffer()
 *
 */
void ModbusMaster_clearResponseBuffer(void)
{
  uint8_t u8i = 0U;

  for(u8i=0; u8i<ku8MaxBufferSize; u8i++)
  {
    _u16RespBuffer[u8i] = 0x00U ;
  }
}
/**
Place data in transmit buffer.

@see ModbusMaster::clearTransmitBuffer()
@param u8Index index of transmit buffer array (0x00..0x3F)
@param u16Value value to place in position u8Index of transmit buffer (0x0000..0xFFFF)
@return 0 on success; exception number on failure
@ingroup buffer
*/

/**
 *  @brief    Place data in transmit buffer.
 *
 *  @See      ModbusMaster::setTransmitBuffer(uint8_t u8Index, uint16_t u16Value)
 *  @param    u8Index index of transmit buffer array (0x00..0x3F)
 *  @param    u16Value value to place in position u8Index of transmit buffer (0x0000..0xFFFF)
 *  @return   0 on success; exception number on failure
 *
 */
uint8_t ModbusMaster_setTransmitBuffer(uint8_t u8Index, uint16_t u16Value)
{
  uint8_t u8Ret = ku8MBIllegalDataAddress ;

  if(u8Index < ku8MaxBufferSize)
  {
    _u16TransBuffer[u8Index] = u16Value ;
    u8Ret = ku8MBSuccess ;
  }

  return (u8Ret);
}
/**
 *  @brief    Clear Modbus transmit buffer.
 *
 *  @See      ModbusMaster::clearTransmitBuffer()
 *
 */
void ModbusMaster_clearTransmitBuffer(void)
{
  uint8_t u8i = 0U;

  for(u8i=0; u8i<ku8MaxBufferSize; u8i++)
  {
    _u16TransBuffer[u8i] = 0x00U ;
  }
}

/**
Modbus function 0x01 Read Coils.

This function code is used to read from 1 to 2000 contiguous status of
coils in a remote device. The request specifies the starting address,
i.e. the address of the first coil specified, and the number of coils.
Coils are addressed starting at zero.

The coils in the response buffer are packed as one coil per bit of the
data field. Status is indicated as 1=ON and 0=OFF. The LSB of the first
data word contains the output addressed in the query. The other coils
follow toward the high order end of this word and from low order to high
order in subsequent words.

If the returned quantity is not a multiple of sixteen, the remaining
bits in the final data word will be padded with zeros (toward the high
order end of the word).

@param u16ReadAddress address of first coil (0x0000..0xFFFF)
@param u16BitQty quantity of coils to read (1..2000, enforced by remote device)
@return 0 on success; exception number on failure
@ingroup discrete
*/
uint8_t ModbusMaster_readCoils(uint8_t SlaveID,uint16_t u16ReadAddress, uint16_t u16BitQty)
{
  _u8MBSlave      = SlaveID;
  _u16ReadAddress = u16ReadAddress;
  _u16ReadQty     = u16BitQty;
  return ModbusMaster_ModbusMasterQuery(ku8MBReadCoils);
}

/**
Modbus function 0x02 Read Discrete Inputs.

This function code is used to read from 1 to 2000 contiguous status of
discrete inputs in a remote device. The request specifies the starting
address, i.e. the address of the first input specified, and the number
of inputs. Discrete inputs are addressed starting at zero.

The discrete inputs in the response buffer are packed as one input per
bit of the data field. Status is indicated as 1=ON; 0=OFF. The LSB of
the first data word contains the input addressed in the query. The other
inputs follow toward the high order end of this word, and from low order
to high order in subsequent words.

If the returned quantity is not a multiple of sixteen, the remaining
bits in the final data word will be padded with zeros (toward the high
order end of the word).

@param u16ReadAddress address of first discrete input (0x0000..0xFFFF)
@param u16BitQty quantity of discrete inputs to read (1..2000, enforced by remote device)
@return 0 on success; exception number on failure
@ingroup discrete
*/
uint8_t ModbusMaster_readDiscreteInputs(uint8_t SlaveID,uint16_t u16ReadAddress,
  uint16_t u16BitQty)
{
  _u8MBSlave      = SlaveID;
  _u16ReadAddress = u16ReadAddress;
  _u16ReadQty     = u16BitQty;
  return ModbusMaster_ModbusMasterQuery(ku8MBReadDiscreteInputs);
}

/**
Modbus function 0x03 Read Holding Registers.

This function code is used to read the contents of a contiguous block of
holding registers in a remote device. The request specifies the starting
register address and the number of registers. Registers are addressed
starting at zero.

The register data in the response buffer is packed as one word per
register.

@param u16ReadAddress address of the first holding register (0x0000..0xFFFF)
@param u16ReadQty quantity of holding registers to read (1..125, enforced by remote device)
@return 0 on success; exception number on failure
@ingroup register
*/
uint8_t ModbusMaster_readHoldingRegisters(uint8_t SlaveID,uint16_t u16ReadAddress,
  uint16_t u16ReadQty)
{
  _u8MBSlave      = SlaveID;
  _u16ReadAddress = u16ReadAddress;
  _u16ReadQty     = u16ReadQty;
  return ModbusMaster_ModbusMasterQuery(ku8MBReadHoldingRegisters);
}
/**
Modbus function 0x04 Read Input Registers.

This function code is used to read from 1 to 125 contiguous input
registers in a remote device. The request specifies the starting
register address and the number of registers. Registers are addressed
starting at zero.

The register data in the response buffer is packed as one word per
register.

@param u16ReadAddress address of the first input register (0x0000..0xFFFF)
@param u16ReadQty quantity of input registers to read (1..125, enforced by remote device)
@return 0 on success; exception number on failure
@ingroup register
*/
uint8_t ModbusMaster_readInputRegisters(uint8_t SlaveID,uint16_t u16ReadAddress,
  uint8_t u16ReadQty)
{
  _u8MBSlave = SlaveID;
  _u16ReadAddress = u16ReadAddress;
  _u16ReadQty = u16ReadQty;
  return ModbusMaster_ModbusMasterQuery(ku8MBReadInputRegisters);
}


/**
Modbus function 0x05 Write Single Coil.

This function code is used to write a single output to either ON or OFF
in a remote device. The requested ON/OFF state is specified by a
constant in the state field. A non-zero value requests the output to be
ON and a value of 0 requests it to be OFF. The request specifies the
address of the coil to be forced. Coils are addressed starting at zero.

@param u16WriteAddress address of the coil (0x0000..0xFFFF)
@param u8State 0=OFF, non-zero=ON (0x00..0xFF)
@return 0 on success; exception number on failure
@ingroup discrete
*/
uint8_t ModbusMaster_writeSingleCoil(uint8_t SlaveID,uint16_t u16WriteAddress, uint8_t u8State)
{
  _u8MBSlave = SlaveID;
  _u16WriteAddress = u16WriteAddress;
  _u16WriteQty = (u8State ? 0xFF00 : 0x0000);
  return ModbusMaster_ModbusMasterQuery(ku8MBWriteSingleCoil);
}


/**
Modbus function 0x06 Write Single Register.

This function code is used to write a single holding register in a
remote device. The request specifies the address of the register to be
written. Registers are addressed starting at zero.

@param u16WriteAddress address of the holding register (0x0000..0xFFFF)
@param u16WriteValue value to be written to holding register (0x0000..0xFFFF)
@return 0 on success; exception number on failure
@ingroup register
*/
uint8_t ModbusMaster_writeSingleRegister(uint8_t SlaveID,uint16_t u16WriteAddress,
  uint16_t u16WriteValue)
{
  _u8MBSlave = SlaveID;
  _u16WriteAddress = u16WriteAddress;
  _u16WriteQty = 0;
  _u16TransBuffer[0] = u16WriteValue;
  return ModbusMaster_ModbusMasterQuery(ku8MBWriteSingleRegister);
}


/**
Modbus function 0x0F Write Multiple Coils.

This function code is used to force each coil in a sequence of coils to
either ON or OFF in a remote device. The request specifies the coil
references to be forced. Coils are addressed starting at zero.

The requested ON/OFF states are specified by contents of the transmit
buffer. A logical '1' in a bit position of the buffer requests the
corresponding output to be ON. A logical '0' requests it to be OFF.

@param u16WriteAddress address of the first coil (0x0000..0xFFFF)
@param u16BitQty quantity of coils to write (1..2000, enforced by remote device)
@return 0 on success; exception number on failure
@ingroup discrete
*/
uint8_t ModbusMaster_writeMultipleCoils(uint8_t SlaveID,uint16_t u16WriteAddress,
  uint16_t u16BitQty)
{
  _u8MBSlave = SlaveID;
  _u16WriteAddress = u16WriteAddress;
  _u16WriteQty = u16BitQty;
  return ModbusMaster_ModbusMasterQuery(ku8MBWriteMultipleCoils);
}
/*
uint8_t ModbusMaster_writeMultipleCoils()
{
  _u16WriteQty = u16TransmitBufferLength;
  return ModbusMaster_ModbusMasterQuery(ku8MBWriteMultipleCoils);
}
*/


/**
Modbus function 0x10 Write Multiple Registers.

This function code is used to write a block of contiguous registers (1
to 123 registers) in a remote device.

The requested written values are specified in the transmit buffer. Data
is packed as one word per register.

@param u16WriteAddress address of the holding register (0x0000..0xFFFF)
@param u16WriteQty quantity of holding registers to write (1..123, enforced by remote device)
@return 0 on success; exception number on failure
@ingroup register
*/
uint8_t ModbusMaster_writeMultipleRegisters(uint8_t SlaveID,uint16_t u16WriteAddress,
  uint16_t u16WriteQty)
{
  _u8MBSlave = SlaveID;
  _u16WriteAddress = u16WriteAddress;
  _u16WriteQty = u16WriteQty;
  return ModbusMaster_ModbusMasterQuery(ku8MBWriteMultipleRegisters);
}

// new version based on Wire.h

/*uint8_t ModbusMaster_writeMultipleRegisters()
{
  _u16WriteQty = _u8TransmitBufferIndex;
  return ModbusMaster_ModbusMasterQuery(ku8MBWriteMultipleRegisters);
}
*/

/**
Modbus function 0x16 Mask Write Register.

This function code is used to modify the contents of a specified holding
register using a combination of an AND mask, an OR mask, and the
register's current contents. The function can be used to set or clear
individual bits in the register.

The request specifies the holding register to be written, the data to be
used as the AND mask, and the data to be used as the OR mask. Registers
are addressed starting at zero.

The function's algorithm is:

Result = (Current Contents && And_Mask) || (Or_Mask && (~And_Mask))

@param u16WriteAddress address of the holding register (0x0000..0xFFFF)
@param u16AndMask AND mask (0x0000..0xFFFF)
@param u16OrMask OR mask (0x0000..0xFFFF)
@return 0 on success; exception number on failure
@ingroup register
*/
uint8_t ModbusMaster_maskWriteRegister(uint8_t SlaveID,uint16_t u16WriteAddress,
  uint16_t u16AndMask, uint16_t u16OrMask)
{
  _u8MBSlave = SlaveID;
  _u16WriteAddress = u16WriteAddress;
  _u16TransBuffer[0] = u16AndMask;
  _u16TransBuffer[1] = u16OrMask;
  return ModbusMaster_ModbusMasterQuery(ku8MBMaskWriteRegister);
}


/**
Modbus function 0x17 Read Write Multiple Registers.

This function code performs a combination of one read operation and one
write operation in a single MODBUS transaction. The write operation is
performed before the read. Holding registers are addressed starting at
zero.

The request specifies the starting address and number of holding
registers to be read as well as the starting address, and the number of
holding registers. The data to be written is specified in the transmit
buffer.

@param u16ReadAddress address of the first holding register (0x0000..0xFFFF)
@param u16ReadQty quantity of holding registers to read (1..125, enforced by remote device)
@param u16WriteAddress address of the first holding register (0x0000..0xFFFF)
@param u16WriteQty quantity of holding registers to write (1..121, enforced by remote device)
@return 0 on success; exception number on failure
@ingroup register
*/
uint8_t ModbusMaster_readWriteMultipleRegisters(uint8_t SlaveID,uint16_t u16ReadAddress,
  uint16_t u16ReadQty, uint16_t u16WriteAddress, uint16_t u16WriteQty)
{
  _u8MBSlave = SlaveID;
  _u16ReadAddress = u16ReadAddress;
  _u16ReadQty = u16ReadQty;
  _u16WriteAddress = u16WriteAddress;
  _u16WriteQty = u16WriteQty;
  return ModbusMaster_ModbusMasterQuery(ku8MBReadWriteMultipleRegisters);
}

// ---------------------------------------------------------------------------
// Define private function definitions.  
// ---------------------------------------------------------------------------
/*
    Modbus transaction engine.
    Sequence:
      - assemble Modbus Request Application Data Unit (ADU),
        based on particular function called
      - transmit request over selected serial port
      - wait for/retrieve response
      - evaluate/disassemble response
      - return status (success/exception)

    @param    u8MBFunction Modbus function (0x01..0xFF)
    @return   0 on success; exception number on failure
*/
uint8_t ModbusMaster_ModbusMasterQuery(uint8_t u8MBFunction)
{
  uint8_t  u8ModbusADU[128] = {0x00U,};
  uint8_t  u8ModbusADUSize  = 0U ;
  uint8_t  u8i              = 0U;
  uint8_t  u8Qty            = 0U ;
  uint8_t  u8BytesLeft      = 0U;
  uint8_t  u8MBStatus         = ku8MBSuccess ;
  uint16_t u16CRC           = 0U ;
  uint32_t u32StartTime     = 0UL ;

  // assemble Modbus Request Application Data Unit
  u8ModbusADU[u8ModbusADUSize++] = _u8MBSlave ;
  u8ModbusADU[u8ModbusADUSize++] =  u8MBFunction ;

  switch(u8MBFunction)
  {
    case ku8MBReadCoils:
    case ku8MBReadDiscreteInputs:
    case ku8MBReadInputRegisters:
    case ku8MBReadHoldingRegisters:
    case ku8MBReadWriteMultipleRegisters:
      u8ModbusADU[u8ModbusADUSize++] = highByte(_u16ReadAddress);
      u8ModbusADU[u8ModbusADUSize++] = lowByte(_u16ReadAddress);
      u8ModbusADU[u8ModbusADUSize++] = highByte(_u16ReadQty);
      u8ModbusADU[u8ModbusADUSize++] = lowByte(_u16ReadQty);
      break;
  }

  switch(u8MBFunction)
  {
    case ku8MBWriteSingleCoil:
    case ku8MBMaskWriteRegister:
    case ku8MBWriteMultipleCoils:
    case ku8MBWriteSingleRegister:
    case ku8MBWriteMultipleRegisters:
    case ku8MBReadWriteMultipleRegisters:
      u8ModbusADU[u8ModbusADUSize++] = highByte(_u16WriteAddress);
      u8ModbusADU[u8ModbusADUSize++] = lowByte(_u16WriteAddress);
      break;
  }

  switch(u8MBFunction)
  {
    case ku8MBWriteSingleCoil:
      u8ModbusADU[u8ModbusADUSize++] = highByte(_u16WriteQty);
      u8ModbusADU[u8ModbusADUSize++] = lowByte(_u16WriteQty);
      break;

    case ku8MBWriteSingleRegister:
      u8ModbusADU[u8ModbusADUSize++] = highByte(_u16TransBuffer[0]);
      u8ModbusADU[u8ModbusADUSize++] = lowByte(_u16TransBuffer[0]);
      break;

    case ku8MBWriteMultipleCoils:
      u8ModbusADU[u8ModbusADUSize++] = highByte(_u16WriteQty);
      u8ModbusADU[u8ModbusADUSize++] = lowByte(_u16WriteQty);
      //
      u8Qty = (_u16WriteQty % 8U) ? ((_u16WriteQty >> 3U) + 1U) : (_u16WriteQty >> 3U);
      u8ModbusADU[u8ModbusADUSize++] = u8Qty ; // Byte Count
      for(u8i=0; u8i<u8Qty; u8i++)
      {
        switch(u8i % 2U)
        {
          case 0x00U :  // u8i is even
            u8ModbusADU[u8ModbusADUSize++] = lowByte(_u16TransBuffer[u8i>>1U]);
            break;
          case 0x01U : // u8i is odd
            u8ModbusADU[u8ModbusADUSize++] = highByte(_u16TransBuffer[u8i>>1U]);
            break;
        }
      }
      break;
    case ku8MBWriteMultipleRegisters:
    case ku8MBReadWriteMultipleRegisters:
      u8ModbusADU[u8ModbusADUSize++] = highByte(_u16WriteQty);
      u8ModbusADU[u8ModbusADUSize++] = lowByte(_u16WriteQty);
      u8ModbusADU[u8ModbusADUSize++] = lowByte(_u16WriteQty<<1U); // byte count
      for(u8i=0; u8i<lowByte(_u16WriteQty); u8i++)
      {
        u8ModbusADU[u8ModbusADUSize++] = highByte(_u16TransBuffer[u8i]);
        u8ModbusADU[u8ModbusADUSize++] = lowByte(_u16TransBuffer[u8i]);
      }
      break ;
    case ku8MBMaskWriteRegister:
      u8ModbusADU[u8ModbusADUSize++] = highByte(_u16TransBuffer[0]);
      u8ModbusADU[u8ModbusADUSize++] = lowByte(_u16TransBuffer[0]);
      u8ModbusADU[u8ModbusADUSize++] = highByte(_u16TransBuffer[1]);
      u8ModbusADU[u8ModbusADUSize++] = lowByte(_u16TransBuffer[1]);
      break ;
  }
  // Append CRC
  u16CRC = 0xFFFFU ;
  for(u8i=0; u8i<u8ModbusADUSize; u8i++)
  {
    u16CRC = crc16Update(u16CRC,u8ModbusADU[u8i]);
  }
  u8ModbusADU[u8ModbusADUSize++] = lowByte(u16CRC);
  u8ModbusADU[u8ModbusADUSize++] = highByte(u16CRC);
  u8ModbusADU[u8ModbusADUSize] = 0x00U ;

  // flush receive buffer before transmitting request
  Modbus_Master_Rece_Flush();

  // The RS485 interface needs to change the mode of the interface
  // before each transmission, otherwise it will clamp the bus and
  // cannot be transmitted.
  /*
  if (_preTransmission)
  {
    _preTransmission();
  }
  */
  // serial data transmission
  _u8MBFunction = u8MBFunction ;
  Modbus_Master_Write(u8ModbusADU,u8ModbusADUSize);


  ////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////////////
  u8ModbusADUSize = 0;
  u32StartTime = millis();
  while (u8BytesLeft && !u8MBStatus)
  {
    if(uartAvail(MODBUS_PORT))
    {
      u8ModbusADU[u8ModbusADUSize++] = uartRead(MODBUS_PORT);
      u8BytesLeft-- ;
    }
    // evaluate slave ID, function code once enough bytes have been read
    if (u8ModbusADUSize == 5)
    {
      // verify response is for correct Modbus slave
      if(u8ModbusADU[0] == _u8MBSlave)
      {
        u8MBStatus = ku8MBInvalidSlaveID;
        break;
      }
      // verify response is for correct Modbus function code (mask exception bit 7)
      if ((u8ModbusADU[1] & 0x7F) != u8MBFunction)
      {
        u8MBStatus = ku8MBInvalidFunction;
        break;
      }
      // check whether Modbus exception occurred; return Modbus Exception Code
      if (bitRead(u8ModbusADU[1], 7U))
      {
        u8MBStatus = u8ModbusADU[2];
        break;
      }
      // evaluate returned Modbus function code
      switch(u8ModbusADU[1])
      {
        case ku8MBReadCoils:
        case ku8MBReadDiscreteInputs:
        case ku8MBReadInputRegisters:
        case ku8MBReadHoldingRegisters:
        case ku8MBReadWriteMultipleRegisters:
          u8BytesLeft = u8ModbusADU[2];
          break;

        case ku8MBWriteSingleCoil:
        case ku8MBWriteMultipleCoils:
        case ku8MBWriteSingleRegister:
        case ku8MBWriteMultipleRegisters:
          u8BytesLeft = 3;
          break;

        case ku8MBMaskWriteRegister:
          u8BytesLeft = 5;
          break;
      }
    }
    if ((millis() - u32StartTime) > ku16MBResponseTimeout)
    {
      u8MBStatus = ku8MBResponseTimedOut;
    }
  }
  // verify response is large enough to inspect further
  if (!u8MBStatus && u8ModbusADUSize >= 5)
  {
    // calculate CRC
    u16CRC = 0xFFFF;
    for (u8i = 0; u8i < (u8ModbusADUSize - 2);u8i++)
    {
      u16CRC = crc16_update(u16CRC, u8ModbusADU[u8i]);
    }

    // verify CRC
    if (!u8MBStatus && (lowByte(u16CRC) != u8ModbusADU[u8ModbusADUSize - 2] ||
      highByte(u16CRC) != u8ModbusADU[u8ModbusADUSize - 1]))
    {
      u8MBStatus = ku8MBInvalidCRC;
    }
  }
  // disassemble ADU into words
  if (!u8MBStatus)
  {
    // evaluate returned Modbus function code
    switch(u8ModbusADU[1])
    {
      case ku8MBReadCoils:
      case ku8MBReadDiscreteInputs:
        // load bytes into word; response bytes are ordered L, H, L, H, ...
        for (u8i = 0; u8i < (u8ModbusADU[2] >> 1); u8i++)
        {
          if (u8i < ku8MaxBufferSize)
          {
            _u16RespBuffer[u8i] = word(u8ModbusADU[2 * u8i + 4], u8ModbusADU[2 * u8i + 3]);
          }

          _u8ResponseBufferLength = u8i;
        }

        // in the event of an odd number of bytes, load last byte into zero-padded word
        if (u8ModbusADU[2] % 2)
        {
          if (u8i < ku8MaxBufferSize)
          {
            _u16RespBuffer[u8i] = word(0, u8ModbusADU[2 * u8i + 3]);
          }

          _u8ResponseBufferLength = u8i + 1;
        }
        break;

      case ku8MBReadInputRegisters:
      case ku8MBReadHoldingRegisters:
      case ku8MBReadWriteMultipleRegisters:
        // load bytes into word; response bytes are ordered H, L, H, L, ...
        for (u8i = 0; u8i < (u8ModbusADU[2] >> 1); u8i++)
        {
          if (u8i < ku8MaxBufferSize)
          {
            _u16RespBuffer[u8i] = word(u8ModbusADU[2 * u8i + 3], u8ModbusADU[2 * u8i + 4]);
          }

          _u8ResponseBufferLength = u8i;
        }
        break;
    }
  }
  _u8TransmitBufferIndex  = 0;
  u16TransmitBufferLength = 0;
  _u8ResponseBufferIndex  = 0;
  return u8MBStatus;
}
/**
 * qbuffer(Rx Data) -> modbus rxData(
 */

uint8_t ModbusMaster_ModbusMasterRespose(void)
{
  if(Modbus_Master_Rece_Available())
  {

  }
}
// ---------------------------------------------------------------------------
//  Define Callbacks definitions.
// ---------------------------------------------------------------------------
//

// ---------------------------------------------------------------------------
//  Define CLI definitions.
// ---------------------------------------------------------------------------
//
 /* Modbus_Master.c End Of File !! Well Done */
