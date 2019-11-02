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
#include "light_blink.h"
#include "pwm.h"
#include "timer.h"
/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
void LightAllOff(LightBlinkCtrl_t *lightctrl);
void LightAllOn(LightBlinkCtrl_t *lightctrl);
void LightTurnLeft(LightBlinkCtrl_t *lightctrl);
void LightTurnRight(LightBlinkCtrl_t *lightctrl);
void LightBlink(LightBlinkCtrl_t *lightctrl);
/* Private variables ---------------------------------------------------------*/
LightBlinkCtrl_t LightBlinkCtrl =
{
  .state      = LIGHT_OFF,
  .blinkstate = LIGHT_OFF,
  .loopmode   = LOOP_MODE_NONE,
  .mode       = ALL_OFF,
  .loopNum    = 0
};

/******************************************************************************/
/*******************************************************************************
* @fn     LightBlinkMode
*
* @brief  Light Blink Mode
*
* @param  enLightBlinkMode mode
*
* @return none
*/
void LightBlinkMode(LightBlinkCtrl_t *lightctrl)
{
  switch(lightctrl->mode)
  {
  case ALL_OFF:
    LightAllOff(lightctrl);
    break;
  case ALL_ON:
    LightAllOn(lightctrl);
    break;
  case TURN_LEFT:
    LightTurnLeft(lightctrl);
    break;
  case TURN_RIGHT:
    LightTurnRight(lightctrl);
    break;
  case FAST_BLINK:
  case SLOW_BLINK:
    LightBlink(lightctrl);
    break;
  default:
    break;
  }
}

/*******************************************************************************
* @fn     LightAllOff
*
* @brief  LightAllOff
*
* @param
*
* @return none
*/
void LightAllOff(LightBlinkCtrl_t *lightctrl)
{
  SetLeftLight1PWM(0);
  SetLeftLight2PWM(0);
  SetRightLight2PWM(0);
  SetRightLight1PWM(0);

  lightctrl->state[0] = LIGHT_OFF;
  lightctrl->state[1] = LIGHT_OFF;
  lightctrl->state[2] = LIGHT_OFF;
  lightctrl->state[3] = LIGHT_OFF;
}

/*******************************************************************************
* @fn     LightAllOn
*
* @brief  LightAllOn
*
* @param
*
* @return none
*/
void LightAllOn(LightBlinkCtrl_t *lightctrl)
{
  SetLeftLight1PWM(TIME_PWM_PERIOD);
  SetLeftLight2PWM(TIME_PWM_PERIOD);
  SetRightLight2PWM(TIME_PWM_PERIOD);
  SetRightLight1PWM(TIME_PWM_PERIOD);

  lightctrl->state[0] = LIGHT_ON;
  lightctrl->state[1] = LIGHT_ON;
  lightctrl->state[2] = LIGHT_ON;
  lightctrl->state[3] = LIGHT_ON;
}

/*******************************************************************************
* @fn     LightTurnLeft
*
* @brief  LightTurnLeft
*
* @param
*
* @return none
*/
void LightTurnLeft(LightBlinkCtrl_t *lightctrl)
{
  if(lightctrl->loopNum == 0)
  {
    lightctrl->loopmode = RIGHT_LIGHT1_ON;
  }
  else if(lightctrl->loopNum >= LIGHT_LOOP_NUM *LIGHT_NUM)
  {
    lightctrl->loopmode = LOOP_MODE_NONE;
    lightctrl->loopNum = 0;
  }

  switch(lightctrl->loopmode)
  {
  case RIGHT_LIGHT1_ON:
    {
      lightctrl->loopmode = RIGHT_LIGHT2_ON;
      lightctrl->state[0] = LIGHT_OFF;
      lightctrl->state[1] = LIGHT_OFF;
      lightctrl->state[2] = LIGHT_OFF;
      lightctrl->state[3] = LIGHT_ON;

      SetLeftLight1PWM(0);
      SetLeftLight2PWM(0);
      SetRightLight2PWM(0);
      SetRightLight1PWM(TIME_PWM_PERIOD);
    }
    break;
  case RIGHT_LIGHT2_ON:
    {
      lightctrl->loopmode = LEFT_LIGHT2_ON;
      lightctrl->state[0] = LIGHT_OFF;
      lightctrl->state[1] = LIGHT_OFF;
      lightctrl->state[2] = LIGHT_ON;
      lightctrl->state[3] = LIGHT_OFF;

      SetLeftLight1PWM(0);
      SetLeftLight2PWM(0);
      SetRightLight2PWM(TIME_PWM_PERIOD);
      SetRightLight1PWM(0);
    }
    break;

  case LEFT_LIGHT2_ON:
    {
      lightctrl->loopmode = LEFT_LIGHT1_ON;
      lightctrl->state[0] = LIGHT_OFF;
      lightctrl->state[1] = LIGHT_ON;
      lightctrl->state[2] = LIGHT_OFF;
      lightctrl->state[3] = LIGHT_OFF;

      SetLeftLight1PWM(0);
      SetLeftLight2PWM(TIME_PWM_PERIOD);
      SetRightLight2PWM(0);
      SetRightLight1PWM(0);
    }
    break;
  case LEFT_LIGHT1_ON:
    {
      lightctrl->loopmode = RIGHT_LIGHT1_ON;
      lightctrl->state[0] = LIGHT_ON;
      lightctrl->state[1] = LIGHT_OFF;
      lightctrl->state[2] = LIGHT_OFF;
      lightctrl->state[3] = LIGHT_OFF;

      SetLeftLight1PWM(TIME_PWM_PERIOD);
      SetLeftLight2PWM(0);
      SetRightLight2PWM(0);
      SetRightLight1PWM(0);
    }
    break;
  case LOOP_MODE_NONE:
    {
      lightctrl->state[0] = LIGHT_OFF;
      lightctrl->state[1] = LIGHT_OFF;
      lightctrl->state[2] = LIGHT_OFF;
      lightctrl->state[3] = LIGHT_OFF;

      SetLeftLight1PWM(0);
      SetLeftLight2PWM(0);
      SetRightLight2PWM(0);
      SetRightLight1PWM(0);
    }
    break;
  default:
    break;
  }

  if(lightctrl->loopmode != LOOP_MODE_NONE)
  {
    lightctrl->loopNum++;
    system_start_timer(USERAPP_LIGHT_BLINK_EVT,TIMER_ONCE_MODE,LIGHT_FLASH_BLINK_TIME);
  }
}

