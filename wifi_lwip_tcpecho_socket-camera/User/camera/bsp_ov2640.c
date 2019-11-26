/**
  ******************************************************************************
  * @file    bsp_sdram.c
  * @author  fire
  * @version V1.0
  * @date    2016-xx-xx
  * @brief   OV2640摄像头驱动
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 STM32F767 开发板  
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */
  

/* Includes ------------------------------------------------------------------*/
#include "./camera/bsp_ov2640.h"
#include "./i2c/bsp_i2c.h"

#include "camera_data_queue.h"
#include "wifi_base_config.h"
#include "cammera_middleware.h"
#include "./delay/core_delay.h"   

/** @addtogroup DCMI_Camera
  * @{
  */ 
DCMI_HandleTypeDef DCMI_Handle;
DMA_HandleTypeDef DMA_Handle_dcmi;

extern uint8_t fps;

const char OV2640_QQVGA[][2]=
{
  0xff, 0x00,
  0x2c, 0xff,
  0x2e, 0xdf,
  0xff, 0x01,
  0x3c, 0x32,
  0x11, 0x00,
  0x09, 0x02,
  0x03, 0xcf,
  0x04, 0x08,
  0x13, 0xe5,
  0x14, 0x48,
  0x2c, 0x0c,
  0x33, 0x78,
  0x3a, 0x33,
  0x3b, 0xfb,
  0x3e, 0x00,
  0x43, 0x11,
  0x16, 0x10,
  0x39, 0x02,
  0x35, 0x88,
  0x22, 0x0a,
  0x37, 0x40,
  0x23, 0x00,
  0x34, 0xa0,
  0x36, 0x1a,
  0x06, 0x02,
  0x07, 0xc0,
  0x0d, 0xb7,
  0x0e, 0x01,
  0x4c, 0x00,
  0x4a, 0x81,
  0x21, 0x99,
  0x24, 0x3a,
  0x25, 0x32,
  0x26, 0x82,
  0x5c, 0x00,
  0x63, 0x00,
  0x5d, 0x55,
  0x5e, 0x7d,
  0x5f, 0x7d,
  0x60, 0x55,
  0x61, 0x70,
  0x62, 0x80,
  0x7c, 0x05,
  0x20, 0x80,
  0x28, 0x30,
  0x6c, 0x00,
  0x6d, 0x80,
  0x6e, 0x00,
  0x70, 0x02,
  0x71, 0x96,
  0x73, 0xe1,
  0x3d, 0x34,
  0x5a, 0x57,
  0x4f, 0xbb,
  0x50, 0x9c,
  0x0f, 0x43,
  0xff, 0x00,
  0xe5, 0x7f,
  0xf9, 0xc0,
  0x41, 0x24,
  0xe0, 0x14,
  0x76, 0xff,
  0x33, 0xa0,
  0x42, 0x20,
  0x43, 0x18,
  0x4c, 0x00,
  0x87, 0xd0,
  0x88, 0x3f,
  0xd7, 0x03,
  0xd9, 0x10,
  0xd3, 0x82,
  0xc8, 0x08,
  0xc9, 0x80,
  0x7c, 0x00,
  0x7d, 0x02,
  0x7c, 0x03,
  0x7d, 0x48,
  0x7d, 0x48,
  0x7c, 0x08,
  0x7d, 0x20,
  0x7d, 0x10,
  0x7d, 0x0e,
  0x90, 0x00,
  0x91, 0x0e,
  0x91, 0x1a,
  0x91, 0x31,
  0x91, 0x5a,
  0x91, 0x69,
  0x91, 0x75,
  0x91, 0x7e,
  0x91, 0x88,
  0x91, 0x8f,
  0x91, 0x96,
  0x91, 0xa3,
  0x91, 0xaf,
  0x91, 0xc4,
  0x91, 0xd7,
  0x91, 0xe8,
  0x91, 0x20,
  0x92, 0x00,
  0x93, 0x06,
  0x93, 0xe3,
  0x93, 0x05,
  0x93, 0x05,
  0x93, 0x00,
  0x93, 0x00,
  0x93, 0x00,
  0x93, 0x00,
  0x93, 0x00,
  0x93, 0x00,
  0x93, 0x00,
  0x93, 0x00,
  0x93, 0x00,
  0x96, 0x00,
  0x97, 0x08,
  0x97, 0x19,
  0x97, 0x02,
  0x97, 0x0c,
  0x97, 0x24,
  0x97, 0x30,
  0x97, 0x28,
  0x97, 0x26,
  0x97, 0x02,
  0x97, 0x98,
  0x97, 0x80,
  0x97, 0x00,
  0x97, 0x00,
  0xc3, 0xed,
  0xa4, 0x00,
  0xa8, 0x00,
  0xbf, 0x00,
  0xba, 0xf0,
  0xbc, 0x64,
  0xbb, 0x02,
  0xb6, 0x3d,
  0xb8, 0x57,
  0xb7, 0x38,
  0xb9, 0x4e,
  0xb3, 0xe8,
  0xb4, 0xe1,
  0xb5, 0x66,
  0xb0, 0x67,
  0xb1, 0x5e,
  0xb2, 0x04,
  0xc7, 0x00,
  0xc6, 0x51,
  0xc5, 0x11,
  0xc4, 0x9c,
  0xcf, 0x02,
  0xa6, 0x00,
  0xa7, 0xe0,
  0xa7, 0x10,
  0xa7, 0x1e,
  0xa7, 0x21,
  0xa7, 0x00,
  0xa7, 0x28,
  0xa7, 0xd0,
  0xa7, 0x10,
  0xa7, 0x16,
  0xa7, 0x21,
  0xa7, 0x00,
  0xa7, 0x28,
  0xa7, 0xd0,
  0xa7, 0x10,
  0xa7, 0x17,
  0xa7, 0x21,
  0xa7, 0x00,
  0xa7, 0x28,
  0xc0, 0xc8,
  0xc1, 0x96,
  0x86, 0x1d,
  0x50, 0x00,
  0x51, 0x90,
  0x52, 0x18,
  0x53, 0x00,
  0x54, 0x00,
  0x55, 0x88,
  0x57, 0x00,
  0x5a, 0x90,
  0x5b, 0x18,
  0x5c, 0x05,
  0xc3, 0xef,
  0x7f, 0x00,
  0xda, 0x00,
  0xe5, 0x1f,
  0xe1, 0x67,
  0xe0, 0x00,
  0xdd, 0xff,
  0x05, 0x00,
  0xff, 0x01,
  0xff, 0x01,
  0x12, 0x00,
  0x17, 0x11,
  0x18, 0x75,
  0x19, 0x01,
  0x1a, 0x97,
  0x32, 0x36,
  0x4f, 0xbb,
  0x6d, 0x80,
  0x3d, 0x34,
  0x39, 0x02,
  0x35, 0x88,
  0x22, 0x0a,
  0x37, 0x40,
  0x23, 0x00,
  0x34, 0xa0,
  0x36, 0x1a,
  0x06, 0x02,
  0x07, 0xc0,
  0x0d, 0xb7,
  0x0e, 0x01,
  0x4c, 0x00,
  0xff, 0x00,
  0xe0, 0x04,
  0x8c, 0x00,
  0x87, 0xd0,
  0xe0, 0x00,
  0xff, 0x00,
  0xe0, 0x14,
  0xe1, 0x77,
  0xe5, 0x1f,
  0xd7, 0x03,
  0xda, 0x10,
  0xe0, 0x00,
  0xff, 0x00,
  0xe0, 0x04,
  0xc0, 0xc8,
  0xc1, 0x96,
  0x86, 0x1d,
  0x50, 0x00,
  0x51, 0x90,
  0x52, 0x2c,
  0x53, 0x00,
  0x54, 0x00,
  0x55, 0x88,
  0x57, 0x00,
  0x5a, 0x90,
  0x5b, 0x2c,
  0x5c, 0x05,
  0xe0, 0x00,
  0xd3, 0x04,
  0xff, 0x00,
  0xc3, 0xef,
  0xa6, 0x00,
  0xa7, 0xdd,
  0xa7, 0x78,
  0xa7, 0x7e,
  0xa7, 0x24,
  0xa7, 0x00,
  0xa7, 0x25,
  0xa6, 0x06,
  0xa7, 0x20,
  0xa7, 0x58,
  0xa7, 0x73,
  0xa7, 0x34,
  0xa7, 0x00,
  0xa7, 0x25,
  0xa6, 0x0c,
  0xa7, 0x28,
  0xa7, 0x58,
  0xa7, 0x6d,
  0xa7, 0x34,
  0xa7, 0x00,
  0xa7, 0x25,
  0xff, 0x00,
  0xe0, 0x04,
  0xe1, 0x67,
  0xe5, 0x1f,
  0xd7, 0x01,
  0xda, 0x08,
  0xda, 0x09,
  0xe0, 0x00,
  0x98, 0x00,
  0x99, 0x00,
  0xff, 0x01,
  0x04, 0x28,
  0xff, 0x01,
  0x12, 0x40,
  0x17, 0x11,
  0x18, 0x43,
  0x19, 0x00,
  0x1a, 0x4b,
  0x32, 0x09,
  0x4f, 0xca,
  0x50, 0xa8,
  0x5a, 0x23,
  0x6d, 0x00,
  0x39, 0x12,
  0x35, 0xda,
  0x22, 0x1a,
  0x37, 0xc3,
  0x23, 0x00,
  0x34, 0xc0,
  0x36, 0x1a,
  0x06, 0x88,
  0x07, 0xc0,
  0x0d, 0x87,
  0x0e, 0x41,
  0x4c, 0x00,
  0xff, 0x00,
  0xe0, 0x04,
  0xc0, 0x64,
  0xc1, 0x4b,
  0x86, 0x35,
  0x50, 0x92,
  0x51, 0xc8,
  0x52, 0x96,
  0x53, 0x00,
  0x54, 0x00,
  0x55, 0x00,
  0x57, 0x00,
  0x5a, 0x28,
  0x5b, 0x1e,
  0x5c, 0x00,
  0xe0, 0x00,
  0xff, 0x01,
  0x11, 0x00,
  0x3d, 0x38,
  0x2d, 0x00,
  0x50, 0x65,
  0xff, 0x00,
  0xd3, 0x04,
  0x7c, 0x00,
  0x7d, 0x04,
  0x7c, 0x09,
  0x7d, 0x28,
  0x7d, 0x00,
};

