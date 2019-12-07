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
#ifndef _DEFINE_H
#define _DEFINE_H

#include "stm8s.h"

#ifndef BV
#define BV(n)      (1 << (n))
#endif

#ifndef MAX
#define MAX(x, y)                 (((x) > (y)) ? (x) : (y))
#endif

#ifndef MIN
#define MIN(x, y)                 (((x) < (y)) ? (x) : (y))
#endif
/* Private typedef -----------------------------------------------------------*/
/*****************************************************************************/
#ifndef DEBUG
#define DEBUG
#endif

/*指示灯状态枚举*/
typedef enum
{
  OFF = (uint8_t)0x00,
  ON  = (uint8_t)0x01
}enLedState;

/*硬件物理IO配置*/
typedef struct
{
  /* Control Pin */
  GPIO_TypeDef *port;
  GPIO_Pin_TypeDef pin;
  GPIO_Mode_TypeDef mode;
}HardLink_t;

/*IO中断配置*/
typedef struct
{
  EXTI_Port_TypeDef exit_port;
  EXTI_Sensitivity_TypeDef exit_Trigger;
}ExitLink_t;


/*****************************************************************************/
#endif

