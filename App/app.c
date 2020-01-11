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
void UserAppHandleKeys(uint8_t keys,UserKeyState_t state,uint8_t clicktimes);
void UserBicycleLightControl(BicycleState_t *state);
void UserKeyChangeWorkMode(UserWorkMode_t *mode);
void UserProcessHaltMode(void);
void UserProcessNormalMode(void);
void UserProcessWirelessData(void);
void UserProcessPhotoData(void);
bool UserProcessMemsData(void);
void UserProcessBicycleData(void);
void UserProcessBicycleDayRunState(void);
void UserProcessBicycleNightRunState(void);
void UserProcessBicycleStopState(void);
void UserProcessBicycleBlockedStartState(void);
void UserProcessBicycleBlockedEndState(void);
void UserProcessBicycleWaittingStartState(void);
void UserProcessBicycleWaittingEndState(void);
/* Private variables ---------------------------------------------------------*/
BicycleState_t BicycleState =
{
  .bicyclesensor.sensorstate  = 0,
  .bicyclesensor.urgentblinkstate = URGENT_BLINK_OFF,
  .bicyclesensor.turnstate    = WIRE_TURN_NONE,
  .bicyclesensor.memsstate    = SPEED_STOP,
  .bicyclesensor.photostate   = DAY,
  .currentstate               = BICYCLE_STOP,
  .nextstate                  = BICYCLE_STOP,
};

UserWorkMode_t UserWorkMode =
{
  .workMode   = HaltMode
};
AxesRaw_t LIS3DH_SensorData = {0};

MEMSQueue_t  BicycleMemsDataQueue;/*传感器采集数据*/
Accelerate_t BicycleAccelerate = {0};/*最终处理数据*/
StateQueue_t BicycleStateQueue;/*传感器采集数据*/