/* QVGA 320x240 */
const unsigned char OV2640_QVGA[][2]=
{
  0xff, 0x00,
  0x2c, 0xff,
  0x2e, 0xdf,
  0xff, 0x01,
  0x3c, 0x32,
  0x11, 0x00,
  0x09, 0x02,
  0x04, 0xd8,
  0x13, 0xe5,
  0x14, 0x48,
  0x2c, 0x0c,
  0x33, 0x78,
  0x3a, 0x33,
  0x3b, 0xfB,
  0x3e, 0x00,
  0x43, 0x11,
  0x16, 0x10,
  0x4a, 0x81,
  0x21, 0x99,
  0x24, 0x40,
  0x25, 0x38,
  0x26, 0x82,
  0x5c, 0x00,
  0x63, 0x00,
  0x46, 0x3f,
  0x0c, 0x3c,
  0x61, 0x70,
  0x62, 0x80,
  0x7c, 0x05,
  0x20, 0x80,
  0x28, 0x30,
  0x6c, 0x00,
  0x6d, 0x80,
  0x6e, 0x00,
  0x70, 0x02,
  0x71, 0x94,
  0x73, 0xc1,
  0x3d, 0x34,
  0x5a, 0x57,
  0x12, 0x00,
  0x11, 0x00,
  0x17, 0x11,
  0x18, 0x75,
  0x19, 0x01,
  0x1a, 0x97,
  0x32, 0x36,
  0x03, 0x0f,
  0x37, 0x40,
  0x4f, 0xbb,
  0x50, 0x9c,
  0x5a, 0x57,
  0x6d, 0x80,
  0x6d, 0x38,
  0x39, 0x02,
  0x35, 0x88,
  0x22, 0x0a,
  0x37, 0x40,
  0x23, 0x00,
  0x34, 0xa0,
  0x36, 0x1a,
  0x06, 0x02,
  0x07, 0xc0,
  0x0d, 0xb7,
  0x0e, 0x01,
  0x4c, 0x00,
  0xff, 0x00,
  0xe5, 0x7f,
  0xf9, 0xc0,
  0x41, 0x24,
  0xe0, 0x14,
  0x76, 0xff,
  0x33, 0xa0,
  0x42, 0x20,
  0x43, 0x18,
  0x4c, 0x00,
  0x87, 0xd0,
  0x88, 0x3f,
  0xd7, 0x03,
  0xd9, 0x10,
  0xd3, 0x82,
  0xc8, 0x08,
  0xc9, 0x80,
  0x7d, 0x00,
  0x7c, 0x03,
  0x7d, 0x48,
  0x7c, 0x08,
  0x7d, 0x20,
  0x7d, 0x10,
  0x7d, 0x0e,
  0x90, 0x00,
  0x91, 0x0e,
  0x91, 0x1a,
  0x91, 0x31,
  0x91, 0x5a,
  0x91, 0x69,
  0x91, 0x75,
  0x91, 0x7e,
  0x91, 0x88,
  0x91, 0x8f,
  0x91, 0x96,
  0x91, 0xa3,
  0x91, 0xaf,
  0x91, 0xc4,
  0x91, 0xd7,
  0x91, 0xe8,
  0x91, 0x20,
  0x92, 0x00,
  0x93, 0x06,
  0x93, 0xe3,
  0x93, 0x02,
  0x93, 0x02,
  0x93, 0x00,
  0x93, 0x04,
  0x93, 0x00,
  0x93, 0x03,
  0x93, 0x00,
  0x93, 0x00,
  0x93, 0x00,
  0x93, 0x00,
  0x93, 0x00,
  0x93, 0x00,
  0x93, 0x00,
  0x96, 0x00,
  0x97, 0x08,
  0x97, 0x19,
  0x97, 0x02,
  0x97, 0x0c,
  0x97, 0x24,
  0x97, 0x30,
  0x97, 0x28,
  0x97, 0x26,
  0x97, 0x02,
  0x97, 0x98,
  0x97, 0x80,
  0x97, 0x00,
  0x97, 0x00,
  0xc3, 0xef,
  0xff, 0x00,
  0xba, 0xdc,
  0xbb, 0x08,
  0xb6, 0x24,
  0xb8, 0x33,
  0xb7, 0x20,
  0xb9, 0x30,
  0xb3, 0xb4,
  0xb4, 0xca,
  0xb5, 0x43,
  0xb0, 0x5c,
  0xb1, 0x4f,
  0xb2, 0x06,
  0xc7, 0x00,
  0xc6, 0x51,
  0xc5, 0x11,
  0xc4, 0x9c,
  0xbf, 0x00,
  0xbc, 0x64,
  0xa6, 0x00,
  0xa7, 0x1e,
  0xa7, 0x6b,
  0xa7, 0x47,
  0xa7, 0x33,
  0xa7, 0x00,
  0xa7, 0x23,
  0xa7, 0x2e,
  0xa7, 0x85,
  0xa7, 0x42,
  0xa7, 0x33,
  0xa7, 0x00,
  0xa7, 0x23,
  0xa7, 0x1b,
  0xa7, 0x74,
  0xa7, 0x42,
  0xa7, 0x33,
  0xa7, 0x00,
  0xa7, 0x23,
  0xc0, 0xc8,
  0xc1, 0x96,
  0x8c, 0x00,
  0x86, 0x3d,
  0x50, 0x92,
  0x51, 0x90,
  0x52, 0x2c,
  0x53, 0x00,
  0x54, 0x00,
  0x55, 0x88,
  0x5a, 0x50,
  0x5b, 0x3c,
  0x5c, 0x00,
  0xd3, 0x04,
  0x7f, 0x00,
  0xda, 0x00,
  0xe5, 0x1f,
  0xe1, 0x67,
  0xe0, 0x00,
  0xdd, 0x7f,
  0x05, 0x00,
  0xff, 0x00,
  0xe0, 0x04,
  0xc0, 0xc8,
  0xc1, 0x96,
  0x86, 0x3d,
  0x50, 0x92,
  0x51, 0x90,
  0x52, 0x2c,
  0x53, 0x00,
  0x54, 0x00,
  0x55, 0x88,
  0x57, 0x00,
  0x5a, 0x50,
  0x5b, 0x3c,
  0x5c, 0x00,
  0xd3, 0x04,
  0xe0, 0x00,
  0xFF, 0x00,
  0x05, 0x00,
  0xDA, 0x08,
  0xda, 0x09,
  0x98, 0x00,
  0x99, 0x00,
  0x00, 0x00,
};

