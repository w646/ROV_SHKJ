/** 
 *****************************Copyright (c) 2023  ZJU****************************
 * @file      : comunication.c
 * @brief     : ����λ��ͨѶ���ݽ���
 * @history   :
 *  Version     Date            Author          Note
 *  V1.0.0    2023-08-01       Hao Lion        1. <note>
 *******************************************************************************
 * @verbatim :
 *==============================================================================
 *                                                                              
 *                                                                              
 *==============================================================================
 * @endverbatim :
 *****************************Copyright (c) 2023  ZJU****************************
 */


#include "comunication.h"
#include "string.h"
#include "stdio.h"
#include "protocol.h"





void init_recieve_struct_data(void)
{
  //TODO: ��ʼ���������ݽṹ��
}

void init_send_struct_data(void)
{
  //TODO: ��ʼ���������ݽṹ��
}



/**
  * @brief          ��������
  * @param[in]      frame 	����������֡
  * @retval         none
  */
void receive_data_solve(uint8_t *frame)
{
    //TODO: �����ݸ�ֵ���ṹ��
}



/**
* @brief     �������ݴ��
* @param[in] cmd_type:  ��������ID
* @param[in] *p_data: ���ݶ�
* @param[in] len:     ���ݶγ���
*/
	
void send_data_pack(uint8_t cmd_type, uint8_t *p_data, uint16_t len)
{
  //TODO: �����ݴ����֡
}



