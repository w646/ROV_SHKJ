/** 
 *****************************Copyright (c) 2023  ZJU****************************
 * @file      : bsp_depth.c
 * @brief     : 深度计设备485串口DMA驱动
 * @history   :
 *  Version     Date            Author          Note
 *  V1.0.0    2023-08-01         WPJ        1. <note>
 *******************************************************************************
 * @verbatim :
 *==============================================================================
 * 这里串口1的DMA配置为                                                                    
 * TX:DMA2 Stream6
 * RX:DMA2 Stream1                                                                     
 *==============================================================================
 * @endverbatim :
 *****************************Copyright (c) 2023  ZJU****************************
 */
#include "bsp_depth.h"
#include "main.h"

#define DEPTH_DMA_TX_ISR   DMA_HISR_TCIF6
#define DEPTH_DMA_RX_ISR   DMA_LISR_TCIF1  

//快速收发不要用HAL库，太慢，直接操作寄存器
#define RE_DE_TX() {GPIOC->BSRR=GPIO_PIN_6;}                       //拉高电平
#define RE_DE_RX() {GPIOC->BRR=(uint32_t)GPIO_PIN_7 << 16;}        //拉低电平

extern UART_HandleTypeDef huart6;
extern DMA_HandleTypeDef hdma_usart6_tx;
extern DMA_HandleTypeDef hdma_usart6_rx;


void usart6_init(uint8_t *rx1_buf, uint8_t *rx2_buf, uint16_t dma_buf_num)
{

    //enable the DMA transfer for the receiver and tramsmit request
    //使能DMA串口接收和发送
    SET_BIT(huart6.Instance->CR3, USART_CR3_DMAR);
    SET_BIT(huart6.Instance->CR3, USART_CR3_DMAT);

    //enalbe idle interrupt
    //使能串口空闲中断，接收不定长数据
    __HAL_UART_ENABLE_IT(&huart6, UART_IT_IDLE);

    //disable DMA
    //失效DMA
    __HAL_DMA_DISABLE(&hdma_usart6_rx);
    
    while(((DMA_Stream_TypeDef   *)hdma_usart6_rx.Instance)->CR & DMA_SxCR_EN)
    {
        __HAL_DMA_DISABLE(&hdma_usart6_rx);
    }
    //清除接收中断标志物位
    __HAL_DMA_CLEAR_FLAG(&hdma_usart6_rx, DEPTH_DMA_RX_ISR);

    ((DMA_Stream_TypeDef   *)hdma_usart6_rx.Instance)->PAR = (uint32_t) & (USART6->RDR);
    //memory buffer 1
    //内存缓冲区1
    ((DMA_Stream_TypeDef   *)hdma_usart6_rx.Instance)->M0AR = (uint32_t)(rx1_buf);
    //memory buffer 2
    //内存缓冲区2
    ((DMA_Stream_TypeDef   *)hdma_usart6_rx.Instance)->M1AR = (uint32_t)(rx2_buf);
    //data length
    //数据长度
    __HAL_DMA_SET_COUNTER(&hdma_usart6_rx, dma_buf_num);

    //enable double memory buffer
    //使能双缓冲区
    SET_BIT(((DMA_Stream_TypeDef   *)hdma_usart6_rx.Instance)->CR, DMA_SxCR_DBM);

    //enable DMA
    //使能usart6_rx的DMA
    __HAL_DMA_ENABLE(&hdma_usart6_rx);


    //disable DMA
    //失效usart6_tx的DMA
    __HAL_DMA_DISABLE(&hdma_usart6_tx);

    while(((DMA_Stream_TypeDef   *)hdma_usart6_tx.Instance)->CR & DMA_SxCR_EN)
    {
        __HAL_DMA_DISABLE(&hdma_usart6_tx);
    }

    ((DMA_Stream_TypeDef   *)hdma_usart6_tx.Instance)->PAR = (uint32_t) & (USART6->TDR);

}


void usart6_tx_dma_enable(uint8_t *data, uint16_t len)
{
    //disable DMA
    //失效usart6_tx的DMA
    __HAL_DMA_DISABLE(&hdma_usart6_tx);

    while(((DMA_Stream_TypeDef   *)hdma_usart6_tx.Instance)->CR & DMA_SxCR_EN)
    {
        __HAL_DMA_DISABLE(&hdma_usart6_tx);
    }
    //清除发送中断标志物位
    __HAL_DMA_CLEAR_FLAG(&hdma_usart6_tx, DEPTH_DMA_TX_ISR);

    ((DMA_Stream_TypeDef   *)hdma_usart6_tx.Instance)->M0AR = (uint32_t)(data);
    __HAL_DMA_SET_COUNTER(&hdma_usart6_tx, len);

    __HAL_DMA_ENABLE(&hdma_usart6_tx);
}
