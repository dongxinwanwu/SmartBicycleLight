/*******************************************************************************
* @file
* @author
* @version
* @date
* @brief
*
*******************************************************************************/
#include "bk2425.h"
#include "spi.h"
/******************************************************************************/
uint32_t TX_ADDRESS[TX_ADR_WIDTH] = {0x0A,0x01,0x07,0x0E,0x01};

/*******************************************************************************
* @fn     BK2425_Init
*
* @brief
*
* @param
*
* @return none
*/
void BK2425_Init(void)
{
  GPIO_Init(BK2425_SPI_CS_PORT,BK2425_SPI_CS_PIN,GPIO_MODE_OUT_PP_HIGH_FAST );
  GPIO_Init(BK2425_EN_PORT,BK2425_EN_PIN,GPIO_MODE_OUT_PP_HIGH_FAST );
  GPIO_Init(BK2425_EN_PORT,BK2425_EN_PIN,GPIO_MODE_OUT_PP_HIGH_FAST );

  GPIO_WriteHigh(BK2425_SPI_CS_PORT,BK2425_SPI_CS_PIN);
  GPIO_WriteLow(BK2425_EN_PORT,BK2425_EN_PIN);
}

/*******************************************************************************
* @fn     BK2425_Write_Reg
*
* @brief
*
* @param
*
* @return none
*/
uint8_t BK2425_Write_Reg(uint8_t reg, uint8_t value)
{
  uint8_t status;

  /*cs low*/
  BK2425_SPI_CS_LOW();

  /*write reg*/
  status = SPI_SendByte(reg);

  /*write data*/
  SPI_SendByte(value);

  /*cs high*/
  BK2425_SPI_CS_HIGH();

  return(status);
}

/*******************************************************************************
* @fn     BK2425_Write_Buf
*
* @brief
*
* @param
*
* @return none
*/
uint8_t BK2425_Write_Buf(uint8_t reg, uint32_t *pBuf, uint8_t bytes)
{
  uint8_t status,i = 0;
  /*cs low*/
  BK2425_SPI_CS_LOW();

  /*write reg*/
  status = SPI_SendByte(reg);

  /*write data*/
  for(i = 0; i < bytes; i++)
  {
    SPI_SendByte(*pBuf++);
  }

  /*cs high*/
  BK2425_SPI_CS_HIGH();

  return(status);
}

/*******************************************************************************
* @fn     BK2425_Read_Reg
*
* @brief
*
* @param
*
* @return none
*/
uint8_t BK2425_Read_Reg(uint8_t reg)
{
  uint8_t value;

  BK2425_SPI_CS_LOW();

  SPI_SendByte(reg);

  value = SPI_SendByte(0x00);

  BK2425_SPI_CS_HIGH();

  return(value);
}

/*******************************************************************************
* @fn     BK2425_Read_Buf
*
* @brief
*
* @param
*
* @return none
*/
uint8_t BK2425_Read_Buf(uint8_t reg, uint32_t *pBuf, uint8_t bytes)
{
  uint8_t status,i;

  BK2425_SPI_CS_LOW();

  status = SPI_SendByte(reg);

  for(i = 0;i < bytes; i++)
  {
    pBuf[i] = SPI_SendByte(0x00);
  }

  BK2425_SPI_CS_HIGH();

  return(status);
}

/*******************************************************************************
* @fn     BK2425_RX_Mode
*
* @brief
*
* @param
*
* @return none
*/
void BK2425_RX_Mode(void)
{
  BK2425_EN_LOW();

  BK2425_Write_Buf(WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH);
  BK2425_Write_Reg(WRITE_REG + EN_AA, 0x01);
  BK2425_Write_Reg(WRITE_REG + EN_RXADDR, 0x01);
  BK2425_Write_Reg(WRITE_REG + RF_CH, 40);
  BK2425_Write_Reg(WRITE_REG + RX_PW_P0, TX_PLOAD_WIDTH);
  BK2425_Write_Reg(WRITE_REG + RF_SETUP, 0x0f);
  BK2425_Write_Reg(WRITE_REG + CONFIG, 0x0f);
  BK2425_Write_Reg(WRITE_REG + STATUS, 0xff);

  BK2425_EN_HIGH();
}

/*******************************************************************************
* @fn     BK2425_TX_Mode
*
* @brief
*
* @param
*
* @return none
*/
void BK2425_TX_Mode(void)
{
  BK2425_EN_LOW();

  BK2425_Write_Buf(WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);
  BK2425_Write_Buf(WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH);

  BK2425_Write_Reg(WRITE_REG + EN_AA, 0x01);
  BK2425_Write_Reg(WRITE_REG + EN_RXADDR, 0x01);
  BK2425_Write_Reg(WRITE_REG + SETUP_RETR, 0x0a);
  BK2425_Write_Reg(WRITE_REG + RF_CH, 40);
  BK2425_Write_Reg(WRITE_REG + RF_SETUP, 0x0f);
  BK2425_Write_Reg(WRITE_REG + CONFIG, 0x0e);

  BK2425_EN_HIGH();
}

/*******************************************************************************
* @fn     BK2425_RxPacket
*
* @brief
*
* @param
*
* @return none
*/
uint8_t BK2425_RxPacket(uint32_t *rxbuf)
{
  uint8_t state;

  state = BK2425_Read_Reg(STATUS);

  BK2425_Write_Reg(WRITE_REG + STATUS,state);

  if(state & RX_DR)
  {
    BK2425_Read_Buf(RD_RX_PLOAD,rxbuf,TX_PLOAD_WIDTH);
    BK2425_Write_Reg(FLUSH_RX,0xff);
    return 0;
  }

  return 1;
}

/*******************************************************************************
* @fn     BK2425_TxPacket
*
* @brief
*
* @param
*
* @return none
*/
uint8_t BK2425_TxPacket(uint32_t *txbuf)
{
  u8 state;
  BK2425_EN_LOW();

  BK2425_Write_Buf(WR_TX_PLOAD, txbuf, TX_PLOAD_WIDTH);

  BK2425_EN_HIGH();

  while(GPIO_ReadInputPin(BK2425_IRQ_PORT,BK2425_IRQ_PIN) == 1);

  state = BK2425_Read_Reg(STATUS);

  BK2425_Write_Reg(WRITE_REG + STATUS, state);

  if(state & MAX_RT)
  {
    BK2425_Write_Reg(FLUSH_TX,0xff);
    return MAX_RT;
  }

  if(state&TX_DS)
  {
    return TX_DS;
  }
  return 0XFF;
}