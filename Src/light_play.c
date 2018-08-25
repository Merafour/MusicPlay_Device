#include <stdio.h>																											 
//#include "NUC1xx.h"
//#include "DrvSYS.h"
//#include "DrvGPIO.h"
//#include "DrvUART.h"
//#include "chase.h"
//#include "TM1803.h"
//#include "TM1809.h"
//#include "LPD1109.h"
//#include "SM16716.h"
//#include "LPD6803.h"
//#include "LPD8803.h"
#include "WS2801.h"
#include "WS2811.h"
//#include "wifi_data.h"
//#include "chase.h"
//#include "delay.h"
//#include "RoadWell_bitband.h"
//#include "colorbar_tab.h"
//#include "cmsis_os.h"
#include "light_play.h"
//static uint32_t time[50]={0};
static uint32_t now=0;
static uint32_t _now=0;
//uint8_t real_time=0;
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

int32_t move_index=0;
int32_t print_frequency=0; // 0:print

#define figer_speed     20

uint16 t=0;

uint8 gradual_tab[10][3];
uint8 colorbar_temp4[10][3];
uint8 colorbar_temp5[30][3];

uint8 cs_mode_num_temp;
extern uint8    IC_Model;
extern uint16 	IC_Length;
extern uint16   Dots;
extern uint16   Dots2;
extern Page_two       Second_page;

extern Page_three     Third_page;
extern Page_four      Fouth_page;

extern Page_five      Fifth_page;
extern Page_six       Sixth_page;

extern uint8  Ch_flag;
extern uint8 CS_mode_num;


uint8 user_speed;

uint8  r_flag,g_flag,b_flag;
uint8  RGB_D[3]={0,0,0};

uint8 jump_flag;

uint8 down_flag[3]={1,1,1};
uint8 date_update;
uint8 cs_date_updata;

uint8 jump_out_flag=0;

//uint8 data_buffer[1000][3];
uint8_t rgb_index[1000];
//typedef struct{
//		uint8_t r;
//		uint8_t g;
//		uint8_t b;
//}rgb;

play_device device={0};

static void default_start_init(void){}
static void default_Outone(uint8_t dr,uint8_t dg,uint8_t db){}
static void default_BrushOut(uint16_t nDots,uint8_t dr,uint8_t dg,uint8_t db){}
//static void Gradual_mode(uint8 speed,uint8 r,uint8 g,uint8 b,uint8 STA);
//static void Flash_mode1(uint16 Dots,uint8 sp,uint8 r,uint8 g,uint8 b);
static void default_reset(uint16_t nDots){}

void play_start(void)
{
		//printf("\r\n@%d %s  %s", __LINE__, __func__, __DATE__);
		vTaskSuspendAll();
		//portNVIC_SYSTICK_CTRL &= ~(portNVIC_SYSTICK_INT );
		device.start_init();
}
void play_end(const int32_t move_index_max)
{
		//printf("\r\n@%d %s  %s", __LINE__, __func__, __DATE__);
		device.reset(IC_Length);
		//portNVIC_SYSTICK_CTRL = portNVIC_SYSTICK_CLK | portNVIC_SYSTICK_INT | portNVIC_SYSTICK_ENABLE;
		xTaskResumeAll();
	  move_index++; t++;
		if(move_index>=move_index_max)
		{
				move_index = 0;
				t = 0;
		}
}
void start_init(void)
{
		printf("\r\n@%d %s  %s", __LINE__, __func__, __DATE__);
}
void Outone(uint8_t dr,uint8_t dg,uint8_t db)
{
		printf("\r\n@%d %s  %s", __LINE__, __func__, __DATE__);
}
void BrushOut(uint16 nDots,uint8_t dr,uint8_t dg,uint8_t db)
{
		printf("\r\n@%d %s  %s", __LINE__, __func__, __DATE__);
}
void reset(uint16 nDots)
{
		printf("\r\n@%d %s  %s", __LINE__, __func__, __DATE__);
}

void create_device(void)
{
				device.start_init = default_start_init;
				device.reset = default_reset;
				device.Outone = default_Outone;
				device.BrushOut = default_BrushOut;
		switch(IC_Model)
		{
			case SM16716:
				new_device_SM16716(&device);
				break;
			case TM1803:
				new_device_TM1803(&device);
				break;
			case WS2801:
				new_device_WS2801(&device);
				break;
			case TM1809:
				new_device_TM1809(&device);
				break;
			case LPD1109:
				new_device_LPD1109(&device);
				break;
			case LPD6803:
				new_device_LPD6803(&device);
				break;
			case LPD8803:
				new_device_LPD8803(&device);
				break;
			default:
				device.start_init = default_start_init;
				device.reset = default_reset;
				device.Outone = default_Outone;
				device.BrushOut = default_BrushOut;
				break;
		}
}


