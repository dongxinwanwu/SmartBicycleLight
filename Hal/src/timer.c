/*******************************************************************************
* @file
* @author
* @version
* @date
* @brief
*
*******************************************************************************/
#include "timer.h"

/******************************************************************************/
static inline uint8_t ffs(uint16_t data);
/******************************************************************************/
/**
 * Variables
 */
SystemTimer_t systemTimer[MAX_TIME_NUM];

__IO uint32_t TimingDelay = 0;

// Milliseconds since last reboot
static uint32_t systemClock = 0;

pTimerEventHandlerFn pTimerEventHandler = NULL;

uint16_t SysEvent = 0;
/*******************************************************************************
* @fn
*
* @brief
*
* @param
*
* @return
*/
void SystemTimer_Config(void)
{
  uint16_t timer_period = 0;
  /* Enable TIM4 CLK */
  CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER2, ENABLE);

  /**/
  timer_period = TIME_HARD_TICK * 125 - 1;

  /* Time base configuration */
  TIM2_TimeBaseInit(TIM2_PRESCALER_128,timer_period);

  /* Clear TIM2 update flag */
  TIM2_ClearFlag(TIM2_FLAG_UPDATE);

  /* Enable update interrupt */
  TIM2_ITConfig(TIM2_IT_UPDATE, ENABLE);

  /* Enable TIM2 */
  TIM2_Cmd(ENABLE);
}

/*******************************************************************************
* @fn
*
* @brief
*
* @param
*
* @return
*/
void DelayMs(__IO uint32_t nTime)
{
  TimingDelay = nTime;

  while (TimingDelay != 0);
}

/*******************************************************************************
* @fn
*
* @brief
*
* @param
*
* @return
*/
void UserTimingDelay(uint16_t delay)
{
  while(delay--)
  {
    nop();nop();nop();nop();nop();
    nop();nop();nop();nop();nop();
    nop();nop();nop();nop();nop();
    nop();nop();nop();nop();nop();
  }
}
/*******************************************************************************
* @fn
*
* @brief
*
* @param
*
* @return
*/
void TimingDelay_Decrement(void)
{
  static uint8_t timecount = 0;
  if (TimingDelay != 0x00)
  {
    TimingDelay--;
  }

  /*系统时钟更新*/
  systemClock = systemClock + TIME_HARD_TICK;
  if(systemClock >= 0xffffffff)
    systemClock = 0;

  /*用户自定义定时功能*/
  /*2018.11.13：由于软件定时器中存在临界区的应用，频繁关闭中断，对系统中断存在大的影响
  因此减少用户软件定时器更新的次数*/
  timecount = timecount + TIME_HARD_TICK;
  if(timecount >= TIME_SOFT_TICK)
  {
    timecount = 0;
    /*update user timer enent*/
    UpdateSystemTimer(systemTimer);
  }

  System_timer_hook();
}

/*******************************************************************************
* @fn
*
* @brief
*
* @param
*
* @return
*/
void system_timer_init( pTimerEventHandlerFn  handlerfn)
{
  /*init user timer*/
  for(uint8_t i = 0; i < MAX_TIME_NUM; i++)
  {
    systemTimer[i].timerid   = 0u;
    systemTimer[i].isuse     = FALSE;
    systemTimer[i].timerflag = 0u;
    systemTimer[i].timermode = TIMER_ONCE_MODE;
    systemTimer[i].preLoad   = 0u;
    systemTimer[i].timeout   = 0u;
  }
  /*register*/
  pTimerEventHandler = handlerfn;

  SysEvent = 0;

  systemClock = 0;
}

/*******************************************************************************
* @fn    Set a Timer
*
* @brief
*
* @param
*
* @return
*/
void UpdateSystemTimer(SystemTimer_t *timer)
{
  /* disenable interrupts */
  disableInterrupts();

  for(uint8_t i = 0; i < MAX_TIME_NUM; i++)
  {
    if(timer->timeout > 0)
    {
      timer->timeout = timer->timeout - TIME_SOFT_TICK;
      if(0 == timer->timeout)
      {
        /*set event flag*/
        timer->timerflag = 1;

        if(timer->timermode == TIMER_AUTO_MODE)
        {
          timer->timeout = timer->preLoad;
        }
      }
    }
    *timer++;
  }
  /* enable interrupts */
  enableInterrupts();
}

/*********************************************************************
 * @fn      GetSystemClock()
 *
 * @brief   Read the local system clock.
 *
 * @param   none
 *
 * @return  local clock in milliseconds
 */
uint32_t GetSystemClock( void )
{
  return ( systemClock );
}

/*******************************************************************************
* @fn    Set a Timer
*
* @brief
*
* @param
*
* @return
*/
uint8_t system_start_timer(uint16_t timer_id,TimerMode_t timer_mode,uint32_t timeout_value)
{
  uint8_t index = 0;

  index = ffs(timer_id);

  if(index >= MAX_TIME_NUM)
  {
    return 0;
  }
  if(timeout_value >= TIME_MAX_PERIOD)
  {
    return 0;
  }
  /* disenable interrupts */
  disableInterrupts();

  systemTimer[index].timerid = timer_id;
  systemTimer[index].isuse = TRUE;
  systemTimer[index].timerflag = 0;
  systemTimer[index].timermode = timer_mode;
  systemTimer[index].preLoad = timeout_value;
  systemTimer[index].timeout = timeout_value;

  /* enable interrupts */
  enableInterrupts();

  return 1;
}

