#include "includes.h"

DIM_PARAM g_dim_param;
SYS_TIM_PARAM sys_tim_param;

/* 失能中断 */
void disable_irq(void)
{
    __disable_irq();
}

/* 使能中断 */
void enable_irq(void)
{
    __enable_irq();
}

/* 独立看门狗初始化 */
void iwdg_init(void)
{
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable); // 使能对寄存器IWDG_PR和IWDG_RLR的写操作
    IWDG_SetPrescaler(4);                         // 设置IWDG预分频值:设置IWDG预分频值为64
    IWDG_SetReload(625);                          // 设置IWDG重装载值
    IWDG_ReloadCounter();                         // 按照IWDG重装载寄存器的值重装载IWDG计数器
    IWDG_Enable();                                // 使能IWDG
}

/* 延时初始化 */
void delay_init()
{
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
}

/* 系统定时器初始化 */
void sys_tim_init(void)
{
    hal_sys_tim_init();
}

/* 参数初始化 */
void param_init(void)
{

}

void sys_task(void)
{
    IWDG_ReloadCounter();
}

void TIM3_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM3, TIM_IT_Update))
    {
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);

        sys_tim_param._500us_counter++;

        if (sys_tim_param._500us_counter >= 2)
        {
            sys_tim_param._500us_counter = 0;

            sys_tim_param.send_buf_counter++;
            if (1000 == sys_tim_param.send_buf_counter)
            {
                sys_tim_param.send_buf_counter = 0;
                sys_tim_param.send_buf_flag = 1;
            }

            sys_tim_param.rec_overtime++;
            if (1000 == sys_tim_param.rec_overtime)
            {
                sys_tim_param.rec_overtime = 0;
                sys_tim_param.rec_overtime_flag = 1;
                rec_counter = 0;
                rec_step = FRAME_HEAD_STEP;
                memset(rec_buf, 0, sizeof(rec_buf));
            }
        }
    }
}
