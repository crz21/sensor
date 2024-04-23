#include "includes.h"

uint8_t distance;
uint8_t sensitivity;
uint8_t transmitted_power;
uint8_t oled_light;
uint16_t oled_close_time;
uint8_t pre_index;

_draw_word_op welcome_page_op[] = {
    {2, 4, "��ӭʹ��", 4, &draw_part},
    {5, 3, "�밴ȷ�ϼ�", 5, &draw_part},
};
_draw_word_op main_page_op[] = {
    {0, 0, "״̬", 2, &draw_part},
    {3, 0, "���˾���", 4, &draw_part},
    {6, 0, "���˾���", 4, &draw_part},
};
_draw_word_op set_page1_op[] = {
    {0, 1, "��������", 4, &draw_part},
    {3, 1, "��Ļ����", 4, &draw_part},
    {6, 1, "�汾", 2, &draw_part},
};
_draw_word_op set_page2_op[] = {
    {0, 1, "�ָ�Ĭ��", 4, &draw_part},
};
_draw_word_op sensor_param_page_op[] = {
    {0, 1, "����", 2, &draw_part},
    {3, 1, "������", 3, &draw_part},
    {6, 1, "���书��", 4, &draw_part},
};
_draw_word_op oled_param_page_op[] = {
    {0, 1, "����", 2, &draw_part},
    {3, 1, "Ϣ��ʱ��", 4, &draw_part},
};
_draw_word_op param_word_op[] = {
    {0, 7, "û��Ŀ��", 4, &draw_part},
    {0, 7, "�˶�Ŀ��", 4, &draw_part},
    {0, 7, "��ֹĿ��", 4, &draw_part},
    {0, 7, "�˶���ֹ", 4, &draw_part},
};
_table_op table_op[MAX_INDEX] = {
    /* ��ӭ�˵� */
    {WELCOME_PAGE, WELCOME_PAGE, WELCOME_PAGE, MAIN_PAGE, WELCOME_PAGE, WELCOME_PAGE, &draw_welcom_page},

    /* ������ */
    {MAIN_PAGE, MAIN_PAGE, MAIN_PAGE, SET_PAGE_1, MAIN_PAGE, MAIN_PAGE, &draw_main_page},

    /* 1���˵� */
    {SET_PAGE_1, SET_PAGE_2, SET_PAGE_4, SENSOR_PARAM_PAGE_1, SET_PAGE_1, SET_PAGE_3, &draw_set_page},
    {SET_PAGE_2, SET_PAGE_3, SET_PAGE_1, OLED_PARAM_PAGE_1, SET_PAGE_1, SET_PAGE_3, &draw_set_page},
    {SET_PAGE_3, SET_PAGE_4, SET_PAGE_2, VISION_PAGE, SET_PAGE_1, SET_PAGE_3, &draw_set_page},

    {SET_PAGE_4, SET_PAGE_1, SET_PAGE_3, FATORY_CONFIRM_PAGE, SET_PAGE_4, SET_PAGE_4, &draw_set_page},

    /* 2���˵� */
    {SENSOR_PARAM_PAGE_1, SENSOR_PARAM_PAGE_2, SENSOR_PARAM_PAGE_1, SENSOR_PARAM_CONFIRM_PAGE, SENSOR_PARAM_PAGE_1, SENSOR_PARAM_PAGE_3, &draw_sensor_param_page},
    {SENSOR_PARAM_PAGE_2, SENSOR_PARAM_PAGE_3, SENSOR_PARAM_PAGE_2, SENSOR_PARAM_CONFIRM_PAGE, SENSOR_PARAM_PAGE_1, SENSOR_PARAM_PAGE_3, &draw_sensor_param_page},
    {SENSOR_PARAM_PAGE_3, SENSOR_PARAM_PAGE_1, SENSOR_PARAM_PAGE_3, SENSOR_PARAM_CONFIRM_PAGE, SENSOR_PARAM_PAGE_1, SENSOR_PARAM_PAGE_3, &draw_sensor_param_page},
    {OLED_PARAM_PAGE_1, OLED_PARAM_PAGE_2, OLED_PARAM_PAGE_1, OLED_PARAM_CONFIRM_PAGE, OLED_PARAM_PAGE_1, OLED_PARAM_PAGE_2, &draw_oled_param_page},
    {OLED_PARAM_PAGE_2, OLED_PARAM_PAGE_1, OLED_PARAM_PAGE_2, OLED_PARAM_CONFIRM_PAGE, OLED_PARAM_PAGE_1, OLED_PARAM_PAGE_2, &draw_oled_param_page},
    {VISION_PAGE, VISION_PAGE, VISION_PAGE, SET_PAGE_3, VISION_PAGE, VISION_PAGE, &draw_vision_page},

    /* ȷ�ϲ˵� */
    {SENSOR_PARAM_CONFIRM_PAGE, SENSOR_PARAM_CONFIRM_PAGE, SENSOR_PARAM_CONFIRM_PAGE, SENSOR_PARAM_PAGE_1, SENSOR_PARAM_CONFIRM_PAGE, SENSOR_PARAM_CONFIRM_PAGE, &draw_param_confirm_page},
    {OLED_PARAM_CONFIRM_PAGE, OLED_PARAM_CONFIRM_PAGE, OLED_PARAM_CONFIRM_PAGE, OLED_PARAM_PAGE_1, OLED_PARAM_CONFIRM_PAGE, OLED_PARAM_CONFIRM_PAGE, &draw_param_confirm_page},
    {FATORY_CONFIRM_PAGE, FATORY_CONFIRM_PAGE, FATORY_CONFIRM_PAGE, SET_PAGE_4, FATORY_CONFIRM_PAGE, FATORY_CONFIRM_PAGE, &draw_fatory_confirm_page},
};

