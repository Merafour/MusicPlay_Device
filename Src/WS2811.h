#ifndef __WS2811_H__
#define __WS2811_H__



#define  uint8   unsigned char
#define  uint16  unsigned int

//void Code0(void);
//void Code1(void);
void WS2811_reset(void);
void WS2811_Outone(uint8 dr,uint8 dg,uint8 db);
void WS2811_BrushOut(uint16 nDots,uint8 dr,uint8 dg,uint8 db);


#endif
