#include "includes.h"
/*  功能帧 头帧 1个字节 + 长度 1个字节 + 类型 1个字节 + 数据域 2个字节 + CRC 2个字节 + 尾帧 1个字节 */
uint8_t r_data[USART_REC_LEN];
uint8_t rec_buf[USART_REC_LEN];
uint8_t rec_step;
uint8_t rec_counter;
uint8_t key_statue;
uint8_t frame_head[2] = {
    0xAA,
};
uint8_t frame_tail[2] = {
    0x55,
};

void uart_init(void)
{
    bsp_uart1_init();
}

void send_buf(uint8_t *data, uint8_t len)
{
    uint8_t counter;

    for (counter = 0; counter < len; counter++)
    {
        USART_SendData(USART1, data[counter]);
        delay_ms(10);
    }
}

void analyze_uart_frame(void)
{
    uint8_t data_type = 0, key_statue = 0;

    data_type = r_data[3];
    switch (data_type)
    {
    case 1:
    {
    }
    break;
    case 2:
    {
    }
    break;
    case CHANGE_TABLE:
    {
        key_statue = r_data[4];
        if (0 == key_statue)
        {
            // func_index = table[func_index].up;
        }
    }
    break;
    default:
    {
    }
    break;
    }
}

void uart_task(void)
{
}

void rec_sensor_data(uint8_t *data, uint8_t len)
{
    uint8_t i;

    for (i = 0; i < len; i++)
    {
        r_data[i] = data[i];
    }
}

void USART1_IRQHandler(void)
{
    uint8_t c_buf = 0;
    static uint8_t rec_len, data_len;

    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
        c_buf = USART_ReceiveData(USART1);
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);

        switch (rec_step)
        {
        case FRAME_HEAD_STEP:
        {
            if (c_buf == 0xAA)
            {
                rec_step = FRAME_LEN_STEP;
                rec_buf[rec_counter] = c_buf;
                rec_counter++;
                sys_tim_param.rec_overtime = 0;
            }
            else
            {
                rec_step = FRAME_HEAD_STEP;
                rec_counter = 0;
                memset(rec_buf, 0, sizeof(rec_buf));
                sys_tim_param.rec_overtime = 0;
            }
        }
        break;

        case FRAME_LEN_STEP:
        {
            rec_step = FRAME_TYPE_STEP;
            rec_buf[rec_counter] = c_buf;
            data_len = c_buf;
            rec_counter++;
            sys_tim_param.rec_overtime = 0;
        }
        break;

        case FRAME_TYPE_STEP:
        {
            if ((CMD_TYPE == c_buf) || (DATA_TYPE == c_buf))
            {
                rec_step = FRAME_DATA_STEP;
                rec_counter++;
                sys_tim_param.rec_overtime = 0;
            }
            else
            {
                rec_step = FRAME_HEAD_STEP;
                rec_counter = 0;
                memset(rec_buf, 0, sizeof(rec_buf));
                sys_tim_param.rec_overtime = 0;
            }
        }

        case FRAME_DATA_STEP:
        {
            if ((data_len - 1) == rec_len)
            {
                rec_step = FRAME_CRC_STEP;
                rec_buf[rec_counter] = c_buf;
                rec_counter++;
                sys_tim_param.rec_overtime = 0;
                rec_len = 0;
            }
            else
            {
                rec_step = FRAME_DATA_STEP;
                rec_buf[rec_counter] = c_buf;
                rec_counter++;
                sys_tim_param.rec_overtime = 0;
                rec_len++;
            }
        }
        break;

        case FRAME_CRC_STEP:
        {
            if (1 == rec_len)
            {
                rec_step = FRAME_TAIL_STEP;
                rec_buf[rec_counter] = c_buf;
                rec_counter++;
                sys_tim_param.rec_overtime = 0;
                rec_len = 0;
            }
            else
            {
                rec_step = FRAME_CRC_STEP;
                rec_buf[rec_counter] = c_buf;
                rec_counter++;
                sys_tim_param.rec_overtime = 0;
                rec_len++;
            }
        }
        break;

        case FRAME_TAIL_STEP:
        {
            if (c_buf == 0x55)
            {
                rec_step = FRAME_HEAD_STEP;
                rec_counter++;
                rec_buf[rec_counter] = c_buf;
                sys_tim_param.rec_overtime = 0;
                rec_counter = 0;
                rec_sensor_data(rec_buf, 7 + data_len);
                memset(rec_buf, 0, sizeof(rec_buf));
            }
            else
            {
                rec_step = FRAME_HEAD_STEP;
                rec_counter = 0;
                memset(rec_buf, 0, sizeof(rec_buf));
            }
        }
        break;

        default:
            break;
        }
    }
}
