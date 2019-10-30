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
#include "spi.h"
/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
uint8_t Soft_SPI_SendByte(uint8_t data);
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
uint8_t Soft_SPI_Init(void)
{
  /* Enable SPI CLK */
  CLK_PeripheralClockConfig(CLK_PERIPHERAL_SPI, ENABLE);

	/*clk*/
  GPIO_Init(SPI_CLK_PORT, SPI_CLK_PIN,GPIO_MODE_OUT_PP_HIGH_FAST);

	/*mosi*/
  GPIO_Init(SPI_MOSI_PORT, SPI_MOSI_PIN,GPIO_MODE_OUT_PP_HIGH_FAST);

	/*miso*/
  GPIO_Init(SPI_MISO_PORT, SPI_MISO_PIN,GPIO_MODE_IN_PU_NO_IT);

  /* Set the MOSI,MISO and SCK at high level */
  SPI_Init(SPI_FIRSTBIT_MSB,SPI_BAUDRATEPRESCALER_8,SPI_MODE_MASTER,
           SPI_CLOCKPOLARITY_HIGH,SPI_CLOCKPHASE_2EDGE,SPI_DATADIRECTION_2LINES_FULLDUPLEX,
           SPI_NSS_SOFT,0x07);

  SPI_Cmd(ENABLE);

	return 1;
}

uint8_t SPI_SendByte(uint8_t Byte)
{
  uint8_t temp;

  while(SPI_GetFlagStatus(SPI_FLAG_TXE) == RESET);
  SPI_SendData(Byte);

  /* Wait to receive a byte */
  while (SPI_GetFlagStatus(SPI_FLAG_RXNE) == RESET);
  temp = SPI_ReceiveData();

  return temp;
}