void oled_send_cmd(uint8_t o_command)
{
    IIC_Start();
    IIC_Send_Byte(DEV_OLED, 0x78);
    IIC_Send_Byte(DEV_OLED, 0x00);
    IIC_Send_Byte(DEV_OLED, o_command);
    IIC_Stop();
}

void oled_send_data(uint8_t o_data)
{
    IIC_Start();
    IIC_Send_Byte(DEV_OLED, 0x78);
    IIC_Send_Byte(DEV_OLED, 0x40);
    IIC_Send_Byte(DEV_OLED, o_data);
    IIC_Stop();
}

void column_set(uint8_t column)
{
    oled_send_cmd(0x10 | (column >> 4));   // �����е�ַ��λ
    oled_send_cmd(0x00 | (column & 0x0f)); // �����е�ַ��λ
}

void page_set(uint8_t page)
{
    oled_send_cmd(0xb0 + page);
}

void draw_half_words(uint8_t page_offset, uint8_t column_offset, uint8_t *half_word, uint8_t half_word_num)
{
    uint8_t page, column, column_word;

    for (column_word = 0; column_word < half_word_num; column_word++)
    {
        for (page = (page_offset); page < (page_offset + 2); page++)
        {
            page_set(page);
            column_set(8 * column_word + column_offset * 8);
            for (column = 0; column < 8; column++)
            {
                oled_send_data(*half_word++);
            }
        }
    }
}

void draw_words(uint8_t page_offset, uint8_t column_offset, uint8_t *word, uint8_t word_num)
{
    uint8_t page, column, column_word;

    for (column_word = 0; column_word < word_num; column_word++)
    {
        for (page = (page_offset); page < (page_offset + 2); page++)
        {
            page_set(page);
            column_set(16 * column_word + column_offset * 8);
            for (column = 0; column < 16; column++)
            {
                oled_send_data(*word++);
            }
        }
    }
}

void oled_clear(void)
{
    uint8_t page, column;

    for (page = 0; page < (MAX_PAGE / 8); page++) // page loop
    {
        page_set(page);
        column_set(0);
        for (column = 0; column < MAX_COLUMN; column++) // column loop
        {
            oled_send_data(0);
        }
    }
}

