#include "bsp_i2c.h"
#include "main.h"


extern I2C_HandleTypeDef hi2c3;


void bsp_I2C_master_transmit(I2C_TypeDef *I2C, uint16_t I2C_address, uint8_t *data, uint16_t len)
{
        HAL_I2C_Master_Transmit(&hi2c3, I2C_address, data, len, 100);
}

void bsp_I2C_master_receive(I2C_TypeDef *I2C, uint16_t I2C_address, uint8_t *data, uint16_t len)
{
        HAL_I2C_Master_Receive(&hi2c3, I2C_address, data, len, 100);
}


