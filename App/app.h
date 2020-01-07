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
#include "state_queue.h"
/*****************************************************************************/
typedef struct
{
  BicycleSensorState_t  bicyclesensor;
  enBicycleState        currentstate;
  enBicycleState        nextstate;
  enBicycleState        blockstate;/*刹车状态，点刹车有效响应2.5S*/
}BicycleState_t;

typedef struct
{
  DeviceWorkMode workMode;
}UserWorkMode_t;
/*****************************************************************************/
extern void System_eventloop_hook(void);
extern uint16_t ProcessSystemTimeEvent(uint16_t event_id);
extern void UserAppHandleKeys(uint8_t keys,UserKeyState_t state,uint8_t clicktimes);
#endif