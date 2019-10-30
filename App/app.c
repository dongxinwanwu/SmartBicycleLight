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

  if(events & USERAPP_LIS3DH_INIT_EVT)
  {
    LIS3DH_Init();
    return events ^ USERAPP_LIS3DH_INIT_EVT;
  }

  if(events & USERAPP_LIS3DH_COLLECT_EVT)
  {

    return events ^ USERAPP_LIS3DH_COLLECT_EVT;
  }

  if(events & USERAPP_WIRELESS_COMM_EVT)
  {

    return events ^ USERAPP_WIRELESS_COMM_EVT;
  }

  if(events & USERAPP_PHOTO_COLLECT_EVT)
  {
    /*光照信息采集*/
    PhotoDev.open();
    PhotoDev.state = PhotoDev.GetState();

    if(PhotoDev.state == DAY)
    {
      /*白天*/
      LedControl(&PhotoLed.hardLink,SET);
    }
    else if(PhotoDev.state == NIGHT)
    {
      /*晚上*/
      LedControl(&PhotoLed.hardLink,RESET);
    }
    else
    {

    }
    PhotoDev.close();

    system_start_timer(USERAPP_PHOTO_COLLECT_EVT,TIMER_ONCE_MODE,PHOTO_CHECK_LOOP_TIME);
    return events ^ USERAPP_PHOTO_COLLECT_EVT;
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
  if(keys & OnOffKey.keyValue)
  {
    /*开关机按键*/

  }
  else
  {

  }
}

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
