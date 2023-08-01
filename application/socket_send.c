/**
  ****************************(C) COPYRIGHT 2022 ZJU****************************
  * @file       socket_send.c/h
  * @brief      tcp��������ÿ10ms����λ������һ��״̬
	*							������֧�ַ����ı���Ϣ���ڵ��ԣ��ı���Ϣ���ᱻ��������Ϣ������
  * @note       
  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0     DEC-13-2022     HaoLion(������)    1. done
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
	* @brief          TCP��������
  * @param[in]      pvParameters: NULL
  * @retval         none
  */
void server_send_task(void const * argument)
{
	int sock = -1,connected;
	char *recv_data;
	struct sockaddr_in server_addr,client_addr;
	socklen_t sin_size;
	/*��ʼ���ṹ��*/
	init_send_struct_data();
	/*�����ڴ�*/
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
	/*��������Ϣ*/
	if (bind(sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1)
	{
		printf("Unable to bind in send task\n");
		goto __exit;
	}
	/*�������������״̬*/
	if (listen(sock, 5) == -1)
	{
		printf("Listen error in send task\n");
		goto __exit;
	}
	while(1)
	{
		sin_size = sizeof(struct sockaddr_in);
		/*�ȴ��������󣬻��������߳�ֱ������TCP����*/
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
			//TODO:��������
			osDelay(10);
		}
		/*�ͻ�����Ϣ����ʧ�ܣ��Ͽ�����*/
		if (connected >= 0)
			closesocket(connected);
		connected = -1;
	}	
__exit:
	if (sock >= 0) closesocket(sock);
	if (recv_data) free(recv_data);
}




/**
  * @brief          rov some measure data updata, such as motor speed, euler angle��status of cabin
  * @param[out]     rov_status_updata: "ext_rov_status_t" valiable point
  * @retval         none
  */
/**
  * @brief          ROV״̬���ݸ��£���������ٶȣ�ŷ���Ƕȣ�����״̬��
  * @param[out]     rov_status_updata:"ext_rov_status_t"����ָ��.
  * @retval         none
  */
static void rov_status_update(void *rov_status_updata)
{
	//TODO:��������
}

/**
  * @brief          send status of rov
  * @param[out]     none
  * @retval         none
  */
/**
	* @brief          ����rov״̬
  * @param[out]     none
  * @retval         none
  */
static void rov_send_status(void)
{
	//TODO������ROV״̬
}


