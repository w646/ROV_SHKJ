/**
  ****************************(C) COPYRIGHT 2022 ZJU****************************
  * @file       tem_hum_task.c/h
  * @brief      ��ʪ�ȴ��������ݴ�������         
  * @note       
  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0     DEC-7-2022     HaoLion(������)    1. done
  *
  @verbatim
  ==============================================================================

  ==============================================================================
  @endverbatim
  ****************************(C) COPYRIGHT 2022 ZJU****************************
  */

#include <stdio.h>
#include "tem_hum_task.h"
#include "main.h"
#include "bsp_i2c.h"
#include "sth31driver.h"
#include "cmsis_os.h"

float temperature;
float humidity;

/**
  * @brief          Data of temperature and humidity processing
  * @param[in]      pvParameters: NULL
  * @retval         none
  */
/**
  * @brief          ��ʪ�����ݴ���
  * @param[in]      pvParameters: NULL
  * @retval         none
  */
void tem_hum_task(void const * argument)
{
    osDelay(1000);
    SHT31_Init();
    while(1)
    {
        if(SHT31_Read_Dat(&temperature,&humidity))
				{
					printf("Temperature:%.2f    Humidity:%.2f\r\n",temperature,humidity);
				}
				else
				{
					printf("Read data of temperature and humidity failed!\n");
				}
        osDelay(1000);
    }
}


float get_cabin_temperature(void)
{
    return (float)(temperature);
}

float get_cabin_humidity(void)
{
    return (float)(humidity);
}

