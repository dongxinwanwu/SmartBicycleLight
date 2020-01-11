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
#include "adc.h"
#include "timer.h"
#include "math.h"
/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define PHOTO_ADC_CHAN                 ADC1_CHANNEL_0
#define PHOTO_ADC_SCHMITTTRIGG_CHAN    ADC1_SCHMITTTRIG_CHANNEL0

#define ADC_SHIFT_NUM       4
#define ADC_SAMPLE_COUNT    ((1 << ADC_SHIFT_NUM) + 2) // 18 (one maximum, one minimum)
#define ADC_RATIO           ((uint32_t) 806) /*ADC_RATIO = ( 3.3 * 1000 * 1000)/4095 */
#define ADC_REF_VOLTAGE     (3300)  /*3.3V*/
#define PHOTO_RES_THRESHOLD (25)    /*25K*/
/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
PhotoControl_t PhotoDev =
{
  .hardLink.port    = GPIOB,
  .hardLink.pin     = GPIO_PIN_0,
  .hardLink.mode    = GPIO_MODE_IN_FL_NO_IT,
  .open             = PhotoADC_Open,
  .close            = PhotoADC_Close,
  .GetState         = GetPhotoState,
  .process          = PhotoState_Process,
  .state            = DAY
};
/* Private macro -------------------------------------------------------------*/
void ADC_Init(PhotoControl_t *dev);
void PhotoADC_Open(void);
void PhotoADC_Close(void);
enPhotoState GetPhotoState(void);
uint16_t GetADC_Vol(void);
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
void ADC_Init(PhotoControl_t *dev)
{
  /*ADC检测相关pin*/
  GPIO_Init(dev->hardLink.port,dev->hardLink.pin,dev->hardLink.mode);
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
void PhotoADC_Open(void)
{
  /* Enable ADC1 clock */
  CLK_PeripheralClockConfig(CLK_PERIPHERAL_ADC, ENABLE);

  /* Init ADC1 peripheral */
  /* Disable SchmittTrigger for ADC_Channel, to save power */
  ADC1_Init(ADC1_CONVERSIONMODE_CONTINUOUS,
            PHOTO_ADC_CHAN,
            ADC1_PRESSEL_FCPU_D2,
            ADC1_EXTTRIG_TIM,ENABLE,
            ADC1_ALIGN_RIGHT,
            PHOTO_ADC_SCHMITTTRIGG_CHAN,
            ENABLE);
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
void PhotoADC_Close(void)
{
  /* DeInitialize ADC1 */
  ADC1_DeInit();

  /* Disable ADC1 clock */
  CLK_PeripheralClockConfig(CLK_PERIPHERAL_ADC, DISABLE);
}

/*******************************************************************************
* @fn     GetADCVol
*
* @brief  获取ADC采样后转换电压值
*
* @param
*
* @return
*/
uint16_t GetADCVol(void)
{
  uint16_t adc_value[ADC_SAMPLE_COUNT] = {0};
  uint16_t adc_avg_value = 0,adc_min_value = 0,adc_max_value = 0;
  uint16_t voltage = 0;

  /*开启转换*/
  /*首次ADC转换不准确，此处单独剔除*/
  ADC1_StartConversion();
  UserTimingDelay(100);

  while(ADC1_GetFlagStatus(ADC1_FLAG_EOC) == RESET);
  adc_value[0] = ADC1_GetConversionValue();
  ADC1_ClearFlag(ADC1_FLAG_EOC);

  /*连续采样18次，去掉最大值，最小值，得到平均值*/
  for(uint8_t i = 0; i < ADC_SAMPLE_COUNT; i++)
  {
    ADC1_StartConversion();
    while(ADC1_GetFlagStatus(ADC1_FLAG_EOC) == RESET);
    adc_value[i] = ADC1_GetConversionValue();
    ADC1_ClearFlag(ADC1_FLAG_EOC);

    adc_avg_value += adc_value[i] / ADC_SHIFT_NUM;
  }

  adc_max_value = adc_value[0];
  adc_min_value = adc_value[0];

  for(uint8_t i = 0; i < ADC_SAMPLE_COUNT; i++)
  {
    adc_min_value = MIN(adc_min_value, adc_value[i]);
    adc_max_value = MAX(adc_max_value, adc_value[i]);
  }

  /* Remove one maximum, one minimum, dividing by 16 is equivalent to right shifting 4 bits */
  adc_avg_value = (adc_avg_value - (adc_max_value >> ADC_SHIFT_NUM) - (adc_min_value >> ADC_SHIFT_NUM));

  /**/
  voltage = (uint16_t)((uint32_t)adc_avg_value * (uint32_t)ADC_RATIO / 1000);//mv

  return voltage;
}

/*******************************************************************************
* @fn     GetPhotoState
*
* @brief
*
* @param
*
* @return
*/
enPhotoState GetPhotoState(void)
{
  uint16_t voltage = 0;
  uint16_t photo_res = 0;/*KΩ*/
  enPhotoState state = DAY;

  voltage = GetADCVol();

  if(voltage >= ADC_REF_VOLTAGE)
  {
    voltage = ADC_REF_VOLTAGE;
    state = NIGHT;

    return state;
  }

  /*分压原理，计算光敏电阻阻值：voltage / R = 3300 / (10 + R)*/
  photo_res = 10 * voltage / (ADC_REF_VOLTAGE - voltage);

  if(photo_res <= PHOTO_RES_THRESHOLD)
  {
    state = DAY;
  }
  else
  {
    state = NIGHT;
  }

  return state;
}