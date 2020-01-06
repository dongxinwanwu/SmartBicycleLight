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
  ALL_OFF         = (uint8_t)0x00,
  ALL_ON          = (uint8_t)0x01,
  TURN_LEFT       = (uint8_t)0x02,
  TURN_RIGHT      = (uint8_t)0x03,
  FAST_BLINK      = (uint8_t)0x04,
  MEDIUM_BLINK    = (uint8_t)0x05,
  SLOW_BLINK      = (uint8_t)0x06,
  WAITTING_BLINK  = (uint8_t)0x07
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
}enLightFlashState;

typedef enum
{
  NUM_OFF = (uint8_t)0x00,
  NUM_ON  = (uint8_t)0x01
}enBlinkMode;

typedef struct
{
  enLightState state[LIGHT_NUM];/*LED1~LED10*/
  enLightState blinkstate;
  enLightBlinkMode lightMode;
  enBlinkMode blinkMode;
  enLightFlashState turnstate;
  int8_t lightTurnIdx;/*ת��*/
  int8_t lightRightIdx;
  int8_t lightLeftIdx;
  int8_t loopNum;
  uint16_t blinkNum;
}LightBlinkCtrl_t;
/******************************************************************************/
extern LightBlinkCtrl_t LightBlinkCtrl;
/******************************************************************************/
void LightBlinkMode(LightBlinkCtrl_t *lightctrl);
/******************************************************************************/

#endif