#ifndef __USART_H__
#define __USART_H__

#include <stdint.h>

#define USART_REC_LEN 50 // 定义最大接收字节数 200

typedef enum
{
  FRAME_HEAD_STEP,
  FRAME_LEN_STEP,
  FRAME_DATA_STEP,
  FRAME_TAIL_STEP,
} REC_STEP;

extern uint8_t r_data[USART_REC_LEN];
extern uint8_t rec_buf[USART_REC_LEN];
extern uint8_t rec_step;
extern uint8_t rec_counter;
extern uint8_t test_start_buf[14];
// 如果想串口中断接收，请不要注释以下宏定义
void uart_init(void);
void send_buf(uint8_t *data, uint8_t len);
void send_message(uint8_t data, uint16_t func_code, uint8_t func_code_len);
void uart_task(void);
#endif
