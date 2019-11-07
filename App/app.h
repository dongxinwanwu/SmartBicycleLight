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
/*****************************************************************************/
/*LIS3DH_FULLSCALE_4 = ±4g = 8g = 8000mg*/
#define MEMS_RATIO           (0.122) /*MEMS_RATIO = ( 8000 / 65536) */

typedef struct
{
  double acc_x;
  double acc_y;
  double acc_z;
} Accelerate_t;

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
  uint8_t turn        :1;
  uint8_t MEMSSensor  :1;
  uint8_t PhotoSensor :1;
}SensorState_t;

typedef struct
{
  SensorState_t   sensorstate;
  enWireTurnState turnstate;
  enMEMSState     memsstate;
  enPhotoState    photostate;
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