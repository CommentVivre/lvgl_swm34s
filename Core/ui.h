#pragma once

#include <lvgl.h>
#include <string.h>
#include "lv_color_def.h"

#ifdef __cplusplus
extern "C" {
#endif

void lv_main_window(void);

/*
* 字体库生成说明
* 详细见博客 https://neucrack.com/p/514
* lvgl字体库生成工具 https://lvgl.io/tools/fontconverter
*
*
*
* 额外字体
* FontAwesome
* 0xF001, 0xF008, 0xF00B, 0xF00C, 0xF00D, 0xF011, 0xF013, 0xF015, 0xF019, 0xF01C, 0xF021, 0xF026, 0xF027, 0xF028, 0xF03E, 0xF043, 0xF048, 0xF04B, 0xF04C, 0xF04D, 0xF051, 0xF052, 0xF053, 0xF054, 0xF067, 0xF068, 0xF06E, 0xF070, 0xF071, 0xF074, 0xF077, 0xF078, 0xF079, 0xF07B, 0xF093, 0xF095, 0xF0C4, 0xF0C5, 0xF0C7, 0xF0C9, 0xF0E0, 0xF0E7, 0xF0EA, 0xF0F3, 0xF11C, 0xF124, 0xF15B, 0xF1EB, 0xF240, 0xF241, 0xF242, 0xF243, 0xF244, 0xF287, 0xF293, 0xF2ED, 0xF304, 0xF55A, 0xF7C2, 0xF8A2
* 从 lv_symbol_def.h 找到定义
*/

// 屏幕尺寸定义
#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 480
#define NAV_WIDTH     200  // 导航栏宽度(1/4屏幕)

// 页面数量
#define PAGE_COUNT    4


typedef struct
{
    // 主层
    lv_obj_t *nav_layer,*main_layer;
    // 主页
    lv_obj_t *pages[PAGE_COUNT];
    // 控件
}lv_windows_def;

// 全局变量
extern lv_obj_t* navigation_layer;       // 导航层
extern lv_obj_t* main_layer;      // 主层
extern lv_obj_t* pages[PAGE_COUNT]; // 主页面数组
extern lv_obj_t* toggle_btn;      // 切换导航按钮
extern bool nav_visible;   // 导航是否可见


lv_obj_t* lv_navigation_layer(lv_obj_t* layer);
void lv_board_control(lv_obj_t* page);



void lv_compressor_control(lv_obj_t* page);

#ifdef __cplusplus
}
#endif
