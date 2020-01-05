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
#ifndef _DEFINE_H
#define _DEFINE_H

#include "stm8s.h"

#ifndef BV
#define BV(n)      (1 << (n))
#endif

#ifndef MAX
#define MAX(x, y)                 (((x) > (y)) ? (x) : (y))
#endif

#ifndef MIN
#define MIN(x, y)                 (((x) < (y)) ? (x) : (y))
#endif
/* Private typedef -----------------------------------------------------------*/
/*****************************************************************************/
/*指示灯状态枚举*/
typedef enum
{
  OFF = (uint8_t)0x00,
  ON  = (uint8_t)0x01
}enLedState;

/*硬件物理IO配置*/
typedef struct
{
  /* Control Pin */
  GPIO_TypeDef *port;
  GPIO_Pin_TypeDef pin;
  GPIO_Mode_TypeDef mode;
}HardLink_t;

/*IO中断配置*/
typedef struct
{
  EXTI_Port_TypeDef exit_port;
  EXTI_Sensitivity_TypeDef exit_Trigger;
}ExitLink_t;

/*LIS3DH_FULLSCALE_2 = ±2g = 4g = 4000mg*/
#define MEMS_MULTIPLE        (1000)   /**/
#define MEMS_RATIO           ((int32_t)61) /*MEMS_RATIO = MEMS_MULTIPLE *( 4000 / 65536) */

typedef struct
{
  double acc_x;
  double acc_y;
  double acc_z;
} Accelerate_t;

typedef enum
{
	DAY    = (uint8_t)0x00,
	NIGHT  = (uint8_t)0x01
}enPhotoState;

typedef enum
{
  URGENT_BLINK_OFF   = (uint8_t)0x00,
  URGENT_BLINK_ON    = (uint8_t)0x01,
}enUrgentBlinkState;

typedef enum
{
  WIRE_TURN_NONE   = (uint8_t)0x00,
  WIRE_TURN_LEFT   = (uint8_t)0x01,
  WIRE_TURN_RIGHT  = (uint8_t)0x02,
}enWireTurnState;

typedef enum
{
  SPEED_STOP    = (uint8_t)0x00,
  SPEED_UNIFORM = (uint8_t)0x01,
  SPEED_UP      = (uint8_t)0x02,
  SPEED_DOWM    = (uint8_t)0x03
}enMEMSState;

typedef enum
{
  BICYCLE_DEFAULT   = (uint8_t)0x00,
  BICYCLE_STOP      = (uint8_t)0x01,
  BICYCLE_RUN       = (uint8_t)0x02,
  BICYCLE_SUSPEND   = (uint8_t)0x03,/*刹车*/
  BICYCLE_WAITTING  = (uint8_t)0x04 /*等待红绿灯*/
}enBicycleState;

typedef struct
{
  uint8_t urgentBlink   :1;
  uint8_t turn        :1;
  uint8_t MEMSSensor  :1;
  uint8_t PhotoSensor :1;
}SensorState_t;

typedef struct
{
  SensorState_t       sensorstate;
  enUrgentBlinkState  urgentblinkstate;
  enWireTurnState     turnstate;
  enMEMSState         memsstate;
  enPhotoState        photostate;
}BicycleSensorState_t;

/*设备工作模式*/
typedef enum
{
  NormalMode = (uint8_t)0x01,/*正常模式*/
  HaltMode   = (uint8_t)0x02 /*待机模式（睡眠模式）*/
}DeviceWorkMode;
/*****************************************************************************/
#endif

