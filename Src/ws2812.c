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

//#include "STC15W.h"
//#include <intrins.h>
#include "ws2812.h"
#include "main.h"
#include "gpio.h"
#include "stm32f1xx_hal.h"
#include "tim.h"

//#define DI1  HAL_GPIO_WritePin(DO_GPIO_Port, DO_Pin, GPIO_PIN_SET);
//#define DI0  HAL_GPIO_WritePin(DO_GPIO_Port, DO_Pin, GPIO_PIN_RESET);
#define DI1  DO_GPIO_Port->BSRR = DO_Pin
#define DI0  DO_GPIO_Port->BSRR = (uint32_t)DO_Pin << 16

//#define _nop_ __nop
//-------------------------------------------------------------------------------
//�ӳ�������:IO_Init(void)
//���ܣ��˿ڳ�ʼ��
//-------------------------------------------------------------------------------
void IO_Init(void)
{
	;
}
__asm void nop(void)
{
	NOP
}
__asm void my_strcpy(const char *src, char *dst)
{
loop
      LDRB  r2, [r0], #1
      STRB  r2, [r1], #1
      CMP   r2, #0
      BNE   loop
      BX    lr
}
// ;      SUB   r2, r0, #1
__asm void inline my_delay(int _delay)
{
loop_delay
	  SUB   r0, #1
      CMP   r0, #0
      BNE   loop_delay
      BX    lr
}
//-------------------------------------------------------------------------------
//�ӳ�������:ResetDateFlow(void)
//���ܣ���λ��Ϊ��һ�η�����׼����
//˵������DI��λΪ0����ʱԼ65us
//-------------------------------------------------------------------------------
//void ResetDataFlow(void)
//{
//	unsigned char i,j;
//	//DI=0;					//DI��Ϊ0����ʱ50us���ϣ�ʵ��֡��λ
//	HAL_GPIO_WritePin(DO_GPIO_Port, DO_Pin, GPIO_PIN_RESET);
//	for(i=0;i<15;i++)		//�˴�33Mhzʱ��ʱ65us
//	{
//		for(j=0;j<20;j++)
//		{
//			;
//		}
//	}
//}
/**
  T=1/48us = 0.02us
 */
