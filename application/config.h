#ifndef __CONFIG_H__
#define __CONFIG_H__

#define OS							1					//�Ƿ����в���ϵͳ�����ִ��


#if OS
	#define ROV_Delay		vTaskDelay
#else
	#define ROV_Delay		HAL_Delay
#endif

#endif
