#include "stdio.h"
#include "string.h"

#include "boards.h"
#include "version.h"

#include "letter_handler.h"
#include "trace.h"
#include "app_timer.h"
#include "hk_peripheral.h"

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
  // letter_shell_init();
  // init_nt_shell();

  trace_info("loop\r\n");
  g_tftlcd_obj.tftlcd_ops.init(&g_tftlcd_obj.tftlcd_cfg, &g_tftlcd_obj.tftlcd_ops);

  // 1. test fill area
  fill_object_t fill_area = {
    .coord_s = {
      .x = 50,
      .y = 10,
    },
    .coord_e = {
      .x = 200,
      .y = 200,
    },
    .color = WHITE,
  };
  g_tftlcd_obj.tftlcd_ops.fill_area(&g_tftlcd_obj.tftlcd_cfg, &g_tftlcd_obj.tftlcd_ops, fill_area);

  // 2. test show_char
  // chars_info_t ch = {
  //   .num = 'A',
  //   .size = 16,
  //   .mode = 1,
  //   .coord = {
  //     .x = 100,
  //     .y = 100,
  //   },
  // };
  // g_tftlcd_obj.tftlcd_ops.show_char(&g_tftlcd_obj.tftlcd_cfg, &g_tftlcd_obj.tftlcd_ops, ch);





  TIMER_INIT(&g_timer3_object);

  TIMER_CREATE(&m_test_timer, false, false, test_timer_handler);
  TIMER_START(m_test_timer, 500);

  trace_info("loop\r\n");
  trace_debug("debug\r\n");
  while (1)
  {
    letter_shell_loop_task();

    TIMER_SCHEDULER_LOOP();

  }
}
