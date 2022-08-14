#ifndef __HK_PERIPHERAL_H
#define __HK_PERIPHERAL_H

#include "usart.h"
#include "gpio.h"
#include "systick.h"
#include "timer.h"
#include "tftlcd.h"
#include "touch.h"
#include "sdio.h"


extern usart_object_t g_usart_object;
extern gpio_object_t g_led_obj;
extern systick_object_t g_systick_obj;
extern timer_object_t   g_timer3_object;
extern tftlcd_object_t g_tftlcd_obj;
extern tftlcd_object_t g_tftlcd3510_obj;
// extern tftlcd_object_t g_tftlcd9341_obj;
extern tftlcd_object_t *g_tftlcd_lvgl_obj;

extern touch_object_t g_touch_obj;
extern touch_object_t *g_touch_lvgl_obj;

extern i2c_object_t eeprom_obj;

extern sdio_obj_t g_sdio_obj;

#endif
