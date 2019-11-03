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
void UserBicycleControl(BicycleState_t *state);
void UserKeyChangeWorkMode(UserWorkMode_t *mode);
void UserProcessHaltMode(void);
void UserProcessNormalMode(void);
/* Private variables ---------------------------------------------------------*/
BicycleState_t BicycleState =
{
  .sensorstate  = 0,
  .turnstate    = WIRE_TURN_NONE,
  .memsstate    = 0,
  .photostate   = DAY
};

UserWorkMode_t UserWorkMode =
{
  .workMode   = NormalMode
};

AxesRaw_t LIS3DH_SensorData = {0};
uint8_t lisid;
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

    LIS3DH_GetWHO_AM_I(&lisid);

    system_start_timer(USERAPP_LIS3DH_COLLECT_EVT,TIMER_AUTO_MODE,LIS3DH_CHECK_LOOP_TIME);
    return events ^ USERAPP_LIS3DH_INIT_EVT;
  }

  if(events & USERAPP_LIS3DH_COLLECT_EVT)
  {
    //get Acceleration Raw data
    status_t response = LIS3DH_GetAccAxesRaw(&LIS3DH_SensorData);

    if(response == MEMS_SUCCESS)
    {
      LedControl(&PhotoLed.hardLink,RESET);
    }
    else
    {
      LedControl(&PhotoLed.hardLink,SET);
    }
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
    PhotoDev.process(PhotoDev.state);
    PhotoDev.close();

    system_start_timer(USERAPP_PHOTO_COLLECT_EVT,TIMER_ONCE_MODE,PHOTO_CHECK_LOOP_TIME);
    return events ^ USERAPP_PHOTO_COLLECT_EVT;
  }

  if(events & USERAPP_LIGHT_BLINK_EVT)
  {
    LightBlinkMode(&LightBlinkCtrl);
    return events ^ USERAPP_LIGHT_BLINK_EVT;
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
    if(state == LONG_HOLD)
    {
      /*按键长按，切换工作模式*/
      UserKeyChangeWorkMode(&UserWorkMode);
    }
  }
  else
  {

  }
}

/*******************************************************************************
* @fn     PhotoState_Process
*
* @brief  PhotoState_Process
*
* @param
*
* @return
*/
void PhotoState_Process(enPhotoState state)
{
  switch(state)
  {
  case DAY:
    {
      /*白天*/
      LedControl(&PhotoLed.hardLink,SET);

      BicycleState.sensorstate.PhotoSensor = 1;
      BicycleState.photostate = DAY;
      UserBicycleControl(&BicycleState);
    }
    break;

  case NIGHT:
    {
      /*晚上*/
      LedControl(&PhotoLed.hardLink,RESET);

      BicycleState.sensorstate.PhotoSensor = 1;
      BicycleState.photostate = NIGHT;
      UserBicycleControl(&BicycleState);
    }
    break;

  default:
    break;
  }
}

/*******************************************************************************
* @fn     UserBicycleControl
*
* @brief  UserBicycleControl
*
* @param
*
* @return
*/
void UserBicycleControl(BicycleState_t *state)
{
  if(state->sensorstate.turn == 1)
  {


  }
  else if(state->sensorstate.MEMSSensor == 1)
  {


  }
  else if(state->sensorstate.PhotoSensor == 1)
  {


  }
  else
  {

  }
}


/*******************************************************************************
* @fn     UserKeyChangeWorkMode
*
* @brief
*
* @param
*
* @return
*/
void UserKeyChangeWorkMode(UserWorkMode_t *mode)
{
  switch(mode->workMode)
  {
  case NormalMode:
    {
      mode->workMode = HaltMode;
      /*Halt mode*/
      UserProcessHaltMode();
    }
    break;
  case HaltMode:
    {
      mode->workMode = NormalMode;
      /*Normal mode*/
      UserProcessNormalMode();
    }
    break;
  default:
    break;
  }
}

/*******************************************************************************
* @fn   UserProcessHaltMode
*
* @brief
*
* @param
*
* @return
*/
void UserProcessHaltMode(void)
{
  /*关闭检测*/
  system_stop_timer(USERAPP_PHOTO_COLLECT_EVT);
  system_stop_timer(USERAPP_WIRELESS_COMM_EVT);
  system_stop_timer(USERAPP_LIS3DH_COLLECT_EVT);

}

/*******************************************************************************
* @fn   UserProcessNormalMode
*
* @brief
*
* @param
*
* @return
*/
void UserProcessNormalMode(void)
{
  /*开启检测*/
  system_start_timer(USERAPP_PHOTO_COLLECT_EVT,TIMER_ONCE_MODE,PHOTO_CHECK_LOOP_TIME);
  system_start_timer(USERAPP_WIRELESS_COMM_EVT,TIMER_AUTO_MODE,WIRELESS_COMM_CHECK_LOOP_TIME);
  system_start_timer(USERAPP_LIS3DH_COLLECT_EVT,TIMER_AUTO_MODE,LIS3DH_CHECK_LOOP_TIME);
}
/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
