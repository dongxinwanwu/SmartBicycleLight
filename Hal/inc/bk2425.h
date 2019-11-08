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
#ifndef BK2425_H
#define BK2425_H

#include "stm8s.h"
/*****************************************************************************/
/*BK2425 PIN DEFINITION*/

#define BK2425_IRQ_PORT         (GPIOB)
#define BK2425_IRQ_PIN          (GPIO_PIN_0)

#define BK2425_EN_PORT          (GPIOD)
#define BK2425_EN_PIN           (GPIO_PIN_0)

#define BK2425_SPI_CS_PORT  	  (GPIOE)
#define BK2425_SPI_CS_PIN   	  (GPIO_PIN_5)

#define BK2425_SPI_CS_LOW()     (GPIO_WriteLow(BK2425_SPI_CS_PORT,BK2425_SPI_CS_PIN))
#define BK2425_SPI_CS_HIGH()    (GPIO_WriteHigh(BK2425_SPI_CS_PORT,BK2425_SPI_CS_PIN))

#define BK2425_EN_LOW()         (GPIO_WriteLow(BK2425_EN_PORT,BK2425_EN_PIN))
#define BK2425_EN_HIGH()        (GPIO_WriteHigh(BK2425_EN_PORT,BK2425_EN_PIN))

#define TX_ADR_WIDTH            5   /*5�ֽڿ�ȵķ���/���յ�ַ*/
#define TX_PLOAD_WIDTH          32  /*����ͨ����Ч���ݿ��*/

/******************************************************************************/
/*SPI(BK2425) commands*/
#define READ_REG                0x00  // Define read command to register
#define WRITE_REG               0x20  // Define write command to register
#define RD_RX_PLOAD             0x61  // Define RX payload register address
#define WR_TX_PLOAD             0xA0  // Define TX payload register address
#define FLUSH_TX                0xE1  // Define flush TX register command
#define FLUSH_RX                0xE2  // Define flush RX register command
#define REUSE_TX_PL             0xE3  // Define reuse TX payload register command
#define NOP                     0xFF  // Define No Operation, might be used to read status register

/******************************************************************************/
/*SPI(BK2425) registers(addresses)*/
#define CONFIG                  0x00  // 'Config' register address
#define EN_AA                   0x01  // 'Enable Auto Acknowledgment' register address
#define EN_RXADDR               0x02  // 'Enabled RX addresses' register address
#define SETUP_AW                0x03  // 'Setup address width' register address
#define SETUP_RETR              0x04  // 'Setup Auto. Retrans' register address
#define RF_CH                   0x05  // 'RF channel' register address
#define RF_SETUP                0x06  // 'RF setup' register address
#define STATUS                  0x07  // 'Status' register address
#define OBSERVE_TX              0x08  // 'Observe TX' register address
#define CD                      0x09  // 'Carrier Detect' register address
#define RX_ADDR_P0              0x0A  // 'RX address pipe0' register address
#define RX_ADDR_P1              0x0B  // 'RX address pipe1' register address
#define RX_ADDR_P2              0x0C  // 'RX address pipe2' register address
#define RX_ADDR_P3              0x0D  // 'RX address pipe3' register address
#define RX_ADDR_P4              0x0E  // 'RX address pipe4' register address
#define RX_ADDR_P5              0x0F  // 'RX address pipe5' register address
#define TX_ADDR                 0x10  // 'TX address' register address
#define RX_PW_P0                0x11  // 'RX payload width, pipe0' register address
#define RX_PW_P1                0x12  // 'RX payload width, pipe1' register address
#define RX_PW_P2                0x13  // 'RX payload width, pipe2' register address
#define RX_PW_P3                0x14  // 'RX payload width, pipe3' register address
#define RX_PW_P4                0x15  // 'RX payload width, pipe4' register address
#define RX_PW_P5                0x16  // 'RX payload width, pipe5' register address
#define FIFO_STATUS             0x17  // 'FIFO Status Register' register address

/******************************************************************************/
/*STATUS Register*/
#define RX_DR						        0x40
#define TX_DS						        0x20
#define MAX_RT					        0x10
/******************************************************************************/
/*BK2425 API Functions*/

void BK2425_Init(void);
uint8_t BK2425_Write_Reg(uint8_t reg, uint8_t value);
uint8_t BK2425_Write_Buf(uint8_t reg, uint32_t *pBuf, uint8_t bytes);
uint8_t BK2425_Read_Reg(uint8_t reg);
uint8_t BK2425_Read_Buf(uint8_t reg, uint32_t *pBuf, uint8_t bytes);

void BK2425_RX_Mode(void);
void BK2425_TX_Mode(void);
uint8_t BK2425_RxPacket(uint32_t *rxbuf);
uint8_t BK2425_TxPacket(uint32_t *txbuf);
uint8_t BK2425_check(void);
/******************************************************************************/
#endif