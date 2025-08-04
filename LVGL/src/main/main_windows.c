#include "main_windows.h"

// 全局变量
static lv_obj_t* nav_layer;       // 导航层
static lv_obj_t* main_layer;      // 主层
static lv_obj_t* pages[PAGE_COUNT]; // 主页面数组
static lv_obj_t* toggle_btn;      // 切换导航按钮
static bool nav_visible = true;   // 导航是否可见

// 页面颜色定义
static const lv_color_t page_colors[4] = {
    LV_COLOR_MAKE(0x70, 0xC8, 0xE8),  // 浅蓝色
    LV_COLOR_MAKE(0xE8, 0x70, 0xC8),  // 粉紫色
    LV_COLOR_MAKE(0xC8, 0xE8, 0x70),  // 黄绿色
    LV_COLOR_MAKE(0x70, 0xE8, 0xC8)   // 蓝绿色
};

// 页面名称
static const char* page_names[PAGE_COUNT] = {
    "协议","主页面1", "主页面2"
};

static void screen_click_cb(lv_event_t* e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if (code == LV_EVENT_CLICKED) {

        // 切换导航栏状态
        nav_visible = !nav_visible;

        if (nav_visible) {
            // 显示导航层
            lv_obj_clear_flag(nav_layer, LV_OBJ_FLAG_HIDDEN);
        }
        else {
            // 隐藏导航层
            lv_obj_add_flag(nav_layer, LV_OBJ_FLAG_HIDDEN);
        }
    }
}
// 页面切换回调
static void page_switch_cb(lv_event_t* e)
{
    lv_obj_t* btn = lv_event_get_target(e);
    uint32_t page_idx = (uint32_t)lv_event_get_user_data(e);

    // 隐藏所有页面
    for (int i = 0; i < PAGE_COUNT; i++) {
        lv_obj_add_flag(pages[i], LV_OBJ_FLAG_HIDDEN);
    }

    // 显示选中的页面
    lv_obj_clear_flag(pages[page_idx], LV_OBJ_FLAG_HIDDEN);
	lv_obj_add_flag(nav_layer, LV_OBJ_FLAG_HIDDEN);
}

