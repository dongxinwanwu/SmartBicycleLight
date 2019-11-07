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
#ifndef TIMER_H
#define TIMER_H

#include "define.h"

#ifndef NULL
  #define NULL          ((void *) 0)
#endif
/******************************************************************************/
#define TIME_MAX_PERIOD                       3600000u/*1h = 1000ms * 60 * 60*/
#define TIME_HARD_TICK                        1u
#define TIME_SOFT_TICK                        5u
#define RELOAD_WDT_TIME                       200u
#define LIS3DH_DELAY_INIT_TIME                20u
#define SENSOR_CHECK_LOOP_TIME                200u
#define LIGHT_FLASH_BLINK_TIME                300u
#define LIGHT_FAST_BLINK_TIME                 600u
#define LIGHT_SLOW_BLINK_TIME                 1000u

#define MAX_TIME_NUM                          7u
#define USERAPP_RELOAD_WDT_EVT                0X0001
#define USERAPP_KEY_POLL_EVT                  0x0002
#define USERAPP_KEY_CLICK_EVENT               0x0004
#define USERAPP_KEY_HOLD_EVENT                0x0008
#define USERAPP_LIS3DH_INIT_EVT               0X0010
#define USERAPP_SENSOR_CHECK_EVT              0X0020
#define USERAPP_LIGHT_BLINK_EVT               0X0040

/******************************************************************************/
typedef enum
{
  TIMER_ONCE_MODE = 0,	/*一次模式*/
  TIMER_AUTO_MODE = 1   /*重复模式*/
}TimerMode_t;

typedef struct
{
  volatile uint16_t timerid;
  volatile bool     isuse;//是否使用状态
  volatile uint8_t timerflag;
  volatile TimerMode_t timermode;
  volatile uint32_t preLoad;
  volatile uint32_t timeout;
}SystemTimer_t;

typedef uint16_t (*pTimerEventHandlerFn)(uint16_t event_id);
/******************************************************************************/
/**
 * Extern Variables
 */
extern SystemTimer_t systemTimer[MAX_TIME_NUM];

extern void SystemTimer_Config(void);

extern void DelayMs(__IO uint32_t nTime);

extern void UserTimingDelay(uint16_t delay);

extern void TimingDelay_Decrement(void);

/*! \brief Get System Clock
 *
 */
extern uint32_t GetSystemClock( void );
/******************************************************************************/
/*! \brief Initialize timer
 *
 */
extern void system_timer_init( pTimerEventHandlerFn  handlerfn);

/*! \brief Update System Timer
 *
 */
extern void UpdateSystemTimer(SystemTimer_t *timer);

/*! \brief System_timer_hook
 *
 */
void System_timer_hook(void);

/*! \brief start timer
 *
 */
extern uint8_t system_start_timer(uint16_t timer_id,TimerMode_t timer_mode,uint32_t timeout_value);

/*! \brief stop timer
 *
 */
extern uint8_t system_stop_timer(uint16_t timer_id);

/*! \Set a Timer Task Event
 *
 */
extern uint8_t system_set_timer_event(uint16_t timer_id);

/*! \ Clear a Timer Task Event
 *
 */
extern uint8_t system_clear_timer_event(uint16_t timer_id);

extern uint8_t GetTimerIDFlag(uint16_t timer_id);

extern bool get_timer_isuse(uint16_t timer_id);

extern uint8_t System_timer_run(void);
#endif