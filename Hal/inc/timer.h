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
#define TIME_HARD_TICK                        10u
#define TIME_SOFT_TICK                        10u
#define RELOAD_WDT_TIME                       200u
#define START_LOOP_CHECK_TIME                 50u
#define LOOP_RBG_PERIOD_TIME                  300u
#define HAL_NFC_COMM_TIME                     1500u
#define HAL_BAT_CHECK_TIME                    600000u
#define HAL_ENCODER_DEBOUNCE_TIME             50u
#define NFC_DIMMER_BLINK_TIME                 600u
#define NFC_COMM_FORBID_TIME                  1000u
#define NFC_ENCODE_FORBID_TIME                3000u
#define NFC_PCD_MODE_TIMEOUT                  120000u
#define HAL_LED_BLINK_TIME                    150u

#define MAX_TIME_NUM                          12u
#define USERAPP_RELOAD_WDT_EVT                0X0001
#define USERAPP_KEY_POLL_EVT                  0x0002
#define USERAPP_KEY_CLICK_EVENT               0x0004
#define USERAPP_KEY_HOLD_EVENT                0x0008
#define USERAPP_COLLECT_COLOR_EVT             0X0010
#define USERAPP_HAL_ENCODE_EVT                0X0020
#define USERAPP_HAL_ENCODE_DEBOUNCE_EVT       0X0040
#define USERAPP_BAT_CHECK_EVT                 0X0080
#define USERAPP_MIMMER_FLASH_EVT              0X0100
#define USERAPP_NFC_FORBID_EVT                0X0200
#define USERAPP_NFC_PCD_TIMEOUT_EVT           0X0400
#define USERAPP_LED_BLINK_EVT                 0X0800
/******************************************************************************/
typedef enum
{
  TIMER_ONCE_MODE = 0,	/*һ��ģʽ*/
  TIMER_AUTO_MODE = 1   /*�ظ�ģʽ*/
}TimerMode_t;

typedef struct
{
  volatile uint16_t timerid;
  volatile bool     isuse;//�Ƿ�ʹ��״̬
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