#ifndef _BSP_SERVO_H
#define _BSP_SERVO_H

#include "struct_typedef.h"

extern void uart7_tx_dma_init(void);
extern void uart7_tx_dma_enable(uint8_t *data, uint16_t len);

#endif /* _BSP_SERVO_H */
