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
#if (defined DEBUG)
LedDeviceControl_t OnOffLed =
{
  .hardLink.port  = GPIOA,
  .hardLink.pin   = GPIO_PIN_2,
  .hardLink.mode  = GPIO_MODE_OUT_PP_HIGH_FAST,
  .Ctrl           = LedControl,
  .state          = OFF
};
#endif

/*转向指示灯*/
LedDeviceControl_t DirLedTab[LIGHT_NUM] =
{
#if (!defined DEBUG)
  {
    .hardLink.port  = GPIOA,
    .hardLink.pin   = GPIO_PIN_2,
    .hardLink.mode  = GPIO_MODE_OUT_PP_HIGH_FAST,
    .Ctrl           = LedControl,
    .state          = OFF

  },
#endif
  {
    .hardLink.port  = GPIOA,
    .hardLink.pin   = GPIO_PIN_1,
    .hardLink.mode  = GPIO_MODE_OUT_PP_HIGH_FAST,
    .Ctrl           = LedControl,
    .state          = OFF
  },
  {
    .hardLink.port  = GPIOD,
    .hardLink.pin   = GPIO_PIN_7,
    .hardLink.mode  = GPIO_MODE_OUT_PP_HIGH_FAST,
    .Ctrl           = LedControl,
    .state          = OFF
  },
  {
    .hardLink.port  = GPIOD,
    .hardLink.pin   = GPIO_PIN_4,
    .hardLink.mode  = GPIO_MODE_OUT_PP_HIGH_FAST,
    .Ctrl           = LedControl,
    .state          = OFF
  },
  {
    .hardLink.port  = GPIOF,
    .hardLink.pin   = GPIO_PIN_4,
    .hardLink.mode  = GPIO_MODE_OUT_PP_HIGH_FAST,
    .Ctrl           = LedControl,
    .state          = OFF
  },
  {
    .hardLink.port  = GPIOC,
    .hardLink.pin   = GPIO_PIN_1,
    .hardLink.mode  = GPIO_MODE_OUT_PP_HIGH_FAST,
    .Ctrl           = LedControl,
    .state          = OFF
  },
  {
    .hardLink.port  = GPIOD,
    .hardLink.pin   = GPIO_PIN_3,
    .hardLink.mode  = GPIO_MODE_OUT_PP_HIGH_FAST,
    .Ctrl           = LedControl,
    .state          = OFF
  },
  {
    .hardLink.port  = GPIOC,
    .hardLink.pin   = GPIO_PIN_3,
    .hardLink.mode  = GPIO_MODE_OUT_PP_HIGH_FAST,
    .Ctrl           = LedControl,
    .state          = OFF
  },
  {
    .hardLink.port  = GPIOB,
    .hardLink.pin   = GPIO_PIN_1,
    .hardLink.mode  = GPIO_MODE_OUT_PP_HIGH_FAST,
    .Ctrl           = LedControl,
    .state          = OFF
  },
  {
    .hardLink.port  = GPIOB,
    .hardLink.pin   = GPIO_PIN_2,
    .hardLink.mode  = GPIO_MODE_OUT_PP_HIGH_FAST,
    .Ctrl           = LedControl,
    .state          = OFF
  }
};

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
  leddevice->Ctrl.ledControl(&leddevice->hardLink,OFF);

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
enLedState LedControl(HardLink_t *hardlink, enLedState state)
{
  enLedState rtnstate = OFF;

  if(state == ON)
  {
    GPIO_WriteHigh(hardlink->port, hardlink->pin);
    rtnstate = ON;
  }
  else
  {
    GPIO_WriteLow(hardlink->port, hardlink->pin);
    rtnstate = OFF;
  }

  return rtnstate;
}

