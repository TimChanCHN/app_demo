#include "lv_test.h"
#include "lv_test_fs.h"
#include "../../lv_examples.h"

LV_FONT_DECLARE(fontCHN12);

static lv_style_t style;
static lv_style_t style_pressed;
static lv_obj_t * btn[5];
static lv_obj_t * label[5];
static int person = 0;
static int vol_input = 0;
static int vol_output = 80;

static void set_input_vol(void)
{
    
}

static void btn_event_cb(lv_obj_t* botton, lv_event_t event)
{
    lv_indev_t * indev = lv_indev_get_act();
    static int index = -1, index_pre = -1;
    static int cnt_table[5] = {0};

    if (event == LV_EVENT_CLICKED) {
        if (indev->driver.type == LV_INDEV_TYPE_POINTER)
        {
            printf("coord(x,y) = (%d,%d)\r\n", indev->proc.types.pointer.act_point.x, indev->proc.types.pointer.act_point.y);

            if (indev->proc.types.pointer.act_point.y > 80)
            {
                if (indev->proc.types.pointer.act_point.y < 160)
                {
                    index = 0;
                }
                else if (indev->proc.types.pointer.act_point.y < 240)
                {
                    index = 1;
                }
                else if (indev->proc.types.pointer.act_point.y < 320)
                {
                    index = 2;
                }
                else if (indev->proc.types.pointer.act_point.y < 400)
                {
                    index = 3;
                }
                else if (indev->proc.types.pointer.act_point.y < 480)
                {
                    index = 4;
                }

                cnt_table[index]++;
                if (index == index_pre)
                {
                    if (cnt_table[index] % 2 != 0)
                    {
                        lv_obj_add_style(btn[index], LV_BTN_PART_MAIN, &style_pressed);
                    }
                    else
                    {
                        lv_obj_add_style(btn[index], LV_BTN_PART_MAIN, &style);
                    }
                }
                else
                {
                    lv_obj_add_style(btn[index], LV_BTN_PART_MAIN, &style_pressed);
                    if (index_pre != -1)
                    {
                        lv_obj_add_style(btn[index_pre], LV_BTN_PART_MAIN, &style);
                    }
                    cnt_table[index_pre] = 0;
                }
                index_pre = index;
            }

        }   
    }
}

void show_lvgl(void)
{

    lv_style_init(&style);
    lv_style_set_radius(&style, LV_STATE_DEFAULT, 0);
    lv_style_set_bg_opa(&style, LV_STATE_DEFAULT, LV_OPA_COVER);
    lv_style_set_bg_color(&style, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_style_set_text_font(&style, LV_STATE_DEFAULT, &fontCHN12);
    lv_style_set_text_color(&style, LV_STATE_DEFAULT, LV_COLOR_BLACK);

    lv_style_init(&style_pressed);
    lv_style_set_radius(&style_pressed, LV_STATE_DEFAULT, 0);
    lv_style_set_bg_opa(&style_pressed, LV_STATE_DEFAULT, LV_OPA_COVER);
    lv_style_set_bg_color(&style_pressed, LV_STATE_DEFAULT, LV_COLOR_YELLOW);
    lv_style_set_text_color(&style_pressed, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    // 菜单
    lv_obj_t * botton = lv_btn_create(lv_scr_act(), NULL);  
    lv_obj_set_pos(botton, 0, 0);                           
    lv_obj_set_size(botton, LV_HOR_RES_MAX, 80);            
    lv_obj_add_style(botton, LV_BTN_PART_MAIN, &style);
    lv_obj_set_event_cb(botton, btn_event_cb);              

    lv_obj_t * label0 = lv_label_create(botton, NULL);      
    lv_label_set_text(label0, "菜单");                     

    // [1] 模式:先进先出
    btn[0] = lv_btn_create(lv_scr_act(), NULL);    
    lv_obj_set_pos(btn[0], 0, 80);                           
    lv_obj_set_size(btn[0], LV_HOR_RES_MAX, 80);                        
    lv_obj_add_style(btn[0], LV_BTN_PART_MAIN, &style);
    lv_obj_set_event_cb(btn[0], btn_event_cb);                 

    label[0] = lv_label_create(btn[0], NULL);         
    lv_label_set_text(label[0], "模式:先进先出");                    

    // [2] 发言人数:[3]
    btn[1] = lv_btn_create(lv_scr_act(), NULL);   
    lv_obj_set_pos(btn[1], 0, 160);                           
    lv_obj_set_size(btn[1], LV_HOR_RES_MAX, 80);                         
    lv_obj_add_style(btn[1], LV_BTN_PART_MAIN, &style);
    lv_obj_set_event_cb(btn[1], btn_event_cb);               

    label[1] = lv_label_create(btn[1], NULL);        
    lv_label_set_text_fmt(label[1], "发言人数:[%d]", person);                 

    // [3] 输入音量:[36]
    btn[2] = lv_btn_create(lv_scr_act(), NULL);    
    lv_obj_set_pos(btn[2], 0, 240);                          
    lv_obj_set_size(btn[2], LV_HOR_RES_MAX, 80);                       
    lv_obj_add_style(btn[2], LV_BTN_PART_MAIN, &style);
    lv_obj_set_event_cb(btn[2], btn_event_cb);              

    label[2] = lv_label_create(btn[2], NULL);          
    lv_label_set_text_fmt(label[2], "输入音量:[%d]", vol_input);                    

    // [4] 输出音量:[56]
    btn[3] = lv_btn_create(lv_scr_act(), NULL);   
    lv_obj_set_pos(btn[3], 0, 320);                         
    lv_obj_set_size(btn[3], LV_HOR_RES_MAX, 80);                        
    lv_obj_add_style(btn[3], LV_BTN_PART_MAIN, &style);
    lv_obj_set_event_cb(btn[3], btn_event_cb);               

    label[3] = lv_label_create(btn[3], NULL);          
    lv_label_set_text_fmt(label[3], "输出音量:[%d]", vol_output);                

    // [5] 系统设置
    btn[4] = lv_btn_create(lv_scr_act(), NULL);  
    lv_obj_set_pos(btn[4], 0, 400);                          
    lv_obj_set_size(btn[4], LV_HOR_RES_MAX, 80);                        
    lv_obj_add_style(btn[4], LV_BTN_PART_MAIN, &style);
    lv_obj_set_event_cb(btn[4], btn_event_cb);               

    label[4] = lv_label_create(btn[4], NULL);        
    lv_label_set_text(label[4], "系统设置");                   

}

void lv_test_demo(void)
{
    // 1. init filesystem
    lv_fs_init();

    // 2. show lvgl
    show_lvgl();
}


