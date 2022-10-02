#include "boards.h"

#include "i2c.h"

#include "hk_gpio.h"
#include "hk_fsmc.h"
#include "hk_usart.h"
#include "hk_i2c.h"
#include "hk_timer.h"
#include "hk_systick.h"
#include "hk_sdio.h"
#include "hk_exit.h"
#include "hk_flash.h"
#include "hk_dma.h"

#include "gt9147.h"
#include "st7789_8080.h"
#include "st7789_fsmc.h"
#include "nt35510_fsmc.h"

#include "hk_peripheral.h"

gpio_ops_t g_gpio_ops = {
            .gpio_init = hk_gpio_obj_init,
            .gpio_output_set = hk_gpio_obj_out_set,
            .gpio_input_get = hk_gpio_obj_in_get,
            .gpio_fix_input = hk_gpio_fix_input,
            .gpio_fix_output = hk_gpio_fix_output,
};

/********************  systick  ********************/
systick_object_t g_systick_obj = {
    .systick_cfg = {
        .clk_div    =   SysTick_CLKSource_HCLK_Div8,
        .ticks_per_us   =   9,
        .ticks_per_sec  =   1000,
    },
    .systick_ops = {
        .systick_init   = hk_systick_obj_init,
        .delay_us       = hk_delay_us,
        .delay_ms       = hk_delay_ms,
        .systick_get    = hk_systick_get,
    },
};

/********************  uart  ********************/
hk_uart_info_t g_hk_uart_info = {
    .uart = USART1,
    .tx_port = TRACE_UART_TX_GPIO_PORT,
    .rx_port = TRACE_UART_RX_GPIO_PORT,

    .clk = TRACE_UART_CLK,
    .tx_clk = TRACE_UART_TX_GPIO_CLK,
    .rx_clk = TRACE_UART_RX_GPIO_CLK,

    .tx_pin = TRACE_UART_TX_PIN,
    .rx_pin = TRACE_UART_RX_PIN,

    .uart_id = HK_UART_1,
};

hk_uart_config_t g_hk_uart_config = {
    .baudrate = 115200,
    .databits = USART_WordLength_8b,
    .stopbit = USART_StopBits_1,
    .parity = USART_Parity_No,
    .hwfc = USART_HardwareFlowControl_None,
    .mode = USART_Mode_Rx | USART_Mode_Tx,
    .interrupt_priority = 0,
};

// 串口obj赋值
usart_object_t g_usart_object = {
  .usart_cfg = {
      .p_pin_cfg = (void *)&g_hk_uart_info,
      .p_usart_cfg = (void *)&g_hk_uart_config,
  },

  .usart_ops = {
      .usart_init = hk_uart_obj_init,
#if defined(__CC_ARM) || defined(__ICCARM__)    //armcc
      .usart_put = hk_uart_obj_put,
#elif defined(__GNUC__)   
      .usart_write = hk_uart_obj_put,
      .usart_write_buffer = hk_uart_obj_puts,
      .usart_read_buffer  = hk_uart_obj_gets,
      .usart_put_string   = hk_uart_obj_putstring,
#endif
  }
};

/********************  led  ********************/
gpio_object_t g_led_obj = {
    .gpio_cfg = {
        .gpio_clk = LED_PORT_PERIPH_CLK,
        .p_port = (void *)LED_PORT,
        .gpio_pin = LED_PIN,
        .gpio_dir = GPIO_DIR_OUTPUR,
        .flag = GPIO_TYPE_IO,
    },

    .gpio_ops = {
        .gpio_init = hk_gpio_obj_init,
        .gpio_output_set = hk_gpio_obj_out_set,
        .gpio_fix_input = hk_gpio_fix_input,
        .gpio_fix_output = hk_gpio_fix_output,
    },
};

/********************  timer  ********************/
timer_object_t g_timer3_object = {
    .timer_cfg = {
        .period = 1000,             //计数到 1K 时溢出
        .prescaler_freq = 1000000,  //分频后得到 1M 时钟
        .ticks = 0,
        .timer = TIM3,
    },
    .timer_ops = {
        .timer_init = hk_timer3_init,
        .timer_start = hk_timer3_start,
        .timer_stop = hk_timer3_stop,
        .timer_irq_enable = hk_timer3_irq_enable,
        .timer_irq_disable = hk_timer3_irq_disable,
    }
};

// PA0
/********************  key1 & exit0  ********************/
gpio_object_t g_key1_obj = {
    .gpio_cfg = {
        .gpio_clk = KEY_PORT_PERIPH_CLK,
        .p_port = (void *)KEY_PORT,
        .gpio_pin = KEY1_PIN,
        .gpio_dir = GPIO_DIR_INPUT,
        .flag = GPIO_TYPE_IO,
        .mode = GPIO_Mode_IPU,
    },

    .gpio_ops = {
        .gpio_init          = hk_gpio_obj_init,
        .gpio_output_set    = hk_gpio_obj_out_set,
        .gpio_fix_input     = hk_gpio_fix_input,
        .gpio_fix_output    = hk_gpio_fix_output,
        .gpio_input_get     = hk_gpio_obj_in_get,
    },
};

