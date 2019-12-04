#include "camera_data_queue.h"
#include "./sdram/bsp_sdram.h"  
#include "wifi_base_config.h"
#include "stdlib.h"
#include "debug.h"
	
//#define LOG_SW  	//�Ƿ��ӡ����
#if defined(LOG_SW)

#endif 

#define camera_data_queue_log(M, ...) custom_log("camera_data_queue", M, ##__VA_ARGS__)

camera_data  cam_data[CAMERA_QUEUE_NUM];
CircularBuffer cam_circular_buff;

//���л��������ڴ��
__align(4) uint8_t queue_buff[CAMERA_QUEUE_NUM][CAMERA_QUEUE_DATA_LEN] __EXRAM;



int32_t find_jpeg_tail(uint8_t *data,uint8_t *jpeg_start,int32_t search_point) ;
void camera_queue_free(void);
int32_t camera_queue_init(void);
int  find_usable_index(void);
int32_t push_data_to_queue(void);
int32_t pull_data_from_queue(uint8_t **data_p, int32_t *len_p);
void clear_array_flag(uint8_t index);




/*���λ������*/

/**
  * @brief  ��ʼ���������
  * @param  cb:������нṹ��
  * @param  size: ������е�Ԫ�ظ���
  * @note 	��ʼ��ʱ����Ҫ��cb->elemsָ�븳ֵ
  */
void cbInit(CircularBuffer *cb, int size) 
{
    cb->size  = size;	/*	�������         */
    cb->start = 0; 		/* index of oldest element              */
    cb->end   = 0; 	 	/*д����Ԫ�ص�����  */

}
 
/**
  * @brief  ���������е�ǰ��״̬��Ϣ
  * @param  cb:������нṹ��
  */
void cbPrint(CircularBuffer *cb) 
{
#if 0
    printf("\r\n size=0x%x, start=%d, end=%d\r\n ", cb->size, cb->start, cb->end);
	  printf("\r\n size=0x%x, start_using=%d, end_using=%d\r\n ", cb->size, cb->start_using, cb->end_using);
#endif
}
 
/**
  * @brief  �жϻ��������(1)��(0)����
  * @param  cb:������нṹ��
  */
int cbIsFull(CircularBuffer *cb) 
{
    return cb->end == (cb->start ^ cb->size); /* This inverts the most significant bit of start before comparison */ 
}
 
/**
  * @brief  �жϻ��������(1)��(0)ȫ��
  * @param  cb:������нṹ��
  */		
int cbIsEmpty(CircularBuffer *cb) 
{
    return cb->end == cb->start; 
}

/**
  * @brief  �Ի�����е�ָ���1
  * @param  cb:������нṹ��
  * @param  p��Ҫ��1��ָ��
  * @return  ���ؼ�1�Ľ��
  */	
int cbIncr(CircularBuffer *cb, int p) 
{
    return (p + 1)&(2*cb->size-1); /* �����յ�ָ��ĵ�����2 * sizeΪģ */
}
 
/**
  * @brief  ��ȡ��д��Ļ�����ָ��
  * @param  cb:������нṹ��
  * @return  �ɽ���д��Ļ�����ָ��
  * @note  �õ�ָ���ɽ���д���������дָ�벻��������1��д������ʱ��Ӧ����cbWriteFinish
  */
camera_data* cbWrite(CircularBuffer *cb) 
{
    if (cbIsFull(cb)) /* ���������ƶ���ʼָ�� */
    {
			return NULL;
		}		
		else
			cb->end_using = cbIncr(cb, cb->end); //δ����������1

		__IO int temp_index=cb->end_using&(cb->size-1);
#if defined(LOG_SW)
		//printf("�ɽ���д���ָ�� %d\r\n",temp_index);
		printf("cbWrite %d\r\n",temp_index);
#endif 				
		
	return  cb->elems[temp_index];
}

/**
  * @brief  ��ȡ�ɵ�ǰ����ʹ�õ�д������ָ��
  * @param  cb:������нṹ��
  * @return  ��ǰ����ʹ�õ�д������ָ��
  */
