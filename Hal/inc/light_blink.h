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

/******************************************************************************/
#define LIGHT_NUM         4
#define LIGHT_LOOP_NUM    4

typedef enum
{
  ALL_OFF     = (uint8_t)0x00,
  ALL_ON      = (uint8_t)0x01,
  TURN_LEFT   = (uint8_t)0x02,
  TURN_RIGHT  = (uint8_t)0x03,
  FAST_BLINK  = (uint8_t)0x04,
  SLOW_BLINK  = (uint8_t)0x05
}enLightBlinkMode;

typedef enum
{
  LIGHT_OFF = (uint8_t)0x00,
  LIGHT_ON  = (uint8_t)0x01
}enLightState;

typedef enum
{
  LOOP_MODE_NONE  = (uint8_t)0x00,
  LEFT_LIGHT1_ON  = (uint8_t)0x01,
  LEFT_LIGHT2_ON  = (uint8_t)0x02,
  RIGHT_LIGHT1_ON = (uint8_t)0x03,
  RIGHT_LIGHT2_ON = (uint8_t)0x04
}enLightLoopMode;

typedef struct
{
  enLightState state[LIGHT_NUM];/*L1 L2 R2 R1*/
  enLightState blinkstate;
  enLightLoopMode loopmode;
  enLightBlinkMode mode;
  uint8_t loopNum;
}LightBlinkCtrl_t;
/******************************************************************************/
extern LightBlinkCtrl_t LightBlinkCtrl;
/******************************************************************************/
void LightBlinkMode(LightBlinkCtrl_t *lightctrl);
/******************************************************************************/

#endif