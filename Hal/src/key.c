/*******************************************************************************
* @file
* @author
* @version
* @date
* @brief
*
*******************************************************************************/
#include "timer.h"
#include "key.h"
/******************************************************************************/
void HAL_KEY_Callback(KeyDeviceControl_t *keydevice);
BitStatus HalKeyRead(KeyDeviceControl_t *keydevice);
void HalKeyPoll (void);
/******************************************************************************/
/*系统开关机按键*/
KeyDeviceControl_t  SysKey =
{
  .hardLink.port                = GPIOB,
  .hardLink.pin                 = GPIO_PIN_3,
  .hardLink.mode                = GPIO_MODE_IN_PU_IT,
  .exitLink.exit_port           = EXTI_PORT_GPIOB,
  .exitLink.exit_Trigger        = EXTI_SENSITIVITY_RISE_FALL,
  .keyValue = HAL_KEY_SW_1
};

halKeyCBack_t pHalKeyProcessFunction;

uint8_t HalKeyCurrentKeys = HAL_KEY_SW_N;

UserKeyDetect_t UserAppKey[HAL_APP_KEY_NUM] =
{
  {
    .keyLink.keyValue = HAL_KEY_SW_N,
    .ClickActive = RESET,
    .timestamp   = 0,
    .ClickValue  = HAL_KEY_CLICKED_VALUE,
    .HoldValue   = HAL_KEY_HOLD_VALUE
  }
};
/*******************************************************************************
* @fn     HalKeyInit
*
* @brief  Hal Key Init
*
* @param
*
* @return
*/
void HalKeyInit(KeyDeviceControl_t *keydevice)
{
  /*key Init*/
  GPIO_Init(keydevice->hardLink.port, keydevice->hardLink.pin, keydevice->hardLink.mode);

  EXTI_SetExtIntSensitivity(keydevice->exitLink.exit_port, keydevice->exitLink.exit_Trigger);


  for(uint8_t i = 0; i < HAL_APP_KEY_NUM; i++)
  {
    if(UserAppKey[i].keyLink.keyValue == HAL_KEY_SW_N)
    {
      UserAppKey[i].keyLink = *keydevice;
    }
  }
}

/*******************************************************************************
* @fn     HalKeyCallbackRegister
*
* @brief  Hal Key Callback Register
*
* @param
*
* @return
*/
void HalKeyCallbackRegister(halKeyCBack_t cback)
{
  /* Register the callback fucntion */
  pHalKeyProcessFunction = cback;
}

/*******************************************************************************
* @fn     HAL_KEY_Callback(void)
*
* @brief  process extint_callback
*
* @param
*
* @return
*/
void HAL_KEY_Callback(KeyDeviceControl_t *keydevice)
{
  HalKeyCurrentKeys |= keydevice->keyValue;

  system_start_timer(USERAPP_KEY_POLL_EVT,TIMER_ONCE_MODE,HAL_KEY_DEBOUNCE_VALUE);
}

/*******************************************************************************
* @fn     HalKeyRead
*
* @brief  HalKeyRead
*
* @param
*
* @return
*/
BitStatus HalKeyRead(KeyDeviceControl_t *keydevice)
{
  BitStatus status = SET;

  /*下升沿，低电平*/
  status = GPIO_ReadInputPin(keydevice->hardLink.port, keydevice->hardLink.pin);

  return status;
}

/*******************************************************************************
* @fn     HalKeyPoll
*
* @brief  HalKeyPoll
*
* @param
*
* @return
*/
void HalKeyPoll(void)
{
  uint8_t keys = 0;
  UserKeyState_t state = NONE_CLICK;
  uint8_t clicktimes = 0;

  for(uint8_t i = 0; i < HAL_APP_KEY_NUM; i++)
  {
    /* Key interrupt debounce handled here */
    if(HalKeyCurrentKeys & UserAppKey[i].keyLink.keyValue)
    {
      /*falling edge*/
      if(HalKeyRead(&UserAppKey[i].keyLink) == RESET)
      {
        if(!UserAppKey[i].ClickActive)
        {
          /*按键首次按下，有效，获取初始时间*/
          /*get currrent time,returns milliseconds,since last reboot*/
          UserAppKey[i].timestamp = GetSystemClock();
          UserAppKey[i].ClickActive = SET;
          UserAppKey[i].clicktimes++;

          /*按键长按检查*/
          system_stop_timer(USERAPP_KEY_CLICK_EVENT);
          system_start_timer (USERAPP_KEY_CLICK_EVENT, TIMER_ONCE_MODE, HAL_KEY_CLICKED_VALUE);
        }
      }
      else if (HalKeyRead(&UserAppKey[i].keyLink) == SET)
      {
        /*rising edge*/
        if(UserAppKey[i].ClickActive)
        {
          HalKeyCurrentKeys &= ~UserAppKey[i].keyLink.keyValue;
          system_stop_timer(USERAPP_KEY_HOLD_EVENT);
          UserAppKey[i].ClickActive = RESET;
          UserAppKey[i].timestamp = 0;
        }
        else
        {


        }
      }
    }
    else
    {

    }
  }

  /* Invoke Callback if new keys were depressed */
  if (keys && (pHalKeyProcessFunction))
  {
    (pHalKeyProcessFunction) (keys,state,clicktimes);
  }
}

/*******************************************************************************
* @fn     HalKeyClickCheck
*
* @brief  HalKeyClickCheck
*
* @param
*
* @return
*/
void HalKeyClickCheck(void)
{
  uint8_t keys = 0x00;
  UserKeyState_t state = NONE_CLICK ;
  uint8_t clicktimes = 0;

  /*check Key state press or release*/
  for(uint8_t i = 0; i < HAL_APP_KEY_NUM; i++)
  {
    //if(UserAppKey[i].ClickActive)
    {
      /*press*/
      if(HalKeyRead(&UserAppKey[i].keyLink) == RESET)
      {
        keys = UserAppKey[i].keyLink.keyValue;
        state = START_HOLD;
        UserAppKey[i].clicktimes = 0;
        /*按键长按检查*/
        system_start_timer (USERAPP_KEY_HOLD_EVENT, TIMER_ONCE_MODE, HAL_KEY_HOLD_VALUE);
      }
      else
      {
        /*release*/
        keys = UserAppKey[i].keyLink.keyValue;
        state = CLICK;
        clicktimes = UserAppKey[i].clicktimes;
        UserAppKey[i].clicktimes = 0;
      }
    }
  }

  /* Invoke Callback if new keys were depressed */
  if(keys && pHalKeyProcessFunction)
  {
    (pHalKeyProcessFunction) (keys,state,clicktimes);
  }
}

/*******************************************************************************
* @fn     HalKeyLongHoldCheck
*
* @brief  HalKeyLongHoldCheck
*
* @param
*
* @return
*/
void HalKeyLongHoldCheck(void)
{
  uint8_t keys = 0x00;
  UserKeyState_t state = NONE_CLICK ;
  uint8_t clicktimes = 0;

  /*check Key state press or release*/
  for(uint8_t i = 0; i < HAL_APP_KEY_NUM; i++)
  {
    //if(UserAppKey[i].ClickActive)
    {
      /*press*/
      if(HalKeyRead(&UserAppKey[i].keyLink) == RESET)
      {
        keys = UserAppKey[i].keyLink.keyValue;
        state = LONG_HOLD;
      }
      else
      {
        /*release*/

      }
    }
  }

  /* Invoke Callback if new keys were depressed */
  if(keys && pHalKeyProcessFunction)
  {
    (pHalKeyProcessFunction) (keys,state,clicktimes);
  }
}