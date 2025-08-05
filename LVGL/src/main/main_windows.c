#include "ui.h"
#include "callback.h"

// 全局变量
lv_obj_t* main_layer;      // 主层
lv_obj_t* pages[PAGE_COUNT]; // 主页面数组
lv_obj_t* toggle_btn;      // 切换导航按钮
bool nav_visible = true;   // 导航是否可见
lv_obj_t* navigation_layer;

// 定义UI组件变量
static lv_obj_t* protocol_dropdown;
static lv_obj_t* serial_port_dropdown;
static lv_obj_t* serial_btn,* data_table;

static void lv_uart_protocol(lv_obj_t * page)
{
    // 创建页面
    lv_obj_set_size(page, 800, 480);
    lv_obj_set_style_bg_color(page, LV_COLOR_WHITE, 0);
    lv_obj_set_style_border_width(page, 0, 0);

    // 协议下拉框
    lv_obj_t* protocol_label = lv_label_create(page);
    lv_label_set_text(protocol_label, "协议:");
    lv_obj_align(protocol_label, LV_ALIGN_TOP_LEFT, 20, 20);

    protocol_dropdown = lv_dropdown_create(page);
    lv_dropdown_set_options(protocol_dropdown, "小米\n松下");
    lv_obj_align_to(protocol_dropdown, protocol_label, LV_ALIGN_OUT_RIGHT_MID, 10, 0);
    lv_obj_set_width(protocol_dropdown, 150);

    // 配置回调函数
    lv_obj_set_user_data(protocol_dropdown, (void*)2001);
    lv_obj_add_event_cb(protocol_dropdown, uart_protocol_event_cb, LV_EVENT_VALUE_CHANGED, NULL);

    // 串口开关按钮
    serial_btn = lv_btn_create(page);
    lv_obj_add_flag(serial_btn, LV_OBJ_FLAG_CHECKABLE);
    lv_obj_align(serial_btn, LV_ALIGN_TOP_RIGHT, -20, 20);
    lv_obj_set_width(serial_btn, 100);
    lv_obj_t* btn_label = lv_label_create(serial_btn);
    lv_label_set_text(btn_label, "打开串口");
    lv_obj_center(btn_label);
    lv_obj_add_event_cb(serial_btn, serial_btn_event_cb, LV_EVENT_VALUE_CHANGED, NULL);

    // 串口号下拉框
    lv_obj_t* serial_label = lv_label_create(page);
    lv_label_set_text(serial_label, "串口号:");
    lv_obj_align(serial_label, LV_ALIGN_TOP_LEFT, 20, 70);

    serial_port_dropdown = lv_dropdown_create(page);
    lv_dropdown_set_options(serial_port_dropdown, "串口1\n串口2");
    lv_obj_align_to(serial_port_dropdown, serial_label, LV_ALIGN_OUT_RIGHT_MID, 10, 0);
    lv_obj_set_width(serial_port_dropdown, 150);

    // 创建表格对象
    data_table = lv_table_create(page);
    lv_obj_clear_flag(data_table, LV_OBJ_FLAG_SCROLLABLE);  // 完全禁用滚动
    // 设置表格大小和位置
    lv_obj_set_size(data_table, 780, 370);
    lv_obj_set_pos(data_table, 10, 100);

    // 设置行数和列数（注意：行列索引从0开始）
    lv_table_set_row_cnt(data_table, 6);   // 5行
    lv_table_set_col_cnt(data_table, 7);   // 8列

    // 设置列宽（单位：像素）
    lv_table_set_col_width(data_table, 0, 100);
    lv_table_set_col_width(data_table, 1, 160);
    lv_table_set_col_width(data_table, 2, 100);
    lv_table_set_col_width(data_table, 3, 160);
    lv_table_set_col_width(data_table, 4, 100);
    lv_table_set_col_width(data_table, 5, 160);

    // 填充数据
    lv_table_set_cell_value(data_table, 0, 0, "电压");
    lv_table_set_cell_value(data_table, 0, 1, "310.0V");
    lv_table_set_cell_value(data_table, 0, 2, "电流");
    lv_table_set_cell_value(data_table, 0, 3, "10.0A");
    lv_table_set_cell_value(data_table, 0, 4, "功率");
    lv_table_set_cell_value(data_table, 0, 5, "200.0W");

    lv_table_set_cell_value(data_table, 1, 0, "电压");
    lv_table_set_cell_value(data_table, 1, 1, "310.0V");
    lv_table_set_cell_value(data_table, 1, 2, "电流");
    lv_table_set_cell_value(data_table, 1, 3, "10.0A");
    lv_table_set_cell_value(data_table, 1, 4, "功率");
    lv_table_set_cell_value(data_table, 1, 5, "200.0W");

    lv_table_set_cell_value(data_table, 2, 0, "电压");
    lv_table_set_cell_value(data_table, 2, 1, "310.0V");
    lv_table_set_cell_value(data_table, 2, 2, "电流");
    lv_table_set_cell_value(data_table, 2, 3, "10.0A");
    lv_table_set_cell_value(data_table, 2, 4, "功率");
    lv_table_set_cell_value(data_table, 2, 5, "200.0W");

    lv_table_set_cell_value(data_table, 3, 0, "电压");
    lv_table_set_cell_value(data_table, 3, 1, "310.0V");
    lv_table_set_cell_value(data_table, 3, 2, "电流");
    lv_table_set_cell_value(data_table, 3, 3, "10.0A");
    lv_table_set_cell_value(data_table, 3, 4, "功率");
    lv_table_set_cell_value(data_table, 3, 5, "200.0W");

    lv_table_set_cell_value(data_table, 4, 0, "电压");
    lv_table_set_cell_value(data_table, 4, 1, "310.0V");
    lv_table_set_cell_value(data_table, 4, 2, "电流");
    lv_table_set_cell_value(data_table, 4, 3, "10.0A");
    lv_table_set_cell_value(data_table, 4, 4, "功率");
    lv_table_set_cell_value(data_table, 4, 5, "200.0W");

    lv_table_set_cell_value(data_table, 5, 0, "电压");
    lv_table_set_cell_value(data_table, 5, 1, "310.0V");
    lv_table_set_cell_value(data_table, 5, 2, "电流");
    lv_table_set_cell_value(data_table, 5, 3, "10.0A");
    lv_table_set_cell_value(data_table, 5, 4, "功率");
    lv_table_set_cell_value(data_table, 5, 5, "200.0W");

    lv_table_set_cell_value(data_table, 6, 0, "电压");
    lv_table_set_cell_value(data_table, 6, 1, "310.0V");
    lv_table_set_cell_value(data_table, 6, 2, "电流");
    lv_table_set_cell_value(data_table, 6, 3, "10.0A");
    lv_table_set_cell_value(data_table, 6, 4, "功率");
    lv_table_set_cell_value(data_table, 6, 5, "200.0W");

    lv_table_set_cell_value(data_table, 7, 0, "电压");
    lv_table_set_cell_value(data_table, 7, 1, "310.0V");
    lv_table_set_cell_value(data_table, 7, 2, "电流");
    lv_table_set_cell_value(data_table, 7, 3, "10.0A");
    lv_table_set_cell_value(data_table, 7, 4, "功率");
    lv_table_set_cell_value(data_table, 7, 5, "200.0W");
}


