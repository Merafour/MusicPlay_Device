/*----------------------------------------------------------------------------   
 * ��Ŀ����:

     RGB LED��������

 * ��������:

     1����������������ݷ��͵õ�LED������ʾ
	 2����ָ�����ؿ�ʼ��������������ݷ��͵�LED��ʾ
	 3�����͸�λ�ź�

 * ��Ȩ��Ϣ:

     (c) �������, 2014.

 * ��ʷ�汾:
     2014-03-15:
       - ��ʼ�汾 V1.0.0;

 * ����˵��:
     MCU:             STC15W204S
     ����:      	  �ڲ�����33MHz
     ��չģ��:  	  -
     ���:            Keil.C51.V9.01

 * ��ע:

------------------------------------------------------------------------------*/   

#ifndef _WS2812_H_
#define _WS2812_H_

#include <stdint.h>
//typedef struct{
//	uint8_t G;
//	uint8_t R;
//	uint8_t B;
//	uint8_t W;
//}rgbw_u;
typedef union {
	//rgbw_u u;
#if 0
	struct{
		uint8_t G;
		uint8_t R;
		uint8_t B;
		uint8_t W;
	}u;
#else
	struct{
		uint8_t W;
		uint8_t B;
		uint8_t R;
		uint8_t G;
	}u;
#endif
	uint32_t rgbw;
}rgbw_t;

/*******************************ͨ�Žӿ�����***********************************/

//sbit DI = P5^5;						    //���ݶ˿�

/*----------------------------���鴮����������--------------------------------*/  
#define SNUM 12 //16
/*--------------------------------��������------------------------------------*/   
void IO_Init(void);						//IO�˿ڳ�ʼ��
void SendOnePix(unsigned char *ptr);	//����һ��LED��24bit����
void ResetDataFlow(void);				//оƬ��λ���壬Ϊ������һ֡��׼��
void SendOneFrame(unsigned char *ptr);	//����һ֡����
void SendSameColor(unsigned char *ptr,unsigned char cnt);  //��ͬ��ɫ�������ɴ�
void SendOneFrameFrom(unsigned char i,unsigned char *ptr);//�ӵ�i�����ؿ�ʼ����
														  //һ֡����
void SendOneFrameSince(unsigned char i,unsigned char *ptr);//�ӵ�i�����ص������
														   //��ʼ����
extern void Send0(void);
extern void Send1(void);
extern void FlushOneFrame(rgbw_t *rgbw);


#endif

//-------------------------THE END------------------------*/   

