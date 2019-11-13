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
void UserBicycleLightControl(BicycleState_t *state);
void UserKeyChangeWorkMode(UserWorkMode_t *mode);
void UserProcessHaltMode(void);
void UserProcessNormalMode(void);
void UserProcessWirelessData(void);
void UserProcessPhotoData(void);
void UserProcessMemsData(void);
/* Private variables ---------------------------------------------------------*/
BicycleState_t BicycleState =
{
  .sensorstate  = 0,
  .turnstate    = WIRE_TURN_NONE,
  .memsstate    = STOP,
  .photostate   = DAY
};

UserWorkMode_t UserWorkMode =
{
  .workMode   = NormalMode
};
AxesRaw_t LIS3DH_SensorData = {0};

MEMSQueue_t  BicycleMemsDataQueue;/*�������ɼ�����*/
Accelerate_t BicycleAccelerate = {0};/*���մ�������*/

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

    MemsQueue_Init(&BicycleMemsDataQueue);

    uint8_t lisid = 0;
    LIS3DH_GetWHO_AM_I(&lisid);

    if(lisid == LIS3DH_DEFAULT_ID_REG)
    {
      system_start_timer(USERAPP_SENSOR_CHECK_EVT,TIMER_ONCE_MODE,SENSOR_CHECK_LOOP_TIME);
    }
    return events ^ USERAPP_LIS3DH_INIT_EVT;
  }

  if(events & USERAPP_SENSOR_CHECK_EVT)
  {
    /*����״̬���(1)�������ݡ�(2)�������ݡ�(3)MEMS����������*/
    UserProcessWirelessData();
    UserProcessPhotoData();
    UserProcessMemsData();

    /*ת��ƿ���*/
    UserBicycleLightControl(&BicycleState);

    system_start_timer(USERAPP_SENSOR_CHECK_EVT,TIMER_ONCE_MODE,SENSOR_CHECK_LOOP_TIME);
    return events ^ USERAPP_SENSOR_CHECK_EVT;
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
    /*���ػ�����*/
    if(state == LONG_HOLD)
    {
      /*�����������л�����ģʽ*/
      UserKeyChangeWorkMode(&UserWorkMode);
    }
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
  /*�رռ��*/
  system_stop_timer(USERAPP_SENSOR_CHECK_EVT);

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
  /*�������*/
  system_start_timer(USERAPP_SENSOR_CHECK_EVT,TIMER_ONCE_MODE,SENSOR_CHECK_LOOP_TIME);
}

/*******************************************************************************
* @fn   UserProcessWireData
*
* @brief
*
* @param
*
* @return
*/
void UserProcessWirelessData(void)
{
  /*����ת�����ݻ�ȡ*/



}

/*******************************************************************************
* @fn   UserProcessPhotoData
*
* @brief
*
* @param
*
* @return
*/
void UserProcessPhotoData(void)
{
  /*������Ϣ�ɼ�*/
  PhotoDev.open();
  PhotoDev.state = PhotoDev.GetState();
  PhotoDev.process(PhotoDev.state);
  PhotoDev.close();
}

