#ifndef _PROTOCOL_H
#define _PROTOCOL_H

#include "struct_typedef.h"


#define REC_TEXT_MAX_SIZE         					255											//最大数据长度
#define REC_PROTOCOL_HEADER_SIZE    sizeof(frame_header_struct_t)		//֡整个帧头长度
#define HEADER_SOF 													0x55AA									//帧头
// #define TAIL_EOF                            0x0D0A                  //帧尾
#define REC_PROTOCOL_FRAME_MAX_SIZE          262    								//整帧最大长度
#define REC_HEADER_CRC_LEN										7											//头部和CRC16的长度									  									

#pragma pack(push, 1)

typedef enum
{
	INFO_REPORT = 0X01,						//ROV信息上报
  DEVICE_DATA_REPORT = 0X02,		//设备数据上报
  CMD_ISSUED = 0X11,						//命令下发
}cmd_type_t;                    //命令类型

typedef enum
{
	BASE_INFO_ID = 0X01,						//基本信息
}info_cmd_t;                    //信息上报子命令

typedef enum
{
  ALIIMETER_ID = 0X01,				        //高度计
  OBSTACLE_AVOIDANCE_SONAR_ID = 0X02,	//避障声呐
  DEPTH_METER_ID = 0X03,				      //深度计
  IMU_ID = 0X04,						          //IMU
  GPS_ID = 0X05,						          //GPS
}device_data_cmd_t;                   //设备数据上报子命令

typedef enum
{
  MOV_CRTL_ID = 0X01,						      //运动控制命令
  LED_ID = 0X02,							        //LED控制命令
  SERVO_ID = 0X03,						        //舵机控制命令
}cmd_issued_cmd_t;                   //命令下发子命令

typedef  struct
{
  uint16_t SOF;
	uint8_t cmd_type;
  uint8_t data_length;
  uint8_t sub_cmd;
} frame_header_struct_t;

typedef enum
{
  STEP_SOF_LOW     = 0,
  STEP_SOF_HIGH    = 1,
	STEP_CMD_TYPE		 = 2,
  STEP_DATA_LENGTH = 3,
  STEP_SUB_CMD     = 4,
  STEP_DATA_CRC16  = 5,
} unpack_tcp_step_e;


typedef struct
{
  frame_header_struct_t *p_header;
  uint8_t       data_len;
  uint8_t        protocol_packet[REC_PROTOCOL_FRAME_MAX_SIZE];
  unpack_tcp_step_e  unpack_step;
  uint16_t       index;
} unpack_data_t;



#pragma pack(pop)

#endif //_PROTOCOL_H
