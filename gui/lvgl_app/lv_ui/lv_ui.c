#include <stdlib.h>

#include "lv_ui.h"
// #include "lvgl.h"
#include "../lvgl/lvgl.h"
 
#include "24cxx.h"
#include "hk_peripheral.h"
#include "trace.h"
#include "hk_exit.h"

#define MEM_TYPE        0       // 0: EEPROM, 1: FLASH

typedef void (*sub_menu_handler)(void);

static lv_style_t style_rel;
static lv_style_t style_pre;
static lv_obj_t * btn[5];
static lv_obj_t * label[5];

static lv_obj_t * submenu;
static lv_obj_t * sublabel;
static lv_obj_t * btn_ok;
static lv_obj_t * btn_fail;

uint8_t temp;

static lv_ui_info_t m_ui_info;
static sub_menu_handler p_sub_menu[5]; 

LV_FONT_DECLARE(fontCHN12);
LV_FONT_DECLARE(myfont)

void set_mode(void)
{
    static uint8_t i = 0;

    if (g_encoder_obj.dir != 0)
    {
        g_encoder_obj.dir = 0;
        i++;

        if (i % 2 == 0)
        {
            lv_label_set_text(sublabel, "模式:后进后出");
        }
        else
        {
            lv_label_set_text(sublabel, "模式:先进先出");
        }
    }
}

void set_statement_person(void)
{
    if (g_encoder_obj.dir == 1)
    {
        g_encoder_obj.dir = 0;
        if (m_ui_info.person != 100)
        {
            m_ui_info.person++;
        }
    }
    else if (g_encoder_obj.dir == 2)
    {
        g_encoder_obj.dir = 0;
        if (m_ui_info.person != 0)
        {
            m_ui_info.person--;
        }
    }

    lv_label_set_text_fmt(sublabel, "请输入:[%d]", m_ui_info.person);
}

void set_input_voice(void)
{ 
    if (g_encoder_obj.dir == 1)
    {
        g_encoder_obj.dir = 0;
        if (m_ui_info.vol_input != 100)
        {
            m_ui_info.vol_input++;
        }
    }
    else if (g_encoder_obj.dir == 2)
    {
        g_encoder_obj.dir = 0;
        if (m_ui_info.vol_input != 0)
        {
            m_ui_info.vol_input--;
        }
    }
    lv_label_set_text_fmt(sublabel, "输入音量:[%d]", m_ui_info.vol_input);
}

void set_output_voice(void)
{ 
    if (g_encoder_obj.dir == 1)
    {
        g_encoder_obj.dir = 0;
        if (m_ui_info.vol_output != 100)
        {
            m_ui_info.vol_output++;
        }
    }
    else if (g_encoder_obj.dir == 2)
    {
        g_encoder_obj.dir = 0;
        if (m_ui_info.vol_output != 0)
        {
            m_ui_info.vol_output--;
        }
    }
    lv_label_set_text_fmt(sublabel, "输出音量:[%d]", m_ui_info.vol_output);
}

// TODO: 从flash读数据必须在lvgl初始化前执行
// 从flash/eeprom中读取数据
void read_data_from_mem(void)
{
    uint8_t *buf = malloc(sizeof(lv_ui_info_t));

    #if (MEM_TYPE == 1)
    uint32_t addr = 0x0807F800;

    g_flash_obj.flash_ops.flash_read(&g_flash_obj.flash_cfg, addr, sizeof(lv_ui_info_t), buf);

    #elif (MEM_TYPE == 0)
    uint16_t addr = 0x00;

    eeprom_read(&eeprom_obj, addr, buf, sizeof(lv_ui_info_t));
    #endif

    memcpy(&m_ui_info, buf, sizeof(lv_ui_info_t));
    free(buf);
}

// 从flash/eeprom中写入数据
void write_data_to_mem(void)
{
    uint8_t *buf = (uint8_t *)&m_ui_info;

    #if (MEM_TYPE == 1)
    uint32_t addr = 0x0807F800;
    g_flash_obj.flash_ops.flash_write(&g_flash_obj.flash_cfg, addr, sizeof(lv_ui_info_t), buf);

    #elif (MEM_TYPE == 0)
    uint16_t addr = 0x00;
    eeprom_write(&eeprom_obj, addr, buf, sizeof(lv_ui_info_t));

    #endif
}

