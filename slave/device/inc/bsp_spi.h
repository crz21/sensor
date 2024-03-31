#ifndef __BSP_SPI_H__
#define __BSP_SPI_H__

#define CE_PIN (GPIO_Pin_0)
#define CSN_PIN (GPIO_Pin_12)
#define SCK_PIN (GPIO_Pin_13)
#define MOSI_PIN (GPIO_Pin_15) 
#define MISO_PIN (GPIO_Pin_14)
#define IRQ_PIN (GPIO_Pin_1)
#define SPI_GPIO (GPIOB)

#define NRF24L01_CE_H (GPIO_WriteBit(SPI_GPIO, CE_PIN, Bit_SET))
#define NRF24L01_CE_L (GPIO_WriteBit(SPI_GPIO, CE_PIN, Bit_RESET))
#define NRF24L01_CSN_H (GPIO_WriteBit(SPI_GPIO, CSN_PIN, Bit_SET))
#define NRF24L01_CSN_L (GPIO_WriteBit(SPI_GPIO, CSN_PIN, Bit_RESET))
#define NRF24L01_SCK_H (GPIO_WriteBit(SPI_GPIO, SCK_PIN, Bit_SET))
#define NRF24L01_SCK_L (GPIO_WriteBit(SPI_GPIO, SCK_PIN, Bit_RESET))
#define NRF24L01_MOSI_H (GPIO_WriteBit(SPI_GPIO, MOSI_PIN, Bit_SET))
#define NRF24L01_MOSI_L (GPIO_WriteBit(SPI_GPIO, MOSI_PIN, Bit_RESET))
#define NRF24L01_IRQ_READ (GPIO_ReadInputDataBit(SPI_GPIO, IRQ_PIN))
#define NRF24L01_MISO_READ (GPIO_ReadInputDataBit(SPI_GPIO, MISO_PIN))

void wireless_pin_init(void);
#endif
