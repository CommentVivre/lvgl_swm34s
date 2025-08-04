#pragma once

#include "lvgl.h"
#include "string.h"


void lv_main_window(void);

extern const lv_font_t lv_font_sy_16;

// 屏幕尺寸定义
#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 480
#define NAV_WIDTH     200  // 导航栏宽度(1/4屏幕)

// 页面数量
#define PAGE_COUNT    3


typedef struct
{
    // 主层
    lv_obj_t *nav_layer,*main_layer;
    // 主页
    lv_obj_t *pages[PAGE_COUNT];
    // 控件
}lv_windows_def;

typedef struct {
    uint32_t baud_rate;
    uint8_t parity;     // 0:None, 1:Odd, 2:Even
    uint8_t stop_bits;  // 1 or 2
    uint8_t data_bits;  // 5-8
    uint16_t tx_interval_ms;
} SerialConfig;
