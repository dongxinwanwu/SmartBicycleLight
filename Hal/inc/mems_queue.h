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
#ifndef _MEMS_QUEUE_H
#define _MEMS_QUEUE_H

#include "define.h"
/* Private typedef -----------------------------------------------------------*/
/*****************************************************************************/
#define MEMS_SHIFT_NUM       3
#define MEMS_SAMPLE_COUNT    (1 << MEMS_SHIFT_NUM)
#define MEMS_QUEUE_NUM       MEMS_SAMPLE_COUNT

typedef int32_t MemsDataType; /*队列元素类型*/

/*****************************************************************************/
typedef struct
{
  volatile uint8_t head;
  volatile uint8_t tail;
  volatile uint8_t count;/*元素个数*/
  MemsDataType data[MEMS_QUEUE_NUM];
}MEMSQueue_t;

/* Private define ------------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Public functions ----------------------------------------------------------*/
uint8_t MemsQueue_Init(MEMSQueue_t *queue);
uint8_t MemsQueue_push(MEMSQueue_t *queue,MemsDataType value);
uint8_t MemsQueue_pop(MEMSQueue_t *queue,MemsDataType *value);
uint8_t MemsQueue_is_empty(MEMSQueue_t *queue);
uint8_t MemsQueue_is_full(MEMSQueue_t *queue);
#endif

