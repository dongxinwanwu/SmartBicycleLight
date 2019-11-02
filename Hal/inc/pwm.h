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
#ifndef _PWM_H
#define _PWM_H

#include "define.h"
/* Private define ------------------------------------------------------------*/
#define PWM_LEFT_LIGHT1_PORT  (GPIOC)
#define PWM_LEFT_LIGHT1_PIN   (GPIO_PIN_4)

#define PWM_LEFT_LIGHT2_PORT  (GPIOC)
#define PWM_LEFT_LIGHT2_PIN   (GPIO_PIN_3)

#define PWM_RIGHT_LIGHT1_PORT (GPIOC)
#define PWM_RIGHT_LIGHT1_PIN  (GPIO_PIN_1)

#define PWM_RIGHT_LIGHT2_PORT (GPIOC)
#define PWM_RIGHT_LIGHT2_PIN  (GPIO_PIN_2)

#define TIME_PWM_PERIOD                   800
#define TIME_PRESCALER                    0
#define TIME_REPTETION_COUNTER            0
/******************************************************************************/

/******************************************************************************/
void PWM_Init(void);
void PWM_Config(uint16_t period);
void PWM_Enable(void);
void PWM_Disable(void);
void ResetPWM(void);
void SetLeftLight1PWM(uint16_t compare);
void SetLeftLight2PWM(uint16_t compare);
void SetRightLight1PWM(uint16_t compare);
void SetRightLight2PWM(uint16_t compare);
#endif