void clear_part(uint8_t page_offset, uint8_t column_offset, char *str, uint8_t word_len)
{
    uint8_t strw[512] = {0};
    uint16_t j, k;

    for (k = 0; k < word_len; k++)
    {
        for (j = 0; j < 32; j++)
        {
            strw[k * 32 + j] = 0;
        }
    }
    draw_words(page_offset, column_offset, strw, word_len);
}

void clear_half_part(uint8_t page_offset, uint8_t column_offset, char *str, uint8_t word_len)
{
    uint8_t strw[512] = {0};
    uint16_t j, k;

    for (k = 0; k < word_len; k++)
    {
        for (j = 0; j < 8; j++)
        {
            strw[k * 8 + j] = 0;
        }
    }
    draw_half_words(page_offset, column_offset, strw, word_len);
}

void draw_part(uint8_t page_offset, uint8_t column_offset, char *str, uint8_t word_len)
{
    uint8_t strw[512] = {0};
    uint16_t i, j, k;

    for (k = 0; k < word_len; k++)
    {
        for (i = 0; i < sizeof(gbk_code); i++)
        {
            if (0 == memcmp(str + k * 2, gbk_code + i * 2, 2))
            {
                for (j = 0; j < 32; j++)
                {
                    strw[k * 32 + j] = font_size_16x16[i * 32 + j];
                }
            }
        }
    }
    draw_words(page_offset, column_offset, strw, word_len);
}

void draw_half_part(uint8_t page_offset, uint8_t column_offset, char *str, uint8_t word_len)
{
    uint8_t strw[512] = {0};
    uint16_t i, j, k;

    for (k = 0; k < word_len; k++)
    {
        for (i = 0; i < sizeof(gbk_size_8x16_code); i++)
        {
            if (0 == memcmp(str + k, gbk_size_8x16_code + i, 1))
            {
                for (j = 0; j < 16; j++)
                {
                    strw[k * 16 + j] = font_size_8x16[i * 16 + j];
                }
            }
        }
    }
    draw_half_words(page_offset, column_offset, strw, word_len);
}

void oled_init(void)
{
    uint8_t i;
    const uint8_t OLED_init_cmd[25] = {
        0xAE, // �ر���ʾ
        0xD5, // ����ʱ�ӷ�Ƶ����,��Ƶ��
        0x80, //[3:0],��Ƶ����;[7:4],��Ƶ��
        0xA8, // ��������·��
        0X3F, // Ĭ��0X3F(1/64)
        0xD3, // ������ʾƫ��
        0X00, // Ĭ��Ϊ0
        0x40, // ������ʾ��ʼ�� [5:0],����.
        0x8D, // ��ɱ�����
        0x14, // bit2������/�ر�
        0x20, // �����ڴ��ַģʽ
        0x02, //[1:0],00���е�ַģʽ;01���е�ַģʽ;10,ҳ��ַģʽ;Ĭ��10;
        0xA1, // ���ض�������,bit0:0,0->0;1,0->127;
        0xC8, // ����COMɨ�跽��;bit3:0,��ͨģʽ;1,�ض���ģʽ COM[N-1]->COM0;N:����·��
        0xDA, // ����COMӲ����������
        0x12, //[5:4]����
        0x81, // �Աȶ�����
        0xEF, // 1~255;Ĭ��0X7F (��������,Խ��Խ��)
        0xD9, // ����Ԥ�������
        0xf1, //[3:0],PHASE 1;[7:4],PHASE 2;
        0xDB, // ����VCOMH ��ѹ����
        0x30, //[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;
        0xA4, // ȫ����ʾ����;bit0:1,����;0,�ر�;(����/����)
        0xA6, // ������ʾ��ʽ;bit0:1,������ʾ;0,������ʾ
        0xAF, // ������ʾ
    };

    hal_i2c_init();

    for (i = 0; i < 25; i++)
    {
        oled_send_cmd(OLED_init_cmd[i]);
    }

    oled_clear();
}

