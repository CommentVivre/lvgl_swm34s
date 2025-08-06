#include "ui.h"
#include "callback.h"

static lv_obj_t* uarta_sw, * bauda_dropdown, * paritya_dropdown, * stopbitsa_dropdown, * protocola_dropdown;
static lv_obj_t* uartb_sw, * baudb_dropdown, * parityb_dropdown, * stopbitsb_dropdown, * protocolb_dropdown;

// 控制界面
// 串口开关与波特率选择 协议配置 停止位配置 奇偶校验位配置 默认为自定义状态
// 继电器控制
// PWM DAC控制 双路
void lv_board_control(lv_obj_t* page)
{
    // 创建串口A面板
    lv_obj_t* uart_panel = lv_obj_create(page);
    lv_obj_set_size(uart_panel, 200, 480);
    lv_obj_align(uart_panel, LV_ALIGN_LEFT_MID, 0, 0);
    lv_obj_set_style_radius(uart_panel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_clear_flag(uart_panel, LV_OBJ_FLAG_SCROLLABLE);
    // 串口A
    lv_obj_t* uarta_label = lv_label_create(uart_panel);
    lv_label_set_text(uarta_label, "串口1");
    lv_obj_set_pos(uarta_label, 5, 5);

    uarta_sw = lv_switch_create(uart_panel);
    lv_obj_set_user_data(uarta_sw, (void*)1001);
    lv_obj_set_size(uarta_sw, 60, 30);
    lv_obj_set_pos(uarta_sw, 110, 0);
    // lv_obj_align_to(swuarta_sw uarta_label, LV_ALIGN_OUT_RIGHT_MID, 10, 0);
    lv_obj_add_event_cb(uarta_sw, serial_init_event, LV_EVENT_VALUE_CHANGED, NULL);

    // 波特率
    lv_obj_t* bauda_label = lv_label_create(uart_panel);
    lv_label_set_text(bauda_label, "波特率");
    lv_obj_set_pos(bauda_label, 5, 35);
    // 波特率下拉选择
    bauda_dropdown = lv_dropdown_create(uart_panel);
    lv_obj_set_user_data(bauda_dropdown, (void*)1002);
    lv_obj_set_size(bauda_dropdown, 100, 30);
    lv_dropdown_set_options(bauda_dropdown, "4800\n9600\n19200\n38400\n57600\n115200\n460800\n921600");
    lv_dropdown_set_selected(bauda_dropdown, 4); // 默认115200
    lv_obj_set_pos(bauda_dropdown, 80, 30);
    lv_obj_add_event_cb(bauda_dropdown, serial_init_event, LV_EVENT_VALUE_CHANGED, NULL);

    // 校验
    lv_obj_t* paritya_label = lv_label_create(uart_panel);
    lv_label_set_text(paritya_label, "校验");
    lv_obj_set_pos(paritya_label, 5, 65);
    // 校验下拉选择
    paritya_dropdown = lv_dropdown_create(uart_panel);
    lv_obj_set_user_data(paritya_dropdown, (void*)1003);
    lv_obj_set_size(paritya_dropdown, 100, 30);
    lv_dropdown_set_options(paritya_dropdown, "无校验\n奇校验\n偶校验");
    lv_dropdown_set_selected(paritya_dropdown, 0);

    lv_obj_set_pos(paritya_dropdown, 80, 60);
    lv_obj_add_event_cb(paritya_dropdown, serial_init_event, LV_EVENT_VALUE_CHANGED, NULL);

    // 停止位
    lv_obj_t* stopbitsa_label = lv_label_create(uart_panel);
    lv_label_set_text(stopbitsa_label, "停止位");
    lv_obj_set_pos(stopbitsa_label, 5, 95);
    stopbitsa_dropdown = lv_dropdown_create(uart_panel);
    lv_obj_set_user_data(stopbitsa_dropdown, (void*)1004);
    lv_obj_set_size(stopbitsa_dropdown, 100, 30);
    lv_dropdown_set_options(stopbitsa_dropdown, "1\n1.5\n2");
    lv_dropdown_set_selected(stopbitsa_dropdown, 0);
    lv_obj_set_pos(stopbitsa_dropdown, 80, 90);
    lv_obj_add_event_cb(stopbitsa_dropdown, serial_init_event, LV_EVENT_VALUE_CHANGED, NULL);

    // 协议
    lv_obj_t* protocola_label = lv_label_create(uart_panel);
    lv_label_set_text(protocola_label, "协议");
    lv_obj_set_pos(protocola_label, 5, 125);
    protocola_dropdown = lv_dropdown_create(uart_panel);
    lv_obj_set_user_data(protocola_dropdown, (void*)1005);
    lv_obj_set_size(protocola_dropdown, 100, 30);
    lv_dropdown_set_options(protocola_dropdown, "自定义\n小米测试\n松下测试");
    lv_dropdown_set_selected(protocola_dropdown, 0);
    lv_obj_set_pos(protocola_dropdown, 80, 120);
    lv_obj_add_event_cb(protocola_dropdown, serial_init_event, LV_EVENT_VALUE_CHANGED, NULL);




    // 串口A
    lv_obj_t* uartb_label = lv_label_create(uart_panel);
    lv_label_set_text(uartb_label, "串口1");
    lv_obj_set_pos(uartb_label, 5, 155);

    uartb_sw = lv_switch_create(uart_panel);
    lv_obj_set_user_data(uartb_sw, (void*)1001);
    lv_obj_set_size(uartb_sw, 60, 30);
    lv_obj_set_pos(uartb_sw, 110, 150);
    // lv_obj_align_to(swuarta_sw uarta_label, LV_ALIGN_OUT_RIGHT_MID, 10, 0);
    lv_obj_add_event_cb(uartb_sw, serial_init_event, LV_EVENT_VALUE_CHANGED, NULL);

    // 波特率
    lv_obj_t* baudb_label = lv_label_create(uart_panel);
    lv_label_set_text(baudb_label, "波特率");
    lv_obj_set_pos(baudb_label, 5, 185);
    // 波特率下拉选择
    baudb_dropdown = lv_dropdown_create(uart_panel);
    lv_obj_set_user_data(baudb_dropdown, (void*)1002);
    lv_obj_set_size(baudb_dropdown, 100, 30);
    lv_dropdown_set_options(baudb_dropdown, "4800\n9600\n19200\n38400\n57600\n115200\n460800\n921600");
    lv_dropdown_set_selected(baudb_dropdown, 4); // 默认115200
    lv_obj_set_pos(baudb_dropdown, 80, 180);
    lv_obj_add_event_cb(baudb_dropdown, serial_init_event, LV_EVENT_VALUE_CHANGED, NULL);

    // 校验
    lv_obj_t* parityb_label = lv_label_create(uart_panel);
    lv_label_set_text(parityb_label, "校验");
    lv_obj_set_pos(parityb_label, 5, 215);
    // 校验下拉选择
    parityb_dropdown = lv_dropdown_create(uart_panel);
    lv_obj_set_user_data(parityb_dropdown, (void*)1003);
    lv_obj_set_size(parityb_dropdown, 100, 30);
    lv_dropdown_set_options(parityb_dropdown, "无校验\n奇校验\n偶校验");
    lv_dropdown_set_selected(parityb_dropdown, 0);

    lv_obj_set_pos(parityb_dropdown, 80, 210);
    lv_obj_add_event_cb(parityb_dropdown, serial_init_event, LV_EVENT_VALUE_CHANGED, NULL);

    // 停止位
    lv_obj_t* stopbitsb_label = lv_label_create(uart_panel);
    lv_label_set_text(stopbitsb_label, "停止位");
    lv_obj_set_pos(stopbitsb_label, 5, 245);
    stopbitsb_dropdown = lv_dropdown_create(uart_panel);
    lv_obj_set_user_data(stopbitsb_dropdown, (void*)1004);
    lv_obj_set_size(stopbitsb_dropdown, 100, 30);
    lv_dropdown_set_options(stopbitsb_dropdown, "1\n1.5\n2");
    lv_dropdown_set_selected(stopbitsb_dropdown, 0);
    lv_obj_set_pos(stopbitsb_dropdown, 80, 240);
    lv_obj_add_event_cb(stopbitsb_dropdown, serial_init_event, LV_EVENT_VALUE_CHANGED, NULL);

    // 协议
    lv_obj_t* protocolb_label = lv_label_create(uart_panel);
    lv_label_set_text(protocolb_label, "协议");
    lv_obj_set_pos(protocolb_label, 5, 275);
    protocolb_dropdown = lv_dropdown_create(uart_panel);
    lv_obj_set_user_data(protocolb_dropdown, (void*)1005);
    lv_obj_set_size(protocolb_dropdown, 100, 30);
    lv_dropdown_set_options(protocolb_dropdown, "自定义\n小米测试\n松下测试");
    lv_dropdown_set_selected(protocolb_dropdown, 0);
    lv_obj_set_pos(protocolb_dropdown, 80, 270);
    lv_obj_add_event_cb(protocolb_dropdown, serial_init_event, LV_EVENT_VALUE_CHANGED, NULL);

}
