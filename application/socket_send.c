/**
  ****************************(C) COPYRIGHT 2022 ZJU****************************
  * @file       socket_send.c/h
  * @brief      tcp发送任务，每10ms向上位机更新一次状态
	*							还可以支持发送文本消息便于调试，文本消息将会被发送在消息队列中
  * @note       
  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0     DEC-13-2022     HaoLion(郝亮亮)    1. done
  *
  @verbatim
  ==============================================================================

  ==============================================================================
  @endverbatim
  ****************************(C) COPYRIGHT 2022 ZJU****************************
  */

#include "socket_send.h"
#include "main.h"
#include "string.h"
#include "comunication.h"

#include "lwip/opt.h"
#include "lwip/sys.h"
#include "lwip/api.h"
#include <lwip/sockets.h>


/**
  * @brief          server_send_task
  * @param[in]      pvParameters: NULL
  * @retval         none
  */
/**
	* @brief          TCP发送任务
  * @param[in]      pvParameters: NULL
  * @retval         none
  */
void server_send_task(void const * argument)
{
	int sock = -1,connected;
	char *recv_data;
	struct sockaddr_in server_addr,client_addr;
	socklen_t sin_size;
	/*初始化结构体*/
	init_send_struct_data();
	/*申请内存*/
	recv_data = (char *)pvPortMalloc(SEND_DATA);
	if (recv_data == NULL)
	{
		printf("No memory in send task\n");
		goto __exit;
	}
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0)
	{
		printf("Socket error in send task\n");
		goto __exit;
	}
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(TCP_SERVER_SEND_PORT);
	memset(&(server_addr.sin_zero), 0, sizeof(server_addr.sin_zero));
	/*绑定网卡信息*/
	if (bind(sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1)
	{
		printf("Unable to bind in send task\n");
		goto __exit;
	}
	/*服务器进入监听状态*/
	if (listen(sock, 5) == -1)
	{
		printf("Listen error in send task\n");
		goto __exit;
	}
	while(1)
	{
		sin_size = sizeof(struct sockaddr_in);
		/*等待连接请求，会阻塞该线程直至建立TCP连接*/
		connected = accept(sock, (struct sockaddr *)&client_addr, &sin_size);
		printf("Send task has new client connected from (%s, %d)\n",
		inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
		{
			int flag = 1;
			setsockopt(connected,
								IPPROTO_TCP, /* set option at TCP level */
								TCP_NODELAY, /* name of option */
								(void *) &flag, /* the cast is historical cruft */
								sizeof(int)); /* length of option value */
		}
		while (1)
		{
			//TODO:发送数据
			osDelay(10);
		}
		/*客户端消息发送失败，断开连接*/
		if (connected >= 0)
			closesocket(connected);
		connected = -1;
	}	
__exit:
	if (sock >= 0) closesocket(sock);
	if (recv_data) free(recv_data);
}




/**
  * @brief          rov some measure data updata, such as motor speed, euler angle，status of cabin
  * @param[out]     rov_status_updata: "ext_rov_status_t" valiable point
  * @retval         none
  */
/**
  * @brief          ROV状态数据更新，包括电机速度，欧拉角度，舱内状态等
  * @param[out]     rov_status_updata:"ext_rov_status_t"变量指针.
  * @retval         none
  */
static void rov_status_update(void *rov_status_updata)
{
	//TODO:更新数据
}

/**
  * @brief          send status of rov
  * @param[out]     none
  * @retval         none
  */
/**
	* @brief          发送rov状态
  * @param[out]     none
  * @retval         none
  */
static void rov_send_status(void)
{
	//TODO：发送ROV状态
}


