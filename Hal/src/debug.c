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

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
#if (defined DEBUG) && (DEBUG == TRUE)
/*******************************************************************************
* @fn
*
* @brief
*
* @param
*
* @return
*/
void DebugUart_Config(enBaudRate baudrate)
{
  /*Enable Clock*/
  CLK_PeripheralClockConfig(CLK_PERIPHERAL_UART2,ENABLE);

  UART2_DeInit();

  /* UART1 configuration ------------------------------------------------------*/
  /* UART1 configured as follow:
        - BaudRate = 115200 baud
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Receive and transmit enabled
        - UART1 Clock disabled
  */
  UART2_Init((uint32_t)baudrate, UART2_WORDLENGTH_8D, UART2_STOPBITS_1, UART2_PARITY_NO,
              UART2_SYNCMODE_CLOCK_DISABLE, UART2_MODE_TXRX_ENABLE);

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
void DebugPrintf(uint8_t *buff,uint8_t len)
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
#else
void DebugUart_Config(uint32_t baudrate){}
void DebugPrintf(uint8_t *buff,uint8_t len){}

#endif