/*******************************************************************************
* @fn     LightTurnRight
*
* @brief  LightTurnRight
*
* @param
*
* @return none
*/
void LightTurnRight(LightBlinkCtrl_t *lightctrl)
{
  if(lightctrl->loopNum == 0)
  {
    lightctrl->loopmode = LEFT_LIGHT1_ON;
  }
  else if(lightctrl->loopNum >= LIGHT_LOOP_NUM *LIGHT_NUM)
  {
    lightctrl->loopmode = LOOP_MODE_NONE;
    lightctrl->loopNum = 0;
  }

  switch(lightctrl->loopmode)
  {
  case LEFT_LIGHT1_ON:
    {
      lightctrl->loopmode = LEFT_LIGHT2_ON;
      lightctrl->state[0] = LIGHT_ON;
      lightctrl->state[1] = LIGHT_OFF;
      lightctrl->state[2] = LIGHT_OFF;
      lightctrl->state[3] = LIGHT_OFF;

      SetLeftLight1PWM(TIME_PWM_PERIOD);
      SetLeftLight2PWM(0);
      SetRightLight2PWM(0);
      SetRightLight1PWM(0);
    }
    break;
  case LEFT_LIGHT2_ON:
    {
      lightctrl->loopmode = RIGHT_LIGHT2_ON;
      lightctrl->state[0] = LIGHT_OFF;
      lightctrl->state[1] = LIGHT_ON;
      lightctrl->state[2] = LIGHT_OFF;
      lightctrl->state[3] = LIGHT_OFF;

      SetLeftLight1PWM(0);
      SetLeftLight2PWM(TIME_PWM_PERIOD);
      SetRightLight2PWM(0);
      SetRightLight1PWM(0);
    }
    break;
  case RIGHT_LIGHT2_ON:
    {
      lightctrl->loopmode = RIGHT_LIGHT1_ON;
      lightctrl->state[0] = LIGHT_OFF;
      lightctrl->state[1] = LIGHT_OFF;
      lightctrl->state[2] = LIGHT_ON;
      lightctrl->state[3] = LIGHT_OFF;

      SetLeftLight1PWM(0);
      SetLeftLight2PWM(0);
      SetRightLight2PWM(TIME_PWM_PERIOD);
      SetRightLight1PWM(0);
    }
    break;
  case RIGHT_LIGHT1_ON:
    {
      lightctrl->loopmode = LEFT_LIGHT1_ON;
      lightctrl->state[0] = LIGHT_OFF;
      lightctrl->state[1] = LIGHT_OFF;
      lightctrl->state[2] = LIGHT_OFF;
      lightctrl->state[3] = LIGHT_ON;

      SetLeftLight1PWM(0);
      SetLeftLight2PWM(0);
      SetRightLight2PWM(0);
      SetRightLight1PWM(TIME_PWM_PERIOD);
    }
    break;
  case LOOP_MODE_NONE:
    {
      lightctrl->state[0] = LIGHT_OFF;
      lightctrl->state[1] = LIGHT_OFF;
      lightctrl->state[2] = LIGHT_OFF;
      lightctrl->state[3] = LIGHT_OFF;

      SetLeftLight1PWM(0);
      SetLeftLight2PWM(0);
      SetRightLight2PWM(0);
      SetRightLight1PWM(0);
    }
    break;
  default:
    break;
  }

  if(lightctrl->loopmode != LOOP_MODE_NONE)
  {
    lightctrl->loopNum++;
    system_start_timer(USERAPP_LIGHT_BLINK_EVT,TIMER_ONCE_MODE,LIGHT_FLASH_BLINK_TIME);
  }
}

/*******************************************************************************
* @fn     LightBlink
*
* @brief  LightBlink
*
* @param
*
* @return none
*/
void LightBlink(LightBlinkCtrl_t *lightctrl)
{
  switch(lightctrl->blinkstate)
  {
  case LIGHT_OFF:
    LightAllOff(lightctrl);
    lightctrl->blinkstate = LIGHT_ON;
    break;
  case LIGHT_ON:
    LightAllOn(lightctrl);
    lightctrl->blinkstate = LIGHT_OFF;
    break;
  default:
    break;
  }
  if(lightctrl->mode == FAST_BLINK)
  {
    system_start_timer(USERAPP_LIGHT_BLINK_EVT,TIMER_ONCE_MODE,LIGHT_FAST_BLINK_TIME);
  }
  else
  {
    system_start_timer(USERAPP_LIGHT_BLINK_EVT,TIMER_ONCE_MODE,LIGHT_SLOW_BLINK_TIME);
  }

}