hk_exit_pin_cfg g_exit0_pin_cfg = {
    .exit_gpio_cfg          = &g_key1_obj,
    .exit_clk               = RCC_APB2Periph_AFIO,
    .exit_pin_port_source   = GPIO_PortSourceGPIOA,
    .exit_pin_source        = GPIO_PinSource0,
};

hk_exit_cfg g_exit0_cfg = {
    .exit_mode      = EXTI_Mode_Interrupt,
    .exit_trigger   = EXTI_Trigger_Rising,
    // .exit_line_cmd  = ,
    .exit_line      = EXTI_Line0,
    .exit_irqn      = EXTI0_IRQn,
    .exit_pre_prio  = 0x02,
    .exit_sub_prio  = 0x01,
};

exit_object_t g_exit0_obj = {
    .exit_cfg = {
        .p_pin_cfg  = (void *)&g_exit0_pin_cfg,
        .p_exit_cfg = (void *)&g_exit0_cfg,
        .delay_ms   = hk_delay_ms,
    },
    .exit_ops = {
        .exit_init      =   hk_exit_init,
        .exit_enable    =   hk_exit_enable,
        .exit_disable   =   hk_exit_disable,
        .exit_irq_cb    =   exit_irq_handler,
    }
};

// PA1
/********************  key2 & exit1  ********************/
gpio_object_t g_key2_obj = {
    .gpio_cfg = {
        .gpio_clk = KEY_PORT_PERIPH_CLK,
        .p_port = (void *)KEY_PORT,
        .gpio_pin = KEY2_PIN,
        .gpio_dir = GPIO_DIR_INPUT,
        .flag = GPIO_TYPE_IO,
        .mode = GPIO_Mode_IPU,
    },

    .gpio_ops = {
        .gpio_init          = hk_gpio_obj_init,
        .gpio_output_set    = hk_gpio_obj_out_set,
        .gpio_fix_input     = hk_gpio_fix_input,
        .gpio_fix_output    = hk_gpio_fix_output,
        .gpio_input_get     = hk_gpio_obj_in_get,
    },
};

hk_exit_pin_cfg g_exit1_pin_cfg = {
    .exit_gpio_cfg          = &g_key2_obj,
    .exit_clk               = RCC_APB2Periph_AFIO,
    .exit_pin_port_source   = GPIO_PortSourceGPIOA,
    .exit_pin_source        = GPIO_PinSource1,
};

hk_exit_cfg g_exit1_cfg = {
    .exit_mode      = EXTI_Mode_Interrupt,
    .exit_trigger   = EXTI_Trigger_Rising,
    // .exit_line_cmd  = ,
    .exit_line      = EXTI_Line1,
    .exit_irqn      = EXTI1_IRQn,
    .exit_pre_prio  = 0x02,
    .exit_sub_prio  = 0x03,
};

exit_object_t g_exit1_obj = {
    .exit_cfg = {
        .p_pin_cfg  = (void *)&g_exit1_pin_cfg,
        .p_exit_cfg = (void *)&g_exit1_cfg,
        .delay_ms   = hk_delay_ms,
    },
    .exit_ops = {
        .exit_init      =   hk_exit_init,
        .exit_enable    =   hk_exit_enable,
        .exit_disable   =   hk_exit_disable,
        .exit_irq_cb    =   exit_irq_handler,
    }
};

// PC13
/********************  key3 & exit13  ********************/
gpio_object_t g_key3_obj = {
    .gpio_cfg = {
        .gpio_clk = KEY3_PORT_PERIPH_CLK,
        .p_port = (void *)KEY3_PORT,
        .gpio_pin = KEY3_PIN,
        .gpio_dir = GPIO_DIR_INPUT,
        .flag = GPIO_TYPE_IO,
        .mode = GPIO_Mode_IPU,
    },

    .gpio_ops = {
        .gpio_init          = hk_gpio_obj_init,
        .gpio_output_set    = hk_gpio_obj_out_set,
        .gpio_fix_input     = hk_gpio_fix_input,
        .gpio_fix_output    = hk_gpio_fix_output,
        .gpio_input_get     = hk_gpio_obj_in_get,
    },
};

hk_exit_pin_cfg g_exit13_pin_cfg = {
    .exit_gpio_cfg          = &g_key3_obj,
    .exit_clk               = RCC_APB2Periph_AFIO,
    .exit_pin_port_source   = GPIO_PortSourceGPIOC,
    .exit_pin_source        = GPIO_PinSource13,
};