void val_to_str(uint16_t data, uint8_t *output_data)
{
    uint8_t value_buf1[4] = {0};
    uint16_t value_buf = 0;
    uint8_t j, i;

    value_buf = data;
    value_buf1[0] = value_buf / 10000;
    value_buf1[1] = value_buf % 1000 / 100;
    value_buf1[2] = value_buf % 100 / 10;
    value_buf1[3] = value_buf % 10;
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 16; j++)
        {
            output_data[j + i * 16] = font_size_8x16[j + value_buf1[i] * 16];
        }
    }
}

void draw_welcom_page(void)
{
    uint8_t i;

    if (pre_index != current_index)
    {
        oled_clear();
    }

    pre_index = current_index;
    for (i = 0; i < sizeof(welcome_page_op) / sizeof(welcome_page_op[0]); i++)
    {
        welcome_page_op[i].dram_func(welcome_page_op[i].page_offset,
                                     welcome_page_op[i].column_offset,
                                     welcome_page_op[i].ptr_word,
                                     welcome_page_op[i].word_len);
    }
}

void draw_main_page(void)
{
    uint8_t i, sta, j;
    uint8_t oled_num[64] = {0};
    uint16_t rec_data;

    if (pre_index != current_index)
    {
        oled_clear();
    }
    pre_index = current_index;

    for (i = 0; i < sizeof(main_page_op) / sizeof(main_page_op[0]); i++)
    {
        main_page_op[i].dram_func(main_page_op[i].page_offset,
                                  main_page_op[i].column_offset,
                                  main_page_op[i].ptr_word,
                                  main_page_op[i].word_len);
    }

    sta = test_get_buf[2];
    param_word_op[sta].dram_func(param_word_op[sta].page_offset,
                                 param_word_op[sta].column_offset,
                                 param_word_op[sta].ptr_word,
                                 param_word_op[sta].word_len);

    for (i = 3, j = 3; i < 6; i += 2, j += 3)
    {
        if ((uint16_t)(test_get_buf[i] | test_get_buf[i + 1] << 8) >= ADJUST_VALUE)
        {
            rec_data = (uint16_t)(test_get_buf[i] | test_get_buf[i + 1] << 8) - ADJUST_VALUE;
        }
        else
        {
            rec_data = 0;
        }
        val_to_str(rec_data, oled_num);
        draw_half_words(j, 9, oled_num, 4);
    }
}

void draw_set_page(void)
{
    uint8_t i;
    uint8_t min_page, max_page;
    uint8_t len;
    _draw_word_op *set_page_op;

    min_page = table_op[current_index].min_page;
    max_page = table_op[current_index].max_page;

    if (SET_PAGE_1 == min_page)
    {
        len = sizeof(set_page1_op) / sizeof(set_page1_op[0]);
        set_page_op = set_page1_op;
    }
    else if (SET_PAGE_4 == min_page)
    {
        len = sizeof(set_page2_op) / sizeof(set_page2_op[0]);
        set_page_op = set_page2_op;
    }

    if ((pre_index < min_page) || (pre_index > max_page))
    {
        oled_clear();
    }
    pre_index = current_index;

    for (i = 0; i < len * 2 + 1; i += 3)
    {
        clear_half_part(i, 0, 0, 1);
    }

    draw_half_part((current_index - min_page) * 3, 0, ">", 1);

    for (i = 0; i < len; i++)
    {
        set_page_op[i].dram_func(set_page_op[i].page_offset,
                                  set_page_op[i].column_offset,
                                  set_page_op[i].ptr_word,
                                  set_page_op[i].word_len);
    }
}

