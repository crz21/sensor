#include "includes.h"

int main(void)
{
    delay_init();

    delay_ms(150);

    disable_irq();

    sys_tim_init();

    uart_init();

    wireless_init();

    // iwdg_init();

    enable_irq();

    while (1)
    {
        sys_task();

        wireless_task();

        uart_task();
    }
}