hk_exit_cfg g_exit13_cfg = {
    .exit_mode      = EXTI_Mode_Interrupt,
    .exit_trigger   = EXTI_Trigger_Rising,
    // .exit_line_cmd  = ,
    .exit_line      = EXTI_Line13,
    .exit_irqn      = EXTI15_10_IRQn,
    .exit_pre_prio  = 0x02,
    .exit_sub_prio  = 0x04,
};

exit_object_t g_exit13_obj = {
    .exit_cfg = {
        .p_pin_cfg  = (void *)&g_exit13_pin_cfg,
        .p_exit_cfg = (void *)&g_exit13_cfg,
        .delay_ms   = hk_delay_ms,
    },
    .exit_ops = {
        .exit_init      =   hk_exit_init,
        .exit_enable    =   hk_exit_enable,
        .exit_disable   =   hk_exit_disable,
        .exit_irq_cb    =   exit_irq_handler,
    }
};

/********************  ST7789  ********************/
st7789_8080_info_t g_st7789_info = {
    .cs_pin = {
        .gpio_cfg = {
            .gpio_clk = LCD_CS_PORT_PERIPH_CLK,
            .p_port = (void *)LCD_CS_PORT,
            .gpio_pin = LCD_CS_PIN,
            .gpio_dir = GPIO_DIR_OUTPUR,
            .flag = GPIO_TYPE_IO,
        },

        .gpio_ops = {
            .gpio_init = hk_gpio_obj_init,
            .gpio_output_set = hk_gpio_obj_out_set,
        },
    },
    .rst_pin = {
        .gpio_cfg = {
            .gpio_clk = LCD_RST_PORT_PERIPH_CLK,
            .p_port = (void *)LCD_RST_PORT,
            .gpio_pin = LCD_RST_PIN,
            .gpio_dir = GPIO_DIR_OUTPUR,
            .flag = GPIO_TYPE_IO,
        },

        .gpio_ops = {
            .gpio_init = hk_gpio_obj_init,
            .gpio_output_set = hk_gpio_obj_out_set,
        },
    },
    .rs_pin = {
        .gpio_cfg = {
            .gpio_clk = LCD_RS_PORT_PERIPH_CLK,
            .p_port = (void *)LCD_RS_PORT,
            .gpio_pin = LCD_RS_PIN,
            .gpio_dir = GPIO_DIR_OUTPUR,
            .flag = GPIO_TYPE_IO,
        },

        .gpio_ops = {
            .gpio_init = hk_gpio_obj_init,
            .gpio_output_set = hk_gpio_obj_out_set,
        },
    },
    .wr_pin = {
        .gpio_cfg = {
            .gpio_clk = LCD_WR_PORT_PERIPH_CLK,
            .p_port = (void *)LCD_WR_PORT,
            .gpio_pin = LCD_WR_PIN,
            .gpio_dir = GPIO_DIR_OUTPUR,
            .flag = GPIO_TYPE_IO,
        },

        .gpio_ops = {
            .gpio_init = hk_gpio_obj_init,
            .gpio_output_set = hk_gpio_obj_out_set,
        },
    },
    .rd_pin = {
        .gpio_cfg = {
            .gpio_clk = LCD_RD_PORT_PERIPH_CLK,
            .p_port = (void *)LCD_RD_PORT,
            .gpio_pin = LCD_RD_PIN,
            .gpio_dir = GPIO_DIR_OUTPUR,
            .flag = GPIO_TYPE_IO,
        },

        .gpio_ops = {
            .gpio_init = hk_gpio_obj_init,
            .gpio_output_set = hk_gpio_obj_out_set,
        },
    },
    .db_pin = {
        .gpio_cfg = {
            .gpio_clk = LCD_DB_PORT_PERIPH_CLK,
            .p_port = (void *)LCD_DB_PORT,
            .gpio_pin = LCD_DB_PIN,
            .gpio_dir = GPIO_DIR_OUTPUR,
            .flag = GPIO_TYPE_IO,
        },

        .gpio_ops = {
            .gpio_init = hk_gpio_obj_init,
            .gpio_output_set = hk_gpio_obj_out_set,
        },
    },
};

tftlcd_driver_t g_lcd_driver = {
    .p_tft_cfg = (void *)&g_st7789_info,
    .lcd_info = {
        .setxcmd = 0x2A,
        .setycmd = 0x2B,
        .wramcmd = 0x2C,
        .width = 320,
        .height = 240,
        .id = 0,
        .dir = 0,
        .background_color = RED,
        .point_color = BLUE,
    },
};


