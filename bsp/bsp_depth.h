#ifndef BSP_DEPTH_H
#define BSP_DEPTH_H

#include "struct_typedef.h"

#define DEPTH_DMA_TX_ISR   DMA_HISR_TCIF6 //DMA发送中断完成标志位
#define DEPTH_DMA_RX_ISR   DMA_LISR_TCIF1 //DMA接收中断完成标志位

#define DEPTH_RE_DE_TX() {GPIOC->BSRR=GPIO_PIN_8;}                       //拉高电平
#define DEPTH_RE_DE_RX() {GPIOC->BSRR=(uint32_t)GPIO_PIN_8 << 16;}        //拉低电平

extern void usart6_init(uint8_t *rx1_buf, uint8_t *rx2_buf, uint16_t dma_buf_num);
extern void usart6_tx_dma_enable(uint8_t *data, uint16_t len);


#endif /* BSP_DEPTH_H */
