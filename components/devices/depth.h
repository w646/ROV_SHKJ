#ifndef _DEPTH_H_
#define _DEPTH_H_

#include "struct_typedef.h"

#define DEPTH_CMD_LENGTH 	8
#define DEPTH_FRAME_LENGTH 	18
#define DEPTH_DATA_LENGTH 	9


#define WATER_DENSITY 	    1000.0f
#define g 	                9.8f

/* ----------------------- Data Struct ------------------------------------- */
#pragma pack(push, 1)
typedef  struct
{
	//TODO: 添加IMU数据结构体
	float atmosphere; //单位为Pa
	float Pressure1; //单位为Pa
	float Pressure2; //单位为Pa
	float depth_result; //单位为m
} DEPTH_data_t;	

#pragma pack(pop)
/* ----------------------- extern Function ----------------------------------- */
extern float Byte_to_Float(uint8_t *four_byte); //4字节转浮点数
extern const DEPTH_data_t *get_depth_data_point(void); //获取深度计结构体指针
extern void depth_init(void); // 深度计初始化（未编写）
extern uint16_t send_depth_pack(uint16_t stdAdd, uint16_t Reg_num); //深度计发送指令打包
extern void get_pressure1_cmd(void); //获取压力值1指令
extern void get_pressure2_cmd(void); //获取压力值2指令
extern void depth_data_solve(volatile const uint8_t *depth_frame, DEPTH_data_t *depth_data); //深度计解码

#endif /* _DEPTH_H_ */