tftlcd_object_t g_tftlcd_obj = {
    .tftlcd_cfg = {
        .p_dri = &g_lcd_driver,

        .hardware_init  = st7789_hardware_init,
        .send_byte      = st7789_send_byte,
        .write_cmd      = st7789_write_cmd, 
        .write_data     = st7789_write_data, 
        .write_burst_data = st7789_write_burst_data,
        .delay_ms       = hk_delay_ms, 

    },
    .tftlcd_ops = {
        // 私有函数
        .init           = st7789_init,
        .set_cursor     = st7789_set_cursor,
        .write_ram_pre  = st7789_write_ram_pre,
        .write_ram      = st7789_write_ram,
        .set_scan_dir   = st7789_set_scan_dir,

        // 公有函数
        .clear_screen   = tftlcd_clear_screen,
        .fill_area      = tftlcd_fill_area,
        .draw_point     = tftlcd_draw_point,
        .show_char      = tftlcd_show_char,
    },
};

/********************  st7789 fsmc  ********************/
//RST--PE1

//CS--PD7
//RD--PD4
//WR--PD5
//RS--PD11
//DB0-DB7 --> PD14,PD15,PD0,PD1,PE7,PE8,PE9,PE10
st7789_fsmc_info_t g_st7789 = {
    .rst_pin = {
        .gpio_cfg = {
            .gpio_clk = LCD_RST1_PORT_PERIPH_CLK,
            .p_port = (void *)LCD_RST1_PORT,
            .gpio_pin = LCD_RST1_PIN,
            .gpio_dir = GPIO_DIR_OUTPUR,
            .flag = GPIO_TYPE_IO,
        },
        .gpio_ops = {
            .gpio_init = hk_gpio_obj_init,
            .gpio_output_set = hk_gpio_obj_out_set,
        },
    },
    .fsmc_pin1 = {
        .gpio_cfg = {
            .gpio_clk = LCD_FSMC1_PORT_PERIPH_CLK,
            .p_port = (void *)LCD_FSMC1_PORT,
            .gpio_pin = (GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_7|GPIO_Pin_11|GPIO_Pin_14|GPIO_Pin_15),
            .flag = GPIO_TYPE_AF,
        },

        .gpio_ops = {
            .gpio_init = hk_gpio_obj_init,
        },
    },
    .fsmc_pin2 = {
        .gpio_cfg = {
            .gpio_clk = LCD_FSMC2_PORT_PERIPH_CLK,
            .p_port = (void *)LCD_FSMC2_PORT,
            .gpio_pin = (GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10),
            .flag = GPIO_TYPE_AF,
        },

        .gpio_ops = {
            .gpio_init = hk_gpio_obj_init,
        },
    },
    .base_addr = ((uint32_t)((FSMC_BASE_ADDR + BANK1_SECTOR1_OFFSET) | BANK_8B_A16_OFFSET)),
    .buswidth = LCDBUSWIDTH_8B,
};

tftlcd_driver_t g_lcd7789_driver = {
    .p_tft_cfg = (void *)&g_st7789,
    .lcd_info = {
        .setxcmd = 0x2A,
        .setycmd = 0x2B,
        .wramcmd = 0x2C,
        .width = 320,
        .height = 240,
        .id = 0,
        .dir = 0,
        .background_color = RED,
        .point_color = BLUE,
    },
};

tftlcd_object_t g_tftlcd7789_obj = {
    .tftlcd_cfg = {
        .p_dri = &g_lcd7789_driver,

        .hardware_init  = st7789_hw_fsmc_init,
        .write_cmd      = st7789_write_cmd_fsmc, 
        .write_data     = st7789_write_data_fsmc, 
        .read_data      = st7789_read_data_fsmc,
        .write_reg      = st7789_write_reg_fsmc,
        .delay_ms       = hk_delay_ms, 
        .delay_us       = hk_delay_us,

    },
    .tftlcd_ops = {
        // 私有函数
        // .read_reg       = nt35510_read_reg,
        .init           = st7789_init,
        .set_cursor     = st7789_set_cursor,
        .write_ram_pre  = st7789_write_ram_pre,
        .write_ram      = st7789_write_ram,
        .set_scan_dir   = st7789_set_scan_dir,

        // 公有函数
        .clear_screen   = st7789_clear_screen,

        .fill_area_color = tftlcd_fill_area_color,
        .fill_area      = tftlcd_fill_area,
        .draw_point     = tftlcd_draw_point,
        .show_char      = tftlcd_show_char,
    },
};