const uint16_t reset_delay = 5*60; // 60us, 0.6001ms, __nop()
const uint16_t T0H = 1; // 0.35us  +/- 0.15us      0.35/(1/48) = 48*0.35= 16.8
const uint16_t T1H = 1; // 0.7us   +/- 0.15us      0.7 /(1/48) = 48*0.7 = 33.66
const uint16_t T0L = 2; // 0.8us   +/- 0.15us      0.8 /(1/48) = 48*0.8 = 38.4
const uint16_t T1L = 1; // 0.6us   +/- 0.15us      0.6 /(1/48) = 48*0.6 = 28.8
volatile uint16_t _delay=0;
void ResetDataFlow(void)
{
	//unsigned char i,j;
	//DI=0;					//DI��Ϊ0����ʱ50us���ϣ�ʵ��֡��λ
	DI0;//HAL_GPIO_WritePin(DO_GPIO_Port, DO_Pin, GPIO_PIN_RESET);
	my_delay(reset_delay);
//	_delay=0;
//	while(reset_delay>_delay++)
//	{
//		//_delay++;
//		//__nop();
//	}
//	DI1;//HAL_GPIO_WritePin(DO_GPIO_Port, DO_Pin, GPIO_PIN_SET);
//	_delay=0;
//	while(reset_delay>_delay)
//	{
//		_delay++;
//		__nop();
//	}
}
//void ResetDataFlow(void)
//{
//	//unsigned char i,j;
//	//DI=0;					//DI��Ϊ0����ʱ50us���ϣ�ʵ��֡��λ
//	DI0;
//	__HAL_TIM_SET_COUNTER(&htim1, 0);
//	_delay=0;
//	while(reset_delay>_delay++)
//	{
//	}
//	DI1;
//	__HAL_TIM_SET_COUNTER(&htim1, 0);
//	_delay=0;
//	while(reset_delay>_delay++)
//	{
//	}
//}
void Send0(void)
{
	while(1)
	{
		DI1;  // 0.35us
		_delay=0;
		//while(T0H>_delay) _delay++;
		__nop();__nop();__nop();__nop();__nop();__nop();
		__nop();__nop();__nop();__nop();__nop();__nop();  // 0.3125
		__nop();__nop();__nop();
		DI0;  // 0.8us
		_delay=0;
		while(T0L>_delay) _delay++;
		__nop();__nop();__nop();__nop();__nop();__nop();
		__nop();__nop();//__nop();
		DI1;
	}
}
void Send1(void)
{
	while(1)
	{
		DI1;  // 0.7us
		_delay=0;
		while(T1H>_delay) _delay++;
		__nop();__nop();__nop();__nop();__nop();__nop();
		__nop();__nop();__nop();__nop();__nop();__nop();  // 
		__nop();//__nop();__nop();
		DI0;  // 0.6us
		_delay=0;
		while(T1L>_delay) _delay++;
		__nop();__nop();__nop();__nop();__nop();__nop();
		__nop();__nop();//__nop();
		DI1;
	}
}
//-------------------------------------------------------------------------------
//�ӳ�������:SendOnePix(unsigned char *ptr)
//���ܣ�����һ�����ص��24bit����
//���������յĲ�����һ��ָ�룬�˺����Ὣ��ָ��ָ��ĵ�ַ������������Byte�����ݷ���
//˵��������������ֱ�ӵ��ô˺���ʱ��������֡���Ϳ�ʼǰ��Ҫ��ִ�� ResetDataFlow()
//		�����ǰ�������ķ�ʽ���ͣ�����Ϊ800KBPS
//-------------------------------------------------------------------------------
#if 0
void SendOnePix(unsigned char *ptr)
{
	unsigned char i,j;
	unsigned char temp;
	for(j=0;j<3;j++)
	{
		temp=ptr[j];
		for(i=0;i<8;i++)
		{
			if(temp&0x80)		 //�Ӹ�λ��ʼ����
			{
				//DI=1;			 //���͡�1����
				DI1;  // 0.7us
				_delay=0;
				while(T1H>_delay) _delay++;
				__nop();__nop();__nop();__nop();__nop();__nop();
				__nop();__nop();__nop();__nop();__nop();__nop();  // 
				__nop();//__nop();__nop();
				DI0;  // 0.6us
				_delay=0;
				while(T1L>_delay) _delay++;
				//__nop();__nop();__nop();__nop();__nop();__nop();
				__nop();__nop();//__nop();
			}
			else				//���͡�0����
			{
				DI1;  // 0.35us
				_delay=0;
				//while(T0H>_delay) _delay++;
				__nop();__nop();__nop();__nop();__nop();__nop();
				__nop();__nop();__nop();__nop();__nop();__nop();  // 0.3125
				__nop();__nop();__nop();
				DI0;  // 0.8us
				_delay=0;
				while(T0L>_delay) _delay++;
				__nop();__nop();__nop();__nop();__nop();__nop();
				__nop();__nop();//__nop();
			}
			temp=(temp<<1);		 //����λ
		}
	}
}
#endif


