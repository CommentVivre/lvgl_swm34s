#include "ui.h"
#include "callback.h"

// 全局状态变量
static lv_obj_t* power_switch = NULL, * compressor_btn = NULL, * protocol_dropdown = NULL, * protocol_menu = NULL, *uart_speed_input = NULL,* keyboard=NULL;

uint8_t cmd_status = false, power_status = false, control_mode = 0;

// 更新按钮状态的回调函数
void update_compressor_btn_state(lv_event_t* e) {
    lv_obj_t* target = lv_event_get_target(e);

    // 更新按钮颜色和文本
    if (lv_obj_has_state(target, LV_EVENT_CLICKED)&& power_status)
    {
        if(cmd_status)
        {
            lv_obj_set_style_bg_color(target, lv_color_hex(0x00AA00), 0); // 绿色
            lv_label_set_text(lv_obj_get_child(target, 0), "已关闭");
            cmd_status = false;
        }
        else
        {
            lv_obj_set_style_bg_color(target, lv_color_hex(0xFF0000), 0); // 红色
            lv_label_set_text(lv_obj_get_child(target, 0), "运行中");
            cmd_status = true;
        }
    }
}

// 按钮点击切换电源状态
void compressor_btn_clicked(lv_event_t* e)
{
    lv_obj_t* btn = lv_event_get_target(e);
    // 切换开关状态
    if (lv_obj_has_state(btn, LV_STATE_CHECKED))
    {
        power_status = true;
    }
    else
    {
        lv_obj_set_style_bg_color(compressor_btn, lv_color_hex(0x00AA00), 0); // 绿色
        lv_label_set_text(lv_obj_get_child(compressor_btn, 0), "已关闭");
        cmd_status = false; power_status = false;
    }
}

