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
  br9600    = (uint8_t)0x01,
  br115200  = (uint8_t)0x02
}enBaudRate;
/******************************************************************************/
#if (defined DEBUG)
extern void DebugUart_Config(enBaudRate baudrate);
extern void DebugPrintf(uint8_t *buff,uint8_t len);
#endif

#endif