/*******************************************************************************
* @fn    Stop a Timer
*
* @brief
*
* @param
*
* @return
*/
uint8_t system_stop_timer(uint16_t timer_id)
{
  uint8_t index = 0;

  index = ffs(timer_id);

  if(index >= MAX_TIME_NUM)
  {
    return 0;
  }
  /* disenable interrupts */
  disableInterrupts();

  systemTimer[index].isuse = FALSE;
  systemTimer[index].timerflag = 0;
  systemTimer[index].timermode = TIMER_ONCE_MODE;
  systemTimer[index].preLoad = 0;
  systemTimer[index].timeout = 0;

  SysEvent &= ~timer_id;

  /* enable interrupts */
  enableInterrupts();

  return 1;
}

/*******************************************************************************
* @fn    Set a Timer Task Event
*
* @brief
*
* @param
*
* @return
*/
uint8_t system_set_timer_event(uint16_t timer_id)
{
  uint8_t index = 0;

  index = ffs(timer_id);

  if(index >= MAX_TIME_NUM)
  {
    return 0;
  }

  /* disenable interrupts */
  disableInterrupts();

  /*如果该定时器已经开启，但是时间没有到达，此处设置定时器，并且时间清零*/
  systemTimer[index].timerid = timer_id;
  systemTimer[index].timeout = 0;
  systemTimer[index].timerflag = 1;
  systemTimer[index].isuse = TRUE;
  /* enable interrupts */
  enableInterrupts();

  return 1;
}

/*******************************************************************************
* @fn    Clear a Timer Task Event
*
* @brief
*
* @param
*
* @return
*/
uint8_t system_clear_timer_event(uint16_t timer_id)
{
  uint8_t index = 0;

  index = ffs(timer_id);

  if(index >= MAX_TIME_NUM)
  {
    return 0;
  }

  /* disenable interrupts */
  disableInterrupts();

  /*如果该定时器已经开启，但是时间没有到达，此处清除定时器，并且时间清零*/
  systemTimer[index].timeout = 0;
  systemTimer[index].timerflag = 0;
  systemTimer[index].isuse = FALSE;

  SysEvent &= ~timer_id;

  /* enable interrupts */
  enableInterrupts();

  return 1;
}

/*******************************************************************************
* @fn     GetTimerIDFlag
*
* @brief
*
* @param
*
* @return
*/
uint8_t GetTimerIDFlag(uint16_t timer_id)
{
  uint8_t index = 0;
  uint8_t rtn = 0;

  index = ffs(timer_id);

  if(index >= MAX_TIME_NUM)
  {
    return 0;
  }
  /* disenable interrupts */
  disableInterrupts();

  if(systemTimer[index].timerflag)
  {
    systemTimer[index].timerflag = 0;
    systemTimer[index].isuse = FALSE;

    rtn = 1;
  }
  else
  {
    rtn = 0;
  }

  /* enable interrupts */
  enableInterrupts();

  return rtn;
}

/*******************************************************************************
* @fn     get_timer_isuse
*
* @brief
*
* @param
*
* @return
*/
bool get_timer_isuse(uint16_t timer_id)
{
  uint8_t index = 0;

  bool rtn = FALSE;

  index = ffs(timer_id);

  if(index >= MAX_TIME_NUM)
  {
    return FALSE;
  }
  /* disenable interrupts */
  disableInterrupts();

  if(systemTimer[index].isuse)
  {
    rtn = TRUE;
  }
  else
  {
    rtn = FALSE;
  }
  /* enable interrupts */
  enableInterrupts();


  return rtn;
}

/*******************************************************************************
* @fn     System_timer_run
*
* @brief
*
* @param
*
* @return
*/
uint8_t System_timer_run(void)
{
  uint8_t idx = 0;

  do
  {
    if (systemTimer[idx].timerflag)// Timer Task is  ready.
    {
      break;
    }
  }while (++idx < MAX_TIME_NUM);

  if (idx < MAX_TIME_NUM)
  {
    /* disenable interrupts */
    disableInterrupts();

    systemTimer[idx].timerflag = 0;
    systemTimer[idx].isuse = FALSE;
    SysEvent |= systemTimer[idx].timerid;

    /* enable interrupts */
    enableInterrupts();

    /* Invoke Callback if new timer event were completed */
    if(pTimerEventHandler)
    {
      SysEvent |= pTimerEventHandler(systemTimer[idx].timerid);
    }
  }
  else// Complete pass through all task events with no activity?
  {
    /*Put the processor/system into sleep*/




  }

  return 1;
}


static inline uint8_t ffs(uint16_t data)
{
  uint16_t temp = 0x01;
  uint8_t i = 0;

  if(data == 0)
  {
    return 0xFF;
  }

  while((data & temp) == 0x0000)
  {
    i = i + 1;
    temp = temp << 1;
  }

  return i;
}