/* JPG 160x120 */
const unsigned char OV2640_160x120_JPEG[][2]=
{
  0xff, 0x01,
  0x12, 0x40,
  0x17, 0x11,
  0x18, 0x43,
  0x19, 0x00,
  0x1a, 0x4b,
  0x32, 0x09,
  0x4f, 0xca,
  0x50, 0xa8,
  0x5a, 0x23,
  0x6d, 0x00,
  0x39, 0x12,
  0x35, 0xda,
  0x22, 0x1a,
  0x37, 0xc3,
  0x23, 0x00,
  0x34, 0xc0,
  0x36, 0x1a,
  0x06, 0x88,
  0x07, 0xc0,
  0x0d, 0x87,
  0x0e, 0x41,
  0x4c, 0x00,
  0xff, 0x00,
  0xe0, 0x04,
  0xc0, 0x64,
  0xc1, 0x4b,
  0x86, 0x35,
  0x50, 0x92,
  0x51, 0xc8,
  0x52, 0x96,
  0x53, 0x00,
  0x54, 0x00,
  0x55, 0x00,
  0x57, 0x00,
  0x5a, 0x28,
  0x5b, 0x1e,
  0x5c, 0x00,
  0xe0, 0x00,
};

/* JPG, 0x176x144 */
const unsigned char OV2640_176x144_JPEG[][2]=
{
  0xff, 0x01,
  0x12, 0x40,
  0x17, 0x11,
  0x18, 0x43,
  0x19, 0x00,
  0x1a, 0x4b,
  0x32, 0x09,
  0x4f, 0xca,
  0x50, 0xa8,
  0x5a, 0x23,
  0x6d, 0x00,
  0x39, 0x12,
  0x35, 0xda,
  0x22, 0x1a,
  0x37, 0xc3,
  0x23, 0x00,
  0x34, 0xc0,
  0x36, 0x1a,
  0x06, 0x88,
  0x07, 0xc0,
  0x0d, 0x87,
  0x0e, 0x41,
  0x4c, 0x00,
  0xff, 0x00,
  0xe0, 0x04,
  0xc0, 0x64,
  0xc1, 0x4b,
  0x86, 0x35,
  0x50, 0x92,
  0x51, 0xc8,
  0x52, 0x96,
  0x53, 0x00,
  0x54, 0x00,
  0x55, 0x00,
  0x57, 0x00,
  0x5a, 0x2c,
  0x5b, 0x24,
  0x5c, 0x00,
  0xe0, 0x00,
};

/* JPG 320x240 */
const unsigned char OV2640_320x240_JPEG[][2]=
{
  0xff, 0x01,
  0x11, 0x00, //ê±?ó
  0x12, 0x40,
  0x17, 0x11,
//	0x04, 0x20|0xC0,	//′1?±·-×a￡?????·-×a
  0x18, 0x43,
  0x19, 0x00,
  0x1a, 0x4b,
  0x32, 0x09,
  0x4f, 0xca,
  0x50, 0xa8,
  0x5a, 0x23,
  0x6d, 0x00,
  0x39, 0x12,
  0x35, 0xda,
  0x22, 0x1a,
  0x37, 0xc3,
  0x23, 0x00,
  0x34, 0xc0,
  0x36, 0x1a,
  0x3d,  0x34,	//
  0x06, 0x88,
  0x07, 0xc0,
  0x0d, 0x87,
  0x0e, 0x41,
  0x4c, 0x00,
  0xff, 0x00,
  0xe0, 0x04,
  0xc0, 0x64,
  0xc1, 0x4b,
  0x86, 0x35,
  0x50, 0x89,
  0x51, 0xc8,
  0x52, 0x96,
  0x53, 0x00,
  0x54, 0x00,
  0x55, 0x00,
  0x57, 0x00,
  0x5a, 0x50,
  0x5b, 0x3c,
  0x5c, 0x00,
  0xe0, 0x00,
};

/* JPG 352x288 */
const unsigned char OV2640_352x288_JPEG[][2]=
{
  0xff, 0x01,
  0x11,  0x00, //ê±?ó
  0x12, 0x40,
  0x17, 0x11,
  0x18, 0x43,
  0x19, 0x00,
  0x1a, 0x4b,
  0x32, 0x09,
  0x4f, 0xca,
  0x50, 0xa8,
  0x5a, 0x23,
  0x6d, 0x00,
  0x39, 0x12,
  0x35, 0xda,
  0x22, 0x1a,
  0x37, 0xc3,
  0x23, 0x00,
  0x34, 0xc0,
  0x36, 0x1a,
  0x3d,  0x34,	//
  0x06, 0x88,
  0x07, 0xc0,
  0x0d, 0x87,
  0x0e, 0x41,
  0x4c, 0x00,
  0xff, 0x00,
  0xe0, 0x04,
  0xc0, 0x64,
  0xc1, 0x4b,
  0x86, 0x35,
  0x50, 0x89,
  0x51, 0xc8,
  0x52, 0x96,
  0x53, 0x00,
  0x54, 0x00,
  0x55, 0x00,
  0x57, 0x00,
  0x5a, 0x58,
  0x5b, 0x48,
  0x5c, 0x00,
  0xe0, 0x00,
};


/* JPG, 0x640x480 */
const u8 OV2640_640x480_JPEG[][2]=
{
   0xff,  0x01,
   0x11,  0x00,
   0x12,  0x00,
   0x17,  0x11,
   0x18,  0x75,
   0x32,  0x09, //0x36:uxvga
   0x19,  0x01,
   0x1a,  0x97,
   0x03,  0x0f,
   0x37,  0x40,
   0x4f,  0xbb,
   0x50,  0x9c,
   0x5a,  0x57,
   0x6d,  0x80,
   0x3d,  0x30,
   0x39,  0x02,
   0x35,  0x88,
   0x22,  0x0a,
   0x37,  0x40,
   0x34,  0xa0,
   0x06,  0x02,
   0x0d,  0xb7,
   0x0e,  0x01,

   0xff,  0x00,
   0xe0,  0x04,
   0xc0,  0xc8,
   0xc1,  0x96,
   0x86,  0x3d,
   0x50,  0x89,
   0x51,  0x90,
   0x52,  0x2c,
   0x53,  0x00,
   0x54,  0x00,
   0x55,  0x88,
   0x57,  0x00,
   0x5a,  0xa0,
   0x5b,  0x78,
   0x5c,  0x00,
   0xd3,  0x02,
   0xe0,  0x00,
};

