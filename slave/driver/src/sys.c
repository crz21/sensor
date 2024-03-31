#include "includes.h"

DIM_PARAM g_dim_param;
SYS_TIM_PARAM sys_tim_param;

/* ʧ���ж� */
void disable_irq(void)
{
    __disable_irq();
}

/* ʹ���ж� */
void enable_irq(void)
{
    __enable_irq();
}

/* �������Ź���ʼ�� */
void iwdg_init(void)
{
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable); // ʹ�ܶԼĴ���IWDG_PR��IWDG_RLR��д����
    IWDG_SetPrescaler(4);                         // ����IWDGԤ��Ƶֵ:����IWDGԤ��ƵֵΪ64
    IWDG_SetReload(625);                          // ����IWDG��װ��ֵ
    IWDG_ReloadCounter();                         // ����IWDG��װ�ؼĴ�����ֵ��װ��IWDG������
    IWDG_Enable();                                // ʹ��IWDG
}

/* ��ʱ��ʼ�� */
void delay_init()
{
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
}

/* ϵͳ��ʱ����ʼ�� */
void sys_tim_init(void)
{
    hal_sys_tim_init();
}

/* ������ʼ�� */
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
