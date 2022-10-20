#include "stdio.h"
#include "string.h"

#include "boards.h"
#include "version.h"

#include "letter_handler.h"
#include "trace.h"
#include "app_timer.h"
#include "hk_peripheral.h"

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

	uint16_t adc_value = 0;

	g_adc_obj.adc_ops.adc_value_get(&g_adc_obj.adc_cfg, &adc_value);

	trace_info("adc value = %d\r\n", adc_value);
	trace_info("adc volt = %d mV\r\n", adc_value * 3300 / 4096);
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

	g_adc_obj.adc_ops.adc_init(&g_adc_obj.adc_cfg);

	trace_init();
	letter_shell_init();
	cm_backtrace_init("app", HARDWARE_VERSION, SOFTWARE_VERSION);

	TIMER_INIT(&g_timer3_object);
	TIMER_CREATE(&m_test_timer, false, false, test_timer_handler);
	TIMER_START(m_test_timer, 1000);

	while (1)
	{
		letter_shell_loop_task();
		TIMER_SCHEDULER_LOOP();

		func_light_ctrl[g_tube_status](); 
	}
}