/* JPG, 0x800x600 */
const u8 OV2640_800x600_JPEG[][2]=
{
   0xff,  0x01,
   0x11,  0x00,
   0x12,  0x00,
   0x17,  0x11,
   0x18,  0x75,
   0x32,  0x36,
   0x19,  0x01,
   0x1a,  0x97,
   0x03,  0x0f,
   0x37,  0x40,
   0x4f,  0xbb,
   0x50,  0x9c,
   0x5a,  0x57,
   0x6d,  0x80,
   0x3d,  0x34,
   0x39,  0x02,
   0x35,  0x88,
   0x22,  0x0a,
   0x37,  0x40,
   0x34,  0xa0,
   0x06,  0x02,
   0x0d,  0xb7,
   0x0e,  0x01,

   0xff,  0x00,
   0xe0,  0x04,
   0xc0,  0xc8,
   0xc1,  0x96,
   0x86,  0x35,
   0x50,  0x89,
   0x51,  0x90,
   0x52,  0x2c,
   0x53,  0x00,
   0x54,  0x00,
   0x55,  0x88,
   0x57,  0x00,
   0x5a,  0xc8,
   0x5b,  0x96,
   0x5c,  0x00,
   0xd3,  0x02,
   0xe0,  0x00,
};

/* JPG, 0x1024x768 */
const u8 OV2640_1024x768_JPEG[][2]=
{
   0xff,  0x01,
   0x11,  0x00,	//ê±?ó￡?ì??ìá??áμ????óê?2?è?
   0x12,  0x00,
   0x17,  0x11,
   0x18,  0x75,
   0x32,  0x36,
   0x19,  0x01,
   0x1a,  0x97,
   0x03,  0x0f,
   0x37,  0x40,
   0x4f,  0xbb,
   0x50,  0x9c,
   0x5a,  0x57,
   0x6d,  0x80,
//   0x3d,  0x34,
   0x39,  0x02,
   0x35,  0x88,
   0x22,  0x0a,
   0x37,  0x40,
   0x34,  0xa0,
   0x06,  0x02,
   0x0d,  0xb7,
   0x0e,  0x01,

   0xff,  0x00,
   0xc0,  0xC8,
   0xc1,  0x96,
   0x8c,  0x00,
   0x86,  0x3D,
   0x50,  0x00,
   0x51,  0x90,
   0x52,  0x2C,
   0x53,  0x00,
   0x54,  0x00,
   0x55,  0x88,
   0x5a,  0x00,
   0x5b,  0xC0,
   0x5c,  0x01,
   0xd3,  0x02,
};

const u8 OV2640_JPEG[][2]=
{
  0xe0, 0x14,
  0xe1, 0x77,
  0xe5, 0x1f,
  0xd7, 0x03,
  0xda, 0x10,
  0xe0, 0x00,
  0xFF, 0x01,
  0x04, 0xd8,
};

const u8 OV2640_YUV422[][2]=
{
  0xFF, 0x00,
  0x05, 0x00,
  0xDA, 0x10,
  0xD7, 0x03,
  0xDF, 0x00,
  0x33, 0x80,
  0x3C, 0x40,
  0xe1, 0x77,
  0x00, 0x00,
};

const u8 OV2640_JPEG_INIT[][2]=
{
  0xff, 0x00,
  0x2c, 0xff,
  0x2e, 0xdf,
  0xff, 0x01,
  0x3c, 0x32,
  0x11, 0x00,
  0x09, 0x02,
  0x04, 0xd8,
  0x13, 0xe5,
  0x14, 0x48,
  0x2c, 0x0c,
  0x33, 0x78,
  0x3a, 0x33,
  0x3b, 0xfB,
  0x3e, 0x00,
  0x43, 0x11,
  0x16, 0x10,
  0x39, 0x92,
  0x35, 0xda,
  0x22, 0x1a,
  0x37, 0xc3,
  0x23, 0x00,
  0x34, 0xc0,
  0x36, 0x1a,
  0x06, 0x88,
  0x07, 0xc0,
  0x0d, 0x87,
  0x0e, 0x41,
  0x4c, 0x00,
  0x48, 0x00,
  0x5B, 0x00,
  0x42, 0x03,
  0x4a, 0x81,
  0x21, 0x99,
  0x24, 0x40,
  0x25, 0x38,
  0x26, 0x82,
  0x5c, 0x00,
  0x63, 0x00,
  0x61, 0x70,
  0x62, 0x80,
  0x7c, 0x05,
  0x20, 0x80,
  0x28, 0x30,
  0x6c, 0x00,
  0x6d, 0x80,
  0x6e, 0x00,
  0x70, 0x02,
  0x71, 0x94,
  0x73, 0xc1,
  0x12, 0x40,//0x40
  0x17, 0x11,
  0x18, 0x43,
  0x19, 0x00,
  0x1a, 0x4b,
  0x32, 0x09,
  0x37, 0xc0,
  0x4f, 0x60,
  0x50, 0xa8,
  0x6d, 0x00,
  0x3d, 0x38,
  0x46, 0x3f,
  0x4f, 0x60,
  0x0c, 0x3c,
  0xff, 0x00,
  0xe5, 0x7f,
  0xf9, 0xc0,
  0x41, 0x24,
  0xe0, 0x14,
  0x76, 0xff,
  0x33, 0xa0,
  0x42, 0x20,
  0x43, 0x18,
  0x4c, 0x00,
  0x87, 0xd5,
  0x88, 0x3f,
  0xd7, 0x03,
  0xd9, 0x10,
  0xd3, 0x82,
  0xc8, 0x08,
  0xc9, 0x80,
  0x7c, 0x00,
  0x7d, 0x00,
  0x7c, 0x03,
  0x7d, 0x48,
  0x7d, 0x48,
  0x7c, 0x08,
  0x7d, 0x20,
  0x7d, 0x10,
  0x7d, 0x0e,
  0x90, 0x00,
  0x91, 0x0e,
  0x91, 0x1a,
  0x91, 0x31,
  0x91, 0x5a,
  0x91, 0x69,
  0x91, 0x75,
  0x91, 0x7e,
  0x91, 0x88,
  0x91, 0x8f,
  0x91, 0x96,
  0x91, 0xa3,
  0x91, 0xaf,
  0x91, 0xc4,
  0x91, 0xd7,
  0x91, 0xe8,
  0x91, 0x20,
  0x92, 0x00,
  0x93, 0x06,
  0x93, 0xe3,
  0x93, 0x05,
  0x93, 0x05,
  0x93, 0x00,
  0x93, 0x04,
  0x93, 0x00,
  0x93, 0x00,
  0x93, 0x00,
  0x93, 0x00,
  0x93, 0x00,
  0x93, 0x00,
  0x93, 0x00,
  0x96, 0x00,
  0x97, 0x08,
  0x97, 0x19,
  0x97, 0x02,
  0x97, 0x0c,
  0x97, 0x24,
  0x97, 0x30,
  0x97, 0x28,
  0x97, 0x26,
  0x97, 0x02,
  0x97, 0x98,
  0x97, 0x80,
  0x97, 0x00,
  0x97, 0x00,
  0xc3, 0xed,
  0xa4, 0x00,
  0xa8, 0x00,
  0xc5, 0x11,
  0xc6, 0x51,
  0xbf, 0x80,
  0xc7, 0x10,
  0xb6, 0x66,
  0xb8, 0xA5,
  0xb7, 0x64,
  0xb9, 0x7C,
  0xb3, 0xaf,
  0xb4, 0x97,
  0xb5, 0xFF,
  0xb0, 0xC5,
  0xb1, 0x94,
  0xb2, 0x0f,
  0xc4, 0x5c,
  0xc0, 0x64,
  0xc1, 0x4B,
  0x8c, 0x00,
  0x86, 0x3D,
  0x50, 0x00,
  0x51, 0xC8,
  0x52, 0x96,
  0x53, 0x00,
  0x54, 0x00,
  0x55, 0x00,
  0x5a, 0xC8,
  0x5b, 0x96,
  0x5c, 0x00,
  0xd3, 0x7f,
  0xc3, 0xed,
  0x7f, 0x00,
  0xda, 0x00,
  0xe5, 0x1f,
  0xe1, 0x67,
  0xe0, 0x00,
  0xdd, 0x7f,
  0x05, 0x00,

  0x12, 0x40,//0x40
  0xd3, 0x7f,
  0xc0, 0x16,
  0xC1, 0x12,
  0x8c, 0x00,
  0x86, 0x3d,
  0x50, 0x00,
  0x51, 0x2C,
  0x52, 0x24,
  0x53, 0x00,
  0x54, 0x00,
  0x55, 0x00,
  0x5A, 0x2c,
  0x5b, 0x24,
  0x5c, 0x00,
};
/**
  * @brief  初始化控制摄像头使用的GPIO(I2C/DCMI)
  * @param  None
  * @retval None
  */
