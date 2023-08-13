#ifndef _BSP_IMU_H_
#define _BSP_IMU_H_

#include "struct_typedef.h"

extern void uart4_tx_dma_init(void);
extern void uart4_tx_dma_enable(uint8_t *data, uint16_t len);

#endif /* _BSP_IMU_H_ */
