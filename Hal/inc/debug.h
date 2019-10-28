/**
******************************************************************************
* @file
* @author
* @version
* @date
* @brief
*
******************************************************************************
*/
#ifndef _DEBUG_H
#define _DEBUG_H

#include "stm8s.h"
#include "stdio.h"

/* Private define ------------------------------------------------------------*/
#ifndef DEBUG
#define DEBUG TRUE
#endif
/******************************************************************************/
typedef enum
{
  br4800    = (uint8_t)0x00,
  br9600    = (uint8_t)0x01,
  br14400   = (uint8_t)0x02,
  br19200   = (uint8_t)0x03,
  br38400   = (uint8_t)0x04,
  br56000   = (uint8_t)0x05,
  br57600   = (uint8_t)0x06,
  br115200  = (uint8_t)0x07
}enBaudRate;
/******************************************************************************/
extern void DebugUart_Config(enBaudRate baudrate);
extern void DebugPrintf(uint8_t *buff,uint8_t len);
#endif