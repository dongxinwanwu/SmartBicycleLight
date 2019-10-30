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
#ifndef _SPI_H
#define _SPI_H

#include "define.h"
#include "timer.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/*spi io configure*/
#define SPI_CLK_PORT  		(GPIOC)
#define SPI_CLK_PIN   		(GPIO_PIN_5)

#define SPI_MISO_PORT  	  (GPIOC)
#define SPI_MISO_PIN   	  (GPIO_PIN_7)

#define SPI_MOSI_PORT  	  (GPIOC)
#define SPI_MOSI_PIN   	  (GPIO_PIN_6)

#define SPI_CLK_LOW()     (GPIO_WriteLow(SOFT_SPI_CLK_PORT,SOFT_SPI_CLK_PIN,RESET))
#define SPI_CLK_HIGH()    (GPIO_WriteHigh(SOFT_SPI_CLK_PORT,SOFT_SPI_CLK_PIN,SET))

#define SPI_MOSI_LOW()    (GPIO_WriteLow(SOFT_SPI_MOSI_PORT,SOFT_SPI_MOSI_PIN,RESET))
#define SPI_MOSI_HIGH()   (GPIO_WriteHigh(SOFT_SPI_MOSI_PORT,SOFT_SPI_MOSI_PIN,SET))

#define SPI_MISO_DATA()   (GPIO_ReadInputPin(SOFT_SPI_MISO_PORT,SOFT_SPI_MISO_PIN))
/******************************************************************************/
/* Functions Prototypes                                                       */
/******************************************************************************/
/*! Initializes the SPI communication peripheral. */
uint8_t Soft_SPI_Init(void);
uint8_t SPI_SendByte(uint8_t Byte);
#endif

