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
#include "led.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/*******************************************************************************
* @fn
*
* @brief
*
* @param
*
* @return
*/
void Led_Init(LedDeviceControl_t *leddevice)
{
  /*led Init*/
  GPIO_Init(leddevice->hardLink.port,leddevice->hardLink.pin,leddevice->hardLink.mode);

	/*led control*/
  leddevice->Ctrl.ledControl(&leddevice->hardLink,SET);

  leddevice->state = OFF;
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
LedState LedControl(HardLink_t *hardlink, BitAction GPIO_BitVal)
{
  LedState state = OFF;

  if(GPIO_BitVal == SET)
  {
    GPIO_WriteHigh(hardlink->port, hardlink->pin);
    state = OFF;
  }
  else
  {
    GPIO_WriteLow(hardlink->port, hardlink->pin);
    state = ON;
  }

  return state;
}

