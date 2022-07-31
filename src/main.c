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
	
	// g_tftlcd9341_obj.tftlcd_ops.init(&g_tftlcd9341_obj.tftlcd_cfg, &g_tftlcd9341_obj.tftlcd_ops);
	// g_tftlcd3510_obj.tftlcd_ops.init(&g_tftlcd3510_obj.tftlcd_cfg, &g_tftlcd3510_obj.tftlcd_ops);
	// g_touch_obj.touch_ops.init(&g_touch_obj.touch_cfg);
	// trace_info("touch init ok\r\n");

	// lvgl设置
	lv_init();
	lv_port_disp_init();
	lv_port_indev_init();
	lv_port_fs_init();

	lv_ex_get_started_3();

	#if 1
	// test sdio
    while (g_sdio_obj.sdio_ops.sd_init(&g_sdio_obj.sdio_cfg))
	{
		g_systick_obj.systick_ops.delay_ms(1000);
		trace_info("sd init fail...\r\n");
	}
	trace_info("sd init ok...\r\n");

	uint8_t *buf;
	uint16_t cnt = 0;
	buf = (uint8_t *)malloc(512);
	trace_info("buf addr = 0x%x\r\n", buf);

	g_sdio_obj.sdio_ops.sd_read_disk(&g_sdio_obj.sdio_cfg, buf, 0, 1);

	trace_info("sector 0 data: \r\n");
	for (cnt = 0; cnt < 512; cnt++)
	{
		printf("%x ", buf[cnt]);
	}
	printf("\r\n");

	// test fatfs
	FATFS fs;
	FRESULT fres;

	fres = f_mount(&fs, "0:", 1);
	while (fres)
	{
		g_systick_obj.systick_ops.delay_ms(1000);
		trace_info("mount sd error %d\r\n", fres);
	}
	trace_info("mount ok\r\n");

	FIL file;
	const char *path = "0:/lvgl.txt";

	fres = f_open(&file, path, (FA_READ | FA_WRITE));
	if (fres)
	{
		trace_info("open file fail: %d\r\n", fres);
		return ;
	}
	else
	{
		trace_info("open file ok\r\n");
	}

	char buff[] = "cxtcfn";
	uint16_t bw;
    fres = f_write(&file, buff, sizeof(buff), &bw);
    if (fres != FR_OK)
    {
        trace_info("write file error : %d\r\n", fres);
    }
    else
    {
        trace_info("write ok. write %d bytes.\r\n", bw);
    }

	char buff1[10];
	uint16_t br;
	fres = f_lseek(&file, 0);
    fres = f_read(&file, buff1, sizeof(buff), &br);
    if (fres != FR_OK)
    {
        trace_info("read file error : %d\r\n", fres);
    }
    else
    {
        trace_info("read ok. read %d bytes.\r\n", bw);
		trace_info("buff : %s.\r\n", buff1);
    }

	fres = f_close(&file);
	if (fres)
	{
		trace_info("close file fail: %d\r\n", fres);
		return ;
	}
	else
	{
		trace_info("close file ok\r\n");
	}
	#endif

	#if 1
	lv_fs_res_t fs_res;
	lv_fs_file_t *file1 = lv_mem_alloc(sizeof(lv_fs_file_t *));
	// lv_fs_file_t *file1;
	uint8_t buffer[10];

	fs_res=lv_fs_open(file1, "0:/lvgl.txt", LV_FS_MODE_WR | LV_FS_MODE_RD);
	if(fs_res != LV_FS_RES_OK)
		printf("open error! code:%d\r\n",fs_res);

	#if 0
	fs_res=lv_fs_write(file1,"test",4,NULL);
	if(fs_res != LV_FS_RES_OK)
		printf("write error! code:%d\r\n",fs_res);

	fs_res=lv_fs_read(file1,buffer,4,NULL);
	if(fs_res != LV_FS_RES_OK)
		printf("read error! code:%d\r\n",fs_res);
	trace_info("read buf = %s\r\n", buffer);
	trace_info("sss\r\n");
	#endif

	fs_res=lv_fs_close(file1);
	if(fs_res != LV_FS_RES_OK)
		printf("close error! code:%d\r\n",fs_res);

	trace_info("xxx\r\n");
	lv_mem_free(file1);
	#endif


	trace_info("loop\r\n");
	trace_debug("debug\r\n");
	while (1)
	{
		letter_shell_loop_task();
		TIMER_SCHEDULER_LOOP();

		lv_task_handler();
	}
}
