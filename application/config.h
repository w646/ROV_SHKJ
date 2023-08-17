#ifndef __CONFIG_H__
#define __CONFIG_H__

#define OS							1					//是否在有操作系统情况下执行


#if OS
	#define ROV_Delay		vTaskDelay
#else
	#define ROV_Delay		HAL_Delay
#endif

#endif