extern int 	frame_counter;
camera_data* cbWriteUsing(CircularBuffer *cb) 
{
	__IO int temp_index=cb->end_using&(cb->size-1);
#if defined(LOG_SW)
		//printf("����ʹ��д������ָ�� %d\r\n",temp_index);
	printf("using buffer %d   frame %d   \r\n",temp_index,frame_counter ++);
#endif 			
	return  cb->elems[temp_index];
}

/**
  * @brief ����д����ϣ�����дָ�뵽����ṹ��
  * @param  cb:������нṹ��
  */
void cbWriteFinish(CircularBuffer *cb)
{
    cb->end = cb->end_using;
}
 
/**
  * @brief  ��ȡ�ɶ�ȡ�Ļ�����ָ��
  * @param  cb:������нṹ��
  * @return  �ɽ��ж�ȡ�Ļ�����ָ��
  * @note  �õ�ָ���ɽ����ȡ����������ָ�벻��������1����ȡ������ʱ��Ӧ����cbReadFinish
  */
camera_data* cbRead(CircularBuffer *cb) 
{
		if(cbIsEmpty(cb))
			return NULL;

		cb->start_using = cbIncr(cb, cb->start);
		
		__IO int temp_index=cb->start_using&(cb->size-1);		
#if defined(LOG_SW)
		//printf("�ɽ��ж�ȡ��ָ�� %d\r\n",temp_index);
		printf("cbRead  %d\r\n",temp_index);
#endif 			
		return cb->elems[temp_index];
}


/**
  * @brief ���ݶ�ȡ��ϣ����¶�ָ�뵽����ṹ��
  * @param  cb:������нṹ��
  */
void cbReadFinish(CircularBuffer *cb) 
{
    cb->start = cb->start_using;
}






//����ͷ���е�ָ��ָ��Ļ�����ȫ������
void camera_queue_free(void)
{
    uint32_t i = 0;

    for(i = 0; i < CAMERA_QUEUE_NUM; i ++)
    {
        if(cam_data[i].head != NULL)
        {
            free(cam_data[i].head);
            cam_data[i].head = NULL;
        }
    }

    return;
}

//������г�ʼ���������ڴ�
int32_t camera_queue_init(void)
{
  uint32_t i = 0;

  memset(cam_data, 0, sizeof(cam_data));
		
	/*��ʼ���������*/
	cbInit(&cam_circular_buff,CAMERA_QUEUE_NUM);

    for(i = 0; i < CAMERA_QUEUE_NUM; i ++)
    {
        cam_data[i].head = queue_buff[i];//cam_global_buf+CAMERA_QUEUE_DATA_LEN*i;
        
        /*��ʼ�����л���ָ�룬ָ��ʵ�ʵ��ڴ�*/
        cam_circular_buff.elems[i] = &cam_data[i];
        
        printf("cam_data[i].head=0x%x,cam_circular_buff.elems[i] =0x%x \r\n", (uint32_t)cam_data[i].head,(uint32_t)cam_circular_buff.elems[i]->head);

        memset(cam_data[i].head, 0, CAMERA_QUEUE_DATA_LEN);
    }

    return kNoErr;
}




__IO int get_data_num=0;
//�Ӷ�����ȡ����, data_p:��ʼ��ַ, len_p:����, array_index:�±�
int32_t pull_data_from_queue(uint8_t **data_p, int32_t *len_p)
{
	
		uint8_t jpeg_start_offset = 0;	
		camera_data *cam_data_pull;	
		
		if(!cbIsEmpty(&cam_circular_buff))//������зǿ�
		{
				/*�ӻ�������ȡ���ݣ����д�����*/
				cam_data_pull = cbRead(&cam_circular_buff);
				
				if (cam_data_pull == NULL)
						return kGeneralErr;

			/*�����ļ�*/	
				
				*len_p = find_jpeg_tail(cam_data_pull->head,&jpeg_start_offset,cam_data_pull->img_dma_len*2/3);
				
				if(*len_p != -1)
				{
						get_data_num++;//��ȡ���ݼ�����
						*data_p = cam_data_pull->head + jpeg_start_offset;
						return kNoErr;
				}else
				{
				}		
		}

    return kGeneralErr;
}

 


