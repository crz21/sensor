#ifndef __OLED_H__
#define __OLED_H__

#include "stdint.h"

#define MAX_COLUMN          128 //最大列数
#define MAX_PAGE            64  //最大行数 1个page代表8小行所以为 64 / 8
#define MAX_COLUMN_WORDS    8   //一行最大显示字数

extern const uint8_t  *point;
extern uint8_t ACK;
extern const uint8_t  OLED_init_cmd[25];
extern const uint8_t  picture_tab[];
extern const uint8_t font_size_8x16[];
extern const uint8_t font_size_16x16[];

void oled_send_cmd(uint8_t o_command);
void oled_send_data(uint8_t o_data);
void column_set(uint8_t column);
void page_set(uint8_t page);
void oled_clear(void);
void oled_full(void);
void oled_init(void);
void picture_display(const uint8_t *ptr_pic);
void picture_reverse_display(const uint8_t *ptr_pic);
void oled_one_word(const uint8_t *ptr_word);
void oled_half_word(const uint8_t *ptr_word);
void oled_title(const uint8_t *ptr_word);
void oled_part(const uint8_t *ptr_word);
void oled_half_part(uint8_t page_num, uint8_t column_word_num, uint8_t *ptr_word);
#endif