void OV2640_HW_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    /***DCMI引脚配置***/
    /* 使能DCMI时钟 */
    DCMI_PWDN_GPIO_CLK_ENABLE();
    DCMI_VSYNC_GPIO_CLK_ENABLE();
    DCMI_HSYNC_GPIO_CLK_ENABLE();
    DCMI_PIXCLK_GPIO_CLK_ENABLE();    
    DCMI_D0_GPIO_CLK_ENABLE();
    DCMI_D1_GPIO_CLK_ENABLE();
    DCMI_D2_GPIO_CLK_ENABLE();
    DCMI_D3_GPIO_CLK_ENABLE();    
    DCMI_D4_GPIO_CLK_ENABLE();
    DCMI_D5_GPIO_CLK_ENABLE();
    DCMI_D6_GPIO_CLK_ENABLE();
    DCMI_D7_GPIO_CLK_ENABLE();

    /*控制/同步信号线*/
    GPIO_InitStructure.Pin = DCMI_VSYNC_GPIO_PIN;
    GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStructure.Pull = GPIO_PULLUP ;
    GPIO_InitStructure.Alternate = DCMI_VSYNC_AF;
    HAL_GPIO_Init(DCMI_VSYNC_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = DCMI_HSYNC_GPIO_PIN;
    GPIO_InitStructure.Alternate = DCMI_VSYNC_AF;
    HAL_GPIO_Init(DCMI_HSYNC_GPIO_PORT, &GPIO_InitStructure);
 

    GPIO_InitStructure.Pin = DCMI_PIXCLK_GPIO_PIN;
    GPIO_InitStructure.Alternate = DCMI_PIXCLK_AF;
    HAL_GPIO_Init(DCMI_PIXCLK_GPIO_PORT, &GPIO_InitStructure);
    
    /*数据信号*/
    GPIO_InitStructure.Pin = DCMI_D0_GPIO_PIN;
    GPIO_InitStructure.Alternate = DCMI_D0_AF;
    HAL_GPIO_Init(DCMI_D0_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = DCMI_D1_GPIO_PIN;
    GPIO_InitStructure.Alternate = DCMI_D1_AF;
    HAL_GPIO_Init(DCMI_D1_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = DCMI_D2_GPIO_PIN;
    GPIO_InitStructure.Alternate = DCMI_D2_AF;
    HAL_GPIO_Init(DCMI_D2_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = DCMI_D3_GPIO_PIN;
    GPIO_InitStructure.Alternate = DCMI_D3_AF;
    HAL_GPIO_Init(DCMI_D3_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = DCMI_D4_GPIO_PIN;
    GPIO_InitStructure.Alternate = DCMI_D4_AF;
    HAL_GPIO_Init(DCMI_D4_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = DCMI_D5_GPIO_PIN;
    GPIO_InitStructure.Alternate = DCMI_D5_AF;
    HAL_GPIO_Init(DCMI_D5_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = DCMI_D6_GPIO_PIN;
    GPIO_InitStructure.Alternate = DCMI_D6_AF;
    HAL_GPIO_Init(DCMI_D6_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = DCMI_D7_GPIO_PIN;
    GPIO_InitStructure.Alternate = DCMI_D7_AF;
    HAL_GPIO_Init(DCMI_D7_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = DCMI_PWDN_GPIO_PIN;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;    
    HAL_GPIO_Init(DCMI_PWDN_GPIO_PORT, &GPIO_InitStructure);
    /*PWDN引脚，高电平关闭电源，低电平供电*/
    HAL_GPIO_WritePin(DCMI_PWDN_GPIO_PORT,DCMI_PWDN_GPIO_PIN,GPIO_PIN_RESET);
    
}
/**
  * @brief  Resets the OV2640 camera.
  * @param  None
  * @retval None
  */
void OV2640_Reset(void)
{
	/*OV2640有两组寄存器，设置0xFF寄存器的值为0或为1时可选择使用不同组的寄存器*/
  OV2640_WriteReg(OV2640_DSP_RA_DLMT, 0x01);
  OV2640_WriteReg(OV2640_SENSOR_COM7, 0x80);
}

/**
  * @brief  读取摄像头的ID.
  * @param  OV2640ID: 存储ID的结构体
  * @retval None
  */
void OV2640_ReadID(OV2640_IDTypeDef *OV2640ID)
{
	/*OV2640有两组寄存器，设置0xFF寄存器的值为0或为1时可选择使用不同组的寄存器*/
  OV2640_WriteReg(OV2640_DSP_RA_DLMT, 0x01);
	
	/*读取寄存芯片ID*/
  OV2640ID->Manufacturer_ID1 = OV2640_ReadReg(OV2640_SENSOR_MIDH);
  OV2640ID->Manufacturer_ID2 = OV2640_ReadReg(OV2640_SENSOR_MIDL);
  OV2640ID->PIDH = OV2640_ReadReg(OV2640_SENSOR_PIDH);
  OV2640ID->PIDL = OV2640_ReadReg(OV2640_SENSOR_PIDL);
}

uint8_t pbuffer_ary[1024*1024*1];

/**
  * @brief  配置 DCMI/DMA 以捕获摄像头数据
  * @param  None
  * @retval None
  */
void OV2640_Init(void) 
{
	/*** 配置DCMI接口 ***/
	/* 使能DCMI时钟 */
	__HAL_RCC_DCMI_CLK_ENABLE();

	/* DCMI 配置*/
	DCMI_Handle.Instance              = DCMI;    
	DCMI_Handle.Init.SynchroMode      = DCMI_MODE_CONTINUOUS;
	DCMI_Handle.Init.SynchroMode      = DCMI_SYNCHRO_HARDWARE;
	DCMI_Handle.Init.PCKPolarity      = DCMI_PCKPOLARITY_RISING;
	DCMI_Handle.Init.VSPolarity       = DCMI_VSPOLARITY_LOW;
	DCMI_Handle.Init.HSPolarity       = DCMI_HSPOLARITY_LOW;
	DCMI_Handle.Init.CaptureRate      = DCMI_CR_ALL_FRAME;
	DCMI_Handle.Init.ExtendedDataMode = DCMI_EXTEND_DATA_8B;
	HAL_DCMI_Init(&DCMI_Handle); 	

	/* 配置中断 */
	HAL_NVIC_SetPriority(DCMI_IRQn, 5, 0);
	HAL_NVIC_EnableIRQ(DCMI_IRQn); 	

}

/**
  * @brief  配置 DCMI/DMA 以捕获摄像头数据
  * @param  DMA_Memory0BaseAddr:本次传输的目的首地址
  * @param DMA_BufferSize：本次传输的数据量(单位为字,即4字节)
  */
void OV2640_DMA_Config(uint32_t DMA_Memory0BaseAddr,uint32_t DMA_BufferSize)
{
  /* 配置DMA从DCMI中获取数据*/
  /* 使能DMA*/
  __HAL_RCC_DMA2_CLK_ENABLE(); 
  DMA_Handle_dcmi.Instance = DMA2_Stream1;
  DMA_Handle_dcmi.Init.Channel = DMA_CHANNEL_1;  
  DMA_Handle_dcmi.Init.Direction = DMA_PERIPH_TO_MEMORY;
  DMA_Handle_dcmi.Init.PeriphInc = DMA_PINC_DISABLE;
  DMA_Handle_dcmi.Init.MemInc = DMA_MINC_ENABLE;			//寄存器地址自增
  DMA_Handle_dcmi.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
  DMA_Handle_dcmi.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
  DMA_Handle_dcmi.Init.Mode = DMA_CIRCULAR;								//循环模式
  DMA_Handle_dcmi.Init.Priority = DMA_PRIORITY_HIGH;
  DMA_Handle_dcmi.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
  DMA_Handle_dcmi.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
  DMA_Handle_dcmi.Init.MemBurst = DMA_MBURST_SINGLE;
  DMA_Handle_dcmi.Init.PeriphBurst = DMA_PBURST_SINGLE;

  /*DMA中断配置 */
  __HAL_LINKDMA(&DCMI_Handle, DMA_Handle, DMA_Handle_dcmi);
  
  HAL_NVIC_SetPriority(DMA2_Stream1_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream1_IRQn);
  
  HAL_DMA_Init(&DMA_Handle_dcmi);
  
  //使能DCMI采集数据
  HAL_DCMI_Start_DMA(&DCMI_Handle, DCMI_MODE_CONTINUOUS, (uint32_t)DMA_Memory0BaseAddr,DMA_BufferSize);
}

/**
  * @brief  Configures the OV2640 camera in QQVGA mode.
  * @param  None
  * @retval None
  */
void OV2640_QQVGAConfig(void)
{
  uint32_t i;

  OV2640_Reset();
  Delay(200);

  /* Initialize OV2640 */
  for(i=0; i<(sizeof(OV2640_QQVGA)/2); i++)
  {
    OV2640_WriteReg(OV2640_QQVGA[i][0], OV2640_QQVGA[i][1]);
    Delay(2);
  }
}

/**
  * @brief  设置图像输出大小，OV2640输出图像的大小(分辨率),完全由该函数确定
  * @param  width,height:宽度(对应:horizontal)和高度(对应:vertical),width和height必须是4的倍数
  * @retval 0,设置成功，其他,设置失败
  */
u8 OV2640_OutSize_Set(u16 width,u16 height)
{
	u16 outh;
	u16 outw;
	u8 temp; 
	if(width%4)return 1;
	if(height%4)return 2;
	outw=width/4;
	outh=height/4; 
	OV2640_WriteReg(0XFF,0X00);	
	OV2640_WriteReg(0XE0,0X04);	
    OV2640_WriteReg(0X50,outw&0X00);		//配置v_divider hdivider
	OV2640_WriteReg(0X5A,outw&0XFF);		//设置OUTW的低八位
	OV2640_WriteReg(0X5B,outh&0XFF);		//设置OUTH的低八位
	temp=(outw>>8)&0X03;
	temp|=(outh>>6)&0X04;
	OV2640_WriteReg(0X5C,temp);				//设置OUTH/OUTW的高位 
	OV2640_WriteReg(0XE0,0X00);	
	return 0;
}

/**未测试*/
/**
  * @brief  设置图像尺寸大小,也就是所选格式的输出分辨率
  * @param  width,height:图像宽度和图像高度
  * @retval 0,设置成功，其他,设置失败
  */
u8 OV2640_ImageSize_Set(u16 width,u16 height)
{
	u8 temp;
	OV2640_WriteReg(0XFF,0X00);
	OV2640_WriteReg(0XE0,0X04);
	OV2640_WriteReg(0XC0,(width)>>3&0XFF);		//设置HSIZE的10:3位
	OV2640_WriteReg(0XC1,(height)>>3&0XFF);		//设置VSIZE的10:3位
	temp=(width&0X07)<<3;
	temp|=height&0X07;
	temp|=(width>>4)&0X80;
	OV2640_WriteReg(0X8C,temp);
	OV2640_WriteReg(0XE0,0X00);
	return 0;
}


/**未测试*/
/**
  * @brief  设置图像输出窗口
  * @param  sx,sy,起始地址
						width,height:宽度(对应:horizontal)和高度(对应:vertical)
  * @retval 0,设置成功， 其他,设置失败
  */
void OV2640_Window_Set(u16 sx,u16 sy,u16 width,u16 height)
{
	u16 endx;
	u16 endy;
	u8 temp;
	endx=sx+width/2;	//V*2
 	endy=sy+height/2;

 	OV2640_WriteReg(0XFF,0X01);
	temp = OV2640_ReadReg(0X03);				//读取Vref之前的值
	temp&=0XF0;
	temp|=((endy&0X03)<<2)|(sy&0X03);
	OV2640_WriteReg(0X03,temp);				//设置Vref的start和end的最低2位
	OV2640_WriteReg(0X19,sy>>2);			//设置Vref的start高8位
	OV2640_WriteReg(0X1A,endy>>2);			//设置Vref的end的高8位

	temp = OV2640_ReadReg(0X32);				//读取Href之前的值
	temp&=0XC0;
	temp|=((endx&0X07)<<3)|(sx&0X07);
	OV2640_WriteReg(0X32,temp);				//设置Href的start和end的最低3位
	OV2640_WriteReg(0X17,sx>>3);			//设置Href的start高8位
	OV2640_WriteReg(0X18,endx>>3);			//设置Href的end的高8位
}

//未测试
/**
  * @brief  设置图像开窗大小
						由:OV2640_ImageSize_Set确定传感器输出分辨率从大小.
						该函数则在这个范围上面进行开窗,用于OV2640_OutSize_Set的输出
						注意:本函数的宽度和高度,必须大于等于OV2640_OutSize_Set函数的宽度和高度
						     OV2640_OutSize_Set设置的宽度和高度,根据本函数设置的宽度和高度,由DSP
						     自动计算缩放比例,输出给外部设备.
  * @param  width,height:宽度(对应:horizontal)和高度(对应:vertical),width和height必须是4的倍数
  * @retval 0,设置成功， 其他,设置失败
  */
u8 OV2640_ImageWin_Set(u16 offx,u16 offy,u16 width,u16 height)
{
	u16 hsize;
	u16 vsize;
	u8 temp;
	if(width%4)return 1;
	if(height%4)return 2;
	hsize=width/4;
	vsize=height/4;
	OV2640_WriteReg(0XFF,0X00);
	OV2640_WriteReg(0XE0,0X04);
	OV2640_WriteReg(0X51,hsize&0XFF);		//设置H_SIZE的低八位
	OV2640_WriteReg(0X52,vsize&0XFF);		//设置V_SIZE的低八位
	OV2640_WriteReg(0X53,offx&0XFF);		//设置offx的低八位
	OV2640_WriteReg(0X54,offy&0XFF);		//设置offy的低八位
	temp=(vsize>>1)&0X80;
	temp|=(offy>>4)&0X70;
	temp|=(hsize>>5)&0X08;
	temp|=(offx>>8)&0X07;
	OV2640_WriteReg(0X55,temp);				//设置H_SIZE/V_SIZE/OFFX,OFFY的高位
	OV2640_WriteReg(0X57,(hsize>>2)&0X80);	//设置H_SIZE/V_SIZE/OFFX,OFFY的高位
	OV2640_WriteReg(0XE0,0X00);
	return 0;
}

/**
  * @brief  配置OV2640为UXGA模式，并设置输出图像大小
  * @param  None
  * @retval None
  */
void OV2640_UXGAConfig(void)
{
//  uint32_t i;
//	
//	/*摄像头复位*/
//  OV2640_Reset();

//	/*进行三次寄存器写入，确保配置写入正常
//	(在使用摄像头长排线时，IIC数据线干扰较大，必须多次写入来保证正常)*/
//  /* 写入寄存器配置 */
//	for(i=0; i<(sizeof(OV2640_UXGA)/2); i++)
//	{
//		OV2640_WriteReg(OV2640_UXGA[i][0], OV2640_UXGA[i][1]);

//	}
//	/* Initialize OV2640 */
//	for(i=0; i<(sizeof(OV2640_UXGA)/2); i++)
//	{
//		OV2640_WriteReg(OV2640_UXGA[i][0], OV2640_UXGA[i][1]);

//	}
//	  /* Initialize OV2640 */
//	for(i=0; i<(sizeof(OV2640_UXGA)/2); i++)
//	{
//		OV2640_WriteReg(OV2640_UXGA[i][0], OV2640_UXGA[i][1]);

//	}	
//	/*设置输出的图像大小*/
//	OV2640_OutSize_Set(img_width,img_height);   
}
/**
* @brief  Configures the OV2640 camera in JPEG mode.
* @param  JPEGImageSize: JPEG image size
* @retval None
*/
void OV2640_JPEGConfig(ImageFormat_TypeDef ImageFormat)
{
    uint32_t i;

    OV2640_Reset();
//    CPU_TS_Tmr_Delay_MS(200);
    /* Initialize OV2640 */
    for(i=0; i<(sizeof(OV2640_JPEG_INIT)/2); i++)
    {
        OV2640_WriteReg(OV2640_JPEG_INIT[i][0], OV2640_JPEG_INIT[i][1]);
    }

    /* Set to output YUV422 */
    for(i=0; i<(sizeof(OV2640_YUV422)/2); i++)
    {
        OV2640_WriteReg(OV2640_YUV422[i][0], OV2640_YUV422[i][1]);
    }

    OV2640_WriteReg(0xff, 0x01);
    OV2640_WriteReg(0x15, 0x00);

    /* Set to output JPEG */
    for(i=0; i<(sizeof(OV2640_JPEG)/2); i++)
    {
        OV2640_WriteReg(OV2640_JPEG[i][0], OV2640_JPEG[i][1]);
    }

//    CPU_TS_Tmr_Delay_MS(200);

    switch(ImageFormat)
    {
     case JPEG_160x120:
        {
            for(i=0; i<(sizeof(OV2640_160x120_JPEG)/2); i++)
            {
                OV2640_WriteReg(OV2640_160x120_JPEG[i][0], OV2640_160x120_JPEG[i][1]);
            }
            break;
        }
     case JPEG_176x144:
        {
            for(i=0; i<(sizeof(OV2640_176x144_JPEG)/2); i++)
            {
                OV2640_WriteReg(OV2640_176x144_JPEG[i][0], OV2640_176x144_JPEG[i][1]);
            }
            break;
        }
     case JPEG_320x240:
        {
            for(i=0; i<(sizeof(OV2640_320x240_JPEG)/2); i++)
            {
                OV2640_WriteReg(OV2640_320x240_JPEG[i][0], OV2640_320x240_JPEG[i][1]);
            }
            break;
        }
     case JPEG_352x288:
        {
            for(i=0; i<(sizeof(OV2640_352x288_JPEG)/2); i++)
            {
                OV2640_WriteReg(OV2640_352x288_JPEG[i][0], OV2640_352x288_JPEG[i][1]);
            }
            break;
        }
     case JPEG_640x480:
        {
            for(i=0; i<(sizeof(OV2640_640x480_JPEG)/2); i++)
            {
                OV2640_WriteReg(OV2640_640x480_JPEG[i][0], OV2640_640x480_JPEG[i][1]);
            }
            break;
        }

     case JPEG_800x600:
        {
            for(i=0; i<(sizeof(OV2640_800x600_JPEG)/2); i++)
            {
                OV2640_WriteReg(OV2640_800x600_JPEG[i][0], OV2640_800x600_JPEG[i][1]);
            }
            break;
        }
     case JPEG_1024x768:
        {
            for(i=0; i<(sizeof(OV2640_1024x768_JPEG)/2); i++)
            {
                OV2640_WriteReg(OV2640_1024x768_JPEG[i][0], OV2640_1024x768_JPEG[i][1]);
            }
            break;
        }
     default:
        {
            for(i=0; i<(sizeof(OV2640_160x120_JPEG)/2); i++)
            {
                OV2640_WriteReg(OV2640_160x120_JPEG[i][0], OV2640_160x120_JPEG[i][1]);
            }
            break;
        }
    }
}

/**
  * @brief  配置光线模式
  * @param  参数用于选择光线模式
  *         0x00 Auto     自动
  *         0x01 Sunny    光照
  *         0x02 Cloudy   阴天
  *         0x03 Office   办公室
  *         0x04 Home     家里

  * @retval None
  */
void OV2640_LightMode(uint8_t mode)
{
  switch(mode)
  {
    
    case 0:     //Auto
    OV2640_WriteReg(0xff, 0x00);
    OV2640_WriteReg(0xc7, 0x00); //AWB on
    break;
    
    case 1:     //Sunny
    OV2640_WriteReg(0xff, 0x00);
    OV2640_WriteReg(0xc7, 0x40); //AWB off
    OV2640_WriteReg(0xcc, 0x5e);
    OV2640_WriteReg(0xcd, 0x41);
    OV2640_WriteReg(0xce, 0x54);
    
    break;
    
    case 2:    //Cloudy 
    OV2640_WriteReg(0xff, 0x00);
    OV2640_WriteReg(0xc7, 0x40); //AWB off
    OV2640_WriteReg(0xcc, 0x65);
    OV2640_WriteReg(0xcd, 0x41);
    OV2640_WriteReg(0xce, 0x4f);
    break;
    
    case 3:   //Office
    OV2640_WriteReg(0xff, 0x00);
    OV2640_WriteReg(0xc7, 0x40); //AWB off
    OV2640_WriteReg(0xcc, 0x52);
    OV2640_WriteReg(0xcd, 0x41);
    OV2640_WriteReg(0xce, 0x66);
    break;
    
    case 4:   //Home
    OV2640_WriteReg(0xff, 0x00);
    OV2640_WriteReg(0xc7, 0x40); //AWB off
    OV2640_WriteReg(0xcc, 0x42);
    OV2640_WriteReg(0xcd, 0x3f);
    OV2640_WriteReg(0xce, 0x71);
    break;

  }
}

/**
  * @brief  特殊效果
  * @param  参数用于选择光线模式
  *         0x00 Antique   复古  
  *         0x01 Bluish    偏蓝
  *         0x02 Greenish  偏绿 
  *         0x03 Reddish   偏红
  *         0x04 B&W       黑白
  *         0x05 Negative  反相
  *         0x06 B&W negative  黑白反相
  *         0x07 Normal     正常 

  * @retval None
  */
void OV2640_SpecialEffects(uint8_t mode)
{
  switch(mode)
  {
    case 0:
   // Antique
    OV2640_WriteReg(0xff, 0x00);
    OV2640_WriteReg(0x7c, 0x00);
    OV2640_WriteReg(0x7d, 0x18);
    OV2640_WriteReg(0x7c, 0x05);
    OV2640_WriteReg(0x7d, 0x40);
    OV2640_WriteReg(0x7d, 0xa6);
    break;
    
    case 1:    
    //Bluish
    OV2640_WriteReg(0xff, 0x00);
    OV2640_WriteReg(0x7c, 0x00);
    OV2640_WriteReg(0x7d, 0x18);
    OV2640_WriteReg(0x7c,0x05);
    OV2640_WriteReg(0x7d, 0xa0);
    OV2640_WriteReg(0x7d, 0x40);
    
    break;
    
    case 2:
    //Greenish
    OV2640_WriteReg(0xff, 0x00);
    OV2640_WriteReg(0x7c, 0x00);
    OV2640_WriteReg(0x7d, 0x18);
    OV2640_WriteReg(0x7c, 0x05);
    OV2640_WriteReg(0x7d, 0x40);
    OV2640_WriteReg(0x7d, 0x40);
    break;
    
    case 3:
   // Reddish
    OV2640_WriteReg(0xff, 0x00);
    OV2640_WriteReg(0x7c, 0x00);
    OV2640_WriteReg(0x7d, 0x18);
    OV2640_WriteReg(0x7c, 0x05);
    OV2640_WriteReg(0x7d, 0x40);
    OV2640_WriteReg(0x7d, 0xc0);
    break;
    
    case 4:
   // B&W
    OV2640_WriteReg(0xff, 0x00);
    OV2640_WriteReg(0x7c, 0x00);
    OV2640_WriteReg(0x7d, 0x18);
    OV2640_WriteReg(0x7c, 0x05);
    OV2640_WriteReg(0x7d, 0x80);
    OV2640_WriteReg(0x7d, 0x80);
    break;
    
    case 5:
    //Negative
    OV2640_WriteReg(0xff, 0x00);
    OV2640_WriteReg(0x7c, 0x00);
    OV2640_WriteReg(0x7d, 0x40);
    OV2640_WriteReg(0x7c, 0x05);
    OV2640_WriteReg(0x7d, 0x80);
    OV2640_WriteReg(0x7d, 0x80);
    
    break;
    
    case 6:
    //B&W negative
    OV2640_WriteReg(0xff, 0x00);
    OV2640_WriteReg(0x7c, 0x00);
    OV2640_WriteReg(0x7d, 0x58);
    OV2640_WriteReg(0x7c, 0x05);
    OV2640_WriteReg(0x7d, 0x80);
    OV2640_WriteReg(0x7d, 0x80);
    
    break;
    
    case 7:
    //Normal
    OV2640_WriteReg(0xff, 0x00);
    OV2640_WriteReg(0x7c, 0x00);
    OV2640_WriteReg(0x7d, 0x00);
    OV2640_WriteReg(0x7c, 0x05);
    OV2640_WriteReg(0x7d, 0x80);
    OV2640_WriteReg(0x7d, 0x80);
    
    break;

  }
}


/**
  * @brief  Configures the OV2640 camera brightness.
  * @param  Brightness: Brightness value, where Brightness can be: 
  *         0x40 for Brightness +2,
  *         0x30 for Brightness +1,
  *         0x20 for Brightness 0,
  *         0x10 for Brightness -1,
  *         0x00 for Brightness -2,
  * @retval None
  */
void OV2640_BrightnessConfig(uint8_t Brightness)
{
  OV2640_WriteReg(0xff, 0x00);
  OV2640_WriteReg(0x7c, 0x00);
  OV2640_WriteReg(0x7d, 0x04);
  OV2640_WriteReg(0x7c, 0x09);
  OV2640_WriteReg(0x7d, Brightness);
  OV2640_WriteReg(0x7d, 0x00);
}

/**
  * @brief  Configures the OV2640 camera Black and white mode.
  * @param  BlackWhite: BlackWhite value, where BlackWhite can be: 
  *         0x18 for B&W,
  *         0x40 for Negative,
  *         0x58 for B&W negative,
  *         0x00 for Normal,
  * @retval None
  */
void OV2640_BandWConfig(uint8_t BlackWhite)
{
  OV2640_WriteReg(0xff, 0x00);
  OV2640_WriteReg(0x7c, 0x00);
  OV2640_WriteReg(0x7d, BlackWhite);
  OV2640_WriteReg(0x7c, 0x05);
  OV2640_WriteReg(0x7d, 0x80);
  OV2640_WriteReg(0x7d, 0x80);
}

/**
  * @brief  Configures the OV2640 camera color effects.
  * @param  value1: Color effects value1
  * @param  value2: Color effects value2
  *         where value1 and value2 can be: 
  *         value1 = 0x40, value2 = 0xa6 for Antique,
  *         value1 = 0xa0, value2 = 0x40 for Bluish,
  *         value1 = 0x40, value2 = 0x40 for Greenish,
  *         value1 = 0x40, value2 = 0xc0 for Reddish,
  * @retval None
  */
void OV2640_ColorEffectsConfig(uint8_t value1, uint8_t value2)
{
  OV2640_WriteReg(0xff, 0x00);
  OV2640_WriteReg(0x7c, 0x00);
  OV2640_WriteReg(0x7d, 0x18);
  OV2640_WriteReg(0x7c, 0x05);
  OV2640_WriteReg(0x7d, value1);
  OV2640_WriteReg(0x7d, value2);
}

/**
  * @brief  Configures the OV2640 camera contrast.
  * @param  value1: Contrast value1
  * @param  value2: Contrast value2
  *         where value1 and value2 can be: 
  *         value1 = 0x28, value2 = 0x0c for Contrast +2,
  *         value1 = 0x24, value2 = 0x16 for Contrast +1,
  *         value1 = 0x20, value2 = 0x20 for Contrast 0,
  *         value1 = 0x1c, value2 = 0x2a for Contrast -1,
  *         value1 = 0x18, value2 = 0x34 for Contrast -2,
  * @retval None
  */
void OV2640_ContrastConfig(uint8_t value1, uint8_t value2)
{
  OV2640_WriteReg(0xff, 0x00);
  OV2640_WriteReg(0x7c, 0x00);
  OV2640_WriteReg(0x7d, 0x04);
  OV2640_WriteReg(0x7c, 0x07);
  OV2640_WriteReg(0x7d, 0x20);
  OV2640_WriteReg(0x7d, value1);
  OV2640_WriteReg(0x7d, value2);
  OV2640_WriteReg(0x7d, 0x06);
}

/**
  * @brief  帧同步回调函数.
  * @param  None
  * @retval None
  */

void HAL_DCMI_VsyncEventCallback(DCMI_HandleTypeDef *hdcmi)
{
    fps++; //帧率计数
    //OV2640_DMA_Config(LCD_FB_START_ADDRESS,LCD_GetXSize()*LCD_GetYSize()/2); 
		DCMI_IRQHandler_Funtion();
}
uint8_t fps=0;
uint8_t dispBuf[100];
/*简单任务管理*/
uint32_t Task_Delay[3]={0};

#define JPEG_MODO

int32_t open_camera(uint32_t *BufferSRC, uint32_t BufferSize)
{
		OV2640_IDTypeDef OV2640_Camera_ID;	
		camera_data *data_p;

//    /* LCD 端口初始化 */ 
//    LCD_Init();
//    /* LCD 第一层初始化 */ 
//    LCD_LayerInit(0, LCD_FB_START_ADDRESS,RGB565);
//	/* LCD 第二层初始化 */ 
//    LCD_LayerInit(1, LCD_FB_START_ADDRESS+(LCD_GetXSize()*LCD_GetYSize()*4),ARGB8888);
//    /* 使能LCD，包括开背光 */ 
//    LCD_DisplayOn(); 

//    /* 选择LCD第一层 */
//    LCD_SelectLayer(0);
		
	//初始化 I2C
	I2CMaster_Init(); 
	OV2640_HW_Init();
	/* 读取摄像头芯片ID，确定摄像头正常连接 */
	OV2640_ReadID(&OV2640_Camera_ID);

	if(OV2640_Camera_ID.PIDH  == 0x26)
	{
		CAMERA_DEBUG("%x%x",OV2640_Camera_ID.PIDH ,OV2640_Camera_ID.PIDL);
	}
	else
	{
		CAMERA_DEBUG("没有检测到OV2640摄像头，请重新检查连接。");
		while(1);  
	}
    /* 配置摄像头输出像素格式 */
	
#if defined(JPEG_MODO)
		OV2640_JPEGConfig(JPEG_IMAGE_FORMAT);
#else
		OV2640_UXGAConfig();
#endif
    /* 初始化摄像头，捕获并显示图像 */
		OV2640_Init();

		
#if defined(JPEG_MODO)
		OV2640_DMA_Config((uint32_t)BufferSRC, BufferSize);
		printf("开始传输JPEG数据\r\n");
#else
	//开始传输，数据大小以32位数据为单位(即像素个数/4，LCD_GetXSize()*LCD_GetYSize()*2/4) 
		OV2640_DMA_Config((uint32_t)BufferSRC, BufferSize);//
		printf("开始传输数据\r\n");
#endif	
	
	DCMI_JPEGCmd(ENABLE); //for ov2640

	start_capture_img();

	DCMI_Cmd(ENABLE); //数据开关
	
		
}