// 创建导航层
static void create_nav_layer(void)
{
    // 创建导航层 (位于主层之上)
    nav_layer = lv_obj_create(lv_scr_act());
    lv_obj_set_size(nav_layer, NAV_WIDTH, SCREEN_HEIGHT);
    lv_obj_set_pos(nav_layer, 0, 0);
    lv_obj_set_flex_flow(nav_layer, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(nav_layer, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_all(nav_layer, 10, 0);

    // 设置半透明样式
    lv_obj_set_style_bg_opa(nav_layer, LV_OPA_70, 0);
    lv_obj_set_style_bg_color(nav_layer, lv_color_hex(0x333333), 0);
    lv_obj_set_style_border_width(nav_layer, 0, 0);

    // 确保导航层位于最顶层
    lv_obj_move_foreground(nav_layer);

    // 创建导航按钮
    for (int i = 0; i < PAGE_COUNT; i++) {
        lv_obj_t* btn = lv_btn_create(nav_layer);
        lv_obj_set_size(btn, NAV_WIDTH - 20, 50);
        lv_obj_add_event_cb(btn, page_switch_cb, LV_EVENT_CLICKED, (void*)(uintptr_t)i);

        // 按钮样式
        lv_obj_set_style_bg_color(btn, lv_color_hex(0x555555), 0);
        lv_obj_set_style_bg_color(btn, lv_color_hex(0x777777), LV_STATE_PRESSED);
        lv_obj_set_style_radius(btn, 10, 0);

        // 按钮标签
        lv_obj_t* label = lv_label_create(btn);
        lv_label_set_text(label, page_names[i]);

		lv_obj_set_style_text_font(label, &lv_font_sy_16, 0);
        lv_obj_set_style_text_color(label, lv_color_white(), 0);
        lv_obj_center(label);
    }
    lv_obj_add_flag(nav_layer, LV_OBJ_FLAG_EVENT_BUBBLE);
}



// 定义UI组件变量
static lv_obj_t* protocol_dropdown;
static lv_obj_t* serial_port_dropdown;
static lv_obj_t* serial_btn,* data_table;

// 串口按钮回调函数
static void serial_btn_event_cb(lv_event_t* e) {
    lv_obj_t* btn = lv_event_get_target(e);
    if (lv_obj_has_state(btn, LV_STATE_CHECKED)) {
        lv_label_set_text(lv_obj_get_child(btn, 0), "关闭串口");
    }
    else {
        lv_label_set_text(lv_obj_get_child(btn, 0), "打开串口");
    }
}
static void event_handler(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);
    if(code == LV_EVENT_VALUE_CHANGED) {
        char buf[32];
        lv_dropdown_get_selected_str(obj, buf, sizeof(buf));
        LV_LOG_USER("Option: %s", buf);
    }
}
static void lv_uart_protocol(lv_obj_t * page)
{
    // 创建页面
	lv_obj_set_size(page, 800, 480);
    lv_obj_set_style_bg_color(page, page_colors[2], 0);
    lv_obj_set_style_border_width(page, 0, 0);

    // 协议下拉框
    lv_obj_t* protocol_label = lv_label_create(page);
    lv_label_set_text(protocol_label, "协议:");
    lv_obj_align(protocol_label, LV_ALIGN_TOP_LEFT, 20, 20);

    protocol_dropdown = lv_dropdown_create(page);
    lv_dropdown_set_options(protocol_dropdown, "Modbus RTU\nModbus TCP\nCAN\nUART");
    lv_obj_align_to(protocol_dropdown, protocol_label, LV_ALIGN_OUT_RIGHT_MID, 10, 0);
    lv_obj_set_width(protocol_dropdown, 150);
	lv_obj_add_event_cb(protocol_dropdown, event_handler, LV_EVENT_ALL, NULL);

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
    lv_dropdown_set_options(serial_port_dropdown, "UART1\nUART2\n");
    lv_obj_align_to(serial_port_dropdown, serial_label, LV_ALIGN_OUT_RIGHT_MID, 10, 0);
    lv_obj_set_width(serial_port_dropdown, 150);

    // 创建表格对象
    data_table = lv_table_create(page);
    lv_obj_clear_flag(data_table, LV_OBJ_FLAG_SCROLLABLE);  // 完全禁用滚动
    // 设置表格大小和位置
    lv_obj_set_size(data_table, 800, 360);
    lv_obj_set_pos(data_table, 0, 120);

    // 设置行数和列数（注意：行列索引从0开始）
    lv_table_set_row_cnt(data_table, 6);   // 5行
    lv_table_set_col_cnt(data_table, 8);   // 8列

    // 设置列宽（单位：像素）
    lv_table_set_col_width(data_table, 0, 80);
    lv_table_set_col_width(data_table, 1, 120);
    lv_table_set_col_width(data_table, 2, 80);
    lv_table_set_col_width(data_table, 3, 120);
    lv_table_set_col_width(data_table, 4, 80);
    lv_table_set_col_width(data_table, 5, 120);
    lv_table_set_col_width(data_table, 6, 80);
    lv_table_set_col_width(data_table, 7, 120);

    // 填充数据
    lv_table_set_cell_value(data_table, 0, 0, "电压");
    lv_table_set_cell_value(data_table, 0, 1, "310.0V");
    lv_table_set_cell_value(data_table, 0, 2, "电流");
    lv_table_set_cell_value(data_table, 0, 3, "10.0A");
    lv_table_set_cell_value(data_table, 0, 4, "功率");
    lv_table_set_cell_value(data_table, 0, 5, "200.0W");
    lv_table_set_cell_value(data_table, 0, 6, "转速");
    lv_table_set_cell_value(data_table, 0, 7, "3000.0RPM");

    lv_table_set_cell_value(data_table, 1, 0, "电压");
    lv_table_set_cell_value(data_table, 1, 1, "310.0V");
    lv_table_set_cell_value(data_table, 1, 2, "电流");
    lv_table_set_cell_value(data_table, 1, 3, "10.0A");
    lv_table_set_cell_value(data_table, 1, 4, "功率");
    lv_table_set_cell_value(data_table, 1, 5, "200.0W");
    lv_table_set_cell_value(data_table, 1, 6, "转速");
    lv_table_set_cell_value(data_table, 1, 7, "3000.0RPM");

    lv_table_set_cell_value(data_table, 2, 0, "电压");
    lv_table_set_cell_value(data_table, 2, 1, "310.0V");
    lv_table_set_cell_value(data_table, 2, 2, "电流");
    lv_table_set_cell_value(data_table, 2, 3, "10.0A");
    lv_table_set_cell_value(data_table, 2, 4, "功率");
    lv_table_set_cell_value(data_table, 2, 5, "200.0W");
    lv_table_set_cell_value(data_table, 2, 6, "转速");
    lv_table_set_cell_value(data_table, 2, 7, "3000.0RPM");
	
}

// 全局变量
static lv_obj_t* serial_output;      // 接收数据显示区
static lv_obj_t* serial_input;       // 发送指令输入框
static lv_obj_t* config_panel;       // 右侧配置面板
static lv_timer_t* send_timer;       // 定时发送计时器
static bool config_visible = true;    // 配置面板可见状态

// 串口配置参数


static SerialConfig serial_cfg = {
    .baud_rate = 115200,
    .parity = 0,
    .stop_bits = 1,
    .data_bits = 8,
    .tx_interval_ms = 1000
};

// 模拟串口发送
static void serial_send(const char* data) {
    // 在接收框显示发送内容（蓝色）
    lv_textarea_add_text(serial_output, "\n# ");
    // lv_textarea_set_text_fmt(serial_output, "%sTx: %s", lv_textarea_get_text(serial_output), data);
    lv_textarea_set_cursor_pos(serial_output, LV_TEXTAREA_CURSOR_LAST);
}

// 定时发送回调
static void timer_send_cb(lv_timer_t* timer) {
    const char* cmd = lv_textarea_get_text(serial_input);
    if (cmd && strlen(cmd) > 0) {
        serial_send(cmd);
    }
}

// 发送按钮事件
static void send_btn_event(lv_event_t* e) {
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_CLICKED) {
        const char* cmd = lv_textarea_get_text(serial_input);
        if (cmd) serial_send(cmd);
    }
}

