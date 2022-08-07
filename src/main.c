#include "stdio.h"
#include "string.h"

#include "boards.h"
#include "version.h"

#include "letter_handler.h"
#include "trace.h"
#include "app_timer.h"
#include "hk_peripheral.h"

#include "lv_obj.h"
#include "lv_port_disp.h"
#include "lv_port_indev.h"
#include "lv_port_fs.h"
// #include "lv_demo_widgets.h"

#include "lv_ex_get_started.h"

#include "gt9147.h"
#include <stdlib.h>

#include "ff.h"  

#include "cm_backtrace.h"

#define HARDWARE_VERSION "V1.0.0" 
#define SOFTWARE_VERSION "V0.1.0"

TIMER_DEF(m_test_timer);
LV_FONT_DECLARE(fontCHN12);

void show_my_font(void)
{
	static lv_style_t style_font;
	lv_style_init(&style_font);
	lv_style_set_text_font(&style_font, LV_STATE_DEFAULT, &fontCHN12);  //样式使用自定义字体
	lv_style_set_text_color(&style_font, LV_STATE_DEFAULT, LV_COLOR_BLACK);   //设置字体颜色


    lv_obj_t * btn = lv_btn_create(lv_scr_act(), NULL);     /*Add a button the current screen*/
    lv_obj_set_pos(btn, 0, 0);                            /*Set its position*/
    lv_obj_set_size(btn, 100, 200);                          /*Set its size*/
    // lv_obj_set_event_cb(btn, btn_event_cb);                 /*Assign a callback to the button*/

    lv_obj_t * label = lv_label_create(btn, NULL);          /*Add a label to the button*/
    lv_label_set_text(label, "我");                     /*Set the labels text*/
	lv_obj_add_style(label, LV_OBJ_PART_MAIN, &style_font);
}

void test_timer_handler(void *p_data)
{
    static bool is_led_off = false;

    if(is_led_off)
    {
        g_led_obj.gpio_ops.gpio_output_set(&g_led_obj.gpio_cfg, is_led_off);
        is_led_off = false;
    }
    else
    {
        g_led_obj.gpio_ops.gpio_output_set(&g_led_obj.gpio_cfg, is_led_off);
        is_led_off = true;
    }

    // trace_debug("ticks = %lu \n\r", (uint32_t)g_timer3_object.timer_cfg.ticks);
}

int main(void)
{
	g_systick_obj.systick_ops.systick_init(&g_systick_obj.systick_cfg);
	g_led_obj.gpio_ops.gpio_init(&g_led_obj.gpio_cfg);
	g_led_obj.gpio_ops.gpio_output_set(&g_led_obj.gpio_cfg, 1);

	trace_init();
	letter_shell_init();
	// init_nt_shell();
	cm_backtrace_init("app", HARDWARE_VERSION, SOFTWARE_VERSION);

	TIMER_INIT(&g_timer3_object);

	TIMER_CREATE(&m_test_timer, false, false, test_timer_handler);
	TIMER_START(m_test_timer, 500);

	// lvgl设置
	lv_init();
	lv_port_disp_init();
	lv_port_indev_init();
	lv_port_fs_init();


	trace_info("loop\r\n");
	trace_debug("debug\r\n");

	show_my_font();

	while (1)
	{
		letter_shell_loop_task();
		TIMER_SCHEDULER_LOOP();

		lv_task_handler();
	}
}
