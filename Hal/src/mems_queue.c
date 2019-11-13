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
#include "mems_queue.h"

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
uint8_t MemsQueue_Init(MEMSQueue_t *queue)
{
  /*init*/
  queue->head = 0;
  queue->tail = 0;
  queue->count = 0;

  for(uint8_t i = 0; i < MEMS_QUEUE_NUM; i++)
    queue->data[i] = 0;

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
uint8_t MemsQueue_push(MEMSQueue_t *queue,MemsDataType value)
{
  /*queue push*/
  queue->data[queue->tail] = value;
  queue->tail = (queue->tail + 1) % MEMS_QUEUE_NUM;
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
uint8_t MemsQueue_pop(MEMSQueue_t *queue,MemsDataType *value)
{
  /*queue pop*/
  *value = queue->data[queue->head];
  queue->head = (queue->head + 1) % MEMS_QUEUE_NUM;
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
uint8_t MemsQueue_is_empty(MEMSQueue_t *queue)
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
uint8_t MemsQueue_is_full(MEMSQueue_t *queue)
{
  return queue->count == MEMS_QUEUE_NUM;
}

