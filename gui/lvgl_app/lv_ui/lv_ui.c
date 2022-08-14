#include "lv_ui.h"
#include "lvgl.h"

#include "24cxx.h"
#include "hk_peripheral.h"
#include "trace.h"

static lv_style_t style_rel;
static lv_style_t style_pre;
static lv_obj_t * btn[5];
static lv_obj_t * label[5];

static lv_ui_info_t m_ui_info;

LV_FONT_DECLARE(fontCHN12);

void main_menu(void)
{
    // 1. read param from eeprom
    m_ui_info.person = 3;
    m_ui_info.vol_input = 33;
    m_ui_info.vol_output = 88;
    m_ui_info.index = 0;

    uint8_t buf1[] = "helloworld!";
    uint8_t buf2[20];

    uint8_t ch = 'a';
    uint8_t ch1;

    eeprom_write_reg(&eeprom_obj, 0x00, &ch, 1);
    eeprom_read_reg(&eeprom_obj, 0x00, &ch1, 1);
    trace_info("ch = %c\r\n", ch1);
    // trace_info("buf2:%s\r\n", buf2);

    // eeprom_write_reg(&eeprom_obj, 0x00, (uint8_t *)&m_ui_info, sizeof(lv_ui_info_t));

    // lv_ui_info_t ui_info;
    // eeprom_read_reg(&eeprom_obj, 0x00, (uint8_t *)&ui_info, sizeof(lv_ui_info_t));

    // 2. show ui
    lv_style_init(&style_rel);
    lv_style_set_radius(&style_rel, LV_STATE_DEFAULT, 0);
    lv_style_set_bg_opa(&style_rel, LV_STATE_DEFAULT, LV_OPA_COVER);
    lv_style_set_bg_color(&style_rel, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_style_set_text_font(&style_rel, LV_STATE_DEFAULT, &fontCHN12);
    lv_style_set_text_color(&style_rel, LV_STATE_DEFAULT, LV_COLOR_BLACK);

    lv_style_init(&style_pre);
    lv_style_set_radius(&style_pre, LV_STATE_DEFAULT, 0);
    lv_style_set_bg_opa(&style_pre, LV_STATE_DEFAULT, LV_OPA_COVER);
    lv_style_set_bg_color(&style_pre, LV_STATE_DEFAULT, LV_COLOR_YELLOW);
    lv_style_set_text_color(&style_pre, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    // 菜单
    lv_obj_t * botton = lv_btn_create(lv_scr_act(), NULL);  
    lv_obj_set_pos(botton, 0, 0);                           
    lv_obj_set_size(botton, LV_HOR_RES_MAX, 80);            
    lv_obj_add_style(botton, LV_BTN_PART_MAIN, &style_rel);           

    lv_obj_t * label0 = lv_label_create(botton, NULL);      
    lv_label_set_text(label0, "菜单");      

    uint8_t i = 0;
    for (i = 0; i < 5; i++)
    {
        btn[i] = lv_btn_create(lv_scr_act(), NULL);    
        lv_obj_set_pos(btn[i], 0, (i+1)*80);                           
        lv_obj_set_size(btn[i], LV_HOR_RES_MAX, 80);                        
        lv_obj_add_style(btn[i], LV_BTN_PART_MAIN, &style_rel);
        // lv_obj_set_event_cb(btn[i], btn_event_cb);                 

        label[i] = lv_label_create(btn[i], NULL);   
    }

    lv_label_set_text(label[0], "模式:先进先出");
    lv_label_set_text_fmt(label[1], "发言人数:[%d]", m_ui_info.person);
    lv_label_set_text_fmt(label[2], "输入音量:[%d]", m_ui_info.vol_input);    
    lv_label_set_text_fmt(label[3], "输出音量:[%d]", m_ui_info.vol_output); 
    lv_label_set_text(label[4], "系统设置");  

    lv_obj_add_style(btn[m_ui_info.index], LV_BTN_PART_MAIN, &style_pre);
}
