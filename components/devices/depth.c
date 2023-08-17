/** 
 *****************************Copyright (c) 2023  ZJU****************************
 * @file      : depth.c
 * @brief     : 深度计设备驱动文件
 * @history   :
 *  Version     Date            Author          Note
 *  V1.0.0    2023-08-14         WPJ        1. <note>
 *******************************************************************************
 * @verbatim :
 *==============================================================================
 * 深度计的数据读取————————中断读取                                                                             
 * 深度计数据解析——————————需要发送指令才能得到回传                                                   
 *==============================================================================
 * @endverbatim :
 *****************************Copyright (c) 2023  ZJU****************************
 */


#include "depth.h"
#include "bsp_depth.h"
#include "main.h"

//深度计通信串口
extern UART_HandleTypeDef huart6;
extern DMA_HandleTypeDef hdma_usart6_tx;
extern DMA_HandleTypeDef hdma_usart6_rx;

//接收原始数据，为n个字节，给了2*n个字节长度，防止DMA传输越界
static uint8_t depth_rx_buf[2][DEPTH_FRAME_LENGTH];

uint8_t depth_tx_buf[8];// {0x01,0x03,0x00,0x02,0x00,0x02,0x65,0xCB,   //读取压力传感器1的值
                        // 0x01,0x03,0x00,0x04,0x00,0x02,0x85,0xCA};  //读取压力传感器2的值
typedef enum
{
    P1_stAdd = 0x0002,      //压力传感器1寄存器地址
    P2_stAdd = 0x0004,      //压力传感器2寄存器地址
}depth_cmd_stAdd;

//depth  data 
//深度计数据
DEPTH_data_t depth_data;

static int atmosphere_flag = 5;


/**
  * @brief          get  depth data point
  * @param[in]      none
  * @retval         imu data point
  */
/**
  * @brief          获取深度计数据指针
  * @param[in]      none
  * @retval         深度计数据指针
  */
const DEPTH_data_t *get_depth_data_point(void)
{
    return &depth_data;
}

/**
  * @brief This function handles USART6 global interrupt.
  */
void USART6_IRQHandler(void)
{ 
	if(huart6.Instance->ISR & UART_FLAG_RXNE)//接收到数据
	{
		__HAL_UART_CLEAR_PEFLAG(&huart6);
	}
	else if(USART6->ISR & UART_FLAG_IDLE)
	{
		static uint16_t this_time_rx_len = 0;
		__HAL_UART_CLEAR_IDLEFLAG(&huart6);
        //清除Cache缓存
		SCB_InvalidateDCache_by_Addr ((uint32_t *)depth_rx_buf, 2*DEPTH_FRAME_LENGTH);
		if ((((DMA_Stream_TypeDef   *)hdma_usart6_rx.Instance)->CR & DMA_SxCR_CT) == RESET)
        {
            /* Current memory buffer used is Memory 0 */

            //disable DMA
            //失效DMA
            __HAL_DMA_DISABLE(&hdma_usart6_rx);

            //get receive data length, length = set_data_length - remain_length
            //获取接收数据长度,长度 = 设定长度 - 剩余长度
            this_time_rx_len = DEPTH_FRAME_LENGTH - ((DMA_Stream_TypeDef   *)hdma_usart6_rx.Instance)->NDTR;
                
            //reset set_data_lenght
            //重新设定数据长度
            ((DMA_Stream_TypeDef   *)hdma_usart6_rx.Instance)->NDTR = DEPTH_FRAME_LENGTH;

            //set memory buffer 1
            //设定缓冲区1
            ((DMA_Stream_TypeDef   *)hdma_usart6_rx.Instance)->CR |= DMA_SxCR_CT;
            
            //使能RS485接收
            RE_DE_RX();
            //enable DMA
            //使能DMA
            __HAL_DMA_ENABLE(&hdma_usart6_rx);
            if(this_time_rx_len == DEPTH_DATA_LENGTH)
            {
                depth_data_solve(depth_rx_buf[0], &depth_data);
            } 
               
		}
        else
        {
            /* Current memory buffer used is Memory 1 */
            //disable DMA
            //失效DMA
            __HAL_DMA_DISABLE(&hdma_usart6_rx);

            //get receive data length, length = set_data_length - remain_length
            //获取接收数据长度,长度 = 设定长度 - 剩余长度
            //this_time_rx_len = IMU_FRAME_LENGTH - ((DMA_Stream_TypeDef   *)hdma_uart4_rx.Instance)->NDTR;
            this_time_rx_len = DEPTH_FRAME_LENGTH - ((DMA_Stream_TypeDef   *)hdma_usart6_rx.Instance)->NDTR;

            //reset set_data_lenght
            //重新设定数据长度
            ((DMA_Stream_TypeDef   *)hdma_usart6_rx.Instance)->NDTR = DEPTH_FRAME_LENGTH;

            //set memory buffer 0
            //设定缓冲区0
            ((DMA_Stream_TypeDef   *)hdma_usart6_rx.Instance)->CR &= ~(DMA_SxCR_CT);
            
            //使能RS485接收
            RE_DE_RX();
            //enable DMA
            //使能DMA
            __HAL_DMA_ENABLE(&hdma_usart6_rx);
            if(this_time_rx_len == DEPTH_DATA_LENGTH)
            {
                depth_data_solve(depth_rx_buf[1], &depth_data);
            }     
        }
		
	}
	
}


