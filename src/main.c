#include "boards.h"
#include "version.h"


#include "trace.h"
#include "hk_peripheral.h"
#include "nt_handler.h"

int main(void)
{
  uint8_t flag = 0;
  uint32_t count = g_systick_obj.systick_ops.systick_get();

  g_systick_obj.systick_ops.systick_init(&g_systick_obj.systick_cfg);
  g_led_obj.gpio_ops.gpio_init(&g_led_obj.gpio_cfg);
  g_led_obj.gpio_ops.gpio_output_set(&g_led_obj.gpio_cfg, 1);

  trace_init();
  letter_shell_init();
  // init_nt_shell();

  g_systick_obj.systick_ops.delay_us(&g_systick_obj.systick_cfg, 1000);
  // trace_info("loop\r\n");
  // trace_debug("debug\r\n");
  while (1)
  {
    #if 0
    g_led_obj.gpio_ops.gpio_output_set(&g_led_obj.gpio_cfg, 1);
    g_systick_obj.systick_ops.delay_ms(&g_systick_obj.systick_cfg, 1000);
    g_led_obj.gpio_ops.gpio_output_set(&g_led_obj.gpio_cfg, 0);
    g_systick_obj.systick_ops.delay_ms(&g_systick_obj.systick_cfg, 1000);
    #else
    if (g_systick_obj.systick_ops.systick_get() - count > 1000)
    {
      count = g_systick_obj.systick_ops.systick_get();
      flag = !flag;
    }
    g_led_obj.gpio_ops.gpio_output_set(&g_led_obj.gpio_cfg, flag);
    #endif
  }
}