// 配置面板开关事件
static void toggle_config(lv_event_t* e) {
    config_visible = !config_visible;

    // 动画效果隐藏/显示配置面板
    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, config_panel);
    lv_anim_set_values(&a,
        config_visible ? -200 : 0,
        config_visible ? 0 : -200);
    lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)lv_obj_set_x);
    lv_anim_set_time(&a, 300);
    lv_anim_start(&a);

    // 更新按钮图标
    lv_obj_t* btn = lv_event_get_target(e);
    lv_label_set_text(lv_obj_get_child(btn, 0),
        config_visible ? LV_SYMBOL_LEFT : LV_SYMBOL_RIGHT);
}

// 1. 先声明所有回调函数
static void baud_rate_event(lv_event_t* e);
static void parity_event(lv_event_t* e);
static void stop_bits_event(lv_event_t* e);
static void interval_event(lv_event_t* e);
static void timer_switch_event(lv_event_t* e);

// 2. 创建配置面板（完整修正版）
static void create_config_panel(lv_obj_t* parent) {
    // 配置面板容器（右侧200px）
    config_panel = lv_obj_create(parent);
    lv_obj_set_size(config_panel, 200, 480);
    lv_obj_align(config_panel, LV_ALIGN_TOP_RIGHT, 0, 0);
    lv_obj_set_style_bg_color(config_panel, lv_color_hex(0xF0F0F0), 0);
    lv_obj_set_style_pad_all(config_panel, 10, 0);

    // 2.1 配置标题
    lv_obj_t* title = lv_label_create(config_panel);
    lv_label_set_text(title, "Serial Config");
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 10);

    // 2.5 定时发送间隔（修正4：使用独立回调）
    lv_obj_t* interval_label = lv_label_create(config_panel);
    lv_label_set_text(interval_label, "间隔");
    lv_obj_align_to(interval_label, title, LV_ALIGN_OUT_BOTTOM_MID, 0, 15);

    lv_obj_t* interval_roller = lv_roller_create(config_panel);
    lv_roller_set_options(interval_roller, "100ms\n500ms\n1s\n2s\n5s\n10s", LV_ROLLER_MODE_NORMAL);
    lv_roller_set_selected(interval_roller, 2, LV_ANIM_OFF);
    lv_obj_set_size(interval_roller, 180, 80);
    lv_obj_align_to(interval_roller, interval_label, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);
    lv_obj_add_event_cb(interval_roller, interval_event, LV_EVENT_VALUE_CHANGED, NULL);

    // 定时发送开关
    lv_obj_t* switch_label = lv_label_create(config_panel);
    lv_label_set_text(switch_label, "Auto Send:");
    lv_obj_align_to(switch_label, interval_roller, LV_ALIGN_OUT_BOTTOM_MID, -40, 20);

    lv_obj_t* sw = lv_switch_create(config_panel);
    lv_obj_align_to(sw, switch_label, LV_ALIGN_OUT_RIGHT_MID, 10, 0);
    lv_obj_add_event_cb(sw, timer_switch_event, LV_EVENT_VALUE_CHANGED, NULL);
}

