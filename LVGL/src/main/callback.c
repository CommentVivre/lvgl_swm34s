#include "callback.h"

// 页面切换回调
void page_switch_cb(lv_event_t* e)
{
    lv_obj_t* btn = lv_event_get_target(e);
    uint32_t page_idx = (uint32_t)lv_event_get_user_data(e);

    // 隐藏所有页面
    for (int i = 0; i < PAGE_COUNT; i++) {
        lv_obj_add_flag(pages[i], LV_OBJ_FLAG_HIDDEN);
    }

    // 显示选中的页面
    lv_obj_clear_flag(pages[page_idx], LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(navigation_layer, LV_OBJ_FLAG_HIDDEN);
}

// 屏幕点击回的调
void screen_click_cb(lv_event_t* e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if (code == LV_EVENT_CLICKED) {

        // 切换导航栏状态
        nav_visible = !nav_visible;

        if (nav_visible) {
            // 显示导航层
            lv_obj_clear_flag(navigation_layer, LV_OBJ_FLAG_HIDDEN);
        }
        else {
            // 隐藏导航层
            lv_obj_add_flag(navigation_layer, LV_OBJ_FLAG_HIDDEN);
        }
    }
}



// 串口配置参数
static SerialConfig serial_cfg = {
    .baud_rate = 115200,
    .parity = 0,
    .stop_bits = 1,
    .data_bits = 8,
    .tx_interval_ms = 1000
};


void serial_init_event(lv_event_t* e)
{
    // 初始化串口配置
    lv_obj_t* obj = lv_event_get_target(e);
    uint32_t user_data = (uint32_t)lv_obj_get_user_data(obj);
    switch (user_data)
    {
    case 1001:
    {
        break;
    }
    case 1002:
    {
        // 波特率选择
        uint16_t baud_rate = lv_dropdown_get_selected(obj);
        switch (baud_rate)
        {
        case 0: serial_cfg.baud_rate = 9600; break;
        case 1: serial_cfg.baud_rate = 19200; break;
        case 2: serial_cfg.baud_rate = 38400; break;
        case 3: serial_cfg.baud_rate = 57600; break;
        case 4: serial_cfg.baud_rate = 115200; break;
        default: serial_cfg.baud_rate = 115200; break;
        }
        break;
    }
    case 1003:
    {
        // 奇偶校验
        uint16_t parity = lv_dropdown_get_selected(obj);
        switch (parity)
        {
        case 0: serial_cfg.parity = 0; break; // 无校验
        case 1: serial_cfg.parity = 1; break; // 奇校验
        case 2: serial_cfg.parity = 2; break; // 偶校验
        default: serial_cfg.parity = 0; break;
        }
        break;
    }
    case 1004:
    {
        // 停止位
        uint16_t stop_bits = lv_dropdown_get_selected(obj);
        switch (stop_bits)
        {
        case 0: serial_cfg.stop_bits = 0; break; // 1位停止位
        case 1: serial_cfg.stop_bits = 1; break; // 1.5位停止位
        case 2: serial_cfg.stop_bits = 2; break; // 2位停止位
        default: serial_cfg.stop_bits = 0; break;
        }
        break;
    }
    case 1005:
    {
        // 协议选择
        uint16_t protocol = lv_dropdown_get_selected(obj);
        switch (protocol)
        {
        case 0:
            // 自定义协议
            break;
        case 1:
            // 小米测试协议
            break;
        case 2:
            // 松下测试协议
            break;
        default:
            // 默认自定义协议
            break;
        }
        break;
    }
    }
}




// 串口按钮回调函数
void serial_btn_event_cb(lv_event_t* e) {
    lv_obj_t* btn = lv_event_get_target(e);
    if (lv_obj_has_state(btn, LV_STATE_CHECKED)) {
        lv_label_set_text(lv_obj_get_child(btn, 0), "关闭串口");
    }
    else {
        lv_label_set_text(lv_obj_get_child(btn, 0), "打开串口");
    }
}


void uart_protocol_event_cb(lv_event_t* e)
{
    lv_obj_t* obj = lv_event_get_target(e);
    uint32_t user_data = (uint32_t)lv_obj_get_user_data(obj);
    switch (user_data)
    {
        // 协议配置
    case 2001:
    {
        uint16_t protocol_select = lv_dropdown_get_selected(obj);
        break;
    }
    }
}