/**
  * @brief          将4个字节数据byte[4]转化为浮点数
  * @param[in]      four_byte: 待转换四字节指针
  * @retval         浮点数float(32字节)
  */
float Byte_to_Float(uint8_t *four_byte)
{
	float float_data=0;
	unsigned long longdata = 0;
	longdata = (*four_byte<< 24) + (*(four_byte+1) << 16) + (*(four_byte + 2) << 8) + (*(four_byte + 3) << 0);
	float_data = *(float*)&longdata;
	return float_data;
}


/**
* @brief     发送深度计相关指令
* @param[in] stdAdd:  寄存器地址
* @param[in] Reg_num: 读取寄存器数量
* @retval			返回要发送的数据大小
*/
static uint16_t send_depth_pack(uint16_t stdAdd, uint16_t Reg_num)
{
    memset(depth_tx_buf, 0, sizeof(depth_tx_buf));
    uint16_t send_len = 8;
    depth_tx_buf[0] = 0x01;
    depth_tx_buf[1] = 0x03;
    depth_tx_buf[2] = stdAdd >> 8;
    depth_tx_buf[3] = stdAdd & 0xFF;
    depth_tx_buf[4] = Reg_num >> 8;
    depth_tx_buf[5] = Reg_num & 0xFF;
    append_CRC16_check_sum(depth_tx_buf, send_len);
    usart6_tx_dma_enable(depth_tx_buf, send_len);
    return send_len;
}

/**
* @brief     发送读取压力值1的指令
* @param[in] stdAdd:  寄存器地址
* @param[in] Reg_num: 读取寄存器数量
* @retval			NULL
*/
void get_pressure1_cmd(void)
{
  RE_DE_TX();
  send_servo_pack(P1_stAdd, 2);
}

/**
* @brief     发送读取压力值2的指令
* @param[in] stdAdd:  寄存器地址
* @param[in] Reg_num: 读取寄存器数量
* @retval			NULL
*/
void get_pressure2_cmd(void)
{
  RE_DE_TX();
  send_servo_pack(P2_stAdd, 2);
}

/**
  * @brief          DEPTH protocol resolution
  * @param[in]      depth_buf: raw data point
  * @param[in]     depth_data: IMU data struct point
  * @retval         none
  */
/**
  * @brief          深度计协议解析
  * @param[in]      depth_frame: 原生数据指针
  * @param[in]     depth_data: 深度计数据指针
  * @retval         none
  */
static void depth_data_solve(volatile const uint8_t *depth_frame, DEPTH_data_t *depth_data)
{
  static int i = 5;
  static float sum = 0;
	/*数据校验*/
    if (depth_frame == NULL || depth_data == NULL)
    {
        return;
    }
    //TODO:进行深度计数据解析
    else if (depth_frame[0] == 0x01 && depth_frame[1] == 0x03 &&  depth_frame[2] == 0x04 && verify_CRC16_check_sum(depth_frame, DEPTH_DATA_LENGTH))
    {
      if(atmosphere_flag > 0) // 深度前5次读取的气压值求平均作为大气压参考值
      {
        sum += Byte_to_Float(&depth_frame[3]);
        atmosphere_flag--;
        if (atmosphere_flag == 0)
        {
          depth_data->atmosphere = sum/5.0f;
        }
      }
      else
      {
        depth_data->Pressure1 = Byte_to_Float(&depth_frame[3]);
        depth_data->depth_result = (depth_data->Pressure1 - depth_data->atmosphere) * 100000.0f / (g * WATER_DENSITY);
      }
    }
}