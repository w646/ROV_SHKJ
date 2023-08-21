#ifndef _IMU_H_
#define _IMU_H_

#include "struct_typedef.h"

#define IMU_CMD_LENGTH 		11
#define IMU_HEAD_SOF        0x3A //帧头
#define IMU_ID              0x0001 //IMU设备ID
#define IMU_END_SOF         0x0A0D //帧尾

#define IMU_FRAME_LENGTH 	400
#define IMU_DATA_LENGTH 	200


/* ----------------------- Data Struct ------------------------------------- */
#pragma pack(push, 1)
typedef  struct
{
	//TODO: 添加IMU数据结构体
	//加速度 单位：g(9.8m/s)
	float Acc_x;
	float Acc_y;
	float Acc_z;
	//角度 单位：rad
	float Angle_x;
	float Angle_y;
	float Angle_z;
	//角速度 单位：rad/s
	float Gyrol_x;
	float Gyrol_y;
	float Gyrol_z;
} IMU_data_t;	

#pragma pack(pop)
/* ----------------------- extern Function ----------------------------------- */
extern const IMU_data_t *get_imu_data_point(void);
extern void imu_init(void);
extern float Byte_to_Float(uint8_t *four_byte);
#endif /* _IMU_H_ */
