/** 
 *****************************Copyright (c) 2023  ZJU****************************
 * @file      : am.c
 * @brief     : 高度计设备驱动文件
 * @history   :
 *  Version     Date            Author          Note
 *  V1.0.0    2023-08-14         WPJ        1. <note>
 *******************************************************************************
 * @verbatim :
 *==============================================================================
 * 高度计数据读取通过上位机设置为定时发送
 * 指令ID122
 * 返回数据帧格式为：ddd.ddd\r\n (直接是字符串形式ASCII码)                                        
 *==============================================================================
 * @endverbatim :
 *****************************Copyright (c) 2023  ZJU****************************
 */


#include "am.h"
#include "bsp_am.h"
#include "main.h"

//高度计通信串口
extern UART_HandleTypeDef huart1;
extern DMA_HandleTypeDef hdma_usart1_tx;
extern DMA_HandleTypeDef hdma_usart1_rx;

//接收原始数据，为n个字节，给了2*n个字节长度，防止DMA传输越界
static uint8_t am_rx_buf[2][AM_FRAME_LENGTH];

//am  data 
//高度计数据
AM_data_t am_data;



/**
  * @brief          获取高度计数据指针
  * @param[in]      none
  * @retval         高度计数据指针
  */
const AM_data_t *get_am_data_point(void)
{
    return &am_data;
}

/**
  * @brief This function handles USART6 global interrupt.
  */
void USART1_IRQHandler(void)
{ 
	if(huart1.Instance->ISR & UART_FLAG_RXNE)//接收到数据
	{
		__HAL_UART_CLEAR_PEFLAG(&huart1);
	}
	else if(USART6->ISR & UART_FLAG_IDLE)
	{
		static uint16_t this_time_rx_len = 0;
		__HAL_UART_CLEAR_IDLEFLAG(&huart1);
        //清除Cache缓存
		SCB_InvalidateDCache_by_Addr ((uint32_t *)am_rx_buf, 2*AM_FRAME_LENGTH);
		if ((((DMA_Stream_TypeDef   *)hdma_usart1_rx.Instance)->CR & DMA_SxCR_CT) == RESET)
        {
            /* Current memory buffer used is Memory 0 */

            //disable DMA
            //失效DMA
            __HAL_DMA_DISABLE(&hdma_usart1_rx);

            //get receive data length, length = set_data_length - remain_length
            //获取接收数据长度,长度 = 设定长度 - 剩余长度
            this_time_rx_len = AM_FRAME_LENGTH - ((DMA_Stream_TypeDef   *)hdma_usart1_rx.Instance)->NDTR;
                
            //reset set_data_lenght
            //重新设定数据长度
            ((DMA_Stream_TypeDef   *)hdma_usart1_rx.Instance)->NDTR = AM_FRAME_LENGTH;

            //set memory buffer 1
            //设定缓冲区1
            ((DMA_Stream_TypeDef   *)hdma_usart1_rx.Instance)->CR |= DMA_SxCR_CT;
            
            //enable DMA
            //使能DMA
            __HAL_DMA_ENABLE(&hdma_usart1_rx);
            if(this_time_rx_len == AM_DATA_LENGTH)
            {
                am_data_solve(am_rx_buf[0], &am_data);
            } 
               
		}
        else
        {
            /* Current memory buffer used is Memory 1 */
            //disable DMA
            //失效DMA
            __HAL_DMA_DISABLE(&hdma_usart1_rx);

            //get receive data length, length = set_data_length - remain_length
            //获取接收数据长度,长度 = 设定长度 - 剩余长度
            //this_time_rx_len = IMU_FRAME_LENGTH - ((DMA_Stream_TypeDef   *)hdma_uart4_rx.Instance)->NDTR;
            this_time_rx_len = AM_FRAME_LENGTH - ((DMA_Stream_TypeDef   *)hdma_usart1_rx.Instance)->NDTR;

            //reset set_data_lenght
            //重新设定数据长度
            ((DMA_Stream_TypeDef   *)hdma_usart1_rx.Instance)->NDTR = AM_FRAME_LENGTH;

            //set memory buffer 0
            //设定缓冲区0
            ((DMA_Stream_TypeDef   *)hdma_usart1_rx.Instance)->CR &= ~(DMA_SxCR_CT);
            
            //enable DMA
            //使能DMA
            __HAL_DMA_ENABLE(&hdma_usart1_rx);
            if(this_time_rx_len == AM_DATA_LENGTH)
            {
                am_data_solve(am_rx_buf[1], &am_data);
            }     
        }
	}
	
}


/**
  * @brief          高度计协议解析
  * @param[in]      am_frame: 原生数据指针
  * @param[in]     am_data: 高度计数据指针
  * @retval         none
  */
void am_data_solve(volatile const uint8_t *am_frame, AM_data_t *am_data)
{
	/*数据校验*/
    if (am_frame == NULL || am_data == NULL)
    {
        return;
    }
    //TODO:进行高度计数据解析
    else if (am_frame[AM_DATA_LENGTH-2] == '\r' && am_frame[AM_DATA_LENGTH-1] == '\n')
    {
      am_data->height = am_frame[0]*100.0f + am_frame[1]*10.0f + am_frame[2]*1.0f
                        + am_frame[4]*0.1f + am_frame[5]*0.01f + am_frame[6]*0.001f;
    }

}