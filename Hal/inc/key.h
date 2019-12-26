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
#ifndef KEY_H
#define KEY_H

#include "define.h"
/*****************************************************************************/
#define HAL_APP_KEY_NUM                       1
#define HAL_KEY_DEBOUNCE_VALUE                30u     //30ms
#define HAL_KEY_CLICKED_VALUE                 1000u   //1000ms
#define HAL_KEY_HOLD_VALUE                    2000u   //2.0s

/* Buttons */
typedef enum
{
  HAL_KEY_SW_N = (uint8_t)0x00,
  HAL_KEY_SW_1 = (uint8_t)0x01,
  HAL_KEY_SW_2 = (uint8_t)0x02,
  HAL_KEY_SW_3 = (uint8_t)0x04,
  HAL_KEY_SW_4 = (uint8_t)0x08,
}enHal_Key_Value;

typedef enum
{
  NONE_CLICK    = (uint8_t)0x00,
  CLICK         = (uint8_t)0x01,
  START_HOLD    = (uint8_t)0x02,//按键长按开始
  LONG_HOLD     = (uint8_t)0x03,//按键长按
  HOLD_RELEASE  = (uint8_t)0x04 //按键长按结束
}UserKeyState_t;

/*key*/
/*key*/
typedef struct
{
  HardLink_t hardLink;  /*硬件连接*/
  ExitLink_t exitLink;  /*中断连接*/
  enHal_Key_Value keyValue;  //按鍵值
}KeyDeviceControl_t;

typedef struct
{
  KeyDeviceControl_t keyLink;//按键物理连接
  FlagStatus ClickActive;    //按键按下有效
  uint8_t clicktimes;        //连续单击次数
  uint32_t timestamp;        //初始按下时间
  uint16_t ClickValue;       //短按时间阀值
  uint16_t HoldValue;        //长按时间阀值
}UserKeyDetect_t;


/******************************************************************************/
extern UserKeyDetect_t UserAppKey[HAL_APP_KEY_NUM] ;
extern KeyDeviceControl_t  OnOffKey;
/******************************************************************************/
typedef void (*halKeyCBack_t) (uint8_t keys,UserKeyState_t state,uint8_t clicktimes);

void HalKeyInit(KeyDeviceControl_t *keydevice);
void HalKeyCallbackRegister(halKeyCBack_t cback);
void HAL_KEY_Callback(KeyDeviceControl_t *keydevice);
void HalKeyPoll(void);
BitStatus HalKeyRead(KeyDeviceControl_t *keydevice);
void HalKeyClickCheck(void);
void HalKeyLongHoldCheck(void);
#endif