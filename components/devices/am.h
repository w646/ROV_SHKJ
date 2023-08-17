#ifndef _AM_H_
#define _AM_H_

#include "struct_typedef.h"

#define AM_FRAME_LENGTH 	18
#define AM_DATA_LENGTH 	9


/* ----------------------- Data Struct ------------------------------------- */
#pragma pack(push, 1)
typedef  struct
{
	//TODO: 添加IMU数据结构体
	float height;
} AM_data_t;	

#pragma pack(pop)
/* ----------------------- extern Function ----------------------------------- */
extern const AM_data_t *get_am_data_point(void);
extern void am_data_solve(volatile const uint8_t *am_frame, AM_data_t *am_data);//高度计解码

#endif /* _AM_H_ */