void SendOnePix(unsigned char *ptr)
{
	unsigned char i,j;
	uint32_t temp;
	//uint32_t rgbw=0;
	temp = (ptr[0]<<24)+(ptr[1]<<16)+(ptr[2]<<8);
	//for(j=0;j<3;j++)
	{
		//temp=ptr[j];
		for(i=0;i<24;i++)
		{
			if(temp&0x80000000)		 //�Ӹ�λ��ʼ����
			{
				//DI=1;			 //���͡�1����
				DI1;  // 0.7us
#if 0
				_delay=0;
				while(T1H>_delay) _delay++;
				__nop();__nop();__nop();__nop();__nop();__nop();
				__nop();__nop();__nop();__nop();__nop();__nop();  // 
				__nop();//__nop();__nop();
#endif
				my_delay(5);
				DI0;  // 0.6us
				//_delay=0;
				//while(T1L>_delay) _delay++;
				//nop();nop();nop();nop();nop();nop();
				//nop();//nop();//__nop();
				//asm volatile("");	// this line is considered to have side-effects
				my_delay(1); //my_delay(T1L);
			}
			else				//���͡�0����
			{
				DI1;  // 0.35us
				my_delay(1);
				__nop();__nop();__nop();__nop();//__nop();
				DI0;  // 0.8us
				my_delay(3);
			}
			temp=(temp<<1);		 //����λ
		}
	}
}
#if 0
void SendOnePix(unsigned char *ptr)
{
	unsigned char i,j;
	unsigned char temp;

	for(j=0;j<3;j++)
	{
		temp=ptr[j];
		for(i=0;i<8;i++)
		{
			if(temp&0x80)		 //�Ӹ�λ��ʼ����
			{
				//DI=1;			 //���͡�1����
				HAL_GPIO_WritePin(DO_GPIO_Port, DO_Pin, GPIO_PIN_SET);
				_nop_();		 //����ʡ�Ե�nop(),��ʱָ��ʱ�����ã�����Ƶ��33MHz
				_nop_();
				_nop_();
				_nop_();
				_nop_();
				_nop_();
				_nop_();
				_nop_();
				_nop_();
				_nop_();
				_nop_();
				_nop_();
				_nop_();
				_nop_();
				_nop_();
				_nop_();
				_nop_();
				_nop_();
				_nop_();
				_nop_();

				//DI=0;
				HAL_GPIO_WritePin(DO_GPIO_Port, DO_Pin, GPIO_PIN_RESET);
				_nop_();
				_nop_();
				_nop_();
				_nop_();
				_nop_();
				_nop_();
				_nop_();
				_nop_();
				_nop_();
				_nop_();
				_nop_();
				_nop_();
				_nop_();
				_nop_();
				_nop_();
			}
			else				//���͡�0����
			{
				//DI=1;
				HAL_GPIO_WritePin(DO_GPIO_Port, DO_Pin, GPIO_PIN_SET);
				_nop_();
				_nop_();
				_nop_();
				_nop_();
				_nop_();
				_nop_();
				_nop_();

				//DI=0;
				HAL_GPIO_WritePin(DO_GPIO_Port, DO_Pin, GPIO_PIN_RESET);
				_nop_();
				_nop_();
				_nop_();
				_nop_();
				_nop_();
				_nop_();
				_nop_();
				_nop_();
				_nop_();
				_nop_();
				_nop_();
				_nop_();
				_nop_();
				_nop_();
				_nop_();
				_nop_();
				_nop_();
				_nop_();
				_nop_();
			}
			temp=(temp<<1);		 //����λ
		}
	}
}
#endif
//-------------------------------------------------------------------------------
//�ӳ�������:SendOneFrame(unsigned char *ptr)
//���ܣ�����һ֡���ݣ�������������������ݣ�
//���������յĲ�����һ��ָ�룬�˺����Ὣ��ָ��ָ��ĵ�ַ��������������ݷ���
//-------------------------------------------------------------------------------
void SendOneFrame(unsigned char *ptr)
{
	unsigned char k;
	//__HAL_TIM_SET_COUNTER(&htim1, 0);
	//HAL_TIM_Base_Start(&htim1);
	
#if 0
	ResetDataFlow();				 //����֡��λ�ź�
	for(k=0;k<SNUM;k++)				 //����һ֡���ݣ�SNUM�ǰ���LED�ĸ���
	{
		SendOnePix(&ptr[(3*k)]);
	}
#else
	const uint16_t len=SNUM*3;
	ResetDataFlow();				 //����֡��λ�ź�
	for(k=0;k<len;k+=3)				 //����һ֡���ݣ�SNUM�ǰ���LED�ĸ���
	{
		SendOnePix(&ptr[k]);
	}
#endif
	ResetDataFlow();				 //����֡��λ�ź�
	//HAL_TIM_Base_Stop(&htim1);
}
void FlushOneFrame(rgbw_t *rgbw)
{
	uint16_t k=0;
	uint16_t len=SNUM;
	uint16_t i=0;
	uint32_t temp=0;
	ResetDataFlow();				 //����֡��λ�ź�
	for(k=0;k<len;k++)				 //����һ֡���ݣ�SNUM�ǰ���LED�ĸ���
	{
		temp = rgbw[k].rgbw;
		for(i=0;i<24;i++)
		{
			if(temp&0x80000000)		 //�Ӹ�λ��ʼ����
			{
				//DI=1;			 //���͡�1����
				DI1;  // 0.7us
				my_delay(5);
				DI0;  // 0.6us
				my_delay(1); //my_delay(T1L);
				__nop();
			}
			else				//���͡�0����
			{
				DI1;  // 0.35us
				my_delay(1);
				__nop();__nop();__nop();//__nop();//__nop();
				DI0;  // 0.8us
				my_delay(3);
				__nop();__nop();
			}
			temp=(temp<<1);		 //����λ
		}
	}
	ResetDataFlow();				 //����֡��λ�ź�
}
void BrushOut(uint16_t nDots,uint8_t dr,uint8_t dg,uint8_t db)
{
	rgbw_t rgbw[12];
	memset(rgbw, 0, sizeof(rgbw));
	rgbw[nDots].u.R = dr;
	rgbw[nDots].u.G = dg;
	rgbw[nDots].u.B = db;
	FlushOneFrame(rgbw);
}
//-------------------------------------------------------------------------------
//�ӳ�������:SendSameColor(unsigned char *ptr,unsigned char cnt)
//���ܣ���ͬ��ɫ�ĵ㷢��cnt��
//���������յĲ�����һ��ָ�룬ָ�����ص���ɫ���飬cnt���ݷ��͸���
//-------------------------------------------------------------------------------
void SendSameColor(unsigned char *ptr,unsigned char cnt)
{
	unsigned char k;

	ResetDataFlow();				 //����֡��λ�ź�

	for(k=0;k<cnt;k++)				 //����һ֡���ݣ�SNUM�ǰ���LED�ĸ���
	{
		SendOnePix(&ptr[0]);
	}

	ResetDataFlow();				 //����֡��λ�ź�
}
//-------------------------------------------------------------------------------
//�ӳ�������:SendOneFrameFrom(unsigned char i,unsigned char *ptr)
//���ܣ���ָ�������ص㿪ʼ����һ֡���ݣ�������������������ݣ�
//���������յĲ�����һ��ָ�룬�˺����Ὣ��ָ��ָ��ĵ�ַ����֡���ݷ���
//		i:������ĵ�0���������ݷ��͵���i�����ص㣨��0�������ǰ��ϱ��Ϊ01�����أ�
//˵������ԭ����Ӧ��һ�����ص����ݻᷢ�͵���i�����ص㣨LED����
//-------------------------------------------------------------------------------
void SendOneFrameFrom(unsigned char i,unsigned char *ptr)
{
	unsigned char k;

	ResetDataFlow();				 //����֡��λ�ź�

   	for(k=(SNUM-i);k<SNUM;k++)		 //����һ֡����
	{
		SendOnePix(&ptr[(3*k)]);
	}
	for(k=0;k<(SNUM-i);k++)
	{
		SendOnePix(&ptr[(3*k)]);
	}

	ResetDataFlow();				 //����֡��λ�ź�
}

//-------------------------------------------------------------------------------
//�ӳ�������:SendOneFrameSince(unsigned char i,unsigned char *ptr)
//���ܣ��ӵ�i�����ص�����ݿ�ʼ����һ֡���ݣ�������������������ݣ�
//���������յĲ�����һ��ָ�룬�˺����Ὣ��ָ��ָ��ĵ�ַ����֡���ݷ���
//		i:������ĵ�i���������ݷ��͵���1�����ص�
//˵������ԭ����Ӧ��i���ص����ݻᷢ�͵���1�����ص㣨LED���ϣ���i+1�����ص������
//		���͵���2��������
//-------------------------------------------------------------------------------
void SendOneFrameSince(unsigned char i,unsigned char *ptr)
{
	unsigned char k;

	ResetDataFlow();				 //����֡��λ�ź�

	for(k=i;k<SNUM;k++)				 //����һ֡����
	{
		SendOnePix(&ptr[(3*k)]);
	}
	for(k=0;k<i;k++)
	{
		SendOnePix(&ptr[(3*k)]);
	}
	ResetDataFlow();				 //����֡��λ�ź�
}
/**********************************THE END**********************************/ 

 

