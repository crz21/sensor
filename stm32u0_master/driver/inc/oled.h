#ifndef __OLED_H__
#define __OLED_H__

#include "stdint.h"

#define MAX_COLUMN 128     // 最大列数
#define MAX_PAGE 64        // 最大行数 1个page代表8小行所以为 64 / 8
#define MAX_COLUMN_WORDS 8 // 一行最大显示字数

extern const uint8_t picture_tab[];
extern const uint8_t font_size_8x16[];
extern const uint8_t font_size_16x16[];
typedef struct
{
    uint8_t page_offset;
    uint8_t column_offset;
    void *ptr_word;
    uint8_t word_len;
    void (*dram_func)(uint8_t, uint8_t, char *, uint8_t);
} _draw_word_op;

typedef struct
{
    uint8_t current;
    uint8_t up;
    uint8_t down;
    uint8_t enter;
    uint8_t min_page;
    uint8_t max_page;
    void (*table_operation)();
} _table_op;

typedef enum
{
    WELCOME_PAGE = 0,
    MAIN_PAGE,
    SET_PAGE_1,
    SET_PAGE_2,
    SET_PAGE_3,
    SET_PAGE_4,
    SENSOR_PARAM_PAGE_1,
    SENSOR_PARAM_PAGE_2,
    SENSOR_PARAM_PAGE_3,
    OLED_PARAM_PAGE_1,
    OLED_PARAM_PAGE_2,
    VISION_PAGE,
    SENSOR_PARAM_CONFIRM_PAGE,
    OLED_PARAM_CONFIRM_PAGE,
    FATORY_CONFIRM_PAGE,
    MAX_INDEX,
} TABLE_INDEX;

extern _table_op table_op[MAX_INDEX];
extern uint8_t distance;
extern uint8_t sensitivity;
extern uint8_t transmitted_power;
extern uint8_t oled_light;
extern uint16_t oled_close_time;

void oled_init(void);
void oled_task(void);
void draw_welcom_page(void);
void draw_main_page(void);
void draw_set_page(void);
void draw_sensor_param_page(void);
void draw_oled_param_page(void);
void draw_vision_page(void);
void draw_param_confirm_page(void);
void draw_fatory_confirm_page(void);
void draw_part(uint8_t page_offset, uint8_t column_offset, char *str, uint8_t word_len);
void draw_half_part(uint8_t page_offset, uint8_t column_offset, char *str, uint8_t word_len);
void clear_part(uint8_t page_offset, uint8_t column_offset, char *str, uint8_t word_len);
void clear_half_part(uint8_t page_offset, uint8_t column_offset, char *str, uint8_t word_len);
void oled_clear(void);
void oled_send_cmd(uint8_t o_command);

#endif
