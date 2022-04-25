#ifndef BOARD_V100_H__
#define BOARD_V100_H__

#include "stm32f10x.h"

/* GPIO */
#define LED_PIN                             GPIO_Pin_0
#define LED_PORT                            GPIOB
#define LED_PORT_PERIPH_CLK                 RCC_APB2Periph_GPIOB



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

#endif
