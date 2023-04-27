#include "stdio.h"
#include "string.h"

#include "boards.h"
#include "version.h"

#include "letter_handler.h"
#include "trace.h"
#include "app_timer.h"
#include "hk_peripheral.h"

#ifdef LVGL_ENABLE
#include "lv_obj.h"
#include "lv_port_disp.h"
#include "lv_port_indev.h"
#include "lv_port_fs.h"

#include "lv_ui.h"
#include "lv_ex_get_started.h"
#endif

#include "24cxx.h"
#include "gt9147.h"
#include <stdlib.h>

#include "cm_backtrace.h"

#ifndef LVGL_ENABLE
#include "little_malloc.h"
#include "ff.h"
#include "exfuns.h"
#include "display_pic.h"
#endif

#define HARDWARE_VERSION "V1.0.0" 
#define SOFTWARE_VERSION "V0.1.0"

TIMER_DEF(m_test_timer);


void test_timer_handler(void *p_data)
{
    static bool is_led_off = false;

	// menu_status_handler();

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


void peripheral_init(void)
{
	// 1. low level外设初始化
	trace_init();
	g_systick_obj.systick_ops.systick_init(&g_systick_obj.systick_cfg);
	g_led_obj.gpio_ops.gpio_init(&g_led_obj.gpio_cfg);
	g_led_obj.gpio_ops.gpio_output_set(&g_led_obj.gpio_cfg, 1);

	// 2. 简单功能组件初始化
	letter_shell_init();
	cm_backtrace_init("app", HARDWARE_VERSION, SOFTWARE_VERSION);

	TIMER_INIT(&g_timer3_object);
	TIMER_CREATE(&m_test_timer, false, false, test_timer_handler);
	// TIMER_START(m_test_timer, 200);

#ifndef LVGL_ENABLE
    FRESULT fres = FR_OK;
	DIR tdir;	 		//临时目录


	g_tftlcd_lvgl_obj->tftlcd_ops.init(&g_tftlcd_lvgl_obj->tftlcd_cfg, &g_tftlcd_lvgl_obj->tftlcd_ops);
	little_mem_init(SRAMIN);
	exfuns_init();

    while (g_sdio_obj.sdio_ops.sd_init(&g_sdio_obj.sdio_cfg))
    {
        g_systick_obj.systick_ops.delay_ms(1000);
        trace_info("sd init fail...\r\n");
    }
    trace_info("sd init ok...\r\n");

    fres = f_mount(fs[0], "0:", 1);
    while (fres)
    {
        g_systick_obj.systick_ops.delay_ms(1000);
        trace_info("mount sd error: %d\r\n", fres);
        fres = f_mount(fs[0], "1:", 1);
    }
    trace_info("mount sd successful\r\n");

    while (f_opendir(&tdir, "1:/picture"))//打开图片文件夹
	{	    
		trace_info("PICTURE文件夹错误!\r\n");
		g_systick_obj.systick_ops.delay_ms(3000);			  		  
	}  

	// f_mount(fs[0], "0:", 1); 		//挂载SD卡 
#endif


	// 3. high level外设初始化
	eeprom_init(&eeprom_obj);
	while (eeprom_check(&eeprom_obj))
	{
		trace_info("eeprom init fail.\r\n");
		trace_info("keep checking\r\n");
	}
	trace_info("eeprom:24c02 is ok.\r\n");
	// read_data_from_mem();
	encoder_init(&g_encoder_obj);

	// 4. 复杂组件初始化
#ifdef LVGL_ENABLE
	lv_init();
	lv_port_disp_init();
	lv_port_indev_init();
	lv_port_fs_init();
#endif
}


int main(void)
{
	peripheral_init();

	trace_info("loop\r\n");
	trace_debug("debug\r\n");

	// monitor heap_stack
	extern unsigned int _user_heap_stack_start;
	extern unsigned int _user_heap_stack_end;
	unsigned int start_value, end_value;
	start_value = (unsigned int)&_user_heap_stack_start;
	end_value = (unsigned int)&_user_heap_stack_end;
	trace_info("user heap stack start : 0x%x\r\n", start_value);
	trace_info("user heap stack end   : 0x%x\r\n", end_value);

#ifndef LVGL_ENABLE
	show_picture();
#else

	while (1)
	{
		letter_shell_loop_task();
		TIMER_SCHEDULER_LOOP();


		lv_task_handler();

	}
#endif
}
