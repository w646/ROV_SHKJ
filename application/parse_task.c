/**
  ****************************(C) COPYRIGHT 2023 ZJU****************************
  * @file       parse_task.c/h
  * @brief      socket receive data slove.
  *             ��λ�������·������߳�.        
  * @note       
  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0     Apr-5-2023     HaoLion(������)    1. done
  *
  @verbatim
  ==============================================================================

  ==============================================================================
  @endverbatim
  ****************************(C) COPYRIGHT 2023 ZJU****************************
  */
	
#include "parse_task.h"
#include "main.h"
#include "cmsis_os.h"

#include "detect_task.h"
#include "protocol.h"
#include "comunication.h"
	
fifo_s_t tcp_fifo;
uint8_t tcp_fifo_buf[TCP_FIFO_BUF_LENGTH];
unpack_data_t tcp_unpack_obj;
	
	
/**
  * @brief          tcp data upacked 
  * @param[in]      data	data of frame
  * @param[in]      length of data
  * @retval         none
  */
/**
  * @brief          ���ݽ��
  * @param[in]      data ֡ԭʼ����
  * @param[in]      framelen ֡����
  * @retval         none
  */
static void tcp_unpack_fifo_data(void);
	
	/**
  * @brief          parse task
  * @param[in]      pvParameters: NULL
  * @retval         none
  */
/**
  * @brief          �������ݽ�������
  * @param[in]      pvParameters: NULL
  * @retval         none
  */
void parse_task(void const * argument)
{
	init_recieve_struct_data();
  fifo_s_init(&tcp_fifo, tcp_fifo_buf, TCP_FIFO_BUF_LENGTH);
	while(1)
	{
		tcp_unpack_fifo_data();
		osDelay(10);
	}
}



/**
  * @brief          tcp data upacked 
  * @param[in]      data	data of frame
  * @param[in]      length of data
  * @retval         none
  */
/**
  * @brief          ���ݽ��
  * @param[in]      data ֡ԭʼ����
  * @param[in]      framelen ֡����
  * @retval         none
  */

void tcp_unpack_fifo_data()
{
	
}
