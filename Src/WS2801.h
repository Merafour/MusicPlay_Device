#ifndef __WS2801_H__
#define __WS2801_H__



#define  uint8   unsigned char
#define  uint16  unsigned int


void WS2801_start_init(void);
void WS2801_reset(void);
void WS2801_Outone(uint8 dr,uint8 dg,uint8 db);
void WS2801_BrushOut(uint16 nDots,uint8 dr,uint8 dg,uint8 db);


#endif
