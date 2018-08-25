/*----------------------------------------------------------------------------   
 * 项目名称:

     RGB LED驱动程序

 * 功能描述:

     1、将整个数组的数据发送得到LED进行显示
	 2、从指定像素开始将整个数组的数据发送到LED显示
	 3、发送复位信号

 * 版权信息:

     (c) 飞翼电子, 2014.

 * 历史版本:
     2014-03-15:
       - 初始版本 V1.0.0;

 * 配置说明:
     MCU:             STC15W204S
     晶振:      	  内部晶振：33MHz
     扩展模块:  	  -
     软件:            Keil.C51.V9.01

 * 备注:

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
//子程序名称:IO_Init(void)
//功能：端口初始化
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
//子程序名称:ResetDateFlow(void)
//功能：复位，为下一次发送做准备，
//说明：将DI置位为0后，延时约65us
//-------------------------------------------------------------------------------
//void ResetDataFlow(void)
//{
//	unsigned char i,j;
//	//DI=0;					//DI置为0后，延时50us以上，实现帧复位
//	HAL_GPIO_WritePin(DO_GPIO_Port, DO_Pin, GPIO_PIN_RESET);
//	for(i=0;i<15;i++)		//此处33Mhz时延时65us
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
	//DI=0;					//DI置为0后，延时50us以上，实现帧复位
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
//	//DI=0;					//DI置为0后，延时50us以上，实现帧复位
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
//子程序名称:SendOnePix(unsigned char *ptr)
//功能：发送一个像素点的24bit数据
//参数：接收的参数是一个指针，此函数会将此指针指向的地址的连续的三个Byte的数据发送
//说明：在主函数中直接调用此函数时，在整个帧发送开始前需要先执行 ResetDataFlow()
//		数据是按归零码的方式发送，速率为800KBPS
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
			if(temp&0x80)		 //从高位开始发送
			{
				//DI=1;			 //发送“1”码
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
			else				//发送“0”码
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
			temp=(temp<<1);		 //左移位
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
			if(temp&0x80000000)		 //从高位开始发送
			{
				//DI=1;			 //发送“1”码
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
			else				//发送“0”码
			{
				DI1;  // 0.35us
				my_delay(1);
				__nop();__nop();__nop();__nop();//__nop();
				DI0;  // 0.8us
				my_delay(3);
			}
			temp=(temp<<1);		 //左移位
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
			if(temp&0x80)		 //从高位开始发送
			{
				//DI=1;			 //发送“1”码
				HAL_GPIO_WritePin(DO_GPIO_Port, DO_Pin, GPIO_PIN_SET);
				_nop_();		 //不可省略的nop(),延时指定时间作用，晶振频率33MHz
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
			else				//发送“0”码
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
			temp=(temp<<1);		 //左移位
		}
	}
}
#endif
//-------------------------------------------------------------------------------
//子程序名称:SendOneFrame(unsigned char *ptr)
//功能：发送一帧数据（即发送整个数组的数据）
//参数：接收的参数是一个指针，此函数会将此指针指向的地址的整个数组的数据发送
//-------------------------------------------------------------------------------
void SendOneFrame(unsigned char *ptr)
{
	unsigned char k;
	//__HAL_TIM_SET_COUNTER(&htim1, 0);
	//HAL_TIM_Base_Start(&htim1);
	
#if 0
	ResetDataFlow();				 //发送帧复位信号
	for(k=0;k<SNUM;k++)				 //发送一帧数据，SNUM是板子LED的个数
	{
		SendOnePix(&ptr[(3*k)]);
	}
#else
	const uint16_t len=SNUM*3;
	ResetDataFlow();				 //发送帧复位信号
	for(k=0;k<len;k+=3)				 //发送一帧数据，SNUM是板子LED的个数
	{
		SendOnePix(&ptr[k]);
	}
#endif
	ResetDataFlow();				 //发送帧复位信号
	//HAL_TIM_Base_Stop(&htim1);
}
void FlushOneFrame(rgbw_t *rgbw)
{
	uint16_t k=0;
	uint16_t len=SNUM;
	uint16_t i=0;
	uint32_t temp=0;
	ResetDataFlow();				 //发送帧复位信号
	for(k=0;k<len;k++)				 //发送一帧数据，SNUM是板子LED的个数
	{
		temp = rgbw[k].rgbw;
		for(i=0;i<24;i++)
		{
			if(temp&0x80000000)		 //从高位开始发送
			{
				//DI=1;			 //发送“1”码
				DI1;  // 0.7us
				my_delay(5);
				DI0;  // 0.6us
				my_delay(1); //my_delay(T1L);
				__nop();
			}
			else				//发送“0”码
			{
				DI1;  // 0.35us
				my_delay(1);
				__nop();__nop();__nop();//__nop();//__nop();
				DI0;  // 0.8us
				my_delay(3);
				__nop();__nop();
			}
			temp=(temp<<1);		 //左移位
		}
	}
	ResetDataFlow();				 //发送帧复位信号
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
//子程序名称:SendSameColor(unsigned char *ptr,unsigned char cnt)
//功能：相同颜色的点发送cnt次
//参数：接收的参数是一个指针，指向像素点颜色数组，cnt传递发送个数
//-------------------------------------------------------------------------------
void SendSameColor(unsigned char *ptr,unsigned char cnt)
{
	unsigned char k;

	ResetDataFlow();				 //发送帧复位信号

	for(k=0;k<cnt;k++)				 //发送一帧数据，SNUM是板子LED的个数
	{
		SendOnePix(&ptr[0]);
	}

	ResetDataFlow();				 //发送帧复位信号
}
//-------------------------------------------------------------------------------
//子程序名称:SendOneFrameFrom(unsigned char i,unsigned char *ptr)
//功能：从指定的像素点开始发送一帧数据（即发送整个数组的数据）
//参数：接收的参数是一个指针，此函数会将此指针指向的地址的整帧数据发送
//		i:把数组的第0个像素数据发送到第i个像素点（第0个像素是板上标号为01的像素）
//说明：即原本对应第一个像素的数据会发送到第i个像素点（LED）上
//-------------------------------------------------------------------------------
void SendOneFrameFrom(unsigned char i,unsigned char *ptr)
{
	unsigned char k;

	ResetDataFlow();				 //发送帧复位信号

   	for(k=(SNUM-i);k<SNUM;k++)		 //发送一帧数据
	{
		SendOnePix(&ptr[(3*k)]);
	}
	for(k=0;k<(SNUM-i);k++)
	{
		SendOnePix(&ptr[(3*k)]);
	}

	ResetDataFlow();				 //发送帧复位信号
}

//-------------------------------------------------------------------------------
//子程序名称:SendOneFrameSince(unsigned char i,unsigned char *ptr)
//功能：从第i个像素点的数据开始发送一帧数据（即发送整个数组的数据）
//参数：接收的参数是一个指针，此函数会将此指针指向的地址的整帧数据发送
//		i:把数组的第i个像素数据发送到第1个像素点
//说明：即原本对应第i像素的数据会发送到第1个像素点（LED）上，第i+1个像素点的数据
//		发送到第2个像素上
//-------------------------------------------------------------------------------
void SendOneFrameSince(unsigned char i,unsigned char *ptr)
{
	unsigned char k;

	ResetDataFlow();				 //发送帧复位信号

	for(k=i;k<SNUM;k++)				 //发送一帧数据
	{
		SendOnePix(&ptr[(3*k)]);
	}
	for(k=0;k<i;k++)
	{
		SendOnePix(&ptr[(3*k)]);
	}
	ResetDataFlow();				 //发送帧复位信号
}
/**********************************THE END**********************************/ 

 

