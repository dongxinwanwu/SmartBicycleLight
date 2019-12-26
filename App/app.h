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
#include "debug.h"
#include "lis3dh_driver.h"
#include "light_blink.h"
#include "spi.h"
#include "bk2425.h"
#include "mems_queue.h"
/*****************************************************************************/
/*LIS3DH_FULLSCALE_2 = ±2g = 4g = 4000mg*/
#define MEMS_MULTIPLE        (1000)   /**/
#define MEMS_RATIO           ((int32_t)61) /*MEMS_RATIO = MEMS_MULTIPLE *( 4000 / 65536) */

typedef struct
{
  double acc_x;
  double acc_y;
  double acc_z;
} Accelerate_t;

typedef enum
{
  STOP_BLINK_OFF   = (uint8_t)0x00,
  STOP_BLINK_ON    = (uint8_t)0x01,
}enStopBlinkState;

typedef enum
{
  WIRE_TURN_NONE   = (uint8_t)0x00,
  WIRE_TURN_LEFT   = (uint8_t)0x01,
  WIRE_TURN_RIGHT  = (uint8_t)0x02,
}enWireTurnState;

typedef enum
{
  STOP          = (uint8_t)0x00,
  RUN           = (uint8_t)0x01,
  SPEED_UP      = (uint8_t)0x02,
  SPEED_DOWM    = (uint8_t)0x03,
  UP_HILL       = (uint8_t)0x04,
  DOWN_HILL     = (uint8_t)0x05
}enMEMSState;

typedef struct
{
  uint8_t stopBlink   :1;
  uint8_t turn        :1;
  uint8_t MEMSSensor  :1;
  uint8_t PhotoSensor :1;
}SensorState_t;

typedef struct
{
  SensorState_t     sensorstate;
  enStopBlinkState  stopblinkstate;
  enWireTurnState   turnstate;
  enMEMSState       memsstate;
  enPhotoState      photostate;
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
extern void UserAppHandleKeys(uint8_t keys,UserKeyState_t state,uint8_t clicktimes);
#endif