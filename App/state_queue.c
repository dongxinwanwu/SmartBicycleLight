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
#include "state_queue.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/*******************************************************************************
* @fn     MemsQueue_Init
*
* @brief
*
* @param
*
* @return
*/
uint8_t StateQueue_Init(StateQueue_t *queue)
{
  /*init*/
  queue->head = 0;
  queue->tail = 0;
  queue->count = 0;

  for(uint8_t i = 0; i < STATE_QUEUE_NUM; i++)
    queue->data[i] = SPEED_STOP;

  return 1;
}

/*******************************************************************************
* @fn     MemsQueue_push
*
* @brief
*
* @param
*
* @return
*/
uint8_t StateQueue_push(StateQueue_t *queue,StateDataType value)
{
  if(StateQueue_is_full(queue))
    return 0;

  /*queue push*/
  queue->data[queue->tail] = value;
  queue->tail = (queue->tail + 1) % STATE_QUEUE_NUM;
  queue->count++;
  return 1;
}

/*******************************************************************************
* @fn     MemsQueue_pop
*
* @brief
*
* @param
*
* @return
*/
uint8_t StateQueue_pop(StateQueue_t *queue,StateDataType *value)
{
  if(StateQueue_is_empty(queue))
    return 0;
  /*queue pop*/
  *value = queue->data[queue->head];
  queue->head = (queue->head + 1) % STATE_QUEUE_NUM;
  queue->count--;

  return 1;
}

/*******************************************************************************
* @fn     MemsQueue_is_empty
*
* @brief
*
* @param
*
* @return
*/
uint8_t StateQueue_is_empty(StateQueue_t *queue)
{
  return queue->count == 0;
}

/*******************************************************************************
* @fn     MemsQueue_is_full
*
* @brief
*
* @param
*
* @return
*/
uint8_t StateQueue_is_full(StateQueue_t *queue)
{
  return queue->count == STATE_QUEUE_NUM;
}

/*******************************************************************************
* @fn     MemsQueue_node_count
*
* @brief
*
* @param
*
* @return
*/
uint8_t StateQueue_node_count(StateQueue_t *queue)
{
  return queue->count;
}

