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
/* Includes ------------------------------------------------------------------*/
#include "app.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
void System_eventloop_hook(void);
uint16_t ProcessSystemTimeEvent(uint16_t event_id);
void UserAppHandleKeys(uint8_t keys,UserKeyState_t state);

/* Private variables ---------------------------------------------------------*/
/*******************************************************************************
* @fn     System_eventloop_hook(void)
*
* @brief  System_eventloop_hook
*
* @param
*
* @return
*/
void System_eventloop_hook(void)
{

}

/*******************************************************************************
* @fn     System_timer_hook
*
* @brief
*
* @param
*
* @return
*/
void System_timer_hook(void)
{

}

/*******************************************************************************
* @fn     ProcessSystemTimeEvent(void)
*
* @brief  Process Sys Time Event
*
* @param
*
* @return
*/
uint16_t ProcessSystemTimeEvent(uint16_t events)
{
  /*user key poll*/
  if(events & USERAPP_RELOAD_WDT_EVT)
  {
    /* Reload IWDG counter */
    IWDG_ReloadCounter();
    return events ^ USERAPP_RELOAD_WDT_EVT;
  }

  if(events & USERAPP_KEY_POLL_EVT)
  {
    HalKeyPoll();
    return events ^ USERAPP_KEY_POLL_EVT;
  }

  if(events & USERAPP_KEY_CLICK_EVENT)
  {
    HalKeyClickCheck();
    return events ^ USERAPP_KEY_CLICK_EVENT;
  }

  if(events & USERAPP_KEY_HOLD_EVENT)
  {
    HalKeyLongHoldCheck();
    return events ^ USERAPP_KEY_HOLD_EVENT;
  }

  return 0;
}

/*******************************************************************************
* @fn     UserAppHandleKeys
*
* @brief  UserAppHandleKeys
*
* @param
*
* @return
*/
void UserAppHandleKeys(uint8_t keys,UserKeyState_t state)
{
  /**/
  if(keys & UserAppKey[0].keyLink.keyValue)
  {
    /*开关机按键*/
    if(state == LONG_HOLD)
    {

    }
    else
    {

    }
  }
  else if(keys & UserAppKey[1].keyLink.keyValue)
  {
    if(state == CLICK)
    {

    }
    else
    {

    }

  }
  else
  {

  }
}

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
