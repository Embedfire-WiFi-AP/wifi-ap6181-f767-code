#include "cammera_middleware.h"

#include "camera_data_queue.h"
#include "wifi_base_config.h"
#include "stm32f7xx.h"
#include "stm32f7xx_hal.h"

uint32_t frame_counter = 0;

void DMA_Cmd(DMA_Stream_TypeDef* DMAy_Streamx, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_DMA_ALL_PERIPH(DMAy_Streamx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Enable the selected DMAy Streamx by setting EN bit */
    DMAy_Streamx->CR |= (uint32_t)DMA_SxCR_EN;
  }
  else
  {
    /* Disable the selected DMAy Streamx by clearing EN bit */
    DMAy_Streamx->CR &= ~(uint32_t)DMA_SxCR_EN;
  }
}





FunctionalState DMA_GetCmdStatus(DMA_Stream_TypeDef* DMAy_Streamx)
{
  FunctionalState state = DISABLE;

  /* Check the parameters */
  assert_param(IS_DMA_ALL_PERIPH(DMAy_Streamx));

  if ((DMAy_Streamx->CR & (uint32_t)DMA_SxCR_EN) != 0)
  {
    /* The selected DMAy Streamx EN bit is set (DMA is still transferring) */
    state = ENABLE;
  }
  else
  {
    /* The selected DMAy Streamx EN bit is cleared (DMA is disabled and 
        all transfers are complete) */
    state = DISABLE;
  }
  return state;
}


/**
  * @brief  Returns the number of remaining data units in the current DMAy Streamx transfer.
  * @param  DMAy_Streamx: where y can be 1 or 2 to select the DMA and x can be 0
  *          to 7 to select the DMA Stream.
  * @retval The number of remaining data units in the current DMAy Streamx transfer.
  */
uint16_t DMA_GetCurrDataCounter(DMA_Stream_TypeDef* DMAy_Streamx)
{
  /* Check the parameters */
  assert_param(IS_DMA_ALL_PERIPH(DMAy_Streamx));

  /* Return the number of remaining data units for DMAy Streamx */
  return ((uint16_t)(DMAy_Streamx->NDTR));
}




extern DCMI_HandleTypeDef DCMI_Handle;
extern DMA_HandleTypeDef DMA_Handle_dcmi;


void DCMI_Start(void)
{
	
	camera_data *data_p;
	
	/*��ȡд������ָ�룬׼��д��������*/
	data_p = cbWrite(&cam_circular_buff);
	
	if (data_p != NULL)	//���������δ������ʼ����
	{				
		HAL_DCMI_Stop(&DCMI_Handle) ;
		/*����DMA����*/
		HAL_DCMI_Start_DMA(&DCMI_Handle, DCMI_MODE_CONTINUOUS, (uint32_t )data_p->head, CAMERA_QUEUE_DATA_LEN);

	}

		
}

__IO int testview=0;
void DCMI_Stop(void)
{	

	camera_data *data_p;
	
	/*�ر�dma*/
	DMA_Cmd(DMA2_Stream1,DISABLE);
	while(DMA_GetCmdStatus(DMA2_Stream1) != DISABLE){}

	/*��ȡ���ڲ�����дָ��*/	
	data_p = cbWriteUsing(&cam_circular_buff);
	
	/*����dma��������ݸ��������ڼ���jpeg�����ļ�β��ʱ��*/	
	if (data_p != NULL)	
	{
		data_p->img_dma_len =0; //��λ	
		data_p->img_dma_len = (DCMI_Handle.XferSize - DMA_GetCurrDataCounter(DMA2_Stream1))*4; //���һ���� __HAL_DMA_GET_COUNTER			
	}
	
	/*д�뻺�������*/
	cbWriteFinish(&cam_circular_buff);
}



/*֡�ж�ʵ��*/
void DCMI_IRQHandler_Funtion(void)
{
	frame_counter ++;
	//1.ֹͣDCMI����
	DCMI_Stop();
	//2.���ݻ�����ʹ����������Ƿ���dma
	DCMI_Start();
	
}


