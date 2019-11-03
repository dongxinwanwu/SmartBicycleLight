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
#ifndef APP_H
#define APP_H

/* ----------------------------------------------------------------------------
 *                                           Includes
 * ----------------------------------------------------------------------------
 */
#include "timer.h"
#include "key.h"
#include "led.h"
#include "adc.h"
#include "pwm.h"
#include "debug.h"
#include "lis3dh_driver.h"
#include "light_blink.h"
/*****************************************************************************/
typedef enum
{
  WIRE_TURN_NONE   = (uint8_t)0x00,
  WIRE_TURN_LEFT   = (uint8_t)0x01,
  WIRE_TURN_RIGHT  = (uint8_t)0x02,
}enWireTurnState;

typedef struct
{
  uint8_t Stop        :1;
  uint8_t SpeedUp     :1;
  uint8_t SpeedDown   :1;
  uint8_t UpHill      :1;
  uint8_t DownHill    :1;
}MEMSState_t;

typedef struct
{
  uint8_t turn        :1;
  uint8_t MEMSSensor  :1;
  uint8_t PhotoSensor :1;
}SensorState_t;

typedef struct
{
  SensorState_t sensorstate;
  enWireTurnState turnstate;
  MEMSState_t memsstate;
  enPhotoState photostate;
}BicycleState_t;

/*设备工作模式*/
typedef enum
{
  NormalMode = (uint8_t)0x01,/*正常模式*/
  HaltMode   = (uint8_t)0x02 /*待机模式（睡眠模式）*/
}DeviceWorkMode;

typedef struct
{
  DeviceWorkMode workMode;
}UserWorkMode_t;
/*****************************************************************************/
extern void System_eventloop_hook(void);
extern uint16_t ProcessSystemTimeEvent(uint16_t event_id);
extern void UserAppHandleKeys(uint8_t keys,UserKeyState_t state);
#endif