// 创建主层和内容页面
static void create_main_layer(void)
{
    // 创建主层 (全屏)
    main_layer = lv_obj_create(lv_scr_act());
    lv_obj_set_size(main_layer, SCREEN_WIDTH, SCREEN_HEIGHT);
    lv_obj_set_pos(main_layer, 0, 0);
    lv_obj_clear_flag(main_layer, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_border_width(main_layer, 0, 0);
    lv_obj_set_style_pad_all(main_layer, 0, 0);

    // 初始化页面数组
    for (int i = 0; i < PAGE_COUNT; i++)
    {
        pages[i] = lv_obj_create(main_layer);
        // 设定大小 禁止滚动
        lv_obj_set_size(pages[i], SCREEN_WIDTH, SCREEN_HEIGHT);
        lv_obj_clear_flag(pages[i], LV_OBJ_FLAG_SCROLLABLE);
        // 绑定回调函数
        lv_obj_add_event_cb(pages[i], screen_click_cb, LV_EVENT_CLICKED, NULL);
        // 去除圆角
        lv_obj_set_style_radius(pages[i], 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        // 清除padding
        lv_obj_set_style_pad_all(pages[i], 0, LV_PART_MAIN);
        // 清除边框
        lv_obj_set_style_border_width(pages[i], 0, LV_PART_MAIN); 
    }
    // 创建各个页面
    lv_uart_protocol(pages[0]);
    lv_board_control(pages[1]);
    lv_compressor_control(pages[3]);
    for (int i = 0; i < PAGE_COUNT; i++) {
        // 默认隐藏除第一个外的所有页面
        lv_obj_add_flag(pages[i], LV_OBJ_FLAG_HIDDEN);
    }
    lv_obj_clear_flag(pages[3], LV_OBJ_FLAG_HIDDEN);
}


// 初始化界面
void lv_main_window(void)
{
    create_main_layer();
    navigation_layer = lv_navigation_layer(navigation_layer);

}
