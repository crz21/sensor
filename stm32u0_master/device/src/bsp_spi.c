#include "includes.h"

void wireless_pin_init(void)
{
    GPIO_InitTypeDef GPIO_Init;

    __HAL_RCC_GPIOB_CLK_ENABLE();

    GPIO_Init.Pin = CSN_PIN;
    GPIO_Init.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_Init.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(SPI_GPIO, &GPIO_Init);

    GPIO_Init.Pin = SCK_PIN;
    GPIO_Init.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_Init.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(SPI_GPIO, &GPIO_Init);

    GPIO_Init.Pin = MOSI_PIN;
    GPIO_Init.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_Init.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(SPI_GPIO, &GPIO_Init);

    GPIO_Init.Pin = CE_PIN;
    GPIO_Init.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_Init.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(SPI_GPIO, &GPIO_Init);

    GPIO_Init.Pin = MISO_PIN;
    GPIO_Init.Mode = GPIO_MODE_INPUT;
    GPIO_Init.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(SPI_GPIO, &GPIO_Init);

    GPIO_Init.Pin = IRQ_PIN;
    GPIO_Init.Mode = GPIO_MODE_INPUT;
    GPIO_Init.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(SPI_GPIO, &GPIO_Init);
}
