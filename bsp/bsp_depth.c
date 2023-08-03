/** 
 *****************************Copyright (c) 2023  ZJU****************************
 * @file      : bsp_depth.c
 * @brief     : ��ȼ��豸485����DMA����
 * @history   :
 *  Version     Date            Author          Note
 *  V1.0.0    2023-08-01         WPJ        1. <note>
 *******************************************************************************
 * @verbatim :
 *==============================================================================
 * ���ﴮ��1��DMA����Ϊ                                                                    
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

//�����շ���Ҫ��HAL�⣬̫����ֱ�Ӳ����Ĵ���
#define RE_DE_TX() {GPIOC->BSRR=GPIO_PIN_6;}                       //���ߵ�ƽ
#define RE_DE_RX() {GPIOC->BRR=(uint32_t)GPIO_PIN_7 << 16;}        //���͵�ƽ

extern UART_HandleTypeDef huart6;
extern DMA_HandleTypeDef hdma_usart6_tx;
extern DMA_HandleTypeDef hdma_usart6_rx;


void usart6_init(uint8_t *rx1_buf, uint8_t *rx2_buf, uint16_t dma_buf_num)
{

    //enable the DMA transfer for the receiver and tramsmit request
    //ʹ��DMA���ڽ��պͷ���
    SET_BIT(huart6.Instance->CR3, USART_CR3_DMAR);
    SET_BIT(huart6.Instance->CR3, USART_CR3_DMAT);

    //enalbe idle interrupt
    //ʹ�ܴ��ڿ����жϣ����ղ���������
    __HAL_UART_ENABLE_IT(&huart6, UART_IT_IDLE);

    //disable DMA
    //ʧЧDMA
    __HAL_DMA_DISABLE(&hdma_usart6_rx);
    
    while(((DMA_Stream_TypeDef   *)hdma_usart6_rx.Instance)->CR & DMA_SxCR_EN)
    {
        __HAL_DMA_DISABLE(&hdma_usart6_rx);
    }
    //��������жϱ�־��λ
    __HAL_DMA_CLEAR_FLAG(&hdma_usart6_rx, DEPTH_DMA_RX_ISR);

    ((DMA_Stream_TypeDef   *)hdma_usart6_rx.Instance)->PAR = (uint32_t) & (USART6->RDR);
    //memory buffer 1
    //�ڴ滺����1
    ((DMA_Stream_TypeDef   *)hdma_usart6_rx.Instance)->M0AR = (uint32_t)(rx1_buf);
    //memory buffer 2
    //�ڴ滺����2
    ((DMA_Stream_TypeDef   *)hdma_usart6_rx.Instance)->M1AR = (uint32_t)(rx2_buf);
    //data length
    //���ݳ���
    __HAL_DMA_SET_COUNTER(&hdma_usart6_rx, dma_buf_num);

    //enable double memory buffer
    //ʹ��˫������
    SET_BIT(((DMA_Stream_TypeDef   *)hdma_usart6_rx.Instance)->CR, DMA_SxCR_DBM);

    //enable DMA
    //ʹ��usart6_rx��DMA
    __HAL_DMA_ENABLE(&hdma_usart6_rx);


    //disable DMA
    //ʧЧusart6_tx��DMA
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
    //ʧЧusart6_tx��DMA
    __HAL_DMA_DISABLE(&hdma_usart6_tx);

    while(((DMA_Stream_TypeDef   *)hdma_usart6_tx.Instance)->CR & DMA_SxCR_EN)
    {
        __HAL_DMA_DISABLE(&hdma_usart6_tx);
    }
    //��������жϱ�־��λ
    __HAL_DMA_CLEAR_FLAG(&hdma_usart6_tx, DEPTH_DMA_TX_ISR);

    ((DMA_Stream_TypeDef   *)hdma_usart6_tx.Instance)->M0AR = (uint32_t)(data);
    __HAL_DMA_SET_COUNTER(&hdma_usart6_tx, len);

    __HAL_DMA_ENABLE(&hdma_usart6_tx);
}
