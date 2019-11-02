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
#include "pwm.h"
/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/*******************************************************************************
* @fn
*
* @brief
*
* @param
*
* @return
*/
void PWM_Init(void)
{
  /* GPIOD configuration: TIM1 channel 1 (PD2), channel 2 (PD4) and channel 3 (PD5) */
  GPIO_Init(PWM_LEFT_LIGHT1_PORT, PWM_LEFT_LIGHT1_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);
  GPIO_Init(PWM_LEFT_LIGHT2_PORT, PWM_LEFT_LIGHT2_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);
  GPIO_Init(PWM_RIGHT_LIGHT1_PORT, PWM_RIGHT_LIGHT1_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);
  GPIO_Init(PWM_RIGHT_LIGHT2_PORT, PWM_RIGHT_LIGHT2_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);

  /*pwm config*/
  PWM_Config(TIME_PWM_PERIOD);
  PWM_Disable();
}
/*******************************************************************************
* @fn       PWM_Config
*
* @brief    PWM_Config
*
* @param
*
* @return
*/
void PWM_Config(uint16_t period)
{
  /* Enable TIM1 clock */
  CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER1, ENABLE);

  /* Time Base configuration */
  TIM1_TimeBaseInit(TIME_PRESCALER, TIM1_COUNTERMODE_UP, period, TIME_REPTETION_COUNTER);

  /* Channels 1, 2, 3 4 Configuration in PWM2 mode */
  TIM1_OC1Init(TIM1_OCMODE_PWM2, TIM1_OUTPUTSTATE_ENABLE, TIM1_OUTPUTNSTATE_DISABLE, 0,
               TIM1_OCPOLARITY_LOW, TIM1_OCNPOLARITY_LOW, TIM1_OCIDLESTATE_SET, TIM1_OCNIDLESTATE_RESET);

  TIM1_OC2Init(TIM1_OCMODE_PWM2, TIM1_OUTPUTSTATE_ENABLE, TIM1_OUTPUTNSTATE_DISABLE, 0,
               TIM1_OCPOLARITY_LOW, TIM1_OCNPOLARITY_LOW, TIM1_OCIDLESTATE_SET, TIM1_OCNIDLESTATE_RESET);

  TIM1_OC3Init(TIM1_OCMODE_PWM2, TIM1_OUTPUTSTATE_ENABLE, TIM1_OUTPUTNSTATE_DISABLE, 0,
               TIM1_OCPOLARITY_LOW, TIM1_OCNPOLARITY_LOW, TIM1_OCIDLESTATE_SET, TIM1_OCNIDLESTATE_RESET);

  TIM1_OC4Init(TIM1_OCMODE_PWM2,TIM1_OUTPUTSTATE_ENABLE, 0,
               TIM1_OCPOLARITY_LOW, TIM1_OCIDLESTATE_RESET);
}

/*******************************************************************************
* @fn   PWM_Enable
*
* @brief
*
* @param
*
* @return
*/
void PWM_Enable(void)
{
  GPIO_WriteLow(PWM_LEFT_LIGHT1_PORT, PWM_LEFT_LIGHT1_PIN);
  GPIO_WriteLow(PWM_LEFT_LIGHT2_PORT, PWM_LEFT_LIGHT2_PIN);
  GPIO_WriteLow(PWM_RIGHT_LIGHT1_PORT, PWM_RIGHT_LIGHT1_PIN);
  GPIO_WriteLow(PWM_RIGHT_LIGHT2_PORT, PWM_RIGHT_LIGHT2_PIN);

  /* Enable TIM1 outputs */
  TIM1_CtrlPWMOutputs(ENABLE);

  /* TIM1 enable counter */
  TIM1_Cmd(ENABLE);
}

/*******************************************************************************
* @fn   PWM_Disable
*
* @brief
*
* @param
*
* @return
*/
void PWM_Disable(void)
{
  GPIO_WriteLow(PWM_LEFT_LIGHT1_PORT, PWM_LEFT_LIGHT1_PIN);
  GPIO_WriteLow(PWM_LEFT_LIGHT2_PORT, PWM_LEFT_LIGHT2_PIN);
  GPIO_WriteLow(PWM_RIGHT_LIGHT1_PORT, PWM_RIGHT_LIGHT1_PIN);
  GPIO_WriteLow(PWM_RIGHT_LIGHT2_PORT, PWM_RIGHT_LIGHT2_PIN);

  /* Enable TIM1 outputs */
  TIM1_CtrlPWMOutputs(DISABLE);

  /* TIM1 enable counter */
  TIM1_Cmd(DISABLE);
}

/*******************************************************************************
* @fn   ResetPWM
*
* @brief
*
* @param
*
* @return
*/
void ResetPWM(void)
{
  TIM1_SetCounter(0x0000);
}

/*******************************************************************************
* @fn   SetLeftLed1PWM
*
* @brief
*
* @param
*
* @return
*/
void SetLeftLight1PWM(uint16_t compare)
{
  TIM1_SetCompare4(compare);
}

/*******************************************************************************
* @fn   SetLeftLed2PWM
*
* @brief
*
* @param
*
* @return
*/
void SetLeftLight2PWM(uint16_t compare)
{
  TIM1_SetCompare3(compare);
}

/*******************************************************************************
* @fn   SetRightLed1PWM
*
* @brief
*
* @param
*
* @return
*/
void SetRightLight1PWM(uint16_t compare)
{
  TIM1_SetCompare1(compare);
}
/*******************************************************************************
* @fn   SetRightLed2PWM
*
* @brief
*
* @param
*
* @return
*/
void SetRightLight2PWM(uint16_t compare)
{
  TIM1_SetCompare2(compare);
}