// ui界面设置
void main_menu(void)
{
    // 1. read param from eeprom
    // m_ui_info.person = 3;
    // m_ui_info.vol_input = 33;
    // m_ui_info.vol_output = 88;
    m_ui_info.index = 0;
    m_ui_info.selected = true;             // 与UI布局的“确定”按键
    m_ui_info.exit_status = 0;

    uint8_t i = 0;

    trace_info("ui_info : person : %d\r\n",     m_ui_info.person);
    trace_info("ui_info : vol_input : %d\r\n",  m_ui_info.vol_input);
    trace_info("ui_info : vol_output : %d\r\n", m_ui_info.vol_output);
    trace_info("ui_info : index : %d\r\n",      m_ui_info.index);

    // 2. show ui
    lv_style_init(&style_rel);
    lv_style_set_radius(&style_rel, LV_STATE_DEFAULT, 0);
    lv_style_set_bg_opa(&style_rel, LV_STATE_DEFAULT, LV_OPA_COVER);
    lv_style_set_bg_color(&style_rel, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_style_set_text_font(&style_rel, LV_STATE_DEFAULT, &myfont);
    lv_style_set_text_color(&style_rel, LV_STATE_DEFAULT, LV_COLOR_BLACK);

    lv_style_init(&style_pre);
    lv_style_set_radius(&style_pre, LV_STATE_DEFAULT, 0);
    lv_style_set_bg_opa(&style_pre, LV_STATE_DEFAULT, LV_OPA_COVER);
    lv_style_set_bg_color(&style_pre, LV_STATE_DEFAULT, LV_COLOR_YELLOW);
    lv_style_set_text_color(&style_pre, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    // 菜单
    lv_obj_t * botton = lv_btn_create(lv_scr_act(), NULL);  
    lv_obj_set_pos(botton, 0, 0);                           
    lv_obj_set_size(botton, LV_HOR_RES_MAX, 40);            
    lv_obj_add_style(botton, LV_BTN_PART_MAIN, &style_rel);           

    lv_obj_t * label0 = lv_label_create(botton, NULL);      
    lv_label_set_text(label0, "菜单");      

    for (i = 0; i < 5; i++)
    {
        btn[i] = lv_btn_create(lv_scr_act(), NULL);    
        lv_obj_set_pos(btn[i], 0, (i+1)*40);                           
        lv_obj_set_size(btn[i], LV_HOR_RES_MAX, 40);                        
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
    p_sub_menu[0] = set_mode;
    p_sub_menu[1] = set_statement_person;
    p_sub_menu[2] = set_input_voice;
    p_sub_menu[3] = set_output_voice;
}

/* 以下函数都是处理窗口调用的状态机 */
// window1 : 不同选项对应的窗口
void open_window(void)
{
    submenu = lv_btn_create(lv_scr_act(), NULL);    
    lv_obj_set_size(submenu, LV_HOR_RES_MAX/2, LV_VER_RES_MAX/2);                        
    lv_obj_align_mid(submenu, lv_scr_act(), LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_style(submenu, LV_BTN_PART_MAIN, &style_rel);

    sublabel = lv_label_create(submenu, NULL); 

    switch (m_ui_info.index)
    {
    case 0:
        temp = 0;
        break;
    
    case 1:
        temp = m_ui_info.person;
        break;

    case 2:
        temp = m_ui_info.vol_input;
        break;

    case 3:
        temp = m_ui_info.vol_output;
        break;

    case 4:
        temp = 0;
        break;

    default:
        break;
    }
}

// window2:确定/取消
void open_window2(void)
{
    lv_obj_del(sublabel);

    lv_obj_t *label_ok, *label_fail;

    btn_ok = lv_btn_create(lv_scr_act(), NULL);
    lv_obj_set_size(btn_ok, LV_HOR_RES_MAX/6, LV_VER_RES_MAX/6);
    lv_obj_add_style(btn_ok, LV_BTN_PART_MAIN, &style_rel);
    lv_obj_align(btn_ok, submenu, LV_ALIGN_CENTER, -40, 0);

    label_ok = lv_label_create(btn_ok, NULL);
    lv_label_set_text(label_ok, "确定");

    btn_fail = lv_btn_create(lv_scr_act(), NULL);
    lv_obj_set_size(btn_fail, LV_HOR_RES_MAX/6, LV_VER_RES_MAX/6);
    lv_obj_add_style(btn_fail, LV_BTN_PART_MAIN, &style_rel);
    lv_obj_align(btn_fail, submenu, LV_ALIGN_CENTER, 40, 0);

    label_fail = lv_label_create(btn_fail, NULL);
    lv_label_set_text(label_fail, "取消");

    lv_obj_add_style(btn_ok, LV_BTN_PART_MAIN, &style_pre);
}

// 关闭窗口
void close_window(void)
{
    lv_obj_del(submenu);

    lv_obj_del(btn_ok);
    lv_obj_del(btn_fail);
    submenu = NULL;
}

// 
void update_ui_info(void)
{
    trace_info("write flash...\r\n");
    write_data_to_mem();

    trace_info("select is %d\r\n", m_ui_info.selected);
    if (!m_ui_info.selected)        // 选择取消
    {
        m_ui_info.selected = true;
        switch (m_ui_info.index)
        {
        case 0:
            temp = 0;
            break;
        
        case 1:
            m_ui_info.person = temp;
            break;

        case 2:
            m_ui_info.vol_input = temp;
            break;

        case 3:
            m_ui_info.vol_output = temp;
            break;

        case 4:
            temp = 0;
            break;

        default:
            break;
        }
    }

    lv_label_set_text(label[0], "模式:先进先出");
    lv_label_set_text_fmt(label[1], "发言人数:[%d]", m_ui_info.person);
    lv_label_set_text_fmt(label[2], "输入音量:[%d]", m_ui_info.vol_input);    
    lv_label_set_text_fmt(label[3], "输出音量:[%d]", m_ui_info.vol_output); 
    lv_label_set_text(label[4], "系统设置");  
}

// 遍历查询encoder的旋转情况以及按键情况
void menu_status_handler(void)
{
    bool is_pressed = g_encoder_obj.pressed;
    uint8_t dir = g_encoder_obj.dir;

    // 1. 处理主菜单选择
    if (m_ui_info.exit_status == 0)
    {
        if (dir != 0)
        {
            lv_obj_add_style(btn[m_ui_info.index], LV_BTN_PART_MAIN, &style_rel);

            if (dir == 1)   // 正转，递增
            {
                m_ui_info.index++;
                if (m_ui_info.index == 5)
                {
                    m_ui_info.index = 0;
                }
            }
            else if (dir == 2)
            {
                if (m_ui_info.index == 0)
                {
                    m_ui_info.index = 5;
                }
                m_ui_info.index--;
            }

            lv_obj_add_style(btn[m_ui_info.index], LV_BTN_PART_MAIN, &style_pre);
            g_encoder_obj.dir = 0;
        }
    }

    // 2. 处理按钮
    if (is_pressed == 1)
    {
        g_encoder_obj.pressed = 0;
        m_ui_info.exit_status++;

        trace_info("exit status : %d\r\n", m_ui_info.exit_status);

        if (m_ui_info.exit_status == 3)
        {
            m_ui_info.exit_status = 0;
        }

        if (m_ui_info.exit_status == LV_UI_MODE_ENTER)
        {
            open_window(); 
        }
        else if (m_ui_info.exit_status == LV_UI_MODE_SELECT)
        {
            open_window2();
        }   
        else if (m_ui_info.exit_status == LV_UI_MODE_EXIT)
        {
            close_window();

            update_ui_info();
        }
    }

    // 处理输入内容
    if (m_ui_info.exit_status == LV_UI_MODE_ENTER)
    {
        p_sub_menu[m_ui_info.index]();
    }
    else if (m_ui_info.exit_status == LV_UI_MODE_SELECT)
    {
        if (g_encoder_obj.dir != 0)
        {
            g_encoder_obj.dir = 0;
            m_ui_info.selected =!m_ui_info.selected;

            if (m_ui_info.selected)
            {
                lv_obj_add_style(btn_ok, LV_BTN_PART_MAIN, &style_pre);
                lv_obj_add_style(btn_fail, LV_BTN_PART_MAIN, &style_rel);
            }
            else
            {
                lv_obj_add_style(btn_ok, LV_BTN_PART_MAIN, &style_rel);
                lv_obj_add_style(btn_fail, LV_BTN_PART_MAIN, &style_pre);
            }
        }
    }
}
