#include "stdio.h"
#include "string.h"

#include "boards.h"
#include "version.h"

#include "letter_handler.h"
#include "trace.h"
#include "app_timer.h"
#include "hk_peripheral.h"

#include "hk_adc.h"

#include <stdlib.h>

#include "cm_backtrace.h"

#include "digital_tube.h"

#define HARDWARE_VERSION "V1.0.0" 
#define SOFTWARE_VERSION "V0.1.0"

#define LEVEL_0			0
#define LEVEL_1			1
#define LEVEL_2			2
#define LEVEL_3			3

TIMER_DEF(m_test_timer);
uint8_t g_tube_status = 0;
typedef void (*p_fun)(void);
p_fun func_light_ctrl[4];
uint16_t adc_value[6] = {0};


void test_timer_handler(void *p_data)
{
	uint8_t vbat = 1;
	uint8_t voltage_level;

	if (vbat < 2)
	{
		voltage_level = LEVEL_0;
	}
	else if (vbat < 3)
	{
		voltage_level = LEVEL_1;
	}
	else if (vbat < 4)
	{
		voltage_level = LEVEL_2;
	}
	else
	{
		voltage_level = LEVEL_3;
	}

	if (g_tube_status == 3)
	{
		g_tube_status = voltage_level-1;
	}
	g_tube_status++;

	hk_adc_cfg *p_hk_adc_cfg = (hk_adc_cfg *)g_adc_obj.adc_cfg.p_adc_cfg;
	adc_value[0] = p_hk_adc_cfg->adc_value[0];
	adc_value[1] = p_hk_adc_cfg->adc_value[1];

	trace_info("adc[0] value = %d\r\n", adc_value[0]);
	trace_info("adc[0] volt = %d mV\r\n", adc_value[0] * 3300 / 4096);
	trace_info("\r\n");
	trace_info("adc[1] value = %d\r\n", adc_value[1]);
	trace_info("adc[1] volt = %d mV\r\n", adc_value[1] * 3300 / 4096);
	trace_info("\r\n");
	trace_info("\r\n");

}

int main(void)
{
	func_light_ctrl[0] = display_boader;
	func_light_ctrl[1] = display_battery1;
	func_light_ctrl[2] = display_battery2;
	func_light_ctrl[3] = display_battery3;

	g_systick_obj.systick_ops.systick_init(&g_systick_obj.systick_cfg);
	g_led_obj.gpio_ops.gpio_init(&g_led_obj.gpio_cfg);
	g_led_obj.gpio_ops.gpio_output_set(&g_led_obj.gpio_cfg, 1);

	g_dig_tube_obj.gpio_ops.gpio_init(&g_dig_tube_obj.gpio_cfg);


	trace_init();
	letter_shell_init();
	cm_backtrace_init("app", HARDWARE_VERSION, SOFTWARE_VERSION);

	g_adc_obj.adc_ops.adc_init(&g_adc_obj.adc_cfg);

	// adc dma setting
	hk_adc_cfg *p_hk_adc_cfg = (hk_adc_cfg *)g_adc_obj.adc_cfg.p_adc_cfg;
	g_adc_dma_obj.dma_ops.dma_init(&g_adc_dma_obj.dma_cfg, (uint32_t)(&(ADC1->DR)), (uint32_t)(p_hk_adc_cfg->adc_value), 
									p_hk_adc_cfg->adc_channel_num, DMA_DIR_PeripheralSRC);
	g_adc_dma_obj.dma_ops.dma_transfer_ctrl(&g_adc_dma_obj.dma_cfg, ENABLE);

	TIMER_INIT(&g_timer3_object);
	TIMER_CREATE(&m_test_timer, false, false, test_timer_handler);
	TIMER_START(m_test_timer, 2000);

	while (1)
	{
		letter_shell_loop_task();
		TIMER_SCHEDULER_LOOP();

		func_light_ctrl[g_tube_status](); 
	}
}
