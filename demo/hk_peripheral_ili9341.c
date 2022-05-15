#include "boards.h"


#include "usart.h"
#include "hk_usart.h"
#include "gpio.h"
#include "hk_gpio.h"
#include "timer.h"
#include "hk_timer.h"
#include "hk_systick.h"
#include "fsmc.h"
#include "st7789_8080.h"
#include "nt35510_fsmc.h"
#include "ili9341_fsmc.h"

#include "hk_peripheral.h"

// systick设置
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
    },
};

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

/* ST7789 */
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

/* ILI9341 */
// 1. 构建一个driver对象
ili9341_fsmc_info_t g_ili9341 = {
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

tftlcd_driver_t g_lcd9341_driver = {
    .p_tft_cfg = (void *)&g_ili9341,
    .lcd_info = {
        .setxcmd = 0x2A,
        .setycmd = 0x2B,
        .wramcmd = 0x2C,
        .width = 240,
        .height = 320,
        .id = 0,
        .dir = 0,
        .background_color = YELLOW,
        .point_color = BLUE,
    },
};

tftlcd_object_t g_tftlcd9341_obj = {
    .tftlcd_cfg = {
        .p_dri = &g_lcd9341_driver,

        .hardware_init  = ili9341_hardware_init,
        .write_cmd      = ili9341_write_cmd, 
        .write_data     = ili9341_write_data, 
        .read_data      = ili9341_read_data,
        .write_reg      = ili9341_write_reg,
        .delay_ms       = hk_delay_ms, 
        .delay_us       = hk_delay_us,

    },
    .tftlcd_ops = {
        // 私有函数
        .read_reg       = ili9341_read_reg,
        .init           = ili9341_init,
        .set_cursor     = ili9341_set_cursor,
        .write_ram_pre  = ili9341_write_ram_pre,
        .write_ram      = ili9341_write_ram,
        .set_scan_dir   = ili9341_set_scan_dir,

        // 公有函数
        .clear_screen   = tftlcd_clear_screen,
        .fill_area      = tftlcd_fill_area,
        .draw_point     = tftlcd_draw_point,
        .show_char      = tftlcd_show_char,
    },
};


tftlcd_object_t *g_tftlcd_lvgl_obj = &g_tftlcd9341_obj;