#include "digital_tube.h"
#include "hk_peripheral.h"
#include "boards.h"

#define LOW_LEVEL       0
#define HIGH_LEVEL      1

void turn_on_light(uint16_t pin_low, uint16_t pin_high)
{
    gpio_cfg_t *p_gpio_cfg = &g_dig_tube_obj.gpio_cfg;

    uint32_t input_pin = 0xFF & (~(pin_low | pin_high));
    uint32_t output_pin = (pin_low | pin_high);

    p_gpio_cfg->gpio_pin = input_pin;
    p_gpio_cfg->gpio_dir = GPIO_DIR_INPUT;
    p_gpio_cfg->mode     = GPIO_Mode_IPD;
    g_dig_tube_obj.gpio_ops.gpio_init(&g_dig_tube_obj.gpio_cfg);

    p_gpio_cfg->gpio_pin = output_pin;
    p_gpio_cfg->gpio_dir = GPIO_DIR_OUTPUR;
    g_dig_tube_obj.gpio_ops.gpio_init(&g_dig_tube_obj.gpio_cfg);

    p_gpio_cfg->gpio_pin = pin_low ;
    g_dig_tube_obj.gpio_ops.gpio_output_set(&g_dig_tube_obj.gpio_cfg, LOW_LEVEL);

    p_gpio_cfg->gpio_pin = pin_high;
    g_dig_tube_obj.gpio_ops.gpio_output_set(&g_dig_tube_obj.gpio_cfg, HIGH_LEVEL);

    g_systick_obj.systick_ops.delay_ms(1);
    p_gpio_cfg->gpio_pin = pin_high;
    g_dig_tube_obj.gpio_ops.gpio_output_set(&g_dig_tube_obj.gpio_cfg, LOW_LEVEL);
}

void display_boader(void)
{
    // 边框
    turn_on_light(DIGITAL_PIN3, DIGITAL_PIN7);        // J1
    turn_on_light(DIGITAL_PIN6, DIGITAL_PIN1|DIGITAL_PIN3);        // J2/J3
    // turn_on_light(DIGITAL_PIN6, DIGITAL_PIN3);        // J3
    turn_on_light(DIGITAL_PIN7, DIGITAL_PIN3);        // J4

    // turn_on_light(DIGITAL_PIN5, DIGITAL_PIN1);        // K2
    // turn_on_light(DIGITAL_PIN2, DIGITAL_PIN6);        // K3
    // turn_on_light(DIGITAL_PIN2, DIGITAL_PIN7);        // K4
}

void display_battery1(void)
{
    display_boader();

    turn_on_light(DIGITAL_PIN7, DIGITAL_PIN4);        // K8
}

void display_battery2(void)
{
    display_boader();

    turn_on_light(DIGITAL_PIN7, DIGITAL_PIN4 | DIGITAL_PIN2);        // K8
    // turn_on_light(DIGITAL_PIN7, DIGITAL_PIN2);        // K7
}

void display_battery3(void)
{
    display_boader();

    turn_on_light(DIGITAL_PIN7, DIGITAL_PIN4|DIGITAL_PIN2|DIGITAL_PIN1);        // K8
    // turn_on_light(DIGITAL_PIN7, DIGITAL_PIN2);        // K7
    // turn_on_light(DIGITAL_PIN7, DIGITAL_PIN1);        // K6
}
