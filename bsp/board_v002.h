#ifndef BOARD_V100_H__
#define BOARD_V100_H__

#include "stm32f10x.h"

/* GPIO */
#define LED_PIN                             GPIO_Pin_0
#define LED_PORT                            GPIOB
#define LED_PORT_PERIPH_CLK                 RCC_APB2Periph_GPIOB

#define KEY1_PIN                            GPIO_Pin_0
#define KEY2_PIN                            GPIO_Pin_1
#define KEY3_PIN                            GPIO_Pin_2
#define KEY_PORT                            GPIOA
#define KEY_PORT_PERIPH_CLK                 RCC_APB2Periph_GPIOA

/* UART */
#define TRACE_UART                          USART1
#define TRACE_UART_CLK                      RCC_APB2Periph_USART1

#define TRACE_UART_TX_PIN                   GPIO_Pin_9
#define TRACE_UART_TX_GPIO_PORT             GPIOA
#define TRACE_UART_TX_GPIO_CLK              RCC_APB2Periph_GPIOA


#define TRACE_UART_RX_PIN                   GPIO_Pin_10
#define TRACE_UART_RX_GPIO_PORT             GPIOA
#define TRACE_UART_RX_GPIO_CLK              RCC_APB2Periph_GPIOA

/* TIMER */
#define TIM3_CLK                            RCC_APB1Periph_TIM3
#define TIM3_IRQ                            TIM3_IRQn
    
#define USER_TIMER_CLK                      TIM3_CLK
#define USER_TIMER_IRQ                      TIM3_IRQ
#define USER_TIMER                          TIM3
    
#define USER_TIMER_CH                       TIM_IT_CC1
#define USER_TIMER_FREQ                     1000000     //定时器经分频后获得的频率
#define USER_TIMER_PERIOD                   1000        //定时器溢出计数

/* LCD -- ST7789 */
// PE0
#define LCD_CS_PIN                          GPIO_Pin_0
#define LCD_CS_PORT                         GPIOE
#define LCD_CS_PORT_PERIPH_CLK              RCC_APB2Periph_GPIOE

// PE1
#define LCD_RST_PIN                         GPIO_Pin_1
#define LCD_RST_PORT                        GPIOE
#define LCD_RST_PORT_PERIPH_CLK             RCC_APB2Periph_GPIOE

// PE2
#define LCD_RS_PIN                          GPIO_Pin_2
#define LCD_RS_PORT                         GPIOE
#define LCD_RS_PORT_PERIPH_CLK              RCC_APB2Periph_GPIOE

// PE3
#define LCD_WR_PIN                          GPIO_Pin_3
#define LCD_WR_PORT                         GPIOE
#define LCD_WR_PORT_PERIPH_CLK              RCC_APB2Periph_GPIOE

// PE4
#define LCD_RD_PIN                          GPIO_Pin_4
#define LCD_RD_PORT                         GPIOE
#define LCD_RD_PORT_PERIPH_CLK              RCC_APB2Periph_GPIOE

/* PD0~PD7 */
// #define LCD_DB_PIN                          0xFF            // bit7~bit0
#define LCD_DB_PIN                          (GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7)
#define LCD_DB_PORT                         GPIOD
#define LCD_DB_PORT_PERIPH_CLK              RCC_APB2Periph_GPIOD

/* LCD -- NT35510 */
#define LCD_RST1_PIN                            GPIO_Pin_1
#define LCD_RST1_PORT                           GPIOE
#define LCD_RST1_PORT_PERIPH_CLK                RCC_APB2Periph_GPIOE

#define LCD_BL_PIN                              GPIO_Pin_12
#define LCD_BL_PORT                             GPIOD
#define LCD_BL_PORT_PERIPH_CLK                  RCC_APB2Periph_GPIOD

#define LCD_FSMC1_PIN                          (GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_14|GPIO_Pin_15)
#define LCD_FSMC1_PORT                         GPIOD
#define LCD_FSMC1_PORT_PERIPH_CLK              RCC_APB2Periph_GPIOD

#define LCD_FSMC2_PIN                          (GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15)
#define LCD_FSMC2_PORT                         GPIOE
#define LCD_FSMC2_PORT_PERIPH_CLK              RCC_APB2Periph_GPIOE

/* Touch cfg */
#define TOUCH_SDA_PIN                           GPIO_Pin_3
#define TOUCH_SDA_PORT                          GPIOE
#define TOUCH_SDA_PERIPH_CLK                    RCC_APB2Periph_GPIOE

#define TOUCH_SCL_PIN                           GPIO_Pin_2
#define TOUCH_SCL_PORT                          GPIOE
#define TOUCH_SCL_PERIPH_CLK                    RCC_APB2Periph_GPIOE

#define TOUCH_RST_PIN                           GPIO_Pin_0
#define TOUCH_RST_PORT                          GPIOE
#define TOUCH_RST_PERIPH_CLK                    RCC_APB2Periph_GPIOE

#define TOUCH_INT_PIN                           GPIO_Pin_4
#define TOUCH_INT_PORT                          GPIOE
#define TOUCH_INT_PERIPH_CLK                    RCC_APB2Periph_GPIOE

/* SDIO cfg  */
#define SDIO_CMD_PIN                            GPIO_Pin_2
#define SDIO_CMD_PORT                           GPIOD
#define SDIO_CMD_PERIPH_CLK                     RCC_APB2Periph_GPIOD

#define SDIO_CLK_PIN                            GPIO_Pin_12
#define SDIO_CLK_PORT                           GPIOC
#define SDIO_CLK_PERIPH_CLK                     RCC_APB2Periph_GPIOC

#define SDIO_DAT_PIN                            (GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11)
#define SDIO_DAT_PORT                           GPIOC
#define SDIO_DAT_PERIPH_CLK                     RCC_APB2Periph_GPIOC

/* EEPROM */
// sda --> PB7, scl --> PB6
#define EEPROM_SDA_PIN                           GPIO_Pin_7
#define EEPROM_SDA_PORT                          GPIOB
#define EEPROM_SDA_PERIPH_CLK                    RCC_APB2Periph_GPIOB

#define EEPROM_SCL_PIN                           GPIO_Pin_6
#define EEPROM_SCL_PORT                          GPIOB
#define EEPROM_SCL_PERIPH_CLK                    RCC_APB2Periph_GPIOB

#endif
