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
#ifndef _STATE_QUEUE_H
#define _STATE_QUEUE_H

#include "define.h"
/* Private typedef -----------------------------------------------------------*/
/*****************************************************************************/
#define STATE_SHIFT_NUM       4
#define STATE_SAMPLE_COUNT    (12)
#define STATE_QUEUE_NUM       STATE_SAMPLE_COUNT

typedef enMEMSState StateDataType; /*队列元素类型*/

/*****************************************************************************/
typedef struct
{
  volatile uint8_t head;
  volatile uint8_t tail;
  volatile uint8_t count;/*元素个数*/
  StateDataType data[STATE_QUEUE_NUM];
}StateQueue_t;

/* Private define ------------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Public functions ----------------------------------------------------------*/
uint8_t StateQueue_Init(StateQueue_t *queue);
uint8_t StateQueue_push(StateQueue_t *queue,StateDataType value);
uint8_t StateQueue_pop(StateQueue_t *queue,StateDataType *value);
uint8_t StateQueue_is_empty(StateQueue_t *queue);
uint8_t StateQueue_is_full(StateQueue_t *queue);
uint8_t StateQueue_node_count(StateQueue_t *queue);
#endif

