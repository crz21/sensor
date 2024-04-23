#include "includes.h"

uint8_t g_key_state; // 按键状态
uint8_t g_key_action_flag;
uint8_t current_index;
uint8_t key_on;
uint8_t add_flag;
uint16_t g_oled_close_time;

// 按键处理函数
// 返回按键值
// mode:0,不支持连续按;1,支持连续按;
// 返回值：
// 0，没有任何按键按下
// KEY0_PRES，KEY0按下
// KEY1_PRES，KEY1按下
// WKUP_PRES，WK_UP按下
// 注意此函数有响应优先级,KEY0>KEY1>WK_UP!!
// g_key_state |00|00|00|00|:00->检测  01->确认  10->释放
uint8_t key_scan(void)
{
    static u8 TimeCnt = 0;
    static u8 lock = 0;
    static u8 key_bit;
    u8 key_bit_buf = 0;

    switch (g_key_state)
    {
        // 按键未按下状态，此时判断Key的值
    case KEY_CHECK: // 如果按键Key值为0，说明按键开始按下，进入下一个状态
    {
        if (!KEY)
        {
            g_key_state = KEY_COMFIRM;
            key_bit = (uint8_t)(KEY_ENTER | (KEY_UP << 1) | (KEY_DOWN << 2) | (KEY_MENU << 3));
        }
        TimeCnt = 0; // 计数复位
        lock = 0;
    }
    break;

    case KEY_COMFIRM: // 查看当前Key是否还是0，再次确认是否按下
    {
        if (!KEY)
        {
            key_bit_buf = (uint8_t)(KEY_ENTER | (KEY_UP << 1) | (KEY_DOWN << 2) | (KEY_MENU << 3)); // 确认跟之前按键是否一样
            if (key_bit_buf == key_bit)
            {
                if (!lock)
                {
                    lock = 1;
                }

                TimeCnt++;

                /*按键时长判断*/
                if (TimeCnt > 100)
                { // 长按 1 s
                    g_key_action_flag = LONG_KEY;
                    TimeCnt = 0;
                    lock = 0;                  // 重新检查
                    g_key_state = KEY_RELEASE; // 需要进入按键释放状态
                }
            }
            else
            {
            }
        }
        else
        {
            if (lock)
            {                                  // 不是第一次进入，  释放按键才执行
                g_key_action_flag = SHORT_KEY; // 短按
                g_key_state = KEY_RELEASE;     // 需要进入按键释放状态
            }
            else
            {                            // 当前Key值为1，确认为抖动，则返回上一个状态
                g_key_state = KEY_CHECK; // 返回上一个状态
            }
        }
    }
    break;

    case KEY_RELEASE: // 当前Key值为1，说明按键已经释放，返回开始状态
    {
        if (KEY || (LONG_KEY == g_key_action_flag))
        {
            g_key_state = KEY_CHECK;
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
    uint8_t read_key_function = 0; // 读取当前按键功能
    uint8_t flash_data[10] = {0};

    if (sys_tim_param.sys_tim_10ms_flag)
    {
        sys_tim_param.sys_tim_10ms_flag = 0;
        read_key_function = key_scan();

        if (KEY_CHECK == g_key_state)
        {
            if (SHORT_KEY == g_key_action_flag) // 短按按键
            {
                sys_tim_param.sys_tim_1min_counter = 0;
                oled_send_cmd(0xAF);
                key_on = 1;
                switch (read_key_function)
                {
                case ENTER_KEY:
                {
                    if (FATORY_CONFIRM_PAGE == current_index)
                    {
                        Flash_Read(FLASH_SAVE_DATA_ADDR, flash_data, sizeof(flash_data));
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
                    else if (SENSOR_PARAM_CONFIRM_PAGE == current_index)
                    {
                        Flash_Read(FLASH_SAVE_DATA_ADDR, flash_data, sizeof(flash_data));
                        flash_data[2] = distance;
                        flash_data[3] = sensitivity;
                        flash_data[4] = transmitted_power;

                        Flash_Write(FLASH_SAVE_DATA_ADDR, flash_data, sizeof(flash_data));
                    }
                    else if (OLED_PARAM_CONFIRM_PAGE == current_index)
                    {
                        Flash_Read(FLASH_SAVE_DATA_ADDR, flash_data, sizeof(flash_data));
                        flash_data[5] = oled_light;
                        flash_data[6] = oled_close_time;

                        Flash_Write(FLASH_SAVE_DATA_ADDR, flash_data, sizeof(flash_data));
                        
                        oled_send_cmd(0x81);
                        oled_send_cmd(oled_light);
                        g_oled_close_time = oled_close_time * 100;
                    }
                    current_index = table_op[current_index].enter;
                }
                break;

                case UP_KEY:
                {
                    current_index = table_op[current_index].up;
                }
                break;

                case DOWN_KEY:
                {
                    if ((SENSOR_PARAM_PAGE_1 <= current_index && current_index <= SENSOR_PARAM_PAGE_3) ||
                        (OLED_PARAM_PAGE_1 <= current_index && current_index <= OLED_PARAM_PAGE_2))
                    {
                        add_flag = 1;
                    }
                    else
                    {
                        current_index = table_op[current_index].down;
                    }
                }
                break;

                case MENU_KEY:
                {
                    if (SENSOR_PARAM_CONFIRM_PAGE == current_index ||
                        SENSOR_PARAM_PAGE_1 == current_index ||
                        SENSOR_PARAM_PAGE_2 == current_index ||
                        SENSOR_PARAM_PAGE_3 == current_index ||
                        OLED_PARAM_CONFIRM_PAGE == current_index ||
                        OLED_PARAM_PAGE_1 == current_index ||
                        OLED_PARAM_PAGE_2 == current_index)
                    {
                        Flash_Read(FLASH_SAVE_DATA_ADDR, flash_data, sizeof(flash_data));
                        distance = flash_data[2];
                        sensitivity = flash_data[3];
                        transmitted_power = flash_data[4];
                        oled_light = flash_data[5];
                        oled_close_time = flash_data[6];
                    }
                    current_index = MAIN_PAGE;
                }
                break;

                default:
                    break;
                }
            }
            g_key_action_flag = NULL_KEY;
        }
        else if (LONG_KEY == g_key_action_flag) // 长按按键
        {
            g_key_action_flag = NULL_KEY;
        }
    }
}
