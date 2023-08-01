#ifndef TEM_HUM_TASK_H
#define TEM_HUM_TASK_H

#include "struct_typedef.h"




/* ----------------------- Data Struct ------------------------------------- */



/* ----------------------- Extern Function ----------------------------------- */
/**
  * @brief          Data of temperature and humidity processing
  * @param[in]      pvParameters: NULL
  * @retval         none
  */
/**
  * @brief          温湿度数据处理
  * @param[in]      pvParameters: NULL
  * @retval         none
  */
extern void tem_hum_task(void const * argument);

extern float get_cabin_humidity(void);

extern float get_cabin_temperature(void);

#endif
