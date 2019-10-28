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
#include "debug.h"
/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define PHOTO_ADC_CHANNEL  ADC1_CHANNEL_3
#define ADC_SHIFT_NUM      4
#define ADC_SAMPLE_COUNT   ((1 << ADC_SHIFT_NUM) + 2) // 18 (one maximum, one minimum)
#define ADC_RATIO          ((uint32_t) 806) /*ADC_RATIO = ( 3.3 * 1000 * 1000)/4095 */
#define ADC_REF_VOLTAGE    (3300)  /*3.3V*/
/* Private macro -------------------------------------------------------------*/
#ifndef MAX
#define MAX(x, y)                 (((x) > (y)) ? (x) : (y))
#endif

#ifndef MIN
#define MIN(x, y)                 (((x) < (y)) ? (x) : (y))
#endif
/* Private variables ---------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
void PhotoADC_Init(HardLink_t *hardlink);
void PhotoADC_Open(void);
void PhotoADC_Close(void);
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
void PhotoADC_Init(HardLink_t *hardlink)
{
  /*ADC检测相关pin*/
  GPIO_Init(hardlink->port,hardlink->pin,hardlink->mode);
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
  ADC1_Init(ADC1_CONVERSIONMODE_CONTINUOUS,PHOTO_ADC_CHANNEL,ADC1_PRESSEL_FCPU_D2,
            ADC1_EXTTRIG_TIM,ENABLE, ADC1_ALIGN_RIGHT,
            ADC1_SCHMITTTRIG_CHANNEL3,ENABLE);
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
  enPhotoState state = DAY;

  voltage = GetADCVol();

  if(voltage > ADC_REF_VOLTAGE)
  {
    voltage = 0;
  }




  return state;
}