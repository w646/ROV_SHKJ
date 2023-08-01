#ifndef __COMUNICATION_H__
#define __COMUNICATION_H__


#include "struct_typedef.h"



/* ----------------------- Data Struct ------------------------------------- */


/* ----------------------- Extern Function ----------------------------------- */
/**
  * @brief          解析数据
  * @param[in]      frame 	解析的数据帧
  * @retval         none
  */
extern void receive_data_solve(uint8_t *frame);

/**
* @brief     发送内容打包
* @param[in] cmd_type:  命令内容ID
* @param[in] *p_data: 数据段
* @param[in] len:     数据段长度
*/
extern void send_data_pack(uint8_t cmd_type, uint8_t *p_data, uint16_t len);



extern void init_recieve_struct_data(void);
extern void init_send_struct_data(void);
#endif
