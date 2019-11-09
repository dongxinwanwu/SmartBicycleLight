/**
  ******************************************************************************
  * @file    Project/main.c
  * @author  MCD Application Team
  * @version V2.3.0
  * @date    16-June-2017
  * @brief   Main program body
   ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"
#include "app.h"
/* Private defines -----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
void SysBspInit(void);
void SysClock_Config(void);
void SysPeriphDeInit(void);
void IWDG_Config(void);
/*******************************************************************************
* @fn
*
* @brief
*
* @param
*
* @return
*/
void main(void)
{
  /* init */
  SysBspInit();

  IWDG_Config();

  /* enable interrupts */
  enableInterrupts();

  /*init mems*/
  system_start_timer(USERAPP_LIS3DH_INIT_EVT,TIMER_ONCE_MODE,LIS3DH_DELAY_INIT_TIME);

  /*reload wdt*/
  system_start_timer(USERAPP_RELOAD_WDT_EVT,TIMER_AUTO_MODE,RELOAD_WDT_TIME);

  /*Infinite loop */
  while (1)
  {
    /*event loop hook function*/
    System_eventloop_hook();

    /*Process System Time Event*/
    System_timer_run();
  }
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
void SysBspInit(void)
{
  SysClock_Config();
  SysPeriphDeInit();

  /*led*/
  for(uint8_t i = 0; i < LIGHT_NUM; i++)
  {
    Led_Init(&DirLedTab[i]);
    LedControl(&DirLedTab[i].hardLink,RESET);
  }

  /*photo sensor*/
  ADC_Init(&PhotoDev);

  /*key*/
  HalKeyInit(&OnOffKey,UserAppHandleKeys);

  /*spi*/
  Soft_SPI_Init();

  /*user timer*/
  SystemTimer_Config();
  system_timer_init(ProcessSystemTimeEvent);
}

/*******************************************************************************
* @fn     SysClockInit
*
* @brief  init clock source
*
* @param  none
*
* @return none
*/
void SysClock_Config(void)
{
  CLK_DeInit();
  /* 启动时，主时钟源默认为HSI RC时钟的8分频，即fHSI/8 */
  CLK_HSICmd(ENABLE);
  /*master clock*/
  CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1); /*HSI= 16MHZ*/
  /*cpu clock*/
  CLK_SYSCLKConfig(CLK_PRESCALER_CPUDIV1);
}

/*******************************************************************************
* @fn     Sys_DeInit
*
* @brief  init clock source
*
* @param  none
*
* @return none
*/
void SysPeriphDeInit(void)
{
	/*Deinit io*/
  GPIO_DeInit(GPIOA);
	GPIO_DeInit(GPIOB);
	GPIO_DeInit(GPIOC);
	GPIO_DeInit(GPIOD);
	GPIO_DeInit(GPIOE);
	GPIO_DeInit(GPIOF);

  ADC1_DeInit();
  TIM4_DeInit();
  SPI_DeInit();
  EXTI_DeInit();
}
/*******************************************************************************
* @fn     IWDG_Config
*
* @brief  500ms
*
* @param
*
* @return
*/
void IWDG_Config(void)
{
  /* Enable IWDG (the LSI oscillator will be enabled by hardware) */
  IWDG_Enable();

  /* Enable write access to IWDG_PR and IWDG_RLR registers */
  IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);

  /* IWDG counter clock: LSI/128 */
  IWDG_SetPrescaler(IWDG_Prescaler_128);

  /* Set counter reload value to obtain 500ms IWDG Timeout */
  /*T = 250 * 1 /(64k / 128) = 500ms*/
  IWDG_SetReload(250);

  IWDG_ReloadCounter();
}

#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval : None
  */
void assert_failed(u8* file, u32 line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {

  }
}
#endif


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
