#include "cmd_management.h"
#include "hk_peripheral.h"
#include "ntlibc.h"

int led_control(int argc, char **argv)
{
	// m_usart_obj = get_usart_object();
    if (argc != 2) {
        static const char *msg = "led control command. pls enter param.\n\r";
        g_usart_object.usart_ops.usart_put_string(msg);
        return 0;
    }
    if (ntlibc_strcmp(argv[1], "on") == 0) {

        // gpio_output_set(&m_gpio_led, 0);
        g_led_obj.gpio_ops.gpio_output_set(&g_led_obj.gpio_cfg, 1);
        return 0;
    }

    if (ntlibc_strcmp(argv[1], "off") == 0) {

        // gpio_output_set(&m_gpio_led, 1);
        g_led_obj.gpio_ops.gpio_output_set(&g_led_obj.gpio_cfg, 1);
        return 0;
    }
	return -1;
}

NTSHELL_EXPORT_CMD(led_control, led_control_function, led_control);