// 控制模式选择
void compressor_model_select(lv_event_t* e)
{
    lv_obj_t* obj = lv_event_get_target(e);
    uint16_t list_selected = lv_dropdown_get_selected(obj);
    // 隐藏相关控件
    lv_obj_set_style_bg_color(compressor_btn, lv_color_hex(0x00AA00), 0); // 绿色
    lv_label_set_text(lv_obj_get_child(compressor_btn, 0), "已关闭");
    lv_obj_add_flag(compressor_btn, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_state(power_switch, LV_STATE_CHECKED);
    cmd_status = false; power_status = false;
    lv_obj_add_flag(protocol_menu, LV_OBJ_FLAG_HIDDEN);
    
    switch (list_selected)
    {
        // PWM控制
    case 0:
    {
        break;
    }
    // 串口控制
    case 1:
    {
        lv_obj_clear_flag(protocol_menu, LV_OBJ_FLAG_HIDDEN);
        break;
    }
    // 机械开关控制
    case 2:
    {
        lv_obj_clear_flag(compressor_btn, LV_OBJ_FLAG_HIDDEN);
        break;
    }
    }
}


static void input_event_handler(lv_event_t* e) {
    lv_obj_t* input = lv_event_get_target(e);
    lv_event_code_t code = lv_event_get_code(e);

    switch (code) {
    case LV_EVENT_FOCUSED:
        // 输入框被选中时的处理
        if (lv_obj_has_flag(keyboard, LV_OBJ_FLAG_HIDDEN)) {
            lv_obj_clear_flag(keyboard, LV_OBJ_FLAG_HIDDEN);
        }
        break;

    case LV_EVENT_DEFOCUSED:
        // 输入框失去焦点时的处理
        lv_obj_add_flag(keyboard, LV_OBJ_FLAG_HIDDEN);
        break;

    case LV_EVENT_READY:
        // 键盘按下确认键时触发
        break;

    case LV_EVENT_CANCEL:
        // 键盘按下取消键时触发
        break;

    case LV_EVENT_VALUE_CHANGED:
        // 文本内容改变时触发
        break;
	default:
		break;
    }
}

void lv_compressor_control(lv_obj_t* page)
{
    // 设置背景样式
    static lv_style_t bg_style;
    lv_style_init(&bg_style);
    lv_style_set_bg_color(&bg_style, lv_color_hex(0xF0F0F0));
    lv_style_set_border_width(&bg_style, 0);
    lv_style_set_radius(&bg_style, 10);
    lv_obj_add_style(page, &bg_style, 0);

    // 创建标题栏
    lv_obj_t* header = lv_obj_create(page);
    lv_obj_set_size(header, 800, 60);
    lv_obj_align(header, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_clear_flag(header, LV_OBJ_FLAG_SCROLLABLE);

    // 去除圆角
    lv_obj_set_style_radius(header, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    // 清除padding
    lv_obj_set_style_pad_all(header, 0, LV_PART_MAIN);
    // 清除边框
    lv_obj_set_style_border_width(header, 0, LV_PART_MAIN);

    // 标题栏样式
    static lv_style_t header_style;
    lv_style_init(&header_style);
    lv_style_set_bg_color(&header_style, lv_color_hex(0x2C3E50));
    lv_style_set_border_width(&header_style, 0);
    lv_obj_add_style(header, &header_style, 0);

    // 添加标题（左上角）
    lv_obj_t* title_label = lv_label_create(header);
    lv_label_set_text(title_label, "压缩机控制界面");
    lv_obj_align(title_label, LV_ALIGN_LEFT_MID, 20, 0);
    lv_obj_set_style_text_color(title_label, lv_color_white(), 0);

    // 添加电源开关（右上角）
    power_switch = lv_switch_create(header);
    lv_obj_align(power_switch, LV_ALIGN_RIGHT_MID, -20, 0);
    lv_obj_set_size(power_switch, 60, 30);

    // 开关标签
    lv_obj_t* switch_label = lv_label_create(header);
    lv_label_set_text(switch_label, "电源:");
    lv_obj_align_to(switch_label, power_switch, LV_ALIGN_OUT_LEFT_MID, -10, 0);
    lv_obj_set_style_text_color(switch_label, lv_color_white(), 0);

    // 添加模式选择标签（左侧）
    lv_obj_t* mode_label = lv_label_create(page);
    lv_label_set_text(mode_label, "控制模式:");
    lv_obj_set_pos(mode_label, 30, 100);

    // 添加模式选择下拉框（右侧）
    lv_obj_t* mode_dropdown = lv_dropdown_create(page);
    lv_dropdown_set_options(mode_dropdown, "PWM控制\n串口控制\n机械开关控制");
    lv_obj_align_to(mode_dropdown, mode_label, LV_ALIGN_OUT_RIGHT_MID, 20, 0);
    lv_obj_set_size(mode_dropdown, 180, 40);

    // 创建串口配置栏
    protocol_menu = lv_obj_create(page);
    lv_obj_set_size(protocol_menu, 300, 320);
    lv_obj_set_pos(protocol_menu, 20, 140);
    lv_obj_clear_flag(protocol_menu, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_flag(protocol_menu, LV_OBJ_FLAG_HIDDEN);

    // 协议
    lv_obj_t* protocol_label = lv_label_create(protocol_menu);
    lv_label_set_text(protocol_label, "协议:");
    lv_obj_align(protocol_label, LV_ALIGN_TOP_LEFT, 20, 20);

    protocol_dropdown = lv_dropdown_create(protocol_menu);
    lv_dropdown_set_options(protocol_dropdown, "小米\n松下");
    lv_obj_align_to(protocol_dropdown, protocol_label, LV_ALIGN_OUT_RIGHT_MID, 10, 0);
    lv_obj_set_width(protocol_dropdown, 150);

    // 转速
    lv_obj_t* speed_label = lv_label_create(protocol_menu);
    lv_label_set_text(speed_label, "转速:");
    lv_obj_align_to(speed_label, protocol_label, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 30);

    uart_speed_input = lv_textarea_create(protocol_menu);
    lv_obj_set_size(uart_speed_input, 150, 40);
    lv_obj_align_to(uart_speed_input, speed_label, LV_ALIGN_OUT_RIGHT_MID, 10, 0);
    lv_textarea_set_one_line(uart_speed_input, true);     // 单行模式
    lv_textarea_set_max_length(uart_speed_input, 6);     // 最大32个字符
    lv_textarea_set_text(uart_speed_input, "");           // 初始文本为空
    lv_obj_add_event_cb(uart_speed_input, input_event_handler, LV_EVENT_FOCUSED, NULL);

    // 添加键盘支持
    keyboard = lv_keyboard_create(page);
    lv_keyboard_set_textarea(keyboard, uart_speed_input);
    lv_obj_add_flag(keyboard, LV_OBJ_FLAG_HIDDEN);

    // 创建压缩机控制按钮（右侧）
    compressor_btn = lv_btn_create(page);
    lv_obj_set_size(compressor_btn, 200, 200);
    lv_obj_align(compressor_btn, LV_ALIGN_RIGHT_MID, -80, 0);
    lv_obj_set_style_radius(compressor_btn, 100, 0); // 圆形按钮
    lv_obj_set_style_bg_color(compressor_btn, lv_color_hex(0x00AA00), 0); // 初始绿色
    lv_obj_add_flag(compressor_btn, LV_OBJ_FLAG_HIDDEN);

    // 按钮标签
    lv_obj_t* btn_label = lv_label_create(compressor_btn);
    lv_label_set_text(btn_label, "已关闭");
    lv_obj_set_style_text_color(btn_label, lv_color_white(), 0);
    lv_obj_center(btn_label);
    

    // 添加事件处理
    lv_obj_add_event_cb(power_switch, compressor_btn_clicked, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_add_event_cb(compressor_btn, update_compressor_btn_state, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(mode_dropdown, compressor_model_select, LV_EVENT_VALUE_CHANGED, NULL);
}