// 3. 独立回调函数实现
static void baud_rate_event(lv_event_t* e) {
    uint16_t sel = lv_dropdown_get_selected(lv_event_get_target(e));
    const uint32_t bauds[] = { 9600, 19200, 38400, 57600, 115200 };
    serial_cfg.baud_rate = bauds[sel];
}

static void parity_event(lv_event_t* e) {
    serial_cfg.parity = lv_dropdown_get_selected(lv_event_get_target(e));
}

static void stop_bits_event(lv_event_t* e) {
    serial_cfg.stop_bits = lv_dropdown_get_selected(lv_event_get_target(e)) + 1;
}

static void interval_event(lv_event_t* e) {
    const uint16_t intervals[] = { 100, 500, 1000, 2000, 5000, 10000 };
    serial_cfg.tx_interval_ms = intervals[lv_roller_get_selected(lv_event_get_target(e))];
}

static void timer_switch_event(lv_event_t* e) {
    lv_obj_t* sw = lv_event_get_target(e);
    if (lv_obj_has_state(sw, LV_STATE_CHECKED)) {
        send_timer = lv_timer_create(timer_send_cb, serial_cfg.tx_interval_ms, NULL);
    }
    else {
        if (send_timer) lv_timer_del(send_timer);
    }
}



// 创建主界面
void create_serial_ui(lv_obj_t* parent) {
    // 1. 设置固定尺寸并禁止滚动
    lv_obj_set_size(parent, 800, 480);
    lv_obj_clear_flag(parent, LV_OBJ_FLAG_SCROLLABLE);

    // 2. 左侧通信区 (600x480)
    lv_obj_t* comm_panel = lv_obj_create(parent);
    lv_obj_set_size(comm_panel, 800, 480);
    lv_obj_align(comm_panel, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_obj_set_style_border_width(comm_panel, 0, 0);

    // 2.1 接收显示区
    serial_output = lv_textarea_create(comm_panel);
    lv_obj_set_size(serial_output, 580, 400);
    lv_obj_align(serial_output, LV_ALIGN_TOP_MID, 0, 10);
    lv_textarea_set_text(serial_output, "Serial Monitor Ready...\n");
    lv_textarea_set_cursor_pos(serial_output, LV_TEXTAREA_CURSOR_LAST);

    // 2.2 发送输入区
    serial_input = lv_textarea_create(comm_panel);
    lv_obj_set_size(serial_input, 500, 40);
    lv_obj_align(serial_input, LV_ALIGN_BOTTOM_LEFT, 10, -10);

    // 2.3 发送按钮
    lv_obj_t* btn = lv_btn_create(comm_panel);
    lv_obj_set_size(btn, 70, 40);
    lv_obj_align_to(btn, serial_input, LV_ALIGN_OUT_RIGHT_MID, 10, 0);
    lv_obj_t* btn_label = lv_label_create(btn);
    lv_label_set_text(btn_label, "Send");
    lv_obj_add_event_cb(btn, send_btn_event, LV_EVENT_CLICKED, NULL);

    // 3. 右侧配置面板
    create_config_panel(parent);

    // 4. 配置面板开关按钮
    lv_obj_t* toggle_btn = lv_btn_create(parent);
    lv_obj_set_size(toggle_btn, 30, 60);
    lv_obj_align(toggle_btn, LV_ALIGN_LEFT_MID, 600, 0);
    lv_obj_t* sym = lv_label_create(toggle_btn);
    lv_label_set_text(sym, LV_SYMBOL_LEFT);
    lv_obj_add_event_cb(toggle_btn, toggle_config, LV_EVENT_CLICKED, NULL);
}


static lv_obj_t* uarta_sw, * bauda_dropdown, *paritya_dropdown,* stopbitsa_dropdown,* protocola_dropdown;

static void serial_init_event(lv_event_t* e) {
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
        case 0: serial_cfg.stop_bits = 1; break; // 1位停止位
        case 1: serial_cfg.stop_bits = 2; break; // 1.5位停止位
        case 2: serial_cfg.stop_bits = 3; break; // 2位停止位
        default: serial_cfg.stop_bits = 1; break;
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
    case 1006:
    {
        // 定时发送开关
        if (lv_obj_has_state(obj, LV_STATE_CHECKED)) {
            // 启动定时发送
            send_timer = lv_timer_create(timer_send_cb, serial_cfg.tx_interval_ms, NULL);
        }
        else {
            // 停止定时发送
            if (send_timer) {
                lv_timer_del(send_timer);
                send_timer = NULL;
            }
        }
        break;
    }

    }
}
// 控制界面
// 串口开关与波特率选择 协议配置 停止位配置 奇偶校验位配置 默认为自定义状态
// 继电器控制
// PWM DAC控制 双路
static void lv_board_control(lv_obj_t* page)
{
    // 设定大小 禁止滚动
    lv_obj_set_size(page, SCREEN_WIDTH, SCREEN_HEIGHT);
    lv_obj_clear_flag(page, LV_OBJ_FLAG_SCROLLABLE);
    // 创建串口A面板
    lv_obj_t* uarta_panel = lv_obj_create(page);
    lv_obj_set_size(uarta_panel, 200, 480);
    lv_obj_align(uarta_panel, LV_ALIGN_LEFT_MID, 0, 0);
    lv_obj_set_style_radius(uarta_panel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_clear_flag(uarta_panel, LV_OBJ_FLAG_SCROLLABLE);
    // 串口A
    lv_obj_t *uarta_label = lv_label_create(uarta_panel);
    lv_label_set_text(uarta_label, "串口1");
    lv_obj_set_pos(uarta_label, 5, 5);

    uarta_sw = lv_switch_create(uarta_panel);
    lv_obj_set_user_data(uarta_sw,(void*)1001);
    lv_obj_set_size(uarta_sw, 40, 25);
    lv_obj_set_pos(uarta_sw, 110, 0);
    lv_obj_add_event_cb(uarta_sw, serial_init_event, LV_EVENT_VALUE_CHANGED, NULL);

    // 波特率
    lv_obj_t* bauda_label = lv_label_create(uarta_panel);
    lv_label_set_text(bauda_label, "波特率");
    lv_obj_set_pos(bauda_label, 5, 35);

    // 波特率下拉选择
    bauda_dropdown = lv_dropdown_create(uarta_panel);
    lv_obj_set_user_data(bauda_dropdown, (void*)1002);
    lv_obj_set_size(bauda_dropdown, 100, 25);
    lv_dropdown_set_options(bauda_dropdown, "9600\n19200\n38400\n57600\n115200");
    lv_dropdown_set_selected(bauda_dropdown, 4); // 默认115200
    lv_obj_set_pos(bauda_dropdown, 80, 30);
    lv_obj_add_event_cb(bauda_dropdown, serial_init_event, LV_EVENT_VALUE_CHANGED, NULL);

    // 校验
    lv_obj_t* paritya_label = lv_label_create(uarta_panel);
    lv_label_set_text(paritya_label, "校验");
    lv_obj_set_pos(paritya_label, 5, 65);
    // 校验下拉选择
    paritya_dropdown = lv_dropdown_create(uarta_panel);
    lv_obj_set_user_data(paritya_dropdown, (void*)1003);
    lv_obj_set_size(paritya_dropdown, 100, 25);
    lv_dropdown_set_options(paritya_dropdown, "无校验\n奇校验\n偶校验");
    lv_dropdown_set_selected(paritya_dropdown, 0);
    lv_obj_set_pos(paritya_dropdown, 80, 60);
    lv_obj_add_event_cb(paritya_dropdown, serial_init_event, LV_EVENT_VALUE_CHANGED, NULL);

    // 停止位
    lv_obj_t* stopbitsa_label = lv_label_create(uarta_panel);
    lv_label_set_text(stopbitsa_label, "停止位");
    lv_obj_set_pos(stopbitsa_label, 5, 95);
	
    stopbitsa_dropdown = lv_dropdown_create(uarta_panel);
    lv_obj_set_user_data(stopbitsa_dropdown, (void*)1004);
    lv_obj_set_size(stopbitsa_dropdown, 100, 25);
    lv_dropdown_set_options(stopbitsa_dropdown, "1\n1.5\n2");
    lv_dropdown_set_selected(stopbitsa_dropdown, 0);
    lv_obj_set_pos(stopbitsa_dropdown, 80, 90);
    lv_obj_add_event_cb(stopbitsa_dropdown, serial_init_event, LV_EVENT_VALUE_CHANGED, NULL);

    // 协议
    lv_obj_t* protocola_label = lv_label_create(uarta_panel);
    lv_label_set_text(protocola_label, "协议");
    lv_obj_set_pos(protocola_label, 5, 125);
	
    protocola_dropdown = lv_dropdown_create(uarta_panel);
    lv_obj_set_user_data(protocola_dropdown, (void*)1005);
    lv_obj_set_size(protocola_dropdown, 100, 25);
    lv_dropdown_set_options(protocola_dropdown, "自定义\n小米测试\n松下测试");
    lv_dropdown_set_selected(protocola_dropdown, 0);
    lv_obj_set_pos(protocola_dropdown, 80, 120);
    lv_obj_add_event_cb(protocola_dropdown, serial_init_event, LV_EVENT_VALUE_CHANGED, NULL);
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
    // create_serial_ui(pages[1]);
	lv_board_control(pages[2]);
#if 0
    for (int i = 0; i < PAGE_COUNT; i++) {
        
        lv_obj_set_size(pages[i], LV_PCT(100), LV_PCT(100));
        lv_obj_set_style_bg_color(pages[i], page_colors[i], 0);
        lv_obj_set_style_border_width(pages[i], 0, 0);

        // 添加页面标题
        lv_obj_t* title = lv_label_create(pages[i]);
        lv_label_set_text(title, page_names[i]);
        // lv_obj_set_style_text_font(title, &lv_font_sy_16, 0);
        lv_obj_set_style_text_color(title, lv_color_black(), 0);
        lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 20);
        // 默认隐藏除第一个外的所有页面
        lv_obj_add_flag(pages[i], LV_OBJ_FLAG_HIDDEN);
    }
	lv_obj_clear_flag(pages[2], LV_OBJ_FLAG_HIDDEN);
#endif
}

// 初始化界面
void lv_main_window(void)
{
	create_main_layer();
    create_nav_layer();
}
