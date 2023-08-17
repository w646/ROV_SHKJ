/** 
 *****************************Copyright (c) 2023  ZJU****************************
 * @file      : imu.c
 * @brief     : IMU设备驱动文件
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


#include "imu.h"
#include "bsp_imu.h"
#include "main.h"

extern UART_HandleTypeDef huart4;
extern DMA_HandleTypeDef hdma_uart4_tx;
extern DMA_HandleTypeDef hdma_uart4_rx;

//接收原始数据，为n个字节，给了2*n个字节长度，防止DMA传输越界
static uint8_t imu_rx_buf[2][IMU_FRAME_LENGTH];

uint8_t imu_tx_buf[IMU_CMD_LENGTH]={0};

//IMU  data 
//九轴数据
IMU_data_t imu_data;


/**
  * @brief          IMU protocol resolution
  * @param[in]      sbus_buf: raw data point
  * @param[out]     imu_data: IMU data struct point
  * @retval         none
  */
/**
  * @brief          IMU协议解析
  * @param[in]      imu_frame: 原生数据指针
  * @param[out]     imu_data: IMU数据指针
  * @retval         none
  */
static void imu_data_solve(volatile const uint8_t *imu_frame, IMU_data_t *imu_data);


/**
  * @brief          get  imu data point
  * @param[in]      none
  * @retval         imu data point
  */
/**
  * @brief          获取IMU数据指针
  * @param[in]      none
  * @retval         IMU数据指针
  */
const IMU_data_t *get_imu_data_point(void)
{
    return &imu_data;
}

/**
  * @brief This function handles UART4 global interrupt.
  */
void UART4_IRQHandler(void)
{
	if(huart4.Instance->ISR & UART_FLAG_RXNE)//接收到数据
	{
		__HAL_UART_CLEAR_PEFLAG(&huart4);
	}
	else if(UART4->ISR & UART_FLAG_IDLE)
	{
		static uint16_t this_time_rx_len = 0;
		__HAL_UART_CLEAR_IDLEFLAG(&huart4);
        //清除Cache缓存
		SCB_InvalidateDCache_by_Addr ((uint32_t *)imu_rx_buf, 2*IMU_FRAME_LENGTH);
		if ((((DMA_Stream_TypeDef   *)hdma_uart4_rx.Instance)->CR & DMA_SxCR_CT) == RESET)
        {
            /* Current memory buffer used is Memory 0 */

            //disable DMA
            //失效DMA
            __HAL_DMA_DISABLE(&hdma_uart4_rx);

            //get receive data length, length = set_data_length - remain_length
            //获取接收数据长度,长度 = 设定长度 - 剩余长度
            this_time_rx_len = IMU_FRAME_LENGTH - ((DMA_Stream_TypeDef   *)hdma_uart4_rx.Instance)->NDTR;
                
            //reset set_data_lenght
            //重新设定数据长度
            ((DMA_Stream_TypeDef   *)hdma_uart4_rx.Instance)->NDTR = IMU_FRAME_LENGTH;

            //set memory buffer 1
            //设定缓冲区1
            ((DMA_Stream_TypeDef   *)hdma_uart4_rx.Instance)->CR |= DMA_SxCR_CT;
            
            //enable DMA
            //使能DMA
            __HAL_DMA_ENABLE(&hdma_uart4_rx);
            if(this_time_rx_len == IMU_DATA_LENGTH)
            {
                imu_data_solve(imu_rx_buf[0], &imu_data);
            } 
               
		}
        else
        {
            /* Current memory buffer used is Memory 1 */
            //disable DMA
            //失效DMA
            __HAL_DMA_DISABLE(&hdma_uart4_rx);

            //get receive data length, length = set_data_length - remain_length
            //获取接收数据长度,长度 = 设定长度 - 剩余长度
            //this_time_rx_len = IMU_FRAME_LENGTH - ((DMA_Stream_TypeDef   *)hdma_uart4_rx.Instance)->NDTR;
            this_time_rx_len = IMU_FRAME_LENGTH - ((DMA_Stream_TypeDef   *)hdma_uart4_rx.Instance)->NDTR;

            //reset set_data_lenght
            //重新设定数据长度
            ((DMA_Stream_TypeDef   *)hdma_uart4_rx.Instance)->NDTR = IMU_FRAME_LENGTH;

            //set memory buffer 0
            //设定缓冲区0
            ((DMA_Stream_TypeDef   *)hdma_uart4_rx.Instance)->CR &= ~(DMA_SxCR_CT);
            
            //enable DMA
            //使能DMA
            __HAL_DMA_ENABLE(&hdma_uart4_rx);
            if(this_time_rx_len == IMU_DATA_LENGTH)
            {
                imu_data_solve(imu_rx_buf[1], &imu_data);
            }     
        }
		
	}
	
}


/**
  * @brief          IMU protocol resolution
  * @param[in]      sbus_buf: raw data point
  * @param[out]     imu_data: IMU data struct point
  * @retval         none
  */
/**
  * @brief          IMU协议解析
  * @param[in]      imu_frame: 原生数据指针
  * @param[out]     imu_data: IMU数据指针
  * @retval         none
  */
static void imu_data_solve(volatile const uint8_t *imu_frame, IMU_data_t *imu_data)
{
	/*数据校验*/
    if (imu_frame == NULL || imu_data == NULL)
    {
        return;
    }
    //TODO:进行IMU数据解析
}