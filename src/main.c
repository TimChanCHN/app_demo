#include "boards.h"
#include "version.h"


#include "trace.h"
#include "hk_peripheral.h"

int main(void)
{
  hk_systick_init();

  trace_init();

  g_led_obj.gpio_ops.gpio_init(&g_led_obj.gpio_cfg);
  g_led_obj.gpio_ops.gpio_output_set(&g_led_obj.gpio_cfg, 0);

  trace_info("loop\r\n");
  trace_debug("debug\r\n");
  while (1)
  {
    g_led_obj.gpio_ops.gpio_output_set(&g_led_obj.gpio_cfg, 1);
    hk_delay_ms(1000);
    g_led_obj.gpio_ops.gpio_output_set(&g_led_obj.gpio_cfg, 0);
    hk_delay_ms(1000);
  }
}
