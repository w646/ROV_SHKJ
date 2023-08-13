/** 
 *****************************Copyright (c) 2023  ZJU****************************
 * @file      : bsp_led.c
 * @brief     : LED485板级支持包
 * @history   :
 *  Version     Date            Author          Note
 *  V1.0.0    2023-08-09       Hao Lion        1. <note>
 *******************************************************************************
 * @verbatim :
 *==============================================================================
 *                                                                              
 *                                                                              
 *==============================================================================
 * @endverbatim :
 *****************************Copyright (c) 2023  ZJU****************************
 */

#include "bsp_led.h"
#include "main.h"

#define LED_DMA_TX_ISR   DMA_HISR_TCIF6

//串口2的485芯片具有硬件流控制，无需IO控制收发


extern UART_HandleTypeDef huart2;
extern DMA_HandleTypeDef hdma_usart2_tx;

void usart2_tx_dma_init(void)
{

    //enable the DMA transfer for the receiver and tramsmit request
    //使能DMA串口接收和发送
    SET_BIT(huart2.Instance->CR3, USART_CR3_DMAR);
    SET_BIT(huart2.Instance->CR3, USART_CR3_DMAT);

    //disable DMA
    //失效DMA
    __HAL_DMA_DISABLE(&hdma_usart2_tx);

    while(((DMA_Stream_TypeDef   *)hdma_usart2_tx.Instance)->CR & DMA_SxCR_EN)
    {
        __HAL_DMA_DISABLE(&hdma_usart2_tx);
    }

    ((DMA_Stream_TypeDef   *)hdma_usart2_tx.Instance)->PAR = (uint32_t) & (USART1->TDR);
    ((DMA_Stream_TypeDef   *)hdma_usart2_tx.Instance)->M0AR = (uint32_t)(NULL);
    ((DMA_Stream_TypeDef   *)hdma_usart2_tx.Instance)->NDTR = 0;


}
void usart2_tx_dma_enable(uint8_t *data, uint16_t len)
{
    //disable DMA
    //失效DMA
    __HAL_DMA_DISABLE(&hdma_usart2_tx);

    while(((DMA_Stream_TypeDef   *)hdma_usart2_tx.Instance)->CR & DMA_SxCR_EN)
    {
        __HAL_DMA_DISABLE(&hdma_usart2_tx);
    }

    __HAL_DMA_CLEAR_FLAG(&hdma_usart2_tx, LED_DMA_TX_ISR);

    ((DMA_Stream_TypeDef   *)hdma_usart2_tx.Instance)->M0AR = (uint32_t)(data);
    __HAL_DMA_SET_COUNTER(&hdma_usart2_tx, len);

    __HAL_DMA_ENABLE(&hdma_usart2_tx);
}