/********************  nt35510  ********************/
nt35510_fsmc_info_t g_nt35510 = {
    .rst_pin = {
        .gpio_cfg = {
            .gpio_clk = LCD_RST1_PORT_PERIPH_CLK,
            .p_port = (void *)LCD_RST1_PORT,
            .gpio_pin = LCD_RST1_PIN,
            .gpio_dir = GPIO_DIR_OUTPUR,
            .flag = GPIO_TYPE_IO,
        },

        .gpio_ops = {
            .gpio_init = hk_gpio_obj_init,
            .gpio_output_set = hk_gpio_obj_out_set,
        },
    },
    .bl_pin = {
        .gpio_cfg = {
            .gpio_clk = LCD_BL_PORT_PERIPH_CLK,
            .p_port = (void *)LCD_BL_PORT,
            .gpio_pin = LCD_BL_PIN,
            .gpio_dir = GPIO_DIR_OUTPUR,
            .flag = GPIO_TYPE_IO,
        },

        .gpio_ops = {
            .gpio_init = hk_gpio_obj_init,
            .gpio_output_set = hk_gpio_obj_out_set,
        },
    },
    .fsmc_pin1 = {
        .gpio_cfg = {
            .gpio_clk = LCD_FSMC1_PORT_PERIPH_CLK,
            .p_port = (void *)LCD_FSMC1_PORT,
            .gpio_pin = LCD_FSMC1_PIN,
            .flag = GPIO_TYPE_AF,
        },

        .gpio_ops = {
            .gpio_init = hk_gpio_obj_init,
        },
    },
    .fsmc_pin2 = {
        .gpio_cfg = {
            .gpio_clk = LCD_FSMC2_PORT_PERIPH_CLK,
            .p_port = (void *)LCD_FSMC2_PORT,
            .gpio_pin = LCD_FSMC2_PIN,
            .flag = GPIO_TYPE_AF,
        },

        .gpio_ops = {
            .gpio_init = hk_gpio_obj_init,
        },
    },
    .base_addr = ((uint32_t)((FSMC_BASE_ADDR + BANK1_SECTOR1_OFFSET) | BANK_16B_A16_OFFSET)),
    .buswidth = LCDBUSWIDTH_16B,
};

tftlcd_driver_t g_lcd35510_driver = {
    .p_tft_cfg = (void *)&g_nt35510,
    .lcd_info = {
        .setxcmd = 0x2A00,
        .setycmd = 0x2B00,
        .wramcmd = 0x2C00,
        .width = 480,
        .height = 800,
        .id = 0,
        .dir = 1,               // 1:横屏， 0:竖屏
        .background_color = RED,
        .point_color = BLUE,
    },
};

tftlcd_object_t g_tftlcd3510_obj = {
    .tftlcd_cfg = {
        .p_dri = &g_lcd35510_driver,

        .hardware_init  = nt35510_hardware_init,
        .write_cmd      = nt35510_write_cmd, 
        .write_data     = nt35510_write_data, 
        .read_data      = nt35510_read_data,
        .write_reg      = nt35510_write_reg,
        .delay_ms       = hk_delay_ms, 
        .delay_us       = hk_delay_us,

    },
    .tftlcd_ops = {
        // 私有函数
        .read_reg       = nt35510_read_reg,
        .init           = nt35510_init,
        .set_cursor     = nt35510_set_cursor,
        .write_ram_pre  = nt35510_write_ram_pre,
        .write_ram      = nt35510_write_ram,
        .set_scan_dir   = nt35510_set_scan_dir,

        // 公有函数
        .clear_screen   = tftlcd_clear_screen,
        .fill_area_color = nt35510_fill_color,
        .fill_area      = tftlcd_fill_area,
        .draw_point     = tftlcd_draw_point,
        .show_char      = tftlcd_show_char,
    },
};


tftlcd_object_t *g_tftlcd_lvgl_obj = &g_tftlcd7789_obj;

/********************  touch setting:9147  ********************/
tp_dev_t g_tp_dev = {
    // .x = 0, 
    // .y = 0,
    .lcd_width = 480,
    .lcd_height = 800,
    .sta = 0,				
    .touchtype = 0,
};

