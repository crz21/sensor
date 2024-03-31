#include "includes.h"

uint8_t g_key_state = 0;//����״̬
uint8_t g_key_action_flag = 0;

//����������
//���ذ���ֵ
//mode:0,��֧��������;1,֧��������;
//����ֵ��
//0��û���κΰ�������
//KEY0_PRES��KEY0����
//KEY1_PRES��KEY1����
//WKUP_PRES��WK_UP���� 
//ע��˺�������Ӧ���ȼ�,KEY0>KEY1>WK_UP!!
//g_key_state |00|00|00|00|:00->���  01->ȷ��  10->�ͷ�
uint8_t key_scan(void)
{
    static u8 TimeCnt = 0;
    static u8 lock = 0;
    static u8 key_bit;
    u8 key_bit_buf = 0;

    switch (g_key_state) 
    {
        //����δ����״̬����ʱ�ж�Key��ֵ
    case   KEY_CHECK:               //�������KeyֵΪ0��˵��������ʼ���£�������һ��״̬
        {
            if (!KEY)   
            {
                g_key_state =  KEY_COMFIRM;
                key_bit = (uint8_t)(KEY_CT_ADD | (KEY_CT_REDUCE << 1) | (KEY_DIM_ADD << 2) | (KEY_DIM_REDUCE << 3) | (KEY_TIME_ADD << 4) | (KEY_TIME_REDUCE << 5));
            }
            TimeCnt = 0;                  //������λ
            lock = 0;
        }
        break;

    case   KEY_COMFIRM:                     //�鿴��ǰKey�Ƿ���0���ٴ�ȷ���Ƿ���
        {
            if (!KEY) 
            {                     
                key_bit_buf = (uint8_t)(KEY_CT_ADD | (KEY_CT_REDUCE << 1) | (KEY_DIM_ADD << 2) | (KEY_DIM_REDUCE << 3));//ȷ�ϸ�֮ǰ�����Ƿ�һ��
                if (key_bit_buf == key_bit) 
                {
                    if (!lock)
                    {
                        lock = 1;
                    }

                    TimeCnt++;

                    /*����ʱ���ж�*/
                    if (TimeCnt > 100) 
                    {                                       // ���� 1 s
                        g_key_action_flag = LONG_KEY;
                        TimeCnt = 0;
                        lock = 0;                           //���¼��
                        g_key_state =  KEY_RELEASE;         // ��Ҫ���밴���ͷ�״̬
                    }
                }
                else
                {

                }
            }   
            else                       
            {
                if (lock)
                {                                           // ���ǵ�һ�ν��룬  �ͷŰ�����ִ��
                    g_key_action_flag = SHORT_KEY;          // �̰�
                    g_key_state =  KEY_RELEASE;             // ��Ҫ���밴���ͷ�״̬
                } 
                else 
                {                                           // ��ǰKeyֵΪ1��ȷ��Ϊ�������򷵻���һ��״̬
                    g_key_state =  KEY_CHECK;               // ������һ��״̬
                }
            }
        }
        break;

    case  KEY_RELEASE:                                  //��ǰKeyֵΪ1��˵�������Ѿ��ͷţ����ؿ�ʼ״̬
        {
            if (KEY || (LONG_KEY == g_key_action_flag)) 
            {                     
                g_key_state =  KEY_CHECK;
            }
        }
        break;

    default:
        break;
    }
    return key_bit;
}

/* ������ʼ�� */        
void key_init(void)
{
    hal_key_init();
}

