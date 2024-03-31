#include "includes.h"

uint8_t g_key_state = 0;//按键状态
uint8_t g_key_action_flag = 0;

//按键处理函数
//返回按键值
//mode:0,不支持连续按;1,支持连续按;
//返回值：
//0，没有任何按键按下
//KEY0_PRES，KEY0按下
//KEY1_PRES，KEY1按下
//WKUP_PRES，WK_UP按下 
//注意此函数有响应优先级,KEY0>KEY1>WK_UP!!
//g_key_state |00|00|00|00|:00->检测  01->确认  10->释放
uint8_t key_scan(void)
{
    static u8 TimeCnt = 0;
    static u8 lock = 0;
    static u8 key_bit;
    u8 key_bit_buf = 0;

    switch (g_key_state) 
    {
        //按键未按下状态，此时判断Key的值
    case   KEY_CHECK:               //如果按键Key值为0，说明按键开始按下，进入下一个状态
        {
            if (!KEY)   
            {
                g_key_state =  KEY_COMFIRM;
                key_bit = (uint8_t)(KEY_CT_ADD | (KEY_CT_REDUCE << 1) | (KEY_DIM_ADD << 2) | (KEY_DIM_REDUCE << 3) | (KEY_TIME_ADD << 4) | (KEY_TIME_REDUCE << 5));
            }
            TimeCnt = 0;                  //计数复位
            lock = 0;
        }
        break;

    case   KEY_COMFIRM:                     //查看当前Key是否还是0，再次确认是否按下
        {
            if (!KEY) 
            {                     
                key_bit_buf = (uint8_t)(KEY_CT_ADD | (KEY_CT_REDUCE << 1) | (KEY_DIM_ADD << 2) | (KEY_DIM_REDUCE << 3));//确认跟之前按键是否一样
                if (key_bit_buf == key_bit) 
                {
                    if (!lock)
                    {
                        lock = 1;
                    }

                    TimeCnt++;

                    /*按键时长判断*/
                    if (TimeCnt > 100) 
                    {                                       // 长按 1 s
                        g_key_action_flag = LONG_KEY;
                        TimeCnt = 0;
                        lock = 0;                           //重新检查
                        g_key_state =  KEY_RELEASE;         // 需要进入按键释放状态
                    }
                }
                else
                {

                }
            }   
            else                       
            {
                if (lock)
                {                                           // 不是第一次进入，  释放按键才执行
                    g_key_action_flag = SHORT_KEY;          // 短按
                    g_key_state =  KEY_RELEASE;             // 需要进入按键释放状态
                } 
                else 
                {                                           // 当前Key值为1，确认为抖动，则返回上一个状态
                    g_key_state =  KEY_CHECK;               // 返回上一个状态
                }
            }
        }
        break;

    case  KEY_RELEASE:                                  //当前Key值为1，说明按键已经释放，返回开始状态
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

/* 按键初始化 */        
void key_init(void)
{
    hal_key_init();
}

/* 按键任务 */
void key_task(void)
{
//    uint8_t read_key_function = 0;  //读取当前按键功能
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
//            if (SHORT_KEY == g_key_action_flag) //短按按键
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
//            else if (LONG_KEY == g_key_action_flag) //长按按键
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

