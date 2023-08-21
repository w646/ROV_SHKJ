#ifndef BSP_AM_H
#define BSP_AM_H

#include "struct_typedef.h"

#define AM_DMA_TX_ISR   DMA_HISR_TCIF7
#define AM_DMA_RX_ISR   DMA_LISR_TCIF2  

//快速收发不要用HAL库，太慢，直接操作寄存器
#define AM_RE_DE_TX() {GPIOA->BSRR=GPIO_PIN_8;}                        //拉高电平
#define AM_RE_DE_RX() {GPIOA->BSRR=(uint32_t)GPIO_PIN_8 << 16;}        //拉低电平


extern void usart1_init(uint8_t *rx1_buf, uint8_t *rx2_buf, uint16_t dma_buf_num);
extern void usart1_tx_dma_enable(uint8_t *data, uint16_t len);


#endif /* BSP_AM_H */
