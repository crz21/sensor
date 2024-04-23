#include "includes.h"

uint8_t g_key_state; // ����״̬
uint8_t g_key_action_flag;
uint8_t current_index;
uint8_t key_on;
uint8_t add_flag;
uint16_t g_oled_close_time;

// ����������
// ���ذ���ֵ
// mode:0,��֧��������;1,֧��������;
// ����ֵ��
// 0��û���κΰ�������
// KEY0_PRES��KEY0����
// KEY1_PRES��KEY1����
// WKUP_PRES��WK_UP����
// ע��˺�������Ӧ���ȼ�,KEY0>KEY1>WK_UP!!
// g_key_state |00|00|00|00|:00->���  01->ȷ��  10->�ͷ�
uint8_t key_scan(void)
{
    static u8 TimeCnt = 0;
    static u8 lock = 0;
    static u8 key_bit;
    u8 key_bit_buf = 0;

    switch (g_key_state)
    {
        // ����δ����״̬����ʱ�ж�Key��ֵ
    case KEY_CHECK: // �������KeyֵΪ0��˵��������ʼ���£�������һ��״̬
    {
        if (!KEY)
        {
            g_key_state = KEY_COMFIRM;
            key_bit = (uint8_t)(KEY_ENTER | (KEY_UP << 1) | (KEY_DOWN << 2) | (KEY_MENU << 3));
        }
        TimeCnt = 0; // ������λ
        lock = 0;
    }
    break;

    case KEY_COMFIRM: // �鿴��ǰKey�Ƿ���0���ٴ�ȷ���Ƿ���
    {
        if (!KEY)
        {
            key_bit_buf = (uint8_t)(KEY_ENTER | (KEY_UP << 1) | (KEY_DOWN << 2) | (KEY_MENU << 3)); // ȷ�ϸ�֮ǰ�����Ƿ�һ��
            if (key_bit_buf == key_bit)
            {
                if (!lock)
                {
                    lock = 1;
                }

                TimeCnt++;

                /*����ʱ���ж�*/
                if (TimeCnt > 100)
                { // ���� 1 s
                    g_key_action_flag = LONG_KEY;
                    TimeCnt = 0;
                    lock = 0;                  // ���¼��
                    g_key_state = KEY_RELEASE; // ��Ҫ���밴���ͷ�״̬
                }
            }
            else
            {
            }
        }
        else
        {
            if (lock)
            {                                  // ���ǵ�һ�ν��룬  �ͷŰ�����ִ��
                g_key_action_flag = SHORT_KEY; // �̰�
                g_key_state = KEY_RELEASE;     // ��Ҫ���밴���ͷ�״̬
            }
            else
            {                            // ��ǰKeyֵΪ1��ȷ��Ϊ�������򷵻���һ��״̬
                g_key_state = KEY_CHECK; // ������һ��״̬
            }
        }
    }
    break;

    case KEY_RELEASE: // ��ǰKeyֵΪ1��˵�������Ѿ��ͷţ����ؿ�ʼ״̬
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

/* ������ʼ�� */
void key_init(void)
{
    hal_key_init();
}

/* �������� */
void key_task(void)
{
    uint8_t read_key_function = 0; // ��ȡ��ǰ��������
    uint8_t flash_data[10] = {0};

    if (sys_tim_param.sys_tim_10ms_flag)
    {
        sys_tim_param.sys_tim_10ms_flag = 0;
        read_key_function = key_scan();

        if (KEY_CHECK == g_key_state)
        {
            if (SHORT_KEY == g_key_action_flag) // �̰�����
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
        else if (LONG_KEY == g_key_action_flag) // ��������
        {
            g_key_action_flag = NULL_KEY;
        }
    }
}
