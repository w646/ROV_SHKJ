#ifndef BSP_GPS_H
#define BSP_GPS_H

#include "struct_typedef.h"

extern void usart1_init(uint8_t *rx1_buf, uint8_t *rx2_buf, uint16_t dma_buf_num);
extern void usart1_tx_dma_enable(uint8_t *data, uint16_t len);


#endif /* BSP_GPS_H */
