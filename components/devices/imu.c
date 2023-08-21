/** 
 *****************************Copyright (c) 2023  ZJU****************************
 * @file      : imu.c
 * @brief     : IMU设备驱动文件
 * @history   :
 *  Version     Date            Author          Note
 *  V1.0.0    2023-08-12         WPJ        1. <note>
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
typedef enum
{
    Goto_Command_Mode   = 0x0006,  //进入命令模式
    Goto_Streaming_Mode = 0x0007, //进入数据发送模式
    Save_Setting        = 0x0004, //保存IMU参数设置

}imu_cmd_e;

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
  * @brief          计算前n字节累加校验和
  * @param[in]      LRC_message: 数据
  * @param[in]      LRC_length: 数据和校验的长度
  * @retval         计算完的校验和
  */
static uint8_t get_LRC_sum(uint8_t *LRC_message, uint16_t LRC_length)
{
    uint16_t check_sum = 0;
    uint16_t len = LRC_length;
    if (LRC_message == NULL)
    {
        return 0XFF;
    }
    while(--len)
    {
        check_sum += *LRC_message++;
    }
    return check_sum;
}

/**
  * @brief          LRC校验
  * @param[in]      LRC_message: 数据
  * @param[in]      LRC_length: 数据的长度
  * @retval         计算完的校验和
  */
static bool_t LRC_check(uint8_t *data_message, uint32_t LRC_length)
{
  uint16_t temp = 0;
  uint16_t len = LRC_length;
  temp = get_LRC_sum(data_message, len-4);
  if(data_message[len-4] == (temp & 0xFF) && data_message[len-3] == (temp >> 8))
  {
    return 1;
  }
  else
  {
    return 0;
  }
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
    if (imu_frame[0] == 0x3A && imu_frame[1] == 0x01 && imu_frame[2] == 0x00 && LRC_check(imu_frame, IMU_DATA_LENGTH))
    {
      imu_data->Acc_x = Byte_to_Float(imu_frame[23]);
      imu_data->Acc_y = Byte_to_Float(imu_frame[27]);
      imu_data->Acc_z = Byte_to_Float(imu_frame[31]);

      imu_data->Gyrol_x = Byte_to_Float(imu_frame[83]);
      imu_data->Gyrol_y = Byte_to_Float(imu_frame[87]);
      imu_data->Gyrol_z = Byte_to_Float(imu_frame[91]);

      imu_data->Angle_x = Byte_to_Float(imu_frame[147]);
      imu_data->Angle_y = Byte_to_Float(imu_frame[151]);
      imu_data->Angle_z = Byte_to_Float(imu_frame[155]);
    }
}



/**
* @brief     发送内容打包
* @param[in] cmd_type:  命令内容ID
* @retval			返回要发送的数据大小
*/
static uint16_t send_imu_pack(uint8_t cmd_type, uint8_t *p_data, uint16_t len)
{

  memset(imu_tx_buf, 0, sizeof(imu_tx_buf));
  uint16_t send_len = 9 + len;
  imu_tx_buf[0] = IMU_HEAD_SOF;
  imu_tx_buf[1] = IMU_ID & 0xFF;
  imu_tx_buf[2] = IMU_ID >> 8;
  imu_tx_buf[3] = cmd_type & 0xFF;
  imu_tx_buf[4] = cmd_type;
  imu_tx_buf[5] = len & 0xFF;
  imu_tx_buf[6] = len >> 8;
  if(len == 0)
  {
    imu_tx_buf[7] = get_LRC_sum(imu_tx_buf, send_len-4) & 0xFF;
    imu_tx_buf[8] = get_LRC_sum(imu_tx_buf, send_len-4) >> 8;
    send_len = send_len + 2;
  }
  else
  {
    memcpy(&imu_tx_buf[7], p_data, len);
  }
  imu_tx_buf[send_len-4] = get_LRC_sum(imu_tx_buf, send_len-4) & 0xFF;
  imu_tx_buf[send_len-3] = get_LRC_sum(imu_tx_buf, send_len-4) >> 8;
  imu_tx_buf[send_len-2] = IMU_END_SOF & 0xFF;
  imu_tx_buf[send_len-1] = IMU_END_SOF >> 8;
  uart7_tx_dma_enable(imu_tx_buf, send_len);
  return send_len;
}

/**
 * @brief 进入IMU命令模式
 * @retval none
*/
void GOTO_IMU_Command_Mode(void)
{ 
  send_imu_pack(Goto_Command_Mode, NULL, 0);
}

/**
 * @brief 进入IMU数据流发送
 * @retval none
*/
void GOTO_IMU_Streaming_Mode(void)
{ 
  send_imu_pack(Goto_Streaming_Mode, NULL, 0);
}

/**
 * @brief IMU参数保存
 * @retval none
*/
void GOTO_IMU_Save_Setting(void)
{ 
  send_imu_pack(Save_Setting, NULL, 0);
}