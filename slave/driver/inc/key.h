#ifndef __KEY_H__
#define __KEY_H__      

#include "stdint.h"
#define CT_ADD              0xE
#define CT_REDUCE           0xD
#define LIGHT_ADD           0xB
#define LIGHT_REDUCE        0x7

#define CT_MAX_VALUE        0xFF
#define LIGHT_MAX_VALUE     0xFF
#define CT_MIN_VALUE        1
#define LIGHT_MIN_VALUE     1

typedef enum
{
    KEY_CHECK = 0,      //�������
    KEY_COMFIRM,        //�����ٴ�ȷ��
    KEY_RELEASE,        //�����ͷ�

}KEY_STATE;//����״̬

typedef enum
{
    NULL_KEY = 0,
    SHORT_KEY,          //�̰�����
    LONG_KEY,           //��������

}KEY_ACTION;//��������

uint8_t key_scan(void);       
void key_init(void);
void key_task(void);                   
#endif
