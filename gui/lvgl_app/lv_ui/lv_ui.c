#include "lv_ui.h"
#include "lvgl.h"

#include "24cxx.h"
#include "hk_peripheral.h"
#include "trace.h"
#include "hk_exit.h"

typedef void (*sub_menu_handler)(void);

static lv_style_t style_rel;
static lv_style_t style_pre;
static lv_obj_t * btn[5];
static lv_obj_t * label[5];

static lv_obj_t * submenu;
static lv_obj_t * sublabel;

static lv_ui_info_t m_ui_info;
static sub_menu_handler p_sub_menu[5]; 

LV_FONT_DECLARE(fontCHN12);

void set_statement_person(void)
{
    submenu = lv_btn_create(lv_scr_act(), NULL);    
    // lv_obj_set_pos(submenu, 0, (i+1)*80);                           
    lv_obj_set_size(submenu, LV_HOR_RES_MAX/2, LV_VER_RES_MAX/2);                        
    lv_obj_align_mid(submenu, lv_scr_act(), LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_style(submenu, LV_BTN_PART_MAIN, &style_rel);

    sublabel = lv_label_create(submenu, NULL);   
    lv_label_set_text_fmt(sublabel, "请输入发言人数:[%d]", m_ui_info.person);
}

void main_menu(void)
{
    // 1. read param from eeprom
    m_ui_info.person = 3;
    m_ui_info.vol_input = 33;
    m_ui_info.vol_output = 88;
    m_ui_info.index = 0;
    m_ui_info.selected = 0;

    uint8_t buf1[] = "helloworld!";
    uint8_t buf2[20];

    uint8_t i = 0;

    #if 0
    trace_info("buf1:%s, strlen(buf1)=%d\r\n", buf1, strlen(buf1));

    for (i = 0; i < strlen(buf1); i++)
    {
        eeprom_write_reg(&eeprom_obj, i, &buf1[i], 1);
    }
    eeprom_obj.i2c_ops.delay_ms(2);
    for (i = 0; i < strlen(buf1); i++)
    {
        eeprom_read_reg(&eeprom_obj, i, &buf2[i], 1);
    }
    // eeprom_write_n_bytes(&eeprom_obj, 0x00, buf1, strlen(buf1));
    // eeprom_read_n_bytes(&eeprom_obj, 0x00, buf2, strlen(buf1));

    // buf2[i] = '\0';
    // eeprom_write_reg(&eeprom_obj, 0x0a, buf1, (strlen(buf1)));
    // eeprom_obj.i2c_ops.delay_ms(2);
    // eeprom_read_reg(&eeprom_obj, 0x0a, buf2, (strlen(buf1)));
    trace_info("buf2:%s\r\n", buf2);
    #endif

    #if 0
    trace_info("sizeof lv_ui_info_t %d\r\n", sizeof(lv_ui_info_t));

    uint8_t *buf = (uint8_t *)&m_ui_info;

    for (i = 0; i < sizeof(lv_ui_info_t); i++)
    {
        trace_info("buf[%d] = %d\r\n", i, buf[i]);
        eeprom_write_reg(&eeprom_obj, i, buf[i], 1);
    }

    eeprom_obj.i2c_ops.delay_ms(2);
    lv_ui_info_t ui_info;

    uint8_t buf1[4];

    // buf1 = (uint8_t *)malloc(sizeof(lv_ui_info_t));
    for (i = 0; i < sizeof(lv_ui_info_t); i++)
    {
        eeprom_read_reg(&eeprom_obj, i, buf1[i], 1);
        trace_info("buf1[%d] = %d\r\n", i, buf1[i]);
    }
    ui_info = *((lv_ui_info_t *)buf1);

    trace_info("ui_info : person : %d\r\n",     ui_info.person);
    trace_info("ui_info : vol_input : %d\r\n",  ui_info.vol_input);
    trace_info("ui_info : vol_output : %d\r\n", ui_info.vol_output);
    trace_info("ui_info : index : %d\r\n",      ui_info.index);
    #endif

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

    // init sub_menu_list
    p_sub_menu[1] = set_statement_person;
}

void scan_menu(void)
{
    hk_exit_pin_cfg *p_hk_exit_pin_cfg = (hk_exit_pin_cfg *)g_exit0_obj.exit_cfg.p_pin_cfg;
    hk_exit_pin_cfg *p_hk_exit_pin_cfg1 = (hk_exit_pin_cfg *)g_exit1_obj.exit_cfg.p_pin_cfg;

    if (p_hk_exit_pin_cfg->press_cnt > 4)
    {
        p_hk_exit_pin_cfg->press_cnt = 0;
    }

    if (m_ui_info.index != p_hk_exit_pin_cfg->press_cnt)
    {
        lv_obj_add_style(btn[m_ui_info.index], LV_BTN_PART_MAIN, &style_rel);

        m_ui_info.index = p_hk_exit_pin_cfg->press_cnt;
        lv_obj_add_style(btn[m_ui_info.index], LV_BTN_PART_MAIN, &style_pre);
    }

    if (p_hk_exit_pin_cfg1->press_cnt == 1)         // 进入子菜单
    {
        if (!submenu)
        {
            p_sub_menu[m_ui_info.index]();
        }
    }
    else if (p_hk_exit_pin_cfg1->press_cnt == 2)   // 退出子菜单
    {
        p_hk_exit_pin_cfg1->press_cnt = 0;

        lv_obj_del(submenu);
        lv_obj_del(sublabel);
    }
}
