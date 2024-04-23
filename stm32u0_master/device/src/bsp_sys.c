#include "includes.h"

void delay_us(u32 nus)
{
    u32 temp;
    SysTick->LOAD = nus * FAC_US;
    SysTick->VAL = 0x00;
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
    do
    {
        temp = SysTick->CTRL;
    } while ((temp & 0x01) && !(temp & (1 << 16)));
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
    SysTick->VAL = 0X00;
}

void delay_ms(u16 nms)
{
    u32 temp;
    SysTick->LOAD = (u32)nms * FAC_MS;
    SysTick->VAL = 0x00;
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
    do
    {
        temp = SysTick->CTRL;
    } while ((temp & 0x01) && !(temp & (1 << 16)));
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
    SysTick->VAL = 0X00;
}

void bsp_sys_tim_init(void)
{
    if (HAL_SYSTICK_Config(SystemCoreClock / (1000U / uwTickFreq)) == 0U)
    {
        /* Configure the SysTick IRQ priority */
        if (TickPriority < (1UL << __NVIC_PRIO_BITS))
        {
            HAL_NVIC_SetPriority(SysTick_IRQn, TickPriority, 0U);
            uwTickPrio = TickPriority;
        }
    }
}
