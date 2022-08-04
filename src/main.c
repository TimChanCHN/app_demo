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


	lv_fs_res_t fs_res;
	lv_fs_file_t *file1 = lv_mem_alloc(sizeof(lv_fs_file_t));
	const char *path1 = "S:/test/lvgl.txt";

	fs_res=lv_fs_open(file1, path1, LV_FS_MODE_WR | LV_FS_MODE_RD);
	if(fs_res != LV_FS_RES_OK)
		printf("open error! code:%d\r\n",fs_res);

	#if 1
	uint16_t bw, br = 0;
	uint8_t read_buff[30] = {0};
	char buff3[] = "TimChan don't like valentine";

	fs_res=lv_fs_write(file1, (void*)buff3, sizeof(buff3), &bw);
	if(fs_res != LV_FS_RES_OK)
	{
		printf("write error! code:%d\r\n",fs_res);
	}
	else
	{
		trace_info("write ok. write %d bytes.\r\n", bw);
	}

	fs_res = lv_fs_seek(file1, 0);
	fs_res = lv_fs_read(file1,read_buff,sizeof(buff3),&br);
	if(fs_res != LV_FS_RES_OK)
	{
		printf("read error! code:%d\r\n",fs_res);
	}
	else
	{
		trace_info("read ok. %d bytes to be read.\r\n",br);
		trace_info("read buf = %s\r\n", read_buff);
	}
	#endif

	fs_res=lv_fs_close(file1);
	if(fs_res != LV_FS_RES_OK)
		printf("close error! code:%d\r\n",fs_res);

	trace_info("xxx\r\n");
	lv_mem_free(file1);

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