void chase_delay_bak(uint16_t sp)
{
    uint16_t i,j;
	uint32_t speed;
	speed=sp*sp;
	speed=speed>>4;
	do
	{	 
	   	  for(i=1200-IC_Length;i>0;i--)
		  {
		  	  for(j=3;j>0;j--)
			  {
			  	  if(Ch_flag) return;
			  }
			  if(Ch_flag) return;
		  }	   
	}while(speed--);
}
void chase_delay(uint16_t sp)
{
	uint16_t speed;
//	speed=sp>>1;
	speed=sp*sp;
	speed=speed>>6;
	speed += 1;
	do
	{
			osDelay(1);
			if(Ch_flag) return;
	}while(speed--);
}

void chase_delay2(uint16_t sp)
{
    uint16 i,j,k;
	uint32_t speed;
	speed=sp*sp;
	speed=speed>>4;
	j=(1200-IC_Length)/10;
#if 0
	do
	{	   
	   	  for(i=j;i>0;i--)
		  {
		  	 for(k=0;k<2;k++)
			 {
			 	if(Ch_flag) return;
			 }
			  if(Ch_flag) return;
		  }	   
	}while(speed--);
#else
	//speed *= j;
	do{
			osDelay(1);
			if(Ch_flag) return;
	}while(speed--);
#endif
}


void chase_delay3(uint16_t sp)
{
    uint16 i,j;
	uint32_t speed;
	speed=sp*sp;
	speed=speed>>3;
	j=(1200-IC_Length)/10;
	do
	{	   
	   	for(i=j;i>0;i--)
		  {		  	 			 				
			  if(Ch_flag) return;			
		  }	
		  if(Ch_flag) return; 
	}while(speed--);

	
}

void Gradual_mode(uint8 speed,uint8 r,uint8 g,uint8 b,uint8 STA)
{
  if(STA!=PAUSE)
  {
   if(r==1)
      {
	      if(r_flag==0)
		     {
			     if(RGB_D[0]<0xE0)
				    {
					    ++RGB_D[0];
						   if(RGB_D[0]>20)
						      {
							    ++RGB_D[0];
							  }
						  
					}
				 else
					{
					   r_flag=1;
					}
			}
		  else
			{
			   if(RGB_D[0]>0x00)
			    {
				   --RGB_D[0];
				       if(RGB_D[0]>20)
					      {
						      --RGB_D[0];
						  }
				}
			   else
			      {
				      r_flag=0;	  jump_flag=1;
				  }
			}
	  }
  
  
  if(g==1)
      {
	      if(g_flag==0)
		     {
			     if(RGB_D[1]<0xE0)
				    {
					    ++RGB_D[1];
						   if(RGB_D[1]>20)
						      {
							    ++RGB_D[1];
							  }
					}
				 else
					{
					   g_flag=1;
					}
			}
		  else
			{
			   if(RGB_D[1]>0x00)
			    {
				   --RGB_D[1];
				       if(RGB_D[1]>20)
					      {
						      --RGB_D[1];
						  }
				}
			   else
			      {
				      g_flag=0;		   jump_flag=1;
				  }
			}
	  }
	  
	  
	if(b==1)
      {
	      if(b_flag==0)
		     {
			     if(RGB_D[2]<0xE0)
				    {
					    ++RGB_D[2];
						   if(RGB_D[2]>20)
						      {
							    ++RGB_D[2];
							  }
					}
				 else
					{
					   b_flag=1;			
					}
			}
		  else
			{
			   if(RGB_D[2]>0x00)
			    {
				   --RGB_D[2];
				       if(RGB_D[2]>20)
					      {
						      --RGB_D[2];
						  }
				}
			   else
			      {
				      b_flag=0;			jump_flag=1;
				  }
			}
	  }
    }
    

	   device.BrushOut(IC_Length,RGB_D[0],RGB_D[1],RGB_D[2]);
	   
	   chase_delay2(speed);				   //
  	 chase_delay2(5);  
}

//uint8 dim_rgb=0,dim_rgb_flag=0;
//uint8  r_temp=255,g_temp=0,b_temp=0;

