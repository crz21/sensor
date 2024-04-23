#include "includes.h"

void bsp_key_init(void)
{
    GPIO_InitTypeDef GPIO_Init;

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    GPIO_Init.Pin = GPIO_PIN_2; // CT+
    GPIO_Init.Mode = GPIO_MODE_INPUT;
    HAL_GPIO_Init(GPIOA, &GPIO_Init);

    GPIO_Init.Pin = GPIO_PIN_3; // CT-
    GPIO_Init.Mode = GPIO_MODE_INPUT;
    HAL_GPIO_Init(GPIOA, &GPIO_Init);

    GPIO_Init.Pin = GPIO_PIN_4; // DIM+
    GPIO_Init.Mode = GPIO_MODE_INPUT;
    HAL_GPIO_Init(GPIOA, &GPIO_Init);

    GPIO_Init.Pin = GPIO_PIN_5; // DIM-
    GPIO_Init.Mode = GPIO_MODE_INPUT;
    HAL_GPIO_Init(GPIOA, &GPIO_Init);

    GPIO_Init.Pin = GPIO_PIN_1; // TIME+
    GPIO_Init.Mode = GPIO_MODE_INPUT;
    HAL_GPIO_Init(GPIOB, &GPIO_Init);

    GPIO_Init.Pin = GPIO_PIN_0; // TIME-
    GPIO_Init.Mode = GPIO_MODE_INPUT;
    HAL_GPIO_Init(GPIOB, &GPIO_Init);
}

