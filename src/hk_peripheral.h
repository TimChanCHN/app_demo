#ifndef __HK_PERIPHERAL_H
#define __HK_PERIPHERAL_H

#include "usart.h"
#include "hk_usart.h"
#include "gpio.h"
#include "systick.h"
#include "hk_systick.h"

extern hk_uart_info_t g_hk_uart_info;
extern hk_uart_config_t g_hk_uart_config;
extern usart_object_t g_usart_object;
extern gpio_object_t g_led_obj;


#endif