/*******************************************************************************
* @fn   UserProcessMemsData
*
* @brief
*
* @param
*
* @return
*/
void UserProcessMemsData(void)
{
  /*get Acceleration Raw data*/
  status_t response = LIS3DH_GetAccAxesRaw(&LIS3DH_SensorData);
  MemsDataType acc_z_new = 0,acc_z_old = 0,acc_z_sum = 0;

  if(response == MEMS_SUCCESS)
  {
    /*ʵ�ʰ�װ��ˮƽ�ƶ�����ΪZ*/
    acc_z_new = (int32_t)LIS3DH_SensorData.AXIS_Z * MEMS_RATIO;
    /*���ݴ�������ѭ�����У�һֱ��;�����к󣬿�ʼ��,Ȼ��ƽ����*/
    /*push*/
    MemsQueue_push(&BicycleMemsDataQueue,acc_z_new);

    if(MemsQueue_is_full(&BicycleMemsDataQueue))
    {
      /*avg*/
      for(uint8_t i = 0; i < MEMS_QUEUE_NUM; i++)
        acc_z_sum += BicycleMemsDataQueue.data[i] >> MEMS_SHIFT_NUM;

      BicycleAccelerate.acc_z = acc_z_sum;

      /*pop*/
      MemsQueue_pop(&BicycleMemsDataQueue,&acc_z_old);
    }

    if(BicycleAccelerate.acc_z >= 50)
    {
      BicycleState.sensorstate.MEMSSensor = 1;
      BicycleState.memsstate = RUN;
    }
    else
    {
      BicycleState.sensorstate.MEMSSensor = 1;
      BicycleState.memsstate = STOP;
    }
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
      /*����*/
      BicycleState.sensorstate.PhotoSensor = 1;
      BicycleState.photostate = DAY;
    }
    break;

  case NIGHT:
    {
      /*����*/
      BicycleState.sensorstate.PhotoSensor = 1;
      BicycleState.photostate = NIGHT;
    }
    break;

  default:
    break;
  }
}
/*******************************************************************************
* @fn     UserBicycleLightControl
*
* @brief  UserBicycleLightControl
*
* @param
*
* @return
*/
void UserBicycleLightControl(BicycleState_t *state)
{
  if(state->sensorstate.turn == 1)
  {
    state->sensorstate.turn = 0;
    switch(state->turnstate)
    {
    case WIRE_TURN_LEFT:
      if(LightBlinkCtrl.mode != TURN_LEFT)
      {
        LightBlinkCtrl.mode = TURN_LEFT;
        system_set_timer_event(USERAPP_LIGHT_BLINK_EVT);
      }
      break;

    case WIRE_TURN_RIGHT:
      if(LightBlinkCtrl.mode != TURN_RIGHT)
      {
        LightBlinkCtrl.mode = TURN_RIGHT;
        system_set_timer_event(USERAPP_LIGHT_BLINK_EVT);
      }
      break;

    default:
      break;
    }
  }
  else if(state->sensorstate.PhotoSensor == 1)
  {
    state->sensorstate.PhotoSensor = 0;
    switch(state->photostate)
    {
    case DAY:
      {
        if((LightBlinkCtrl.turnstate == FINSHED) && (LightBlinkCtrl.mode != ALL_OFF))
        {
          LightBlinkCtrl.mode = ALL_OFF;
          system_set_timer_event(USERAPP_LIGHT_BLINK_EVT);
        }
      }
      break;

    case NIGHT:
      {
        if(state->sensorstate.MEMSSensor == 1)
        {
          state->sensorstate.MEMSSensor = 0;
          switch(state->memsstate)
          {
          case STOP:
            if((LightBlinkCtrl.turnstate == FINSHED) && (LightBlinkCtrl.mode != SLOW_BLINK))
            {
              LightBlinkCtrl.mode = SLOW_BLINK;
              system_set_timer_event(USERAPP_LIGHT_BLINK_EVT);
            }
            break;

          case RUN:
            if((LightBlinkCtrl.turnstate == FINSHED) && (LightBlinkCtrl.mode != FAST_BLINK))
            {
              LightBlinkCtrl.mode = FAST_BLINK;
              system_set_timer_event(USERAPP_LIGHT_BLINK_EVT);
            }
            break;

          default:
            if(LightBlinkCtrl.mode != ALL_ON)
            {
              LightBlinkCtrl.mode = ALL_ON;
              system_set_timer_event(USERAPP_LIGHT_BLINK_EVT);
            }
            break;
          }
        }
        else
        {
          if(LightBlinkCtrl.mode != ALL_ON)
          {
            LightBlinkCtrl.mode = ALL_ON;
            system_set_timer_event(USERAPP_LIGHT_BLINK_EVT);
          }
        }
      }
      break;

    default:
      break;
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
