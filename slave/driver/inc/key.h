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
    KEY_CHECK = 0,      //按键检测
    KEY_COMFIRM,        //按键再次确认
    KEY_RELEASE,        //按键释放

}KEY_STATE;//按键状态

typedef enum
{
    NULL_KEY = 0,
    SHORT_KEY,          //短按按键
    LONG_KEY,           //长按按键

}KEY_ACTION;//按键动作

uint8_t key_scan(void);       
void key_init(void);
void key_task(void);                   
#endif
