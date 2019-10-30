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

/*****************************************************************************/
/*****************************************************************************/
extern void System_eventloop_hook(void);
extern uint16_t ProcessSystemTimeEvent(uint16_t event_id);
extern void UserAppHandleKeys(uint8_t keys,UserKeyState_t state);
#endif