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
#ifndef _LIGHT_BLINK_H
#define _LIGHT_BLINK_H

#include "define.h"
#include "led.h"
/******************************************************************************/

#define LIGHT_LOOP_NUM    10

typedef enum
{
  ALL_OFF       = (uint8_t)0x00,
  ALL_ON        = (uint8_t)0x01,
  TURN_LEFT     = (uint8_t)0x02,
  TURN_RIGHT    = (uint8_t)0x03,
  FAST_BLINK    = (uint8_t)0x04,
  MEDIUM_BLINK  = (uint8_t)0x05,
  SLOW_BLINK    = (uint8_t)0x06
}enLightBlinkMode;

typedef enum
{
  LIGHT_OFF = (uint8_t)0x00,
  LIGHT_ON  = (uint8_t)0x01
}enLightState;

typedef enum
{
  NFINSHED = (uint8_t)0x00,
  FINSHED  = (uint8_t)0x01
}enLightTurnState;

typedef struct
{
  enLightState state[LIGHT_NUM];/*LED1~LED10*/
  enLightState blinkstate;
  enLightBlinkMode mode;
  enLightTurnState turnstate;
  int8_t lightIdx;
  int8_t loopNum;
}LightBlinkCtrl_t;
/******************************************************************************/
extern LightBlinkCtrl_t LightBlinkCtrl;
/******************************************************************************/
void LightBlinkMode(LightBlinkCtrl_t *lightctrl);
/******************************************************************************/

#endif