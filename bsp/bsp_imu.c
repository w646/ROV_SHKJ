/** 
 *****************************Copyright (c) 2023  ZJU****************************
 * @file      : bsp_imu.c
 * @brief     : IMU设备232串口DMA板级支持包
 * @history   :
 *  Version     Date            Author          Note
 *  V1.0.0    2023-08-01       Hao Lion        1. <note>
 *******************************************************************************
 * @verbatim :
 *==============================================================================
 * 这里串口4的DMA配置为                                                                    
 * TX:DMA1 Stream7
 * RX:DMA2 Stream0                                                                       
 *==============================================================================
 * @endverbatim :
 *****************************Copyright (c) 2023  ZJU****************************
 */
#include "bsp_imu.h"
#include "main.h"

#define IMU_DMA_TX_ISR   DMA_HISR_TCIF7
#define IMU_DMA_RX_ISR   DMA_LISR_TCIF0

//232无需硬件流控制

extern UART_HandleTypeDef huart4;
extern DMA_HandleTypeDef hdma_uart4_tx;
extern DMA_HandleTypeDef hdma_uart4_rx;


void uart4_init(uint8_t *rx1_buf, uint8_t *rx2_buf, uint16_t dma_buf_num)
{

    //enable the DMA transfer for the receiver and tramsmit request
    //使能DMA串口接收和发送
    SET_BIT(huart4.Instance->CR3, USART_CR3_DMAR);
    SET_BIT(huart4.Instance->CR3, USART_CR3_DMAT);

    //enalbe idle interrupt
    //使能空闲中断
    __HAL_UART_ENABLE_IT(&huart4, UART_IT_IDLE);

    //disable DMA
    //失效DMA
    __HAL_DMA_DISABLE(&hdma_uart4_rx);
    
    while(((DMA_Stream_TypeDef   *)hdma_uart4_rx.Instance)->CR & DMA_SxCR_EN)
    {
        __HAL_DMA_DISABLE(&hdma_uart4_rx);
    }
    //清除接收中断标志物位
    __HAL_DMA_CLEAR_FLAG(&hdma_uart4_rx, IMU_DMA_RX_ISR);

    ((DMA_Stream_TypeDef   *)hdma_uart4_rx.Instance)->PAR = (uint32_t) & (UART4->RDR);
    //memory buffer 1
    //内存缓冲区1
    ((DMA_Stream_TypeDef   *)hdma_uart4_rx.Instance)->M0AR = (uint32_t)(rx1_buf);
    //memory buffer 2
    //内存缓冲区2
    ((DMA_Stream_TypeDef   *)hdma_uart4_rx.Instance)->M1AR = (uint32_t)(rx2_buf);
    //data length
    //数据长度
    __HAL_DMA_SET_COUNTER(&hdma_uart4_rx, dma_buf_num);

    //enable double memory buffer
    //使能双缓冲区
    SET_BIT(((DMA_Stream_TypeDef   *)hdma_uart4_rx.Instance)->CR, DMA_SxCR_DBM);

    //enable DMA
    //使能DMA
    __HAL_DMA_ENABLE(&hdma_uart4_rx);


    //disable DMA
    //失效DMA
    __HAL_DMA_DISABLE(&hdma_uart4_tx);

    while(((DMA_Stream_TypeDef   *)hdma_uart4_tx.Instance)->CR & DMA_SxCR_EN)
    {
        __HAL_DMA_DISABLE(&hdma_uart4_tx);
    }

    ((DMA_Stream_TypeDef   *)hdma_uart4_tx.Instance)->PAR = (uint32_t) & (UART4->TDR);

}


void uart4_tx_dma_enable(uint8_t *data, uint16_t len)
{
    //disable DMA
    //失效DMA
    __HAL_DMA_DISABLE(&hdma_uart4_tx);

    while(((DMA_Stream_TypeDef   *)hdma_uart4_tx.Instance)->CR & DMA_SxCR_EN)
    {
        __HAL_DMA_DISABLE(&hdma_uart4_tx);
    }

    __HAL_DMA_CLEAR_FLAG(&hdma_uart4_tx, IMU_DMA_TX_ISR);

    ((DMA_Stream_TypeDef   *)hdma_uart4_tx.Instance)->M0AR = (uint32_t)(data);
    __HAL_DMA_SET_COUNTER(&hdma_uart4_tx, len);

    __HAL_DMA_ENABLE(&hdma_uart4_tx);
}
