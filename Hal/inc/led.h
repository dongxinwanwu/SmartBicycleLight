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
#ifndef _LED_H
#define _LED_H

#include "define.h"
/* Private typedef -----------------------------------------------------------*/
/*****************************************************************************/
#define LIGHT_NUM               10
#define RIGHT_LIGHT_START_IDX   4
#define LEFT_LIGHT_START_IDX    5


/*****************************************************************************/
typedef enLedState (*DeviceControl)(HardLink_t *hardlink, enLedState state);

typedef struct
{
  DeviceControl ledControl;
}DevCtrl_t;

typedef struct
{
  HardLink_t hardLink;  /*硬件连接*/
  DevCtrl_t Ctrl;       /*控制方法*/
  enLedState state;        /*设备状态*/
}LedDeviceControl_t;

/* Private define ------------------------------------------------------------*/
/* Evalboard I/Os configuration */

/*led*/
extern LedDeviceControl_t DirLedTab[LIGHT_NUM];
/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Public functions ----------------------------------------------------------*/
void Led_Init(LedDeviceControl_t *leddevice);
enLedState LedControl(HardLink_t *hardlink, enLedState state);
#endif

