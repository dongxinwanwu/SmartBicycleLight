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
/* Private typedef -----------------------------------------------------------*/
/*****************************************************************************/

/*ָʾ��״̬ö��*/
typedef enum
{
  OFF = (uint8_t)0x00,
  ON  = (uint8_t)0x01
}LedState;

/*Ӳ������IO����*/
typedef struct
{
  /* Control Pin */
  GPIO_TypeDef *port;
  GPIO_Pin_TypeDef pin;
  GPIO_Mode_TypeDef mode;
}HardLink_t;

/*IO�ж�����*/
typedef struct
{
  EXTI_Port_TypeDef exit_port;
  EXTI_Sensitivity_TypeDef exit_Trigger;
}ExitLink_t;


/*****************************************************************************/
#endif