touch_object_t g_touch_obj = {
    .touch_cfg = {
        .p_i2c_obj = {
            .i2c_cfg = {
                .sda = {
                    .gpio_cfg = {
                        .gpio_clk = TOUCH_SDA_PERIPH_CLK,
                        .p_port = (void *)TOUCH_SDA_PORT,
                        .gpio_pin = TOUCH_SDA_PIN,
                        .flag = GPIO_TYPE_IO,
                        .gpio_dir = GPIO_DIR_OUTPUR,
                        .mode = GPIO_Mode_IPU,
                    },

                    .gpio_ops = {
                        .gpio_init = hk_gpio_obj_init,
                        .gpio_output_set = hk_gpio_obj_out_set,
                        .gpio_input_get = hk_gpio_obj_in_get,
                        .gpio_fix_input = hk_gpio_fix_input,
                        .gpio_fix_output = hk_gpio_fix_output,
                    },
                },
                .scl = {
                    .gpio_cfg = {
                        .gpio_clk = TOUCH_SCL_PERIPH_CLK,
                        .p_port = (void *)TOUCH_SCL_PORT,
                        .gpio_pin = TOUCH_SCL_PIN,
                        .flag = GPIO_TYPE_IO,
                        .gpio_dir = GPIO_DIR_OUTPUR,
                    },

                    .gpio_ops = {
                        .gpio_init = hk_gpio_obj_init,
                        .gpio_output_set = hk_gpio_obj_out_set,
                        .gpio_fix_input = hk_gpio_fix_input,
                        .gpio_fix_output = hk_gpio_fix_output,
                    },
                },
                .delay_us = hk_delay_us,
            },
            .i2c_ops = {
                .delay_ms = hk_delay_ms,
                .init = hk_virt_i2c_init,
                .xfer_start = hk_virt_i2c_start,
                .xfer_stop = hk_virt_i2c_stop,
                .wait_ack = hk_virt_i2c_wait_ack,
                .set_ack = hk_virt_i2c_set_ack,
                .set_nack = hk_virt_i2c_set_nack,
                .send_byte = hk_virt_i2c_send_byte,
                .read_byte = hk_virt_i2c_read_byte,
            },
        },
        .p_rst_obj = {
            .gpio_cfg = {
                .gpio_clk = TOUCH_RST_PERIPH_CLK,
                .p_port = (void *)TOUCH_RST_PORT,
                .gpio_pin = TOUCH_RST_PIN,
                .flag = GPIO_TYPE_IO,
                .gpio_dir = GPIO_DIR_OUTPUR,
            },

            .gpio_ops = {
                .gpio_init = hk_gpio_obj_init,
                .gpio_output_set = hk_gpio_obj_out_set,
            },
        },
        .p_int_obj = {
            .gpio_cfg = {
                .gpio_clk = TOUCH_INT_PERIPH_CLK,
                .p_port = (void *)TOUCH_INT_PORT,
                .gpio_pin = TOUCH_INT_PIN,
                .flag = GPIO_TYPE_IO,
                .gpio_dir = GPIO_DIR_INPUT,
                .mode = GPIO_Mode_IPU,
            },

            .gpio_ops = {
                .gpio_init = hk_gpio_obj_init,
                .gpio_output_set = hk_gpio_obj_out_set,
                .gpio_fix_input = hk_gpio_fix_input,
                .gpio_fix_output = hk_gpio_fix_output,
            },
        },
        .p_touch_dev = &g_tp_dev,
    },
    .touch_ops = {
        .send_cfg = gt9147_send_cfg,
        .write_reg = gt9147_write_reg,
        .read_reg = gt9147_read_reg,
        .init = gt9147_init,
        .scan = gt9147_scan,  
    },
};
touch_object_t *g_touch_lvgl_obj = &g_touch_obj;

// sda --> PB7, scl --> PB6
/********************  eeprom  ********************/
i2c_object_t eeprom_obj = {
    .i2c_cfg = {
        .sda = {
            .gpio_cfg = {
                .gpio_clk = EEPROM_SDA_PERIPH_CLK,
                .p_port = (void *)EEPROM_SDA_PORT,
                .gpio_pin = EEPROM_SDA_PIN,
                .flag = GPIO_TYPE_IO,
                .gpio_dir = GPIO_DIR_OUTPUR,
                .mode = GPIO_Mode_IPU,
            },

            .gpio_ops = {
                .gpio_init = hk_gpio_obj_init,
                .gpio_output_set = hk_gpio_obj_out_set,
                .gpio_input_get = hk_gpio_obj_in_get,
                .gpio_fix_input = hk_gpio_fix_input,
                .gpio_fix_output = hk_gpio_fix_output,
            },
        },
        .scl = {
            .gpio_cfg = {
                .gpio_clk = EEPROM_SCL_PERIPH_CLK,
                .p_port = (void *)EEPROM_SCL_PORT,
                .gpio_pin = EEPROM_SCL_PIN,
                .flag = GPIO_TYPE_IO,
                .gpio_dir = GPIO_DIR_OUTPUR,
                .mode = GPIO_Mode_IPU,
            },

            .gpio_ops = {
                .gpio_init = hk_gpio_obj_init,
                .gpio_output_set = hk_gpio_obj_out_set,
                .gpio_fix_input = hk_gpio_fix_input,
                .gpio_fix_output = hk_gpio_fix_output,
            },
        },
        .delay_us = hk_delay_us,
    },
    .i2c_ops = {
        .delay_ms = hk_delay_ms,
        .init = hk_virt_i2c_init,
        .xfer_start = hk_virt_i2c_start,
        .xfer_stop = hk_virt_i2c_stop,
        .wait_ack = hk_virt_i2c_wait_ack,
        .set_ack = hk_virt_i2c_set_ack,
        .set_nack = hk_virt_i2c_set_nack,
        .send_byte = hk_virt_i2c_send_byte,
        .read_byte = hk_virt_i2c_read_byte,
    },
};

