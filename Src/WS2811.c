#include <stdio.h>																											 
//#include "NUC1xx.h"
//#include "DrvSYS.h"
//#include "DrvGPIO.h"
#include "WS2811.h"
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


void WS2811_reset(void)
{
   LOW_SDO();
   DrvSYS_Delay(50);
}



void WS2811_Outone(uint8 dr,uint8 dg,uint8 db)                                    
{   
    uint8 j,mask; 
    mask=0x80;
    for(j=8;j>0;j--)
    {   
	    if(mask & dr)    
		{		   
		   HI_SDO();
		   HI_SDO();
		   HI_SDO();
		   HI_SDO();
		   HI_SDO();  //5

		   HI_SDO();
		   HI_SDO();
		   HI_SDO();
		   HI_SDO();
		   HI_SDO();  //10

		   HI_SDO();
		   HI_SDO();
		   HI_SDO();
		   HI_SDO();
		   HI_SDO();  //15

		   HI_SDO();
		   HI_SDO();
		   HI_SDO();
		   HI_SDO();
		   HI_SDO(); //20

		   HI_SDO();
		   HI_SDO();
		   HI_SDO();
		   HI_SDO();
		   HI_SDO(); //25

		   HI_SDO();
		   HI_SDO();
		   HI_SDO();
		   HI_SDO();
		   HI_SDO(); //30

		   HI_SDO();
		   HI_SDO();
		   HI_SDO();
		   HI_SDO();
		   HI_SDO(); //35

		   HI_SDO();
		   HI_SDO();
		   HI_SDO();
		   HI_SDO();
		   HI_SDO(); //40

		   HI_SDO();
		   HI_SDO();
		   HI_SDO();
		   HI_SDO();
		   HI_SDO(); //45

		   HI_SDO();
		   HI_SDO();
		   HI_SDO(); //48

		   LOW_SDO();
		   LOW_SDO();  //3
//		   LOW_SDO();  
//		   LOW_SDO();
//		   LOW_SDO();  //5

		   //LOW_SDO();
		   //LOW_SDO();  //7
		  
		}
        else            
		{
		   HI_SDO();
		   HI_SDO();
		   HI_SDO();
		   HI_SDO();
		   HI_SDO();  //5

		   HI_SDO();
		   HI_SDO();
		   HI_SDO();
		   HI_SDO();
		   HI_SDO();  //10
	
//	       HI_SDO();
//		   HI_SDO();  //12
		
		   LOW_SDO();
		   LOW_SDO();
		   LOW_SDO();
		   LOW_SDO();
		   LOW_SDO();    //5

		   LOW_SDO();
		   LOW_SDO();
		   LOW_SDO();
		   LOW_SDO();
		   LOW_SDO();   //10

		   LOW_SDO();
		   LOW_SDO();
		   LOW_SDO();
		   LOW_SDO();
		   LOW_SDO();  //15

		   LOW_SDO();
		   LOW_SDO();
		   LOW_SDO();
		   LOW_SDO();
		   LOW_SDO();  //20

		   LOW_SDO();
		   LOW_SDO();
		   LOW_SDO();
		   LOW_SDO();
		   LOW_SDO(); //25

		   LOW_SDO();
		   LOW_SDO();
		   LOW_SDO();
		   LOW_SDO();
		   LOW_SDO(); //30

		   LOW_SDO();
		   LOW_SDO();
		   LOW_SDO();
		   LOW_SDO();
		   LOW_SDO(); //35

		   LOW_SDO();
		   LOW_SDO();
		   LOW_SDO();
		   LOW_SDO();
		   LOW_SDO(); //40

		   LOW_SDO(); //41
		  
		}        
         mask>>=1;      
		     
	}
      
    mask=0x80;
    for(j=8;j>0;j--)
    { 
	    if(mask & dg)    
		{		   	
		   HI_SDO();
		   HI_SDO();
		   HI_SDO();
		   HI_SDO();
		   HI_SDO();  //5

		   HI_SDO();
		   HI_SDO();
		   HI_SDO();
		   HI_SDO();
		   HI_SDO();  //10

		   HI_SDO();
		   HI_SDO();
		   HI_SDO();
		   HI_SDO();
		   HI_SDO();  //15

		   HI_SDO();
		   HI_SDO();
		   HI_SDO();
		   HI_SDO();
		   HI_SDO(); //20

		   HI_SDO();
		   HI_SDO();
		   HI_SDO();
		   HI_SDO();
		   HI_SDO(); //25

		   HI_SDO();
		   HI_SDO();
		   HI_SDO();
		   HI_SDO();
		   HI_SDO(); //30

		   HI_SDO();
		   HI_SDO();
		   HI_SDO();
		   HI_SDO();
		   HI_SDO(); //35

		   HI_SDO();
		   HI_SDO();
		   HI_SDO();
		   HI_SDO();
		   HI_SDO(); //40

		   HI_SDO();
		   HI_SDO();
		   HI_SDO();
		   HI_SDO();
		   HI_SDO(); //45

		   HI_SDO();
		   HI_SDO();
		   HI_SDO(); //48

		   LOW_SDO();
		   LOW_SDO();  //3
//		   LOW_SDO();  
//		   LOW_SDO();
//		   LOW_SDO();  //5

		   //LOW_SDO();
		   //LOW_SDO();  //7
		}
        else            
		{
		   HI_SDO();
		   HI_SDO();
		   HI_SDO();
		   HI_SDO();
		   HI_SDO();  //5

		   HI_SDO();
		   HI_SDO();
		   HI_SDO();
		   HI_SDO();
		   HI_SDO();  //10
	
//	       HI_SDO();
//		   HI_SDO();  //12
		
		   LOW_SDO();
		   LOW_SDO();
		   LOW_SDO();
		   LOW_SDO();
		   LOW_SDO();    //5

		   LOW_SDO();
		   LOW_SDO();
		   LOW_SDO();
		   LOW_SDO();
		   LOW_SDO();   //10

		   LOW_SDO();
		   LOW_SDO();
		   LOW_SDO();
		   LOW_SDO();
		   LOW_SDO();  //15

		   LOW_SDO();
		   LOW_SDO();
		   LOW_SDO();
		   LOW_SDO();
		   LOW_SDO();  //20

		   LOW_SDO();
		   LOW_SDO();
		   LOW_SDO();
		   LOW_SDO();
		   LOW_SDO(); //25

		   LOW_SDO();
		   LOW_SDO();
		   LOW_SDO();
		   LOW_SDO();
		   LOW_SDO(); //30

		   LOW_SDO();
		   LOW_SDO();
		   LOW_SDO();
		   LOW_SDO();
		   LOW_SDO(); //35

		   LOW_SDO();
		   LOW_SDO();
		   LOW_SDO();
		   LOW_SDO();
		   LOW_SDO(); //40

		   LOW_SDO(); //41
		}        
         mask>>=1;   
		    
    }
    mask=0x80;
    for(j=8;j>0;j--)
    {  

	    if(mask & db)    
		{		   
		   HI_SDO();
		   HI_SDO();
		   HI_SDO();
		   HI_SDO();
		   HI_SDO();  //5

		   HI_SDO();
		   HI_SDO();
		   HI_SDO();
		   HI_SDO();
		   HI_SDO();  //10

		   HI_SDO();
		   HI_SDO();
		   HI_SDO();
		   HI_SDO();
		   HI_SDO();  //15

		   HI_SDO();
		   HI_SDO();
		   HI_SDO();
		   HI_SDO();
		   HI_SDO(); //20

		   HI_SDO();
		   HI_SDO();
		   HI_SDO();
		   HI_SDO();
		   HI_SDO(); //25

		   HI_SDO();
		   HI_SDO();
		   HI_SDO();
		   HI_SDO();
		   HI_SDO(); //30

		   HI_SDO();
		   HI_SDO();
		   HI_SDO();
		   HI_SDO();
		   HI_SDO(); //35

		   HI_SDO();
		   HI_SDO();
		   HI_SDO();
		   HI_SDO();
		   HI_SDO(); //40

		   HI_SDO();
		   HI_SDO();
		   HI_SDO();
		   HI_SDO();
		   HI_SDO(); //45

		   HI_SDO();
		   HI_SDO();
		   HI_SDO(); //48

		   LOW_SDO();
		   LOW_SDO();  //3
//		   LOW_SDO();  
//		   LOW_SDO();
//		   LOW_SDO();  //5

		   //LOW_SDO();
		   //LOW_SDO();  //7
		}
        else            
		{
		   HI_SDO();
		   HI_SDO();
		   HI_SDO();
		   HI_SDO();
		   HI_SDO();  //5

		   HI_SDO();
		   HI_SDO();
		   HI_SDO();
		   HI_SDO();
		   HI_SDO();  //10
	
//	       HI_SDO();
//		   HI_SDO();  //12
		
		   LOW_SDO();
		   LOW_SDO();
		   LOW_SDO();
		   LOW_SDO();
		   LOW_SDO();    //5

		   LOW_SDO();
		   LOW_SDO();
		   LOW_SDO();
		   LOW_SDO();
		   LOW_SDO();   //10

		   LOW_SDO();
		   LOW_SDO();
		   LOW_SDO();
		   LOW_SDO();
		   LOW_SDO();  //15

		   LOW_SDO();
		   LOW_SDO();
		   LOW_SDO();
		   LOW_SDO();
		   LOW_SDO();  //20

		   LOW_SDO();
		   LOW_SDO();
		   LOW_SDO();
		   LOW_SDO();
		   LOW_SDO(); //25

		   LOW_SDO();
		   LOW_SDO();
		   LOW_SDO();
		   LOW_SDO();
		   LOW_SDO(); //30

		   LOW_SDO();
		   LOW_SDO();
		   LOW_SDO();
		   LOW_SDO();
		   LOW_SDO(); //35

		   LOW_SDO();
		   LOW_SDO();
		   LOW_SDO();
		   LOW_SDO();
		   LOW_SDO(); //40

		   LOW_SDO(); //41
		}            
         mask>>=1;   	  
		 
    } 		 
}

void WS2811_BrushOut(uint16 nDots,uint8 dr,uint8 dg,uint8 db)                                                 
{
   uint16 i;

    for(i=0;i<nDots;i++)
    {   
	   WS2811_Outone(dr,dg,db);
	 
    }
   
    WS2811_reset();
}