void draw_sensor_param_page(void)
{
    uint8_t i;
    uint8_t oled_num[64] = {0};

    if ((pre_index < SENSOR_PARAM_PAGE_1) || (pre_index > SENSOR_PARAM_PAGE_3))
    {
        oled_clear();
    }
    pre_index = current_index;

    for (i = 0; i < sizeof(sensor_param_page_op) / sizeof(sensor_param_page_op[0]) * 2 + 1; i += 3)
    {
        if (((current_index - SENSOR_PARAM_PAGE_1) * 3) != i)
        {
            clear_half_part(i, 0, 0, 1);
        }
    }

    draw_half_part((current_index - SENSOR_PARAM_PAGE_1) * 3, 0, ">", 1);
    for (i = 0; i < sizeof(sensor_param_page_op) / sizeof(sensor_param_page_op[0]); i++)
    {
        sensor_param_page_op[i].dram_func(sensor_param_page_op[i].page_offset,
                                          sensor_param_page_op[i].column_offset,
                                          sensor_param_page_op[i].ptr_word,
                                          sensor_param_page_op[i].word_len);
    }

    if (add_flag)
    {
        switch (current_index)
        {
        case SENSOR_PARAM_PAGE_1:
            distance++;
            if (200 <= distance)
            {
                distance = 0;
            }
            break;

        case SENSOR_PARAM_PAGE_2:
            sensitivity++;
            if (100 <= sensitivity)
            {
                sensitivity = 0;
            }
            break;

        case SENSOR_PARAM_PAGE_3:
            transmitted_power++;
            if (200 <= transmitted_power)
            {
                transmitted_power = 0;
            }
            break;

        default:
            break;
        }
        add_flag = 0;
    }

    val_to_str(distance, oled_num);
    draw_half_words(0, 10, oled_num, 4);
    val_to_str(sensitivity, oled_num);
    draw_half_words(3, 10, oled_num, 4);
    val_to_str(transmitted_power, oled_num);
    draw_half_words(6, 10, oled_num, 4);
}

void draw_oled_param_page(void)
{
    uint8_t i;
    uint8_t oled_num[64] = {0};

    if ((pre_index < OLED_PARAM_PAGE_1) || (pre_index > OLED_PARAM_PAGE_2))
    {
        oled_clear();
    }

    pre_index = current_index;

    for (i = 0; i < (sizeof(oled_param_page_op) / sizeof(oled_param_page_op[0])) * 2 + 1; i += 3)
    {
        if (((current_index - OLED_PARAM_PAGE_1) * 3) != i)
        {
            clear_half_part(i, 0, 0, 1);
        }
    }

    draw_half_part((current_index - OLED_PARAM_PAGE_1) * 3, 0, ">", 1);
    for (i = 0; i < sizeof(oled_param_page_op) / sizeof(oled_param_page_op[0]); i++)
    {
        oled_param_page_op[i].dram_func(oled_param_page_op[i].page_offset,
                                        oled_param_page_op[i].column_offset,
                                        oled_param_page_op[i].ptr_word,
                                        oled_param_page_op[i].word_len);
    }

    if (add_flag)
    {
        switch (current_index)
        {
        case OLED_PARAM_PAGE_1:
            oled_light++;
            if (255 <= oled_light)
            {
                oled_light = 0;
            }
            break;

        case OLED_PARAM_PAGE_2:
            oled_close_time++;
            if (999 <= oled_close_time)
            {
                oled_close_time = 0;
            }
            break;

        default:
            break;
        }
        add_flag = 0;
    }

    val_to_str(oled_light, oled_num);
    draw_half_words(0, 10, oled_num, 4);
    val_to_str(oled_close_time, oled_num);
    draw_half_words(3, 10, oled_num, 4);
}

void draw_vision_page(void)
{
    if (pre_index != current_index)
    {
        oled_clear();
    }

    pre_index = current_index;
    draw_part(3, 2, "�汾��", 3);
    draw_half_part(3, 8, ":V1.0", 5);
}

void draw_param_confirm_page(void)
{
    if (pre_index != current_index)
    {
        oled_clear();
    }

    pre_index = current_index;
    draw_part(3, 2, "ȷ�����Ĳ���", 6);
    draw_half_part(3, 14, "?", 1);
}

void draw_fatory_confirm_page(void)
{
    if (pre_index != current_index)
    {
        oled_clear();
    }

    pre_index = current_index;
    draw_part(3, 2, "ȷ�ϻָ�Ĭ��", 6);
    draw_half_part(3, 14, "?", 1);
}

void oled_task(void)
{
    if (key_on)
    {
        key_on = 0;
        table_op[current_index].table_operation();
    }
}