/********************  sdio  ********************/
// sdio dma
// DMA_DIR_PeripheralDST ,MEM 2 PER;
// DMA_DIR_PeripheralSRC ,PER 2 MEM;
DMA_InitTypeDef dma_cfg = {
	.DMA_DIR 					= DMA_DIR_PeripheralDST,  		//数据传输方向，从内存读取发送到外设
	.DMA_BufferSize 			= 512 / 4,  			    //DMA通道的DMA缓存的大小
	.DMA_PeripheralInc 		    = DMA_PeripheralInc_Disable,  	//外设地址寄存器不变
	.DMA_MemoryInc 			    = DMA_MemoryInc_Enable,			//内存地址寄存器递增
	.DMA_PeripheralDataSize 	= DMA_PeripheralDataSize_Word,  //数据宽度为32位
	.DMA_MemoryDataSize 		= DMA_MemoryDataSize_Word, 		//数据宽度为32位
	.DMA_Mode 					= DMA_Mode_Normal,  			//工作在正常缓存模式
	.DMA_Priority 				= DMA_Priority_High, 			//DMA通道 x拥有高优先级 
	.DMA_M2M 					= DMA_M2M_Disable,  			//DMA通道x没有设置为内存到内存传输
};

dma_object_t g_sdio_dma = {
    .dma_cfg = {
        .dma_clk = RCC_AHBPeriph_DMA2,
        .dma_param = (void *)&dma_cfg,
        .channel = DMA2_Channel4,
    },
    .dma_ops = {
        .dma_init = hk_dma_obj_init,
        .dma_transfer_ctrl = hk_dma_transfer_ctrl,
    },
};

hk_sdio_hw_cfg_t g_sdio_hw_cfg = {
    .sd_gpio1   = {
        .gpio_cfg = {
            .gpio_clk = SDIO_CLK_PERIPH_CLK,
            .p_port = (void *)SDIO_CLK_PORT,
            .gpio_pin = (SDIO_CLK_PIN | SDIO_DAT_PIN),
            .flag = GPIO_TYPE_AF,
        },
        .gpio_ops = {
            .gpio_init = hk_gpio_obj_init,
        },
    },
    .sd_gpio2   = {
        .gpio_cfg = {
            .gpio_clk = SDIO_CMD_PERIPH_CLK,
            .p_port = (void *)SDIO_CMD_PORT,
            .gpio_pin = SDIO_CMD_PIN,
            .flag = GPIO_TYPE_AF,
        },
        .gpio_ops = {
            .gpio_init = hk_gpio_obj_init,
        },
    },
};

sd_card_info_t g_card_info = {	
    .rca        = 0,
    .cardtype   = SDIO_STD_CAPACITY_SD_CARD_V1_1,	
};

sdio_obj_t g_sdio_obj = {
    .sdio_cfg = {
        .dma_obj        = &g_sdio_dma,
        .cardinfo       = &g_card_info,
        .devicemode     = SD_DMA_MODE,
        .stopcondition  = 0,
        .transfererror  = SD_OK,
        .transferend    = 0,
        .p_hw_cfg       = (void *)&g_sdio_hw_cfg,
    },
    .sdio_ops = {
        .sd_init        = hk_sd_init,
        .show_card_info = hk_sdio_show_cardinfo,
        .sd_read_disk   = hk_sd_read_disk,
        .sd_write_disk  = hk_sd_write_disk,
    },
};

/********************  ENCODER  ********************/
// encoder sw
gpio_object_t g_enc_sw = {
    .gpio_cfg = {
        .gpio_clk = ENCODER_SW_PORT_PERIPH_CLK,
        .p_port = (void *)ENCODER_SW_PORT,
        .gpio_pin = ENCODER_SW_PIN,
        .gpio_dir = GPIO_DIR_INPUT,
        .flag = GPIO_TYPE_IO,
        .mode = GPIO_Mode_IPU,
    },

    .gpio_ops = {
        .gpio_init          = hk_gpio_obj_init,
        .gpio_output_set    = hk_gpio_obj_out_set,
        .gpio_fix_input     = hk_gpio_fix_input,
        .gpio_fix_output    = hk_gpio_fix_output,
        .gpio_input_get     = hk_gpio_obj_in_get,
    },
};

hk_exit_pin_cfg g_exit3_encsw_pin_cfg = {
    .exit_gpio_cfg          = &g_enc_sw,
    .exit_clk               = RCC_APB2Periph_AFIO,
    .exit_pin_port_source   = GPIO_PortSourceGPIOC,
    .exit_pin_source        = GPIO_PinSource3,
};

