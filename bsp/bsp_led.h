#ifndef _BSP_LED_H
#define _BSP_LED_H

#include "struct_typedef.h"

extern void usart2_tx_dma_init(void);
extern void usart2_tx_dma_enable(uint8_t *data, uint16_t len);

#endif /* _BSP_LED_H */
