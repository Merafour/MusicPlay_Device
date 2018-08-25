#include <stdio.h>																											 
//#include "NUC1xx.h"
//#include "DrvSYS.h"
//#include "DrvGPIO.h"
#include "WS2801.h"
//#include "delay.h"
//#include "RoadWell_bitband.h"
#include "gpio.h"

//#define HI_SDO()   GPIOB10= 1  
//#define LOW_SDO()  GPIOB10= 0  

//#define HI_SCLK()   GPIOB9= 1    
//#define LOW_SCLK()  GPIOB9= 0  
#define HI_SCLK()   HAL_GPIO_WritePin(DI_GPIO_Port, DI_Pin, GPIO_PIN_SET)//GPIOB9= 1  
#define LOW_SCLK()  HAL_GPIO_WritePin(DI_GPIO_Port, DI_Pin, GPIO_PIN_RESET)//GPIOB9= 0  
#define HI_SDO()    HAL_GPIO_WritePin(DO_GPIO_Port, DO_Pin, GPIO_PIN_SET)//GPIOB10= 1    
#define LOW_SDO()   HAL_GPIO_WritePin(DO_GPIO_Port, DO_Pin, GPIO_PIN_RESET)//GPIOB10= 0 

void DrvSYS_Delay(uint32_t us)
{
#if 0
    SysTick->LOAD = us * CyclesPerUs;
    SysTick->VAL  = (0x00);
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;

    /* Waiting for down-count to zero */
    while((SysTick->CTRL & (1 << 16)) == 0);
#endif
	us *= 12; //us
//	us += SysTick->VAL;
//	if(us<SysTick->VAL) while(us<SysTick->VAL);
//	while(us>SysTick->VAL);
	while(--us) __nop();
}

void WS2801_start_init(void)
{	 

}

void WS2801_reset(void)
{
//    LOW_SCLK();
//		LOW_SDO();
//		DrvSYS_Delay(500);
} 


void WS2801_Outone_bak(uint8 dr,uint8 dg,uint8 db)//输出3个5位RGB颜色数据子程序，参数格式：红颜色数据、    //绿颜色数据、蓝颜色数据
{   	
		unsigned char j,mask;	 
	
		mask=0x80;

		for(j=0;j<8;j++)
		{ 

				LOW_SCLK(); 

				DrvSYS_Delay(1);

				if(mask & dr)   
				{
						HI_SDO();
				} 
				else 
				{
						LOW_SDO();		   
				}		 		            

				HI_SCLK();	

				mask>>=1; 

				DrvSYS_Delay(1);		 

		}
      
   
    mask=0x80;

		for(j=0;j<8;j++)
		{ 

				LOW_SCLK(); 

				DrvSYS_Delay(1);

				if(mask & dg)   
				{
						HI_SDO();
				} 
				else 
				{
						LOW_SDO();		   
				}		 		            

				HI_SCLK();	

				mask>>=1; 

				DrvSYS_Delay(1);		 

		}
		
		
		mask=0x80;

		for(j=0;j<8;j++)
		{ 

				LOW_SCLK(); 

				DrvSYS_Delay(1);

				if(mask & db)   
				{
						HI_SDO();
				} 
				else 
				{
						LOW_SDO();		   
				}		 		            

				HI_SCLK();	

				mask>>=1; 

				DrvSYS_Delay(1);		 

		}
    
}

void WS2801_OutoneByte(uint8 data)//输出3个5位RGB颜色数据子程序，参数格式：红颜色数据、    //绿颜色数据、蓝颜色数据
{   	
		unsigned char j,mask;	 
		mask=0x80;
		for(j=0;j<8;j++)
		{ 
				LOW_SCLK(); 
				//DrvSYS_Delay(1);
				if(mask & data)   
				{
						HI_SDO();
				} 
				else 
				{
						LOW_SDO();		   
				}		 		            
				HI_SCLK();	
				mask>>=1; 
				//DrvSYS_Delay(1);		 
		}    
}
//#include "usart.h"
extern void OS_ENTER_CRITICAL(void);
extern void OS_EXIT_CRITICAL(void);
/* Constants required to manipulate the NVIC. */
#define portNVIC_SYSTICK_CTRL		( *( ( volatile uint32_t *) 0xe000e010 ))
#define portNVIC_SYSTICK_LOAD		( *( ( volatile uint32_t *) 0xe000e014 ))
#define portNVIC_SYSTICK_CURRENT_VALUE	( * ( ( volatile uint32_t * ) 0xe000e018 ) )
#define portNVIC_INT_CTRL			( ( volatile uint32_t *) 0xe000ed04 )
#define portNVIC_SYSPRI2			( ( volatile uint32_t *) 0xe000ed20 )
#define portNVIC_SYSTICK_CLK		0x00000004
#define portNVIC_SYSTICK_INT		0x00000002
#define portNVIC_SYSTICK_ENABLE		0x00000001
#define portNVIC_SYSTICK_COUNT_FLAG		( 1UL << 16UL )
#define portNVIC_PENDSVSET			0x10000000
#define portMIN_INTERRUPT_PRIORITY	( 255UL )
#define portNVIC_PENDSV_PRI			( portMIN_INTERRUPT_PRIORITY << 16UL )
#define portNVIC_SYSTICK_PRI		( portMIN_INTERRUPT_PRIORITY << 24UL )
void WS2801_Outone(uint8 dr,uint8 dg,uint8 db)//输出3个5位RGB颜色数据子程序，参数格式：红颜色数据、    //绿颜色数据、蓝颜色数据
{
		//__HAL_UART_DISABLE_IT(&huart1,UART_IT_RXNE);
		WS2801_OutoneByte(dr);
		WS2801_OutoneByte(dg);
		WS2801_OutoneByte(db);
		//__HAL_UART_CLEAR_IT(&huart1, UART_CLEAR_OREF);
		//__HAL_UART_ENABLE_IT(&huart1,UART_IT_RXNE);
}

void WS2801_BrushOut(uint16 nDots,uint8 dr,uint8 dg,uint8 db)//输出ndots点相同灰度颜色子程序，参数格式：点数、红颜色数据、                                                 //绿颜色数据、蓝颜色数据
{	 
	uint16 i;
		//vTaskSuspendAll();
		//__asm("CPSID  I");
		portNVIC_SYSTICK_CTRL &= ~(portNVIC_SYSTICK_INT );
	WS2801_start_init();
	
	for(i=0;i<nDots;i++)
	{  
	    WS2801_Outone(dr,dg,db);
	}
    WS2801_reset();
		portNVIC_SYSTICK_CTRL = portNVIC_SYSTICK_CLK | portNVIC_SYSTICK_INT | portNVIC_SYSTICK_ENABLE;
		//OS_EXIT_CRITICAL(); //恢复中断
		//xTaskResumeAll();
}
#include "light_play.h"
void new_device_WS2801(play_device *_device)
{
		_device->start_init = WS2801_start_init;
		_device->reset = WS2801_reset;
		_device->Outone = WS2801_Outone;
		_device->BrushOut = WS2801_BrushOut;
}

