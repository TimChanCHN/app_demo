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
#include "exfuns.h"

#include "cm_backtrace.h"

#define HARDWARE_VERSION "V1.0.0" 
#define SOFTWARE_VERSION "V0.1.0"

TIMER_DEF(m_test_timer);

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
	
	// g_tftlcd9341_obj.tftlcd_ops.init(&g_tftlcd9341_obj.tftlcd_cfg, &g_tftlcd9341_obj.tftlcd_ops);
	// g_tftlcd3510_obj.tftlcd_ops.init(&g_tftlcd3510_obj.tftlcd_cfg, &g_tftlcd3510_obj.tftlcd_ops);
	// g_touch_obj.touch_ops.init(&g_touch_obj.touch_cfg);
	// trace_info("touch init ok\r\n");

	// lvgl设置
	lv_init();
	lv_port_disp_init();
	lv_port_indev_init();
	// lv_ex_get_started_3();
	
	#if 1
	lv_port_fs_init();
	lv_fs_res_t fs_res;
	lv_fs_file_t *file1 = lv_mem_alloc(sizeof(lv_fs_file_t *));

	// fs_res=lv_fs_open(file1, "P:/lvgl.txt", LV_FS_MODE_WR | LV_FS_MODE_RD);
	// if(fs_res != LV_FS_RES_OK)
	// 	printf("open error! code:%d\r\n",fs_res);

	fs_res=lv_fs_write(file1,"test",4,NULL);
	if(fs_res != LV_FS_RES_OK)
		printf("write error! code:%d\r\n",fs_res);
	trace_info("sss\r\n");
	fs_res=lv_fs_close(file1);
	if(fs_res != LV_FS_RES_OK)
		printf("close error! code:%d\r\n",fs_res);
	trace_info("xxx\r\n");
	lv_mem_free(file1);
	#endif

	trace_info("loop\r\n");
	trace_debug("debug\r\n");
	lv_ex_get_started_3();
	while (1)
	{
		letter_shell_loop_task();
		TIMER_SCHEDULER_LOOP();

		lv_task_handler();
	}
}