void Colorbar_Mode(uint8 colorbar[10][3],uint8 CHM,uint8 DIR,uint8 SP,uint8 Brt,uint8 STA)
{
	//uint32_t time=0;
	if(0==print_frequency)
	{
			printf("\r\n@%d %s  CHM:[%d]\n", __LINE__, __func__, CHM);
			printf("\r\n@%d %s  DIR:[%d]\n", __LINE__, __func__, DIR);
			printf("\r\n@%d %s  SP:[%d]\n", __LINE__, __func__, SP);
			printf("\r\n@%d %s  Brt:[%d]\n", __LINE__, __func__, Brt);
			printf("\r\n@%d %s  STA:[%d]\n", __LINE__, __func__, STA);
			printf("\r\n@%d %s  Color_Mode:[%d]\n", __LINE__, __func__, Fouth_page.Color_Mode);
	}
	if(STA==OFF)
	{
			device.BrushOut(IC_Length,0,0,0);
			return;
	}
	if((STA!=ON) && (STA!=PAUSE))
	{
			device.BrushOut(IC_Length,0,0,0);
			return;
	}
	//CHM = 0;
	//Fouth_page.Color_Mode = 8;
	//IC_Length = 60;
	switch(CHM)
	{
		case 1:
		{
			if(0==print_frequency)printf("\r\n@%d %s", __LINE__, __func__);
			//if(STA!=OFF)Colorbar_flow(colorbar,SP,DIR,Brt,STA);
			//else BrushOut(IC_Length,0,0,0);
			//time = HAL_GetTick();
			Colorbar_flow(colorbar,SP,DIR,Brt,STA);
			//time = HAL_GetTick()-time;
			//printf("\r\n%s [%d]", __func__, time);
		}
			break;
		case 2:
			{
				if(0==print_frequency)printf("\r\n@%d %s", __LINE__, __func__);
#if 1
				colorbar_section(colorbar,SP,DIR,Brt,STA);
#else
				if(STA!=OFF)
				 colorbar_section(colorbar,SP,DIR,Brt,STA);
				else
					BrushOut(IC_Length,0,0,0);
#endif
			}
			break;
		case 3:
			{
				if(0==print_frequency)printf("\r\n@%d %s", __LINE__, __func__);
#if 0
				colorbar_flash(colorbar,SP,DIR,STA);
#else
					if(STA!=OFF)
					colorbar_flash(colorbar,SP,DIR,STA);
				else
					BrushOut(IC_Length,0,0,0);
#endif
			}
			break;
		case 4:
			{
				if(0==print_frequency)printf("\r\n@%d %s", __LINE__, __func__);
#if 1
				colorbar_flash2(colorbar_temp5,SP,DIR,STA);
#else
				if(IC_Length>59) colorbar_flash2(colorbar_temp5,SP,DIR,STA);
				else colorbar_flash(colorbar,SP,DIR,STA);
#endif
			}
			break;
		case 0:
		default:
		{
				if(0==print_frequency)printf("\r\n@%d %s", __LINE__, __func__);
				//if(STA!=OFF)	 
				{
				 if(Fouth_page.Color_Mode<7)
				 {
						if(Fouth_page.Color_Mode==0)
						 Gradual_mode(SP,1,0,0,STA);
					 //Gradual_rgb(SP,STA);
					 //rgb_gradual_change();
					if(Fouth_page.Color_Mode==1)
						 Gradual_mode(SP,0,1,0,STA);
					if(Fouth_page.Color_Mode==2)
						 Gradual_mode(SP,0,0,1,STA);

					if(Fouth_page.Color_Mode==3)
						 Gradual_mode(SP,1,1,0,STA);
					if(Fouth_page.Color_Mode==4)
						 Gradual_mode(SP,1,0,1,STA);
					if(Fouth_page.Color_Mode==5)
						 Gradual_mode(SP,0,1,1,STA);
					if(Fouth_page.Color_Mode==6)
						 Gradual_mode(SP,1,1,1,STA);
					 //Gradual_rgb(SP,STA);
				 }
				 else
				 {
						colorbar_smooth(colorbar,SP,DIR,STA);	
				 }
				}
				//else
				//	BrushOut(IC_Length,0,0,0);
			}
			break;
	}
}

void Defined_Mode(Page_four F_page)
{
    
	Colorbar_Mode(colorbar_temp4,F_page.Change_Mode,F_page.Direction_Mode,F_page.Speed,F_page.Brt,F_page.State);

}

