#include "wifi_base_config.h"
#include "appliance.h"
#include "stm32f7xx.h"
#include "debug.h"
/* FreeRTOSͷ�ļ� */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "./sdram/bsp_sdram.h"  
#include "./camera/bsp_ov5640.h"
				
#include "camera_data_queue.h"
#include "tcp_server.h"
#include "stdlib.h"
#include "string.h"

				
				
/**
 * @brief app_main
 *
 */
#define fire_demo_log(M, ...) custom_log("WIFI", M, ##__VA_ARGS__)

extern uint32_t frame_counter;
extern int send_fream;
extern int cbReadFinish_num;
extern int get_data_num;
void app_main( void )
{
		host_thread_type_t    wwd_thread;
		camera_data * cambuf;
		int32_t err = kNoErr;

		/*����wifi lwip��Ϣ*/
		Config_WIFI_LwIP_Info();

		err = camera_queue_init();
	
		cambuf = cbWrite(&cam_circular_buff);
	
		err = open_camera((uint32_t *)cambuf->head, CAMERA_QUEUE_DATA_LEN);
	
		SDRAM_Init();//��ʼ���ⲿsdram
		printf("��ʼ�� TCP_server\r\n");
	
		host_rtos_create_thread( &wwd_thread, (void *)tcp_server_thread, "TCP_server", NULL,4096, 1);

		frame_counter=0;//֡����������
		send_fream=0;//֡����������
    while(1)
    {
			/*��ʱ*/
			vTaskDelay(1000);
			/*���֡��*/
			printf("------------------------------------>>>>>>>>frame_counter=%d fps/s , get_data_num -> %d ,send_fream ->%d fps/s \r\n",frame_counter,get_data_num,send_fream);
			frame_counter=0;			
			send_fream=0;
			get_data_num=0;
    }

}



