#pragma once
#include "ui.h"



typedef struct {
    uint32_t baud_rate;
    uint8_t parity;     // 0:None, 1:Odd, 2:Even
    uint8_t stop_bits;  // 1 or 2
    uint8_t data_bits;  // 5-8
    uint16_t tx_interval_ms;
} SerialConfig;


void page_switch_cb(lv_event_t* e);
void screen_click_cb(lv_event_t* e);
void serial_btn_event_cb(lv_event_t* e);
void serial_init_event(lv_event_t* e);


void uart_protocol_event_cb(lv_event_t* e);
