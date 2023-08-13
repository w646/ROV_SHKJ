#ifndef _SERVO_H_
#define _SERVO_H_

#include "main.h"

/**
 * @brief 设置舵机控制角度
 * @param[in] servo_id:   舵机ID
 * @param[in] angle_set:  舵机角度
 * @param[in] time_ms:    舵机运行时间
 * @retval none
*/
extern void set_servo_angle(uint8_t servo_id, int16_t angle_set, uint16_t time_ms);

/**
 * @brief 获取舵机角度
 * @param[in] servo_id:   舵机ID
 * @retval none
*/
extern void get_servo_angle(uint8_t servo_id);

/**
 * @brief 修改舵机ID
 * @param[in] servo_id:   舵机原来ID
 * @param[in] new_id:     舵机新ID
 * @retval none
*/
extern void modify_servo_id(uint8_t servo_id, uint8_t new_id);

#endif /* _SERVO_H_ */
