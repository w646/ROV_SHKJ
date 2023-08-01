/** 
 *****************************Copyright (c) 2023  ZJU****************************
 * @file      : comunication.c
 * @brief     : 上下位机通讯数据解析
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
  //TODO: 初始化接收数据结构体
}

void init_send_struct_data(void)
{
  //TODO: 初始化发送数据结构体
}



/**
  * @brief          解析数据
  * @param[in]      frame 	解析的数据帧
  * @retval         none
  */
void receive_data_solve(uint8_t *frame)
{
    //TODO: 把数据赋值给结构体
}



/**
* @brief     发送内容打包
* @param[in] cmd_type:  命令内容ID
* @param[in] *p_data: 数据段
* @param[in] len:     数据段长度
*/
	
void send_data_pack(uint8_t cmd_type, uint8_t *p_data, uint16_t len)
{
  //TODO: 把数据打包成帧
}



