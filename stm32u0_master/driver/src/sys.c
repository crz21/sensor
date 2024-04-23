#include "includes.h"

DIM_PARAM g_dim_param;
SYS_TIM_PARAM sys_tim_param;
uint8_t poweron_flag;
uint8_t flash_data[10];

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
  uint8_t flash_data[10];

  poweron_flag = 1;
  current_index = 0;
  key_on = 1;
  Flash_Read(FLASH_SAVE_DATA_ADDR, flash_data, sizeof(flash_data));
  if (flash_data[0] != 0x55 || flash_data[1] != 0xA8)
  {
    flash_data[0] = 0x55;
    flash_data[1] = 0xA8;
    distance = 100;
    flash_data[2] = 100;
    sensitivity = 50;
    flash_data[3] = 50;
    transmitted_power = 100;
    flash_data[4] = 100;
    oled_light = 0xEF;
    flash_data[5] = 0xEF;
    oled_close_time = 30;
    flash_data[6] = 30;
    Flash_Write(FLASH_SAVE_DATA_ADDR, flash_data, sizeof(flash_data));
  }
  else
  {
    distance = flash_data[2];
    sensitivity = flash_data[3];
    transmitted_power = flash_data[4];
    oled_light = flash_data[5];
    oled_close_time = flash_data[6];
  }
}

void sys_task(void)
{
  IWDG_ReloadCounter();
  if (current_index == 0 || current_index == 1)
  {
    sys_tim_param.sys_tim_1min_counter = 0;
  }

  if (sys_tim_param.sys_tim_1min_flag)
  {
    sys_tim_param.sys_tim_1min_flag = 0;
    key_on = 1;
    current_index = 1;
    oled_clear();
  }
}

/* ϵͳ��ʱ���ж� */
void TIM3_IRQHandler(void)
{
  if (TIM_GetITStatus(TIM3, TIM_IT_Update))
  {
    TIM_ClearITPendingBit(TIM3, TIM_IT_Update);

    sys_tim_param.sys_tim_500us_flag = 1; // 500us��־
    sys_tim_param.sys_tim_500us_counter++;

    if (sys_tim_param.sys_tim_500us_counter >= 2)
    {
      sys_tim_param.sys_tim_500us_counter = 0;
      sys_tim_param.sys_tim_1ms_flag = 1; // 1ms��־
      sys_tim_param.sys_tim_1ms_counter++;
      sys_tim_param.rec_overtime++;
      if (1000 == sys_tim_param.rec_overtime)
      {
        sys_tim_param.rec_overtime = 0;
        sys_tim_param.rec_overtime_flag = 1;
        rec_counter = 0;
        rec_step = FRAME_HEAD_STEP;
        memset(rec_buf, 0, sizeof(rec_buf));
      }

      if (sys_tim_param.sys_tim_1ms_counter >= 2)
      {
        sys_tim_param.sys_tim_1ms_counter = 0;

        sys_tim_param.sys_tim_2ms_flag = 1; // 2ms��־
        sys_tim_param.sys_tim_2ms_counter++;

        if (sys_tim_param.sys_tim_2ms_counter >= 5)
        {
          sys_tim_param.sys_tim_2ms_counter = 0;

          sys_tim_param.sys_tim_10ms_flag = 1; // 10ms��־
          sys_tim_param.sys_tim_10ms_counter++;

          if (sys_tim_param.sys_tim_10ms_counter >= 10)
          {
            sys_tim_param.sys_tim_10ms_counter = 0;
            sys_tim_param.sys_tim_100ms_flag = 1; // 100ms��־
            sys_tim_param.sys_tim_100ms_counter++;

            if (sys_tim_param.sys_tim_100ms_counter >= 5)
            {
              sys_tim_param.sys_tim_100ms_counter = 0;
              sys_tim_param.sys_tim_500ms_flag = 1; // 1s��־
              sys_tim_param.sys_tim_1min_counter++;
              if (sys_tim_param.sys_tim_1min_counter >= 10)
              {
                sys_tim_param.sys_tim_1min_counter = 0;
                sys_tim_param.sys_tim_1min_flag = 1;
              }
            }
          }
        }
      }
    }
  }
}