/**
  * @brief  ����jpeg�ļ�ͷ���ļ�β
  * @param  data:������ָ��
	* @param  jpeg_start:����jpeg�ļ�ͷ����������ƫ��
	* @param  search_point:����jpeg�ļ�βʱ������������������ʼλ��
  */

#define JPEG_HEAD_SEARCH_MAX 	10000

int32_t find_jpeg_tail(uint8_t *data,uint8_t *jpeg_start,int32_t search_point) 
{
    uint32_t i = 0,j=0;//,z=0;
    __IO uint8_t *p = data;
	
	/*Ĭ���ļ�ͷλ��*/
	*jpeg_start = 0;

	for(j = 0;j<JPEG_HEAD_SEARCH_MAX;j++)
	{
		if(! ((*p == 0xFF) && (*(p + 1) == 0xD8)) ) //������֡ͷ
		{
			p++;
		}
    else if((*p == 0xFF) && (*(p + 1) == 0xD8)) //֡ͷ�ж�
    {
//			printf("jpeg֡ͷ->0xFF--0xD8\r\n");
			if(j!=0)//����ڴ���ʼ������Ƭͷ���ͽ�ָ��ƫ��
			{
				//camera_data_queue_log("JPEG_HEAD_SEARCH_MAX *p=0x%x,j=%d", *p,j );
				/*�ļ�ͷʵ��ƫ��*/
				*jpeg_start=j;
			}

			/*�������*/
			p += search_point;
			for(i = search_point ; i < CAMERA_QUEUE_DATA_LEN - 2; i ++)
        {

            if((*p == 0xFF) && (*(p + 1) == 0xD8))//�ٴ��ж�
            {
                if(i == 0)
                {
                    p++;
                    continue;
                }else
                {
                    return -1;
                }
            }
						else if((*p == 0xFF) && (*(p + 1) == 0xD9))
            {//�ҵ�ͼƬβ
//								camera_data_queue_log("pic len = %d", i+2 );
								//printf("jpeg֡β->0xFF--0xD9\r\n");
                return  i + 2;
            }

            p++;
        }  	
				
				camera_data_queue_log("picture tail error!");
				return -1;				
    }
	}	
	
  camera_data_queue_log("picture head error!");	
	
  return -1;
}



/**
  * @brief  ����jpeg�ļ�ͷ���ļ�β
  * @param  data:������ָ��
	*	@return �����ҵ��ļ���kNoErr���쳣:kGeneralErr
  */
int32_t find_jpeg(camera_data *cambuf) 
{
	uint32_t i = 0,j=0;
	
	uint8_t *p = cambuf->head;	
	
	/*�����ļ�β���*/
	uint32_t end_search_point = cambuf->img_dma_len*99/100;
	
	/*����*/
	cambuf->img_real_start = NULL;	
	cambuf->img_real_len = -1;

	for(j = 0;j<JPEG_HEAD_SEARCH_MAX;j++)
	{
		if(! ((*p == 0xFF) && (*(p + 1) == 0xD8)) ) //������֡ͷ
		{
			p++;
		}
    else if((*p == 0xFF) && (*(p + 1) == 0xD8)) //֡ͷ�ж�
    {
			if(j!=0)			
			{	
				/*�ļ�ͷʵ��ƫ��*/
				//camera_data_queue_log("JPEG_HEAD_SEARCH *p=0x%x,j=%d", *p,j );
			}
			
				/*�ļ�ͷʵ��ָ��*/
				cambuf->img_real_start = p;

				/*�ļ�β�������*/
				p += end_search_point;
			
        for(i = end_search_point ; i < CAMERA_QUEUE_DATA_LEN - 2; i ++)
        {
            if((*p == 0xFF) && (*(p + 1) == 0xD8))
            {
                if(i == 0)
                {
                    p++;
                    continue;
                }else
                {
                    return kGeneralErr;
                }
            }else if((*p == 0xFF) && (*(p + 1) == 0xD9))
            {
								cambuf->img_real_len = i+2;
                return  kNoErr;
            }

            p++;
        }  	
				
				camera_data_queue_log("picture tail error!");
				return kGeneralErr;				
    }
	}

	
  camera_data_queue_log("picture head error!");	
	
  return kGeneralErr;
}




