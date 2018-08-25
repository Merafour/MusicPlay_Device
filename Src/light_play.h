#ifndef __LIGHT_PLAY_H__
#define __LIGHT_PLAY_H__

#include <stdint.h>
//#include "wifi_data.h"

#define  uint8   unsigned char
#define  uint16  unsigned int

typedef struct{
		uint8_t r;
		uint8_t g;
		uint8_t b;
}rgb;

typedef struct{
		void (*start_init)(void);
		void (*reset)(uint16_t nDots);
		void (*Outone)(uint8_t dr, uint8_t dg, uint8_t db);
		void (*BrushOut)(uint16_t nDots, uint8_t dr, uint8_t dg, uint8_t db);
}play_device;

extern int32_t move_index;
extern int32_t print_frequency; // 0:print
extern uint8 gradual_tab[10][3];
extern uint8 colorbar_temp4[10][3];
extern uint8 colorbar_temp5[30][3];
extern uint8 RGB_D[3];
extern uint8 date_update;
extern uint8 cs_date_updata;
extern uint8 user_speed;
extern uint8_t rgb_index[1000];
extern uint8 cs_mode_num_temp;
extern uint16 t;
extern uint8 const chase_tab[10];
extern uint8 jump_flag;
extern uint8_t ll;
extern uint8 const cs_colorbar[64][10][3];
extern uint8 const chase_tab_dy[20];

//----------------------------------
extern uint8    IC_Model;
extern uint16 	IC_Length;
extern uint16   Dots;
extern uint16   Dots2;
//extern Page_two       Second_page;
//extern Page_three     Third_page;
//extern Page_four      Fouth_page;
//extern Page_five      Fifth_page;
//extern Page_six       Sixth_page;
extern uint8  Ch_flag;
extern uint8 CS_mode_num;

extern void play_start(void);
extern void play_end(const int32_t move_index_max);

//void start_init(void);
//void Outone(uint8 dr,uint8 dg,uint8 db);
//void BrushOut(uint16 nDots,uint8 dr,uint8 dg,uint8 db);
void Gradual_mode(uint8 speed,uint8 r,uint8 g,uint8 b,uint8 STA);
void Flash_mode1(uint16 Dots,uint8 sp,uint8 r,uint8 g,uint8 b);
//void reset(uint16 nDots);

void mode_flash(void);

//void Defined_Mode(Page_four F_page);



void customer_mode(uint8 CHM,uint8 DIR,uint8 SP,uint8 Brt,uint8 STA);

//-------------------
extern play_device device;
extern void create_device(void);
//extern void Colorbar_section_right(uint8_t colorbar[10][3],uint8_t brt,uint8_t br,uint8_t bg,uint8_t bb);
//extern void Colorbar_section_left(uint8_t colorbar[10][3],uint8_t brt,uint8_t br,uint8_t bg,uint8_t bb);
//extern void Colorbar_section_mid(uint8_t colorbar[10][3], uint8_t brt, uint8_t br, uint8_t bg, uint8_t bb);
//extern void Colorbar_section_side(uint8_t colorbar[10][3],uint8_t brt,uint8_t br,uint8_t bg,uint8_t bb);
extern void colorbar_section(uint8 colorbar[10][3],uint8 speed,uint8 dir,uint8 brt,uint8 STA);
extern void colorbar_flash(uint8_t colorbar[10][3],uint8_t speed,uint8_t dir,uint8_t STA);
extern void colorbar_flash2(uint8_t colorbar[30][3],uint8_t speed,uint8_t dir,uint8_t STA);
extern void Colorbar_flow(uint8 colorbar[10][3],uint8 speed,uint8 dir,uint8 brt,uint8 STA);
extern void colorbar_smooth(uint8 colorbar[10][3],uint8 SP,uint8 dir,uint8 STA);
extern void customer_mode(uint8_t CHM, uint8_t DIR, uint8_t SP, uint8_t Brt, uint8_t STA);
extern void mode_flash(void);


//---------------------------
extern void new_device_SM16716(play_device *_device);
extern void new_device_TM1803(play_device *_device);
extern void new_device_WS2801(play_device *_device);
extern void new_device_TM1809(play_device *_device);
extern void new_device_LPD1109(play_device *_device);
extern void new_device_LPD6803(play_device *_device);
extern void new_device_LPD8803(play_device *_device);


#endif //__LIGHT_PLAY_H__
