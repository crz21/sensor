#include "includes.h"

uint8_t r_data[USART_REC_LEN];
uint8_t rec_buf[USART_REC_LEN];
uint8_t rec_step;
uint8_t rec_counter;
uint8_t report_head[4] = {0xF4, 0xF3, 0xF2, 0xF1};
uint8_t config_head[4] = {0xFD, 0xFC, 0xFB, 0xFA};
uint8_t report_tail[4] = {0xF8, 0xF7, 0xF6, 0xF5};
uint8_t config_tail[4] = {4, 3, 2, 1};
uint8_t test_send_buf[14] = {0xFD, 0xFC, 0xFB, 0xFA, 0x04, 0, 0xA5, 0x0, 0x01, 0, 0x04, 0x03, 0x02, 0x01};
uint8_t test_start_buf[14] = {0xFD, 0xFC, 0xFB, 0xFA, 0x04, 0, 0xFF, 0x0, 0x01, 0, 0x04, 0x03, 0x02, 0x01};

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

void uart_task(void)
{

}

void rec_sensor_data(uint8_t *data, uint8_t len)
{
    uint8_t i;

    // memset(r_data, 0, sizeof(r_data));
    for (i = 0; i < len; i++)
    {
        r_data[i] = data[i];
    }
}

void USART1_IRQHandler(void)
{
    uint8_t c_buf = 0;
    static uint8_t rec_len, data_len;
    static uint8_t frame_head[4] = {0};
    static uint8_t frame_tail[4] = {0};

    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
        c_buf = USART_ReceiveData(USART1);
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);

        switch (rec_step)
        {
        case FRAME_HEAD_STEP:
        {
            rec_buf[rec_counter] = c_buf;
            if (0 == rec_counter)
            {
                if (report_head[0] == rec_buf[0])
                {
                    memcpy(frame_head, report_head, 4);
                }
                else if (config_head[0] == rec_buf[0])
                {
                    memcpy(frame_head, config_head, 4);
                }
                else
                {
                    rec_step = FRAME_HEAD_STEP;
                    rec_counter = 0;
                    memset(rec_buf, 0, sizeof(rec_buf));
                    rec_len = 0;
                }
            }

            if (3 == rec_len)
            {
                if (rec_buf[rec_counter] == frame_head[rec_counter])
                {
                    rec_step = FRAME_LEN_STEP;
                    rec_counter++;
                    sys_tim_param.rec_overtime = 0;
                    rec_len = 0;
                }
                else
                {
                    rec_step = FRAME_HEAD_STEP;
                    rec_counter = 0;
                    memset(rec_buf, 0, sizeof(rec_buf));
                    rec_len = 0;
                }
            }
            else
            {
                if (rec_buf[rec_counter] == frame_head[rec_counter])
                {
                    rec_step = FRAME_HEAD_STEP;
                    rec_counter++;
                    sys_tim_param.rec_overtime = 0;
                    rec_len++;
                }
                else
                {
                    rec_step = FRAME_HEAD_STEP;
                    rec_counter = 0;
                    memset(rec_buf, 0, sizeof(rec_buf));
                    rec_len = 0;
                }
            }
        }
        break;

        case FRAME_LEN_STEP:
        {
            if (1 == rec_len)
            {
                rec_step = FRAME_DATA_STEP;
                rec_buf[rec_counter] = c_buf;
                rec_counter++;
                sys_tim_param.rec_overtime = 0;
                rec_len = 0;
            }
            else
            {
                rec_step = FRAME_LEN_STEP;
                rec_buf[rec_counter] = c_buf;
                data_len = c_buf;
                rec_counter++;
                sys_tim_param.rec_overtime = 0;
                rec_len++;
            }
        }
        break;

        case FRAME_DATA_STEP:
        {
            if ((data_len - 1) == rec_len)
            {
                rec_step = FRAME_TAIL_STEP;
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

        case FRAME_TAIL_STEP:
        {
            rec_buf[rec_counter] = c_buf;

            if ((6 + data_len) == rec_counter)
            {
                if (0xF8 == rec_buf[6 + data_len])
                {
                    memcpy(frame_tail, report_tail, 4);
                }
                else if (0x04 == rec_buf[6 + data_len])
                {
                    memcpy(frame_tail, config_tail, 4);
                }
                else
                {
                    rec_step = FRAME_HEAD_STEP;
                    rec_counter = 0;
                    memset(rec_buf, 0, sizeof(rec_buf));
                    rec_len = 0;
                }
            }

            if (3 == rec_len)
            {
                if (rec_buf[rec_counter] == frame_tail[rec_len])
                {
                    rec_step = FRAME_HEAD_STEP;
                    rec_counter++;
                    sys_tim_param.rec_overtime = 0;
                    rec_len = 0;
                    rec_buf[rec_counter] = c_buf;
                    rec_counter = 0;
                    rec_sensor_data(rec_buf, 10 + data_len);
                    memset(rec_buf, 0, sizeof(rec_buf));
                }
                else
                {
                    rec_step = FRAME_HEAD_STEP;
                    rec_counter = 0;
                    memset(rec_buf, 0, sizeof(rec_buf));
                    rec_len = 0;
                }
            }
            else
            {
                if (rec_buf[rec_counter] == frame_tail[rec_len])
                {
                    rec_step = FRAME_TAIL_STEP;
                    rec_counter++;
                    sys_tim_param.rec_overtime = 0;
                    rec_len++;
                }
                else
                {
                    rec_step = FRAME_HEAD_STEP;
                    rec_counter = 0;
                    memset(rec_buf, 0, sizeof(rec_buf));
                    rec_len = 0;
                }
            }
        }
        break;

        default:
            break;
        }
    }
}