/* �������� */
void key_task(void)
{
//    uint8_t read_key_function = 0;  //��ȡ��ǰ��������
//    uint8_t value_buf1[3] = {0};
//    uint8_t value_buf = 0;
//    uint8_t oled_num[48] = {0};
//    uint8_t i, j, func_code, func_code_len;

//    if (sys_tim_param.sys_tim_10ms_flag)
//    {
//        sys_tim_param.sys_tim_10ms_flag = 0;
//        read_key_function = key_scan();

//        if (KEY_CHECK == g_key_state) 
//        {
//            if (SHORT_KEY == g_key_action_flag) //�̰�����
//            {
//                switch (read_key_function) 
//                {
//                case CT_ADD:
//                    {
//                        g_dim_param.ct_level++;
//                        if (g_dim_param.ct_level > CT_MAX_VALUE) 
//                        {
//                            g_dim_param.ct_level = CT_MAX_VALUE;
//                        }
//                        value_buf = g_dim_param.ct_level;
//                        value_buf1[0] = value_buf / 100;
//                        value_buf1[1] = value_buf % 100 / 10;
//                        value_buf1[2] = value_buf % 10;
//                        for (i = 0; i < 3; i++) 
//                        {
//                            for (j = 0; j < 16; j++) 
//                            {
//                                oled_num[j + i * 16] = font_size_8x16[j + value_buf1[i] * 16];
//                            }
//                        }

//                        oled_half_part(0, 3, oled_num);
//                        func_code = 1;
//                        func_code_len = 3;
//                    }
//                    break;
//                case CT_REDUCE:
//                    {
//                        g_dim_param.ct_level--;
//                        if (g_dim_param.ct_level < CT_MIN_VALUE) 
//                        {
//                            g_dim_param.ct_level = CT_MIN_VALUE;
//                        }
//                        value_buf = g_dim_param.ct_level;
//                        value_buf1[0] = value_buf / 100;
//                        value_buf1[1] = value_buf % 100 / 10;
//                        value_buf1[2] = value_buf % 10;
//                        for (i = 0; i < 3; i++) 
//                        {
//                            for (j = 0; j < 16; j++) 
//                            {
//                                oled_num[j + i * 16] = font_size_8x16[j + value_buf1[i] * 16];
//                            }
//                        }

//                        oled_half_part(0, 3, oled_num);
//                        func_code = 1;
//                        func_code_len = 3;
//                    }
//                    break;
//                case LIGHT_ADD:
//                    {
//                        g_dim_param.light_level++;
//                        if (g_dim_param.light_level > LIGHT_MAX_VALUE) 
//                        {
//                            g_dim_param.light_level = LIGHT_MAX_VALUE;
//                        }
//                        value_buf = g_dim_param.light_level;
//                        value_buf1[0] = value_buf / 100;
//                        value_buf1[1] = value_buf % 100 / 10;
//                        value_buf1[2] = value_buf % 10;
//                        for (i = 0; i < 3; i++) 
//                        {
//                            for (j = 0; j < 16; j++) 
//                            {
//                                oled_num[j + i * 16] = font_size_8x16[j + value_buf1[i] * 16];
//                            }
//                        }
//                        
//                        oled_half_part(4, 3, oled_num);
//                        func_code = 2;
//                        func_code_len = 4;
//                    }
//                    break;
//                case LIGHT_REDUCE:
//                    {
//                        g_dim_param.light_level--;
//                        if (g_dim_param.light_level < LIGHT_MIN_VALUE) 
//                        {
//                            g_dim_param.light_level = LIGHT_MIN_VALUE;
//                        }
//                        value_buf = g_dim_param.light_level;
//                        value_buf1[0] = value_buf / 100;
//                        value_buf1[1] = value_buf % 100 / 10;
//                        value_buf1[2] = value_buf % 10;
//                        for (i = 0; i < 3; i++) 
//                        {
//                            for (j = 0; j < 16; j++) 
//                            {
//                                oled_num[j + i * 16] = font_size_8x16[j + value_buf1[i] * 16];
//                            }
//                        }

//                        oled_half_part(4, 3, oled_num);
//                        func_code = 2;
//                        func_code_len = 4;
//                    }
//                default:
//                    break;
//                }
//                g_key_action_flag = NULL_KEY;
////                send_message(value_buf, func_code, func_code_len);  
//            }
//            else if (LONG_KEY == g_key_action_flag) //��������
//            {
//                 switch (read_key_function) 
//                {
//                case CT_ADD:
//                    {
//                        g_dim_param.ct_level+=5;
//                        if (g_dim_param.ct_level > CT_MAX_VALUE) 
//                        {
//                            g_dim_param.ct_level = CT_MAX_VALUE;
//                        }
//                        value_buf = g_dim_param.ct_level;
//                        value_buf1[0] = value_buf / 100;
//                        value_buf1[1] = value_buf % 100 / 10;
//                        value_buf1[2] = value_buf % 10;
//                        for (i = 0; i < 3; i++)
//                        {
//                            for (j = 0; j < 16; j++) 
//                            {
//                                oled_num[j + i * 16] = font_size_8x16[j + value_buf1[i] * 16];
//                            }
//                        }

//                        oled_half_part(0, 3, oled_num);
//                        func_code = 1;
//                        func_code_len = 3;
//                    }
//                    break;
//                case CT_REDUCE:
//                    {
//                        g_dim_param.ct_level-=5;
//                        if (g_dim_param.ct_level < CT_MIN_VALUE) 
//                        {
//                            g_dim_param.ct_level = CT_MIN_VALUE;
//                        }
//                        value_buf = g_dim_param.ct_level;
//                        value_buf1[0] = value_buf / 100;
//                        value_buf1[1] = value_buf % 100 / 10;
//                        value_buf1[2] = value_buf % 10;
//                        for (i = 0; i < 3; i++) 
//                        {
//                            for (j = 0; j < 16; j++) 
//                            {
//                                oled_num[j + i * 16] = font_size_8x16[j + value_buf1[i] * 16];
//                            }
//                        }

//                        oled_half_part(0, 3, oled_num);
//                        func_code = 1;
//                        func_code_len = 3;
//                    }
//                    break;
//                case LIGHT_ADD:
//                    {
//                        g_dim_param.light_level+=5;
//                        if (g_dim_param.light_level > LIGHT_MAX_VALUE) 
//                        {
//                            g_dim_param.light_level = LIGHT_MAX_VALUE;
//                        }
//                        value_buf = g_dim_param.light_level;
//                        value_buf1[0] = value_buf / 100;
//                        value_buf1[1] = value_buf % 100 / 10;
//                        value_buf1[2] = value_buf % 10;
//                        for (i = 0; i < 3; i++) 
//                        {
//                            for (j = 0; j < 16; j++) 
//                            {
//                                oled_num[j + i * 16] = font_size_8x16[j + value_buf1[i] * 16];
//                            }
//                        }

//                        oled_half_part(4, 3, oled_num);
//                        func_code = 2;
//                        func_code_len = 4;
//                    }
//                    break;
//                case LIGHT_REDUCE:
//                    {
//                        g_dim_param.light_level-=5;
//                        if (g_dim_param.light_level < LIGHT_MIN_VALUE) 
//                        {
//                            g_dim_param.light_level = LIGHT_MIN_VALUE;
//                        }
//                        value_buf = g_dim_param.light_level;
//                        value_buf1[0] = value_buf / 100;
//                        value_buf1[1] = value_buf % 100 / 10;
//                        value_buf1[2] = value_buf % 10;
//                        for (i = 0; i < 3; i++) 
//                        {
//                            for (j = 0; j < 16; j++) 
//                            {
//                                oled_num[j + i * 16] = font_size_8x16[j + value_buf1[i] * 16];
//                            }
//                        }

//                        oled_half_part(4, 3, oled_num);
//                        func_code = 2;
//                        func_code_len = 4;
//                    }
//                    break;
//                default:
//                    break;
//                }
//                 g_key_action_flag = NULL_KEY;
////                 send_message(value_buf, func_code, func_code_len);
//            }
////          value_buf1[0] = value_buf / 100;
////          value_buf1[1] = value_buf % 100 / 10;
////          value_buf1[2] = value_buf % 10;
////          for (i = 0; i < 3; i++)
////          {
////              for (j = 0; j < 16; j++)
////              {
////                  oled_num[j] = font_size_8x16[j + value_buf1[i] * 16];
////              }
////          }
////
////          oled_half_part(2, 3, oled_num);
//        }
//    }
}