bool MemsSensorInitState = FALSE;
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
    StateQueue_Init(&BicycleStateQueue);

    uint8_t lisid = 0;
    LIS3DH_GetWHO_AM_I(&lisid);

    if(lisid == LIS3DH_DEFAULT_ID_REG)
    {
      MemsSensorInitState = TRUE;
    }
    return events ^ USERAPP_LIS3DH_INIT_EVT;
  }

  if(events & USERAPP_SENSOR_CHECK_EVT)
  {
    /*单车状态检查(1)无线数据、(2)光照数据、(3)MEMS传感器数据*/
    UserProcessWirelessData();
    UserProcessPhotoData();
    if(UserProcessMemsData())
    {
      /*自行车状态*/
      UserProcessBicycleData();
      /*转向灯控制*/
      UserBicycleLightControl(&BicycleState);
    }
    system_start_timer(USERAPP_SENSOR_CHECK_EVT,TIMER_ONCE_MODE,SENSOR_CHECK_LOOP_TIME);
    return events ^ USERAPP_SENSOR_CHECK_EVT;
  }

  if(events & USERAPP_LIGHT_BLINK_EVT)
  {
    LightBlinkMode(&LightBlinkCtrl);
    return events ^ USERAPP_LIGHT_BLINK_EVT;
  }

  if(events & USERAPP_SUSPEND_END_EVT)
  {
    /* disenable interrupts */
    disableInterrupts();

    if((BicycleState.currentstate == BICYCLE_BLOCKED_START) &&
       (BicycleState.blockstate != BICYCLE_BLOCKED_ONGOING))
    {
      BicycleState.currentstate = BICYCLE_BLOCKED_END;
    }

    /* enable interrupts */
    enableInterrupts();
    return events ^ USERAPP_SUSPEND_END_EVT;
  }

  if(events & USERAPP_WAITTING_END_EVT)
  {
    /* disenable interrupts */
    disableInterrupts();

    if(BicycleState.currentstate == BICYCLE_WAITTING_START)
      BicycleState.currentstate = BICYCLE_WAITTING_END;

    /* enable interrupts */
    enableInterrupts();
    return events ^ USERAPP_WAITTING_END_EVT;
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
void UserAppHandleKeys(uint8_t keys,UserKeyState_t state,uint8_t clicktimes)
{
  if(keys & SysKey.keyValue)
  {
    if(state == CLICK)
    {
      if(UserWorkMode.workMode == NormalMode)
      {
        if(clicktimes == 1)
        {
          /*关闭紧急模式*/
          if(BicycleState.bicyclesensor.urgentblinkstate == URGENT_BLINK_ON)
          {
            BicycleState.bicyclesensor.urgentblinkstate = URGENT_BLINK_OFF;
          }
          BicycleState.bicyclesensor.sensorstate.urgentBlink = 1;
          UserBicycleLightControl(&BicycleState);
        }
        else if(clicktimes == 2)
        {
          /*开启紧急模式*/
          if(BicycleState.bicyclesensor.urgentblinkstate == URGENT_BLINK_OFF)
          {
            BicycleState.bicyclesensor.urgentblinkstate = URGENT_BLINK_ON;
          }
          BicycleState.bicyclesensor.sensorstate.urgentBlink = 1;
          UserBicycleLightControl(&BicycleState);
        }
      }
    }
    else if(state == LONG_HOLD)
    {
      /*按键长按，开关机*/
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
  /*关闭检测*/
  system_stop_timer(USERAPP_SENSOR_CHECK_EVT);
  /*关闭指示灯*/
  LightBlinkCtrl.blinkMode = NUM_ON;
  LightBlinkCtrl.blinkNum = 2;
  LightBlinkCtrl.lightMode = SLOW_BLINK;
  LightBlinkCtrl.blinkstate = LIGHT_OFF;
  system_set_timer_event(USERAPP_LIGHT_BLINK_EVT);

  /*状态恢复初始化*/
  BicycleState.bicyclesensor.sensorstate.urgentBlink  = 0;
  BicycleState.bicyclesensor.sensorstate.MEMSSensor   = 0;
  BicycleState.bicyclesensor.sensorstate.PhotoSensor  = 0;
  BicycleState.bicyclesensor.sensorstate.turn         = 0;
  BicycleState.bicyclesensor.urgentblinkstate         = URGENT_BLINK_OFF;
  BicycleState.bicyclesensor.turnstate                = WIRE_TURN_NONE;
  BicycleState.bicyclesensor.memsstate                = SPEED_STOP;
  BicycleState.bicyclesensor.photostate               = DAY;
  BicycleState.currentstate                           = BICYCLE_STOP;
  BicycleState.nextstate                              = BICYCLE_STOP;
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
  /*开机指示*/
  /*关闭指示灯*/
  LightBlinkCtrl.blinkMode = NUM_ON;
  LightBlinkCtrl.blinkNum = 4;
  LightBlinkCtrl.lightMode = SLOW_BLINK;
  LightBlinkCtrl.blinkstate = LIGHT_OFF;
  system_set_timer_event(USERAPP_LIGHT_BLINK_EVT);

  if(MemsSensorInitState == TRUE)
  {
    /*开启检测*/
    system_start_timer(USERAPP_SENSOR_CHECK_EVT,TIMER_ONCE_MODE,8000);
  }
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
  /*无线转向数据获取*/



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
  /*光照信息采集*/
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
bool UserProcessMemsData(void)
{
  /*get Acceleration Raw data*/
  status_t response = LIS3DH_GetAccAxesRaw(&LIS3DH_SensorData);
  MemsDataType acc_z_new = 0,acc_z_old = 0,acc_z_sum = 0;

  if(response == MEMS_SUCCESS)
  {
    /*加速度计算：角度，加速度大小*/
    acc_z_new = (MemsDataType)LIS3DH_SensorData.AXIS_Z;
    /*2020.01.08:数据处理方案：连续的几个数据取平均数*/
    /*push*/
    MemsQueue_push(&BicycleMemsDataQueue,acc_z_new);

    if(MemsQueue_is_full(&BicycleMemsDataQueue))
    {
      /*avg*/
      for(uint8_t i = 0; i < MEMS_QUEUE_NUM; i++)
        acc_z_sum += BicycleMemsDataQueue.data[i] >> MEMS_SHIFT_NUM;

      BicycleAccelerate.acc_z = acc_z_sum;

      /*pop*/
      for(uint8_t i = 0; i < MEMS_QUEUE_NUM; i++)
        MemsQueue_pop(&BicycleMemsDataQueue,&acc_z_old);

      if(BicycleAccelerate.acc_z >= 3500)
      {
        BicycleState.bicyclesensor.sensorstate.MEMSSensor = 1;
        BicycleState.bicyclesensor.memsstate = SPEED_DOWM;
      }
      else if(BicycleAccelerate.acc_z >= 500)
      {
        BicycleState.bicyclesensor.sensorstate.MEMSSensor = 1;
        BicycleState.bicyclesensor.memsstate = SPEED_UNIFORM;
      }
      else if(BicycleAccelerate.acc_z < 500)
      {
        BicycleState.bicyclesensor.sensorstate.MEMSSensor = 1;
        BicycleState.bicyclesensor.memsstate = SPEED_UP;
      }

      return TRUE;
    }

    return FALSE;
  }
  return FALSE;
}

/*******************************************************************************
* @fn     UserProcessBicycleData
*
* @brief  UserProcessBicycleData
*
* @param
*
* @return
*/
void UserProcessBicycleData(void)
{
  /*数据处理方案：循环队列，一直进;满队列后，开始出*/
  StateDataType memsState = SPEED_STOP;
  uint8_t runNum = 0;
  uint8_t stopNum = 0;
  uint8_t uniformNum = 0;

  if(BicycleState.bicyclesensor.sensorstate.MEMSSensor == 1)
  {
    StateQueue_push(&BicycleStateQueue,BicycleState.bicyclesensor.memsstate);

    if(StateQueue_is_full(&BicycleStateQueue))
    {
      /*根据队列中传感器状态，得出自行车状态*/
      for(uint8_t i = 0; i < STATE_QUEUE_NUM; i++)
      {
        if(BicycleStateQueue.data[i] == SPEED_UP)
          runNum++;
        else if(BicycleStateQueue.data[i] == SPEED_DOWM)
          stopNum++;
        else
          uniformNum++;
      }
      /*队列长度：12*/
      if(stopNum >= 10)
      {
        /*刹车等待状态*/
        if(BicycleState.currentstate == BICYCLE_BLOCKED_START)
        {
          BicycleState.blockstate = BICYCLE_BLOCKED_ONGOING;
        }
        else
        {

        }
      }
      else if(stopNum >= 4)
      {
        if(BicycleState.currentstate == BICYCLE_RUN)
          BicycleState.nextstate = BICYCLE_BLOCKED_START;
      }
      else if(runNum >= 10)
      {
        if((BicycleState.currentstate == BICYCLE_BLOCKED_START) &&
           (BicycleState.blockstate == BICYCLE_BLOCKED_ONGOING))
        {
          BicycleState.nextstate = BICYCLE_RUN;
          BicycleState.blockstate = BICYCLE_BLOCKED_END;
        }
        if(BicycleState.currentstate != BICYCLE_BLOCKED_START)
          BicycleState.nextstate = BICYCLE_RUN;
      }
      else if(uniformNum >= 10)
      {
        if((BicycleState.currentstate == BICYCLE_BLOCKED_START) &&
           (BicycleState.blockstate == BICYCLE_BLOCKED_ONGOING))
        {
          BicycleState.nextstate = BICYCLE_BLOCKED_END;
          BicycleState.blockstate = BICYCLE_BLOCKED_END;
        }
        else if(BicycleState.currentstate == BICYCLE_BLOCKED_END)
          BicycleState.nextstate = BICYCLE_WAITTING_START;
        else if(BicycleState.currentstate == BICYCLE_WAITTING_START)
        {

        }
        else if(BicycleState.currentstate == BICYCLE_WAITTING_END)
        {
          BicycleState.nextstate = BICYCLE_STOP;
        }
      }
      else
      {


      }

      /*pop*/
      StateQueue_pop(&BicycleStateQueue,&memsState);
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
      /*白天*/
      BicycleState.bicyclesensor.sensorstate.PhotoSensor = 1;
      BicycleState.bicyclesensor.photostate = DAY;
    }
    break;

  case NIGHT:
    {
      /*晚上*/
      BicycleState.bicyclesensor.sensorstate.PhotoSensor = 1;
      BicycleState.bicyclesensor.photostate = NIGHT;
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
  /*指示灯控制：
  （1）无线转向灯
  （2）光照
  （3）紧急
  （4）运动传感器*/
  if(state->bicyclesensor.sensorstate.turn == 1)
  {
    state->bicyclesensor.sensorstate.turn = 0;
    switch(state->bicyclesensor.turnstate)
    {
    case WIRE_TURN_LEFT:
      if(LightBlinkCtrl.lightMode != TURN_LEFT)
      {
        LightBlinkCtrl.lightMode = TURN_LEFT;
        system_set_timer_event(USERAPP_LIGHT_BLINK_EVT);
      }
      break;

    case WIRE_TURN_RIGHT:
      if(LightBlinkCtrl.lightMode != TURN_RIGHT)
      {
        LightBlinkCtrl.lightMode = TURN_RIGHT;
        system_set_timer_event(USERAPP_LIGHT_BLINK_EVT);
      }
      break;

    default:
      break;
    }
  }
  else if(state->bicyclesensor.sensorstate.PhotoSensor == 1)
  {
    state->bicyclesensor.sensorstate.PhotoSensor = 0;
    switch(state->bicyclesensor.photostate)
    {
    case DAY:
      {
        /*白天停车闪灯*/
        if(state->bicyclesensor.sensorstate.MEMSSensor == 1)
        {
          state->bicyclesensor.sensorstate.MEMSSensor = 0;
          if(state->currentstate != state->nextstate)
          {
            state->currentstate = state->nextstate;
            switch(state->currentstate)
            {
            case BICYCLE_STOP:
              UserProcessBicycleStopState();
              break;

            case BICYCLE_RUN:
              UserProcessBicycleDayRunState();
              break;

            case BICYCLE_BLOCKED_START:
              UserProcessBicycleBlockedStartState();
              break;

            case BICYCLE_BLOCKED_END:
              UserProcessBicycleBlockedEndState();
              break;

            case BICYCLE_WAITTING_START:
              UserProcessBicycleWaittingStartState();
              break;

            case BICYCLE_WAITTING_END:
              UserProcessBicycleWaittingEndState();
              break;
            default:
              break;
            }
          }
        }
      }
      break;

    case NIGHT:
      {
        if(state->bicyclesensor.sensorstate.urgentBlink == 1)
        {
          if(state->bicyclesensor.urgentblinkstate == URGENT_BLINK_ON)
          {
            if(LightBlinkCtrl.lightMode != FAST_BLINK)
            {
              LightBlinkCtrl.lightMode = FAST_BLINK;
              LightBlinkCtrl.blinkMode = NUM_OFF;
              LightBlinkCtrl.blinkstate = LIGHT_OFF;
              system_set_timer_event(USERAPP_LIGHT_BLINK_EVT);
            }
          }
          else
          {
            state->bicyclesensor.sensorstate.urgentBlink = 0;
            LightBlinkCtrl.lightMode = ALL_OFF;
            system_set_timer_event(USERAPP_LIGHT_BLINK_EVT);
          }
        }
        else if(state->bicyclesensor.sensorstate.MEMSSensor == 1)
        {
          state->bicyclesensor.sensorstate.MEMSSensor = 0;
          if(state->currentstate != state->nextstate)
          {
            state->currentstate = state->nextstate;

            switch(state->currentstate)
            {
            case BICYCLE_STOP:
              UserProcessBicycleStopState();
              break;

            case BICYCLE_RUN:
              UserProcessBicycleNightRunState();
              break;

            case BICYCLE_BLOCKED_START:
              UserProcessBicycleBlockedStartState();
              break;

            case BICYCLE_BLOCKED_END:
              UserProcessBicycleBlockedEndState();
              break;

            case BICYCLE_WAITTING_START:
              UserProcessBicycleWaittingStartState();
              break;

            case BICYCLE_WAITTING_END:
              UserProcessBicycleWaittingEndState();
              break;

            default:
              break;
            }
          }
        }
        else
        {

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

/*******************************************************************************
* @fn     UserProcessBicycleDayRunState
*
* @brief  UserProcessBicycleDayRunState
*
* @param
*
* @return
*/
void UserProcessBicycleDayRunState(void)
{
  if(LightBlinkCtrl.turnstate == FINSHED)
  {
    LightBlinkCtrl.lightMode = ALL_OFF;
    system_set_timer_event(USERAPP_LIGHT_BLINK_EVT);
  }
}

/*******************************************************************************
* @fn     UserProcessBicycleNightRunState
*
* @brief  UserProcessBicycleNightRunState
*
* @param
*
* @return
*/
void UserProcessBicycleNightRunState(void)
{
  if(LightBlinkCtrl.turnstate == FINSHED)
  {
    LightBlinkCtrl.lightMode = MEDIUM_BLINK;
    LightBlinkCtrl.blinkMode = NUM_OFF;
    LightBlinkCtrl.blinkstate = LIGHT_OFF;
    system_set_timer_event(USERAPP_LIGHT_BLINK_EVT);
  }
}

/*******************************************************************************
* @fn     UserProcessBicycleStopState
*
* @brief  UserProcessBicycleStopState
*
* @param
*
* @return
*/
void UserProcessBicycleStopState(void)
{
  if(LightBlinkCtrl.turnstate == FINSHED)
  {
    LightBlinkCtrl.lightMode = ALL_OFF;
    system_set_timer_event(USERAPP_LIGHT_BLINK_EVT);
  }
}

/*******************************************************************************
* @fn     UserProcessBicycleBlockedStartState
*
* @brief  UserProcessBicycleBlockedStartState
*
* @param
*
* @return
*/
void UserProcessBicycleBlockedStartState(void)
{
  if(LightBlinkCtrl.turnstate == FINSHED)
  {
    LightBlinkCtrl.lightMode = ALL_ON;
    system_set_timer_event(USERAPP_LIGHT_BLINK_EVT);
    system_start_timer(USERAPP_SUSPEND_END_EVT,TIMER_ONCE_MODE,LIGHT_BLOCKED_LIGHT_OFF_TIME);
  }
}

/*******************************************************************************
* @fn     UserProcessBicycleBlockedEndState
*
* @brief  UserProcessBicycleBlockedEndState
*
* @param
*
* @return
*/
void UserProcessBicycleBlockedEndState(void)
{
  LightBlinkCtrl.lightMode = ALL_OFF;
  LightBlinkMode(&LightBlinkCtrl);
  system_set_timer_event(USERAPP_LIGHT_BLINK_EVT);
}

/*******************************************************************************
* @fn     UserProcessBicycleWaittingStartState
*
* @brief  UserProcessBicycleWaittingStartState
*
* @param
*
* @return
*/
void UserProcessBicycleWaittingStartState(void)
{
  if(LightBlinkCtrl.turnstate == FINSHED)
  {
    LightBlinkCtrl.lightMode = WAITTING_BLINK;
    LightBlinkCtrl.blinkMode = NUM_OFF;
    LightBlinkCtrl.blinkstate = LIGHT_OFF;
    LightBlinkCtrl.lightRightIdx = RIGHT_LIGHT_START_IDX;
    LightBlinkCtrl.lightLeftIdx = LEFT_LIGHT_START_IDX;
    system_set_timer_event(USERAPP_LIGHT_BLINK_EVT);
    system_start_timer(USERAPP_WAITTING_END_EVT,TIMER_ONCE_MODE,LIGHT_BLINK_OFF_TIME);
  }
}

/*******************************************************************************
* @fn     UserProcessBicycleWaittingEndState
*
* @brief  UserProcessBicycleWaittingEndState
*
* @param
*
* @return
*/
void UserProcessBicycleWaittingEndState(void)
{
  LightBlinkCtrl.lightMode = ALL_OFF;
  LightBlinkMode(&LightBlinkCtrl);
  system_set_timer_event(USERAPP_LIGHT_BLINK_EVT);
}
/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
