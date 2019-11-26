#include "wifi_base_config.h"
#include "appliance.h"
#include "stm32f7xx.h"
#include "debug.h"
/* FreeRTOS头文件 */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "./sdram/bsp_sdram.h"  
#include "./camera/bsp_ov2640.h"
				
#include "camera_data_queue.h"
#include "tcp_server.h"
#include "stdlib.h"
#include "string.h"

				
				
/**
 * @brief app_main
 *
 */
#define fire_demo_log(M, ...) custom_log("WIFI", M, ##__VA_ARGS__)


void app_main( void )
{
		host_thread_type_t    wwd_thread;
		camera_data * cambuf;
		int32_t err = kNoErr;

		/*配置wifi lwip信息*/
		Config_WIFI_LwIP_Info();

//		err = camera_queue_init();
//		cambuf = cbWrite(&cam_circular_buff);
//	


//		err = open_camera((uint32_t *)cambuf->head, CAMERA_QUEUE_DATA_LEN);
	
//printf("current	line=%d\r\n",__LINE__);
//while(1)
//{
//}
		SDRAM_Init();//初始化外部sdram
		printf("初始化 TCP_server\r\n");
		host_rtos_create_thread( &wwd_thread, (void *)tcp_server_thread, "TCP_server", NULL,4096, 1);


    while(1)
    {

    }

}


