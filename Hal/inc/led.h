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
#define LIGHT_NUM         4
/*****************************************************************************/
typedef enLedState (*DeviceControl)(HardLink_t *hardlink, BitAction GPIO_BitVal);

typedef struct
{
  DeviceControl ledControl;
}DevCtrl_t;

typedef struct
{
  HardLink_t hardLink;  /*Ӳ������*/
  DevCtrl_t Ctrl;       /*���Ʒ���*/
  enLedState state;        /*�豸״̬*/
}LedDeviceControl_t;

/* Private define ------------------------------------------------------------*/
/* Evalboard I/Os configuration */

/*led*/
extern LedDeviceControl_t PhotoLed;
extern LedDeviceControl_t DirLedTab[LIGHT_NUM];
/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Public functions ----------------------------------------------------------*/
void Led_Init(LedDeviceControl_t *leddevice);
enLedState LedControl(HardLink_t *hardlink, BitAction GPIO_BitVal);
#endif

