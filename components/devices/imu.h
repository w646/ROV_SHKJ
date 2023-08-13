#ifndef _IMU_H_
#define _IMU_H_

#define IMU_CMD_LENGTH 		6
#define IMU_FRAME_LENGTH 	80
#define IMU_DATA_LENGTH 	40


/* ----------------------- Data Struct ------------------------------------- */
#pragma pack(push, 1)
typedef  struct
{
	//TODO: 添加IMU数据结构体
} IMU_data_t;	

#pragma pack(pop)
/* ----------------------- extern Function ----------------------------------- */
extern const IMU_data_t *get_imu_data_point(void);
extern void imu_init(void);

#endif /* _IMU_H_ */
