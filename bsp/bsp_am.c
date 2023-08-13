/** 
 *****************************Copyright (c) 2023  ZJU****************************
 * @file      : bsp_am.c
 * @brief     : 高度计设备485串口DMA板级支持包
 * @history   :
 *  Version     Date            Author          Note
 *  V1.0.0    2023-08-01       Hao Lion        1. <note>
 *******************************************************************************
 * @verbatim :
 *==============================================================================
 * 这里串口1的DMA配置为                                                                    
 * TX:DMA2 Stream7
 * RX:DMA1 Stream2                                                                       
 *==============================================================================
 * @endverbatim :
 *****************************Copyright (c) 2023  ZJU****************************
 */
#include "bsp_am.h"
#include "main.h"

#define AM_DMA_TX_ISR   DMA_HISR_TCIF7
#define AM_DMA_RX_ISR   DMA_LISR_TCIF2  

//快速收发不要用HAL库，太慢，直接操作寄存器
#define RE_DE_TX() {GPIOA->BSRR=GPIO_PIN_9;}                        //拉高电平
#define RE_DE_RX() {GPIOA->BRR=(uint32_t)GPIO_PIN_10 << 16;}        //拉低电平

extern UART_HandleTypeDef huart1;
extern DMA_HandleTypeDef hdma_usart1_tx;
extern DMA_HandleTypeDef hdma_usart1_rx;


void usart1_init(uint8_t *rx1_buf, uint8_t *rx2_buf, uint16_t dma_buf_num)
{

    //enable the DMA transfer for the receiver and tramsmit request
    //使能DMA串口接收和发送
    SET_BIT(huart1.Instance->CR3, USART_CR3_DMAR);
    SET_BIT(huart1.Instance->CR3, USART_CR3_DMAT);

    //enalbe idle interrupt
    //使能空闲中断
    __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);

    //disable DMA
    //失效DMA
    __HAL_DMA_DISABLE(&hdma_usart1_rx);
    
    while(((DMA_Stream_TypeDef   *)hdma_usart1_rx.Instance)->CR & DMA_SxCR_EN)
    {
        __HAL_DMA_DISABLE(&hdma_usart1_rx);
    }
    //清除接收中断标志物位
    __HAL_DMA_CLEAR_FLAG(&hdma_usart1_rx, AM_DMA_RX_ISR);

    ((DMA_Stream_TypeDef   *)hdma_usart1_rx.Instance)->PAR = (uint32_t) & (USART1->RDR);
    //memory buffer 1
    //内存缓冲区1
    ((DMA_Stream_TypeDef   *)hdma_usart1_rx.Instance)->M0AR = (uint32_t)(rx1_buf);
    //memory buffer 2
    //内存缓冲区2
    ((DMA_Stream_TypeDef   *)hdma_usart1_rx.Instance)->M1AR = (uint32_t)(rx2_buf);
    //data length
    //数据长度
    __HAL_DMA_SET_COUNTER(&hdma_usart1_rx, dma_buf_num);

    //enable double memory buffer
    //使能双缓冲区
    SET_BIT(((DMA_Stream_TypeDef   *)hdma_usart1_rx.Instance)->CR, DMA_SxCR_DBM);

    //enable DMA
    //使能DMA
    __HAL_DMA_ENABLE(&hdma_usart1_rx);


    //disable DMA
    //失效DMA
    __HAL_DMA_DISABLE(&hdma_usart1_tx);

    while(((DMA_Stream_TypeDef   *)hdma_usart1_tx.Instance)->CR & DMA_SxCR_EN)
    {
        __HAL_DMA_DISABLE(&hdma_usart1_tx);
    }

    ((DMA_Stream_TypeDef   *)hdma_usart1_tx.Instance)->PAR = (uint32_t) & (USART1->TDR);

}


void usart1_tx_dma_enable(uint8_t *data, uint16_t len)
{
    //disable DMA
    //失效DMA
    __HAL_DMA_DISABLE(&hdma_usart1_tx);

    while(((DMA_Stream_TypeDef   *)hdma_usart1_tx.Instance)->CR & DMA_SxCR_EN)
    {
        __HAL_DMA_DISABLE(&hdma_usart1_tx);
    }

    __HAL_DMA_CLEAR_FLAG(&hdma_usart1_tx, AM_DMA_TX_ISR);

    ((DMA_Stream_TypeDef   *)hdma_usart1_tx.Instance)->M0AR = (uint32_t)(data);
    __HAL_DMA_SET_COUNTER(&hdma_usart1_tx, len);

    __HAL_DMA_ENABLE(&hdma_usart1_tx);
}
