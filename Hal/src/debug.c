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
#include "debug.h"
#include "timer.h"
/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void Uart2_Config(uint32_t baudrate);
void UartSendCommand(uint8_t funcode,uint8_t *data,uint8_t len);
/*******************************************************************************/

/*******************************************************************************
* @fn
*
* @brief
*
* @param
*
* @return
*/
void Uart2_Config(uint32_t baudrate)
{
  /* Deinitializes the UART2 peripheral */
  UART2_DeInit();

  /* UART2 configuration */
  /* UART2 configured as follow:
  - BaudRate = 9600 baud
  - Word Length = 8 Bits
  - One Stop Bit
  - No parity
  - Receive and transmit enabled
  - UART2 Clock disabled
  */

  UART2_Init((uint32_t)baudrate, UART2_WORDLENGTH_8D, UART2_STOPBITS_1, UART2_PARITY_NO,
             UART2_SYNCMODE_CLOCK_DISABLE, UART2_MODE_TXRX_ENABLE);

  /* Enable UART2 Transmit interrupt*/
  UART2_ITConfig(UART2_IT_RXNE_OR, ENABLE);

  /*Enable*/
  UART2_Cmd(ENABLE);
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
void UartSendData(uint8_t *buff,uint8_t len)
{
	for(uint8_t i = 0; i < len; i++)
	{
    /* Transmit Data Register Empty flag */
    while (UART2_GetFlagStatus(UART2_FLAG_TXE) == RESET);
		UART2_SendData8(buff[i]);
    /* Transmission Complete flag */
    while (UART2_GetFlagStatus(UART2_FLAG_TC) == RESET);
	}
}
