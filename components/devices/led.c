/** 
 *****************************Copyright (c) 2023  ZJU****************************
 * @file      : led.c
 * @brief     : LED设备驱动文件
 * @history   :
 *  Version     Date            Author          Note
 *  V1.0.0    2023-08-12       Hao Lion        1. <note>
 *******************************************************************************
 * @verbatim :
 *==============================================================================
 *                                                                              
 *                                                                              
 *==============================================================================
 * @endverbatim :
 *****************************Copyright (c) 2023  ZJU****************************
 */


#include "led.h"
#include "bsp_led.h"

uint8_t led_tx_buffer[7] = {0xFF, 0X01, 0x00, 0x07, 0x00,};


/**
 * @brief 手动关闭LED灯
 * @param none
 * @retval none
*/
void led_off(void)
{
    led_tx_buffer[5] = 0x99;
    led_tx_buffer[6] = 0xA1;

    usart2_tx_dma_enable(led_tx_buffer, sizeof(led_tx_buffer));
}


/**
 * @brief 灯光增强
 * @param none
 * @retval none
*/
void led_enhance(void)
{
    led_tx_buffer[5] = 0x96;
    led_tx_buffer[6] = 0x9E;

    usart2_tx_dma_enable(led_tx_buffer, sizeof(led_tx_buffer));
}

/**
 * @brief 灯光减弱
 * @param none
 * @retval none
*/
void led_weaken(void)
{
    led_tx_buffer[5] = 0x97;
    led_tx_buffer[6] = 0x9F;

    usart2_tx_dma_enable(led_tx_buffer, sizeof(led_tx_buffer));
}

/**
 * @brief 灯光亮度存储
 * @param none
 * @retval none
*/
void led_save(void)
{
    led_tx_buffer[5] = 0xFA;
    led_tx_buffer[6] = 0x02;

    usart2_tx_dma_enable(led_tx_buffer, sizeof(led_tx_buffer));
}

/**
 * @brief 灯光亮度恢复
 * @param none
 * @retval none
*/
void led_recover(void)
{
    led_tx_buffer[5] = 0xFB;
    led_tx_buffer[6] = 0x03;

    usart2_tx_dma_enable(led_tx_buffer, sizeof(led_tx_buffer));
}
