/** 
 *****************************Copyright (c) 2023  ZJU****************************
 * @file      : bsp_servo.c
 * @brief     : 舵机485串口板级支持包
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


#include "bsp_servo.h"
#include "main.h"

#define SERVO_DMA_TX_ISR   DMA_LISR_TCIF3

//串口7的485芯片具有硬件流控制，无需IO控制收发


extern UART_HandleTypeDef huart7;
extern DMA_HandleTypeDef hdma_uart7_tx;

void uart7_tx_dma_init(void)
{

    //enable the DMA transfer for the receiver and tramsmit request
    //使能DMA串口接收和发送
    SET_BIT(huart7.Instance->CR3, USART_CR3_DMAR);
    SET_BIT(huart7.Instance->CR3, USART_CR3_DMAT);

    //disable DMA
    //失效DMA
    __HAL_DMA_DISABLE(&hdma_uart7_tx);

    while(((DMA_Stream_TypeDef   *)hdma_uart7_tx.Instance)->CR & DMA_SxCR_EN)
    {
        __HAL_DMA_DISABLE(&hdma_uart7_tx);
    }

    ((DMA_Stream_TypeDef   *)hdma_uart7_tx.Instance)->PAR = (uint32_t) & (UART7->TDR);
    ((DMA_Stream_TypeDef   *)hdma_uart7_tx.Instance)->M0AR = (uint32_t)(NULL);
    ((DMA_Stream_TypeDef   *)hdma_uart7_tx.Instance)->NDTR = 0;


}
void uart7_tx_dma_enable(uint8_t *data, uint16_t len)
{
    //disable DMA
    //失效DMA
    __HAL_DMA_DISABLE(&hdma_uart7_tx);

    while(((DMA_Stream_TypeDef   *)hdma_uart7_tx.Instance)->CR & DMA_SxCR_EN)
    {
        __HAL_DMA_DISABLE(&hdma_uart7_tx);
    }

    __HAL_DMA_CLEAR_FLAG(&hdma_uart7_tx, SERVO_DMA_TX_ISR);

    ((DMA_Stream_TypeDef   *)hdma_uart7_tx.Instance)->M0AR = (uint32_t)(data);
    __HAL_DMA_SET_COUNTER(&hdma_uart7_tx, len);

    __HAL_DMA_ENABLE(&hdma_uart7_tx);
}
