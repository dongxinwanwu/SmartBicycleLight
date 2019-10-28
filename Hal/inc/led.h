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
/*****************************************************************************/
typedef LedState (*DeviceControl)(HardLink_t *hardlink, BitAction GPIO_BitVal);

typedef struct
{
  DeviceControl ledControl;
}DevCtrl_t;

typedef struct
{
  HardLink_t hardLink;  /*Ӳ������*/
  DevCtrl_t Ctrl;       /*���Ʒ���*/
  LedState state;        /*�豸״̬*/
}LedDeviceControl_t;

/* Private define ------------------------------------------------------------*/
extern LedDeviceControl_t LedSensor;
extern LedDeviceControl_t LedRGB;
extern LedDeviceControl_t LedWhite;
extern LedDeviceControl_t LedNFC;
/* Evalboard I/Os configuration */

/*led*/

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Public functions ----------------------------------------------------------*/
void Led_Init(LedDeviceControl_t *leddevice);
LedState LedControl(HardLink_t *hardlink, BitAction GPIO_BitVal);
#endif

