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
// #include "lv_demo_widgets.h"

#include "lv_ex_get_started.h"

#include "gt9147.h"
#include <stdlib.h>

TIMER_DEF(m_test_timer);

void test_timer_handler(void *p_data)
{
    static bool is_led_off = false;
    uint8_t t = 0;
    uint16_t lastpos[5][2];		//记录最后一次的数据 

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

	TIMER_INIT(&g_timer3_object);

	TIMER_CREATE(&m_test_timer, false, false, test_timer_handler);
	TIMER_START(m_test_timer, 500);
	
	// g_tftlcd9341_obj.tftlcd_ops.init(&g_tftlcd9341_obj.tftlcd_cfg, &g_tftlcd9341_obj.tftlcd_ops);
	// g_tftlcd3510_obj.tftlcd_ops.init(&g_tftlcd3510_obj.tftlcd_cfg, &g_tftlcd3510_obj.tftlcd_ops);
	// g_touch_obj.touch_ops.init(&g_touch_obj.touch_cfg);
	// trace_info("touch init ok\r\n");

	// lvgl设置
	// lv_init();
	// lv_port_disp_init();
	// lv_port_indev_init();
	// lv_ex_get_started_3();

	trace_info("loop\r\n");
	trace_debug("debug\r\n");

	while (g_sdio_obj.sdio_ops.sd_init(&g_sdio_obj.sdio_cfg))
	{
		g_systick_obj.systick_ops.delay_ms(1000);
		trace_info("sd init fail...\r\n");
	}
	trace_info("sd init ok...\r\n");
	g_sdio_obj.sdio_ops.show_card_info(&g_sdio_obj.sdio_cfg);

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

	while (1)
	{
		letter_shell_loop_task();
		TIMER_SCHEDULER_LOOP();

		// lv_task_handler();
	}
}