hk_exit_cfg g_exit3_encsw_cfg = {
    .exit_mode      = EXTI_Mode_Interrupt,
    .exit_trigger   = EXTI_Trigger_Rising,
    // .exit_line_cmd  = ,
    .exit_line      = EXTI_Line3,
    .exit_irqn      = EXTI3_IRQn,
    .exit_pre_prio  = 0x01,
    .exit_sub_prio  = 0x02,
};

exit_object_t g_exit3_obj = {
    .exit_cfg = {
        .p_pin_cfg  = (void *)&g_exit3_encsw_pin_cfg,
        .p_exit_cfg = (void *)&g_exit3_encsw_cfg,
        .delay_ms   = hk_delay_ms,
    },
    .exit_ops = {
        .exit_init      =   hk_exit_init,
        .exit_enable    =   hk_exit_enable,
        .exit_disable   =   hk_exit_disable,
        .exit_irq_cb    =   encoder_sw_handler,
    }
};

// encoder exit pin
gpio_object_t g_enc_exit = {
    .gpio_cfg = {
        .gpio_clk = ENCODER_PA_PORT_PERIPH_CLK,
        .p_port = (void *)ENCODER_PA_PORT,
        .gpio_pin = ENCODER_PA_PIN,
        .gpio_dir = GPIO_DIR_INPUT,
        .flag = GPIO_TYPE_IO,
        .mode = GPIO_Mode_IPU,
    },

    .gpio_ops = {
        .gpio_init          = hk_gpio_obj_init,
        .gpio_output_set    = hk_gpio_obj_out_set,
        .gpio_fix_input     = hk_gpio_fix_input,
        .gpio_fix_output    = hk_gpio_fix_output,
        .gpio_input_get     = hk_gpio_obj_in_get,
    },
};

hk_exit_pin_cfg g_exit4_encexit_pin_cfg = {
    .exit_gpio_cfg          = &g_enc_exit,
    .exit_clk               = RCC_APB2Periph_AFIO,
    .exit_pin_port_source   = GPIO_PortSourceGPIOC,
    .exit_pin_source        = GPIO_PinSource4,
};

hk_exit_cfg g_exit4_encexit_cfg = {
    .exit_mode      = EXTI_Mode_Interrupt,
    .exit_trigger   = EXTI_Trigger_Rising_Falling,
    // .exit_line_cmd  = ,
    .exit_line      = EXTI_Line4,
    .exit_irqn      = EXTI4_IRQn,
    .exit_pre_prio  = 0x01,
    .exit_sub_prio  = 0x01,
};

exit_object_t g_exit4_obj = {
    .exit_cfg = {
        .p_pin_cfg  = (void *)&g_exit4_encexit_pin_cfg,
        .p_exit_cfg = (void *)&g_exit4_encexit_cfg,
        .delay_ms   = hk_delay_ms,
    },
    .exit_ops = {
        .exit_init      =   hk_exit_init,
        .exit_enable    =   hk_exit_enable,
        .exit_disable   =   hk_exit_disable,
        .exit_irq_cb    =   encoder_data_handler,
    }
};

// encoder gpio
gpio_object_t g_enc_gpio = {
    .gpio_cfg = {
        .gpio_clk = ENCODER_PB_PORT_PERIPH_CLK,
        .p_port = (void *)ENCODER_PB_PORT,
        .gpio_pin = ENCODER_PB_PIN,
        .gpio_dir = GPIO_DIR_INPUT,
        .flag = GPIO_TYPE_IO,
        .mode = GPIO_Mode_IPU,
    },

    .gpio_ops = {
        .gpio_init          = hk_gpio_obj_init,
        .gpio_output_set    = hk_gpio_obj_out_set,
        .gpio_fix_input     = hk_gpio_fix_input,
        .gpio_fix_output    = hk_gpio_fix_output,
        .gpio_input_get     = hk_gpio_obj_in_get,
    },
};

encoder_object_t g_encoder_obj = {
    .pin_sw     = &g_exit3_obj,
    .pin_exit   = &g_exit4_obj,
    .pin_gpio   = &g_enc_gpio,
};

/********************  FLASH  ********************/
flash_object_t g_flash_obj = {
    .flash_cfg = {
        .flash_start_addr   = 0x0807F800,
        .flash_page_size    = 0x800,
        .flash_offset       = 0,
        .flash_max_size     = 0x800,
    },
    .flash_ops = {
        .flash_write    = hk_flash_write ,
        .flash_read     = hk_flash_read,
    },
};

/********************  DMA  ********************/
// dma_object_t g_dma_obj = {
//     .dma_cfg = {
//         .dma_clk = RCC_AHBPeriph_DMA2,
//         .channel = DMA2_Channel4,
//     },
//     .dma_ops = {
//         .dma_init = hk_dma_obj_init,
//         .dma_transfer_ctrl = hk_dma_transfer_ctrl,
//     },
// };