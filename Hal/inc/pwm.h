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
#define PWM_LEFT_LED1_PORT  (GPIOC)
#define PWM_LEFT_LED1_PIN   (GPIO_PIN_4)

#define PWM_LEFT_LED2_PORT  (GPIOC)
#define PWM_LEFT_LED2_PIN   (GPIO_PIN_3)

#define PWM_RIGHT_LED1_PORT (GPIOC)
#define PWM_RIGHT_LED1_PIN  (GPIO_PIN_1)

#define PWM_RIGHT_LED2_PORT (GPIOC)
#define PWM_RIGHT_LED2_PIN  (GPIO_PIN_2)

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
void SetLeftLed1PWM(uint16_t compare);
void SetLeftLed2PWM(uint16_t compare);
void SetRightLed1PWM(uint16_t compare);
void SetRightLed2PWM(uint16_t compare);
#endif