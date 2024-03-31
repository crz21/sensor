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

void hal_sys_tim_init(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); // ʱ��ʹ��
    TIM_DeInit(TIM3);

    // �ж����ȼ�NVIC����
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;           // TIM3�ж�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; // ��ռ���ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;        // �����ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           // IRQͨ����ʹ��
    NVIC_Init(&NVIC_InitStructure);                           // ��ʼ��NVIC�Ĵ���

    // ��ʱ��TIM3��ʼ��
    TIM_TimeBaseStructure.TIM_Period = 500 - 1;                 // ��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
    TIM_TimeBaseStructure.TIM_Prescaler = 71;                   // ����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;     // ����ʱ�ӷָ�:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; // TIM���ϼ���ģʽ
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);             // ����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE); // ʹ��ָ����TIM3�ж�,��������ж�
    TIM_ClearFlag(TIM3, TIM_FLAG_Update);

    TIM_Cmd(TIM3, ENABLE); // ʹ��TIMx
}
