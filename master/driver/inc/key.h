#ifndef __KEY_H__
#define __KEY_H__

#include "stdint.h"
#define ENTER_KEY (0xE)
#define UP_KEY (0xD)
#define DOWN_KEY (0xB)
#define MENU_KEY (0x7)


#define CT_MAX_VALUE (0xFF)
#define LIGHT_MAX_VALUE (0xFF)
#define CT_MIN_VALUE (1)
#define LIGHT_MIN_VALUE (1)

typedef enum
{
    KEY_CHECK = 0, // �������
    KEY_COMFIRM,   // �����ٴ�ȷ��
    KEY_RELEASE,   // �����ͷ�
} KEY_STATE;       // ����״̬

typedef enum
{
    NULL_KEY = 0,
    SHORT_KEY, // �̰�����
    LONG_KEY,  // ��������
} KEY_ACTION;  // ��������

extern uint8_t current_index;
extern uint8_t key_on;
extern uint8_t add_flag;
extern uint8_t reduce_flag;

uint8_t key_scan(void);
void key_init(void);
void key_task(void);
#endif
