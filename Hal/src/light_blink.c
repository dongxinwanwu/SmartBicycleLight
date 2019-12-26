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
  .mode       = ALL_OFF,
  .turnstate  = FINSHED,
  .lightIdx   = 0,
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
  case MEDIUM_BLINK:
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
  for(uint8_t i = 0; i < LIGHT_NUM; i++)
  {
    LedControl(&DirLedTab[i].hardLink,OFF);
    lightctrl->state[i] = LIGHT_OFF;
  }
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
  for(uint8_t i = 0; i < LIGHT_NUM; i++)
  {
    LedControl(&DirLedTab[i].hardLink,ON);
    lightctrl->state[i] = LIGHT_ON;
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
    lightctrl->lightIdx = LIGHT_NUM - 1;
    lightctrl->turnstate = NFINSHED;
  }

  /*off*/
  for(uint8_t i = 0; i < LIGHT_NUM; i++)
  {
    LedControl(&DirLedTab[i].hardLink,OFF);
    lightctrl->state[i] = LIGHT_OFF;
  }

  /*loop num*/
  if(lightctrl->loopNum < LIGHT_LOOP_NUM *LIGHT_NUM)
  {
    lightctrl->loopNum++;

    /*on*/
    LedControl(&DirLedTab[lightctrl->lightIdx].hardLink,ON);
    lightctrl->state[lightctrl->lightIdx] = LIGHT_ON;

    /*next led index*/
    lightctrl->lightIdx--;
    if(lightctrl->lightIdx < 0)
      lightctrl->lightIdx = LIGHT_NUM - 1;

    system_start_timer(USERAPP_LIGHT_BLINK_EVT,TIMER_ONCE_MODE,LIGHT_FLASH_BLINK_TIME);
  }
  else
  {
    lightctrl->lightIdx = 0;
    lightctrl->loopNum = 0;
    lightctrl->turnstate = FINSHED;
    lightctrl->mode = ALL_OFF;
  }
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
    lightctrl->lightIdx = 0;
    lightctrl->turnstate = NFINSHED;
  }

  /*off*/
  for(uint8_t i = 0; i < LIGHT_NUM; i++)
  {
    LedControl(&DirLedTab[i].hardLink,OFF);
    lightctrl->state[i] = LIGHT_OFF;
  }

  /*loop num*/
  if(lightctrl->loopNum < LIGHT_LOOP_NUM *LIGHT_NUM)
  {
    lightctrl->loopNum++;

    /*on*/
    LedControl(&DirLedTab[lightctrl->lightIdx].hardLink,ON);
    lightctrl->state[lightctrl->lightIdx] = LIGHT_ON;

    /*next led index*/
    lightctrl->lightIdx++;
    if(lightctrl->lightIdx >= LIGHT_NUM)
      lightctrl->lightIdx = 0;

    system_start_timer(USERAPP_LIGHT_BLINK_EVT,TIMER_ONCE_MODE,LIGHT_FLASH_BLINK_TIME);
  }
  else
  {
    lightctrl->lightIdx = 0;
    lightctrl->loopNum = 0;
    lightctrl->turnstate = FINSHED;
    lightctrl->mode = ALL_OFF;
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

  switch(lightctrl->mode)
  {
  case FAST_BLINK:
    system_start_timer(USERAPP_LIGHT_BLINK_EVT,TIMER_ONCE_MODE,LIGHT_FAST_BLINK_TIME);
    break;

  case MEDIUM_BLINK:
    system_start_timer(USERAPP_LIGHT_BLINK_EVT,TIMER_ONCE_MODE,LIGHT_MEDIUM_BLINK_TIME);
    break;

  case SLOW_BLINK:
    system_start_timer(USERAPP_LIGHT_BLINK_EVT,TIMER_ONCE_MODE,LIGHT_SLOW_BLINK_TIME);
    break;
  default:
    break;
  }
}