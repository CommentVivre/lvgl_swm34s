#include "ui.h"
#include "callback.h"


// 页面名称
const char* page_names[PAGE_COUNT] = {
    "协议", "控制","风机控制界面","压缩机控制界面"
};

// 创建导航层
lv_obj_t* lv_navigation_layer(lv_obj_t* layer)
{
    // 创建导航层 (位于主层之上)
    layer = lv_obj_create(lv_scr_act());
    lv_obj_set_size(layer, NAV_WIDTH, SCREEN_HEIGHT);
    lv_obj_set_pos(layer, 0, 0);
    lv_obj_set_flex_flow(layer, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(layer, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_all(layer, 10, 0);

    // 设置半透明样式
    lv_obj_set_style_bg_opa(layer, LV_OPA_70, 0);
    lv_obj_set_style_bg_color(layer, lv_color_hex(0x333333), 0);
    lv_obj_set_style_border_width(layer, 0, 0);

    // 确保导航层位于最顶层
    lv_obj_move_foreground(layer);

    // 去除圆角
    lv_obj_set_style_radius(layer, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    // 清除padding
    lv_obj_set_style_pad_all(layer, 0, LV_PART_MAIN);
    // 清除边框
    lv_obj_set_style_border_width(layer, 0, LV_PART_MAIN);

    // 创建导航按钮
    for (int i = 0; i < PAGE_COUNT; i++) {
        lv_obj_t* btn = lv_btn_create(layer);
        lv_obj_set_size(btn, NAV_WIDTH - 20, 50);
        lv_obj_add_event_cb(btn, page_switch_cb, LV_EVENT_CLICKED, (void*)(uintptr_t)i);

        // 按钮样式
        lv_obj_set_style_bg_color(btn, lv_color_hex(0x555555), 0);
        lv_obj_set_style_bg_color(btn, lv_color_hex(0x777777), LV_STATE_PRESSED);
        lv_obj_set_style_radius(btn, 10, 0);

        // 按钮标签
        lv_obj_t* label = lv_label_create(btn);
        lv_label_set_text(label, page_names[i]);

        lv_obj_set_style_text_color(label, lv_color_white(), 0);
        lv_obj_center(label);
    }

    lv_obj_add_flag(layer, LV_OBJ_FLAG_EVENT_BUBBLE| LV_OBJ_FLAG_HIDDEN);
    return layer;
}
