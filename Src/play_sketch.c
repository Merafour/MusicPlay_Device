/**
  ******************************************************************************
  * File Name          : play_sketch.c
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "play_sketch.h"
//#include "cmsis_os.h"
#include <stdint.h>

extern void BrushOut(uint16_t nDots,uint8_t dr,uint8_t dg,uint8_t db);
static void BrushOutTest(const note *_note, const float R, const float G, const float B);
static void play_tone(const note *_note, const uint8_t _tone, const uint16_t _beat, const uint16_t _hold, const float R, const float G, const float B);
//static void play_tone_t(const uint8_t _tone, const uint16_t _beat, const uint16_t _hold, const float R, const float G, const float B);
static void play_tone_t(const uint8_t _tone, const uint16_t _beat, const uint16_t _hold, const uint16_t R, const uint16_t G, const uint16_t B);

typedef union {
	uint8_t buf[3];
	struct {
		uint8_t r;
		uint8_t g;
		uint8_t b;
	}rgb;
}RGB;

//static RGB rgb_list_l[][8]={
//	{{255,0,0},{205,50,0},{155,100,0},{105,150,0},{55,200,0},{5,250,0},{0,255,50},{0,205,100}},
//	{{255,0,0},{205,50,0},{155,100,0},{105,150,0},{55,200,0},{5,250,0},{0,255,50},{0,205,100}}, // -8
//};
//static RGB rgb_list_h[][8]={
//	{{255,0,0},{205,50,0},{155,100,0},{105,150,0},{55,200,0},{5,250,0},{0,255,50},{0,205,100}}, // +8
//	{{255,0,0},{205,50,0},{155,100,0},{105,150,0},{55,200,0},{5,250,0},{0,255,50},{0,205,100}}, // +16
//};
const static RGB rgb_list[3][8]={
	{{0,0,0},{0xFF,0,0},{0xF4,0xA4,0x60},{0xFF,0xFF,0},{0,0xFF,0},{0,0xFF,0xFF},{0,0,0xFF},{0x8B,0,0xFF}},
	{{0,0,0},{0xFF,0,0},{0xF4,0xA4,0x60},{0xFF,0xFF,0},{0,0xFF,0},{0,0xFF,0xFF},{0,0,0xFF},{0x8B,0,0xFF}},
	{{0,0,0},{0xFF,0,0},{0xF4,0xA4,0x60},{0xFF,0xFF,0},{0,0xFF,0},{0,0xFF,0xFF},{0,0,0xFF},{0x8B,0,0xFF}}
};
const static RGB note_list[3*8]={
	{0,0,0},{0xFF,0,0},{0xF4,0xA4,0x60},{0xFF,0xFF,0},{0,0xFF,0},{0,0xFF,0xFF},{0,0,0xFF},{0x8B,0,0xFF},
	{0,0,0},{0xFF,0,0},{0xF4,0xA4,0x60},{0xFF,0xFF,0},{0,0xFF,0},{0,0xFF,0xFF},{0,0,0xFF},{0x8B,0,0xFF},
	{0,0,0},{0xFF,0,0},{0xF4,0xA4,0x60},{0xFF,0xFF,0},{0,0xFF,0},{0,0xFF,0xFF},{0,0,0xFF},{0x8B,0,0xFF}
};
static const RGB rgb_list_base[8]={
	// ºì³È»ÆÂÌÇàÀ¶×Ï
	{0,0,0},{0xFF,0,0},{0xFF,0x7F,0},{0xFF,0xFF,0},{0,0xFF,0},{0,0xFF,0xFF},{0,0,0xFF},{0x8B,0,0xFF}
};
/**
  5 |1.113 |2.12^3 |1.135 |6--^6 |
  5.331 |2.12^3 |1.665 |1--^
  */

typedef struct melody{
	uint8_t* sketch;
	uint8_t* tone;
	uint8_t* time;
	uint8_t* note;
	//uint16_t lenght;
}melody_t;
typedef struct{
		uint8_t toneH:4;
		uint8_t tone:4;  // 1234567
		uint16_t beat;    // tick/256;
		uint16_t hold;    // tick/128
}note_melody_t;
//static const uint8_t sketch[] = "5|1113|2123|1135|6--6|5331|2123|1665|1--|";
//static const uint8_t tone[]   = ".|----|----|----|----|----|----|-...|---|";
//static const uint8_t time[]   = "0|0100|0100|0100|0000|0100|0100|0100|000|";
//static const uint8_t time[]   = " | -  | -  | -  |    | -  | -  | -  |   |";
//static const uint8_t note[]   = "-|._--|._--|._--|----|._--|._--|._--|---|";

const melody_t melody[6] = {
	{   // you yi tian chang di jiu   V1
		"5|1113|2123|1135|6--6|5331|2123|1665|1---|0000|",
		".|----|----|----|----|----|----|-...|----|----|",
		" | -  | -  | -  |    | -  | -  | -  |    |    |",
		"-|._--|._--|._--|----|._--|._--|._--|----|----|",
	},
	{
		"5|1113|2123|1135|6--6|5331|2123|1665|1--|",
		".|----|----|----|----|----|----|-...|---|",
		" | -  | -  | -  |    | -  | -  | -  |   |",
		"-|._--|._--|._--|----|.___|._--|._--|---|",
		//40,
	},
	{   // chong er fei
		"33345|322-|11123|377-|632-|632-|6321|1---|00032|5--43|322-54|3453|2--01|6321|5211-|43431-|434312|1---|0000|",
		"-----|----|-----|-..-|.---|.---|.---|----|-----|-----|------|----|-----|.---|.----|------|------|----|----|",
		" --  | -  | --  | -  |    |    |    |    |   --|   --|--  --| -  |   --|   -| --  |----  |----  |    |    |",
		"-----|.---|-----|.---|----|----|--.-|----|-----|-----|------|--.-|-----|--.-|-----|------|----.-|----|----|",
		//40,
	},
	{   // zhi dao bu zhi dao
		"33212|35323-|332153|32200|351656|16120|226165|5000|52-3|22165|332153|32200|351656|1612-|26165|5-00|",
		"-----|------|------|-----|---...|-.---|--.-..|.---|----|---..|------|-----|---...|-.---|-.-..|.---|",
		" --  |----  | -- --|--   |---- -| --  | -- --|    |    | --  | -- --|--   |---- -| --  | - --|    |",
		"-----|------|------|-----|----.-|-----|------|----|----|-----|------|-----|----.-|-----|.----|----|",
		//40,
	},
	{   // zhi dao bu zhi dao   V2
		"33212|35323-|332153|32200|35165.6|16120|226165|5000|52-3|22165|332153|32200|35165.6|1612-|2.6165|5-00|",
		"-----|------|------|-----|---....|-.---|--.-..|.---|----|---..|------|-----|---....|-.---|--.-..|.---|",
		" --  |----  | -- --|--   |----  -| --  | -- --|    |    | --  | -- --|--   |----  -| --  |  - --|    |",
		//"-----|------|------|-----|----.-|-----|------|----|----|-----|------|-----|----.-|-----|.----|----|",
		//40,
	},
	{   // you yi tian chang di jiu
		"5|1.113|2.123|1.135|6--6|5.331|2.123|1.665|1---|0000|",
		".|-----|-----|-----|----|-----|-----|--...|----|----|",
		" |  -  |  -  |  -  |    |  -  |  -  |  -  |    |    |",
		//"-|._--|._--|._--|----|._--|._--|._--|----|----|",
	},
};
//#define  TONE_DIV_1     16
//#define  TONE_DIV_2     8
//#define  TONE_DIV_4     4
//#define  TONE_DIV_8     2
//#define  TONE_DIV_16    1
#define  TONE_DIV_1     64
#define  TONE_DIV_2     32
#define  TONE_DIV_4     16
#define  TONE_DIV_8     8
#define  TONE_DIV_16    4

#define  TONE_LOW       0
#define  TONE_NOR       8
#define  TONE_HIGH      16
typedef struct{
	uint8_t recive:3;
	uint8_t tone:5;  // 01234567
	uint8_t beat;  // tone, tone/2, tone/4, tone/8, tone/16
	uint16_t hold;  // tone, tone*2, tone*3
	/**
	sizeof(note_t) = 16bit = 2byte ==> buf512 > decode 256tone
	define tone=16 ==> tone/16=1, tone.=24, beat:[1-24], hold:[16-48]
	*/
}note_t;

//static const uint16_t tick = 1024;//500; //ms
//const float tick = 1000.0f/TICK_DIV_1; //ms
//static float brightness = 50.0f;
static uint16_t brightness = 50;
const uint16_t brigh_low = 10;
static const uint8_t tone_div_16 = 16; // tone = 64*16=1024, 1s

void play_sketch5(void)
{
	note_t note_buf[60];
	const melody_t *_melody = &melody[5];
	uint16_t len = strlen(_melody->sketch);
	//if(('\0'!=_melody->tone[len]) || ('\0'!=_melody->time[len]) || ('\0'!=_melody->note[len])) return ;
	uint16_t i=0;
	const RGB *rgb=0;
	//note *_note=0;
	note_t _note;
	uint16_t delay = 0;
	//uint16_t count=0;
	len = strlen(_melody->sketch);
	//if(('\0'!=_melody->tone[len]) || ('\0'!=_melody->time[len]) || ('\0'!=_melody->note[len])) return ;
	//for(i=0; i<len; i++)
	i=0;
	while(i<len)
	{
		uint8_t sketch_tmp = 0;
		uint8_t tone_tmp = 0;
		uint8_t time_tmp = 0;
		//uint8_t note_tmp = 0;
		uint8_t count=0;
		memset(note_buf, 0, sizeof(note_buf));
		while(i<len) //while(1)
		{
			sketch_tmp = _melody->sketch[i];
			tone_tmp = _melody->tone[i];
			time_tmp = _melody->time[i];
			//note_tmp = _melody->note[i];
			i++;
			if(('0'<=sketch_tmp) && ('7'>=sketch_tmp))
			{
#if 0
				note_buf[count].tone = sketch_tmp-'0'; // 1234567
				// 1. or 1* or 1
				if('-'==tone_tmp) note_buf[count].tone += TONE_NOR;
				else if('*'==tone_tmp) note_buf[count].tone += TONE_HIGH;
				if(' '==time_tmp) note_buf[count].beat = TONE_DIV_1;//(TICK_DIV_1+1)/tone_div_16;
				else if('-'==time_tmp) note_buf[count].beat = TONE_DIV_2;//(TICK_DIV_2+1)/tone_div_16;
				else if('='==time_tmp) note_buf[count].beat = TONE_DIV_4;//(TICK_DIV_4+1)/tone_div_16;
				else note_buf[count].beat = TONE_DIV_4;//(TICK_DIV_8+1)/tone_div_16;
				//if('.'==note_tmp) note_buf[count].beat += note_buf[count].beat>>1;
				sketch_tmp = _melody->sketch[i];
				note_buf[count].hold = 0;
				if('.'==sketch_tmp)
				{
					note_buf[count].beat += note_buf[count].beat>>1;
					i++;
				}
				sketch_tmp = _melody->sketch[i];
				while('-'==sketch_tmp)
				{
					i++;
					note_buf[count].hold += TONE_DIV_1;
					sketch_tmp = _melody->sketch[i];
				}
#else
				_note.tone = sketch_tmp-'0'; // 1234567
				// 1. or 1* or 1
				if('-'==tone_tmp) _note.tone += TONE_NOR;
				else if('*'==tone_tmp) _note.tone += TONE_HIGH;
				if(' '==time_tmp) _note.beat = TONE_DIV_1;//(TICK_DIV_1+1)/tone_div_16;
				else if('-'==time_tmp) _note.beat = TONE_DIV_2;//(TICK_DIV_2+1)/tone_div_16;
				else if('='==time_tmp) _note.beat = TONE_DIV_4;//(TICK_DIV_4+1)/tone_div_16;
				else _note.beat = TONE_DIV_4;//(TICK_DIV_8+1)/tone_div_16;
				//if('.'==note_tmp) _note.beat += _note.beat>>1;
				sketch_tmp = _melody->sketch[i];
				_note.hold = 0;
				if('.'==sketch_tmp)
				{
					_note.beat += _note.beat>>1;
					i++;
				}
				while('-'==sketch_tmp)
				{
					i++;
					_note.hold += TONE_DIV_1;
					sketch_tmp = _melody->sketch[i];
				}
				note_buf[count].tone = _note.tone;
				note_buf[count].beat = _note.beat;
				note_buf[count].hold = _note.hold;
#endif
				count++;
			}
			//else break;
			if(count>=sizeof(note_buf)/sizeof(note_buf[0])) break;
		}
		//if('|'==_melody->sketch[i])
		{
			// play
			uint16_t index=0;
			//count=4;
			for(index=0; index<count; index++)
			{
				//_note = &note_buf[index];
				_note.tone = note_buf[index].tone;
				_note.beat = note_buf[index].beat;
				_note.hold = note_buf[index].hold;
				rgb = &note_list[_note.tone];
				//play_tone_t((_note->tone%8), _note->beat*tone_div_16, _note->hold*tone_div_16, rgb->rgb.r, rgb->rgb.g, rgb->rgb.b);
				play_tone_t(_note.tone, _note.beat*tone_div_16, _note.hold*tone_div_16, rgb->rgb.r*brightness, rgb->rgb.g*brightness, rgb->rgb.b*brightness);
			}
			count=0;
		}
	}
}
void play_sketch4(void)
{
	note note_buf[20];
	const melody_t *_melody = &melody[5];
	uint16_t len = strlen(_melody->sketch);
	//if(('\0'!=_melody->tone[len]) || ('\0'!=_melody->time[len]) || ('\0'!=_melody->note[len])) return ;
	uint16_t i=0;
	const RGB *rgb=0;
	note *_note=0;
	uint16_t delay = 0;
	//uint16_t count=0;
	len = strlen(_melody->sketch);
	//if(('\0'!=_melody->tone[len]) || ('\0'!=_melody->time[len]) || ('\0'!=_melody->note[len])) return ;
	//for(i=0; i<len; i++)
	i=0;
	while(i<len)
	{
		uint8_t sketch_tmp = 0;
		uint8_t tone_tmp = 0;
		uint8_t time_tmp = 0;
		//uint8_t note_tmp = 0;
		uint8_t count=0;
		memset(note_buf, 0, sizeof(note_buf));
		while(1)
		{
			sketch_tmp = _melody->sketch[i];
			tone_tmp = _melody->tone[i];
			time_tmp = _melody->time[i];
			//note_tmp = _melody->note[i];
			i++;
			if(('0'<=sketch_tmp) && ('7'>=sketch_tmp))
			{
				note_buf[count].tone = sketch_tmp-'0'; // 1234567
				// 1. or 1* or 1
//				if('.'==tone_tmp) note_buf[count].toneH = VALUE_LOW;
//				else if('-'==tone_tmp) note_buf[count].toneH = VALUE_NOR;
//				else if('*'==tone_tmp) note_buf[count].toneH = VALUE_HIGH;
				if('-'==tone_tmp) note_buf[count].tone += TONE_NOR;
				else if('*'==tone_tmp) note_buf[count].tone += TONE_HIGH;
//				if(' '==time_tmp) note_buf[count].beat = TICK_DIV_1;
//				else if('-'==time_tmp) note_buf[count].beat = TICK_DIV_2;
//				else if('='==time_tmp) note_buf[count].beat = TICK_DIV_4;
//				else note_buf[count].beat = TICK_DIV_8;
				if(' '==time_tmp) note_buf[count].beat = TONE_DIV_1;//(TICK_DIV_1+1)/tone_div_16;
				else if('-'==time_tmp) note_buf[count].beat = TONE_DIV_2;//(TICK_DIV_2+1)/tone_div_16;
				else if('='==time_tmp) note_buf[count].beat = TONE_DIV_4;//(TICK_DIV_4+1)/tone_div_16;
				else note_buf[count].beat = TONE_DIV_4;//(TICK_DIV_8+1)/tone_div_16;
				//if('.'==note_tmp) note_buf[count].beat += note_buf[count].beat>>1;
				sketch_tmp = _melody->sketch[i];
				note_buf[count].hold = 0;
				if('.'==sketch_tmp)
				{
					note_buf[count].beat += note_buf[count].beat>>1;
					i++;
				}
				while('-'==sketch_tmp)
				{
					i++;
					//note_buf[count].hold += TICK_DIV_1;
					note_buf[count].hold += TONE_DIV_1;
					sketch_tmp = _melody->sketch[i];
				}
				count++;
			}
			else break;
		}
		//if('|'==_melody->sketch[i])
		{
			// play
			uint16_t index=0;
			//count=4;
			for(index=0; index<count; index++)
			{
				_note = &note_buf[index];
				//rgb = &rgb_list[_note->toneH][_note->tone];
				//rgb = &rgb_list[_note->toneH][0];
				rgb = &note_list[_note->tone];
				//rgb = &rgb_list[_note->toneH][_note->tone%8];
				//BrushOutTest(_note, rgb->rgb.r, rgb->rgb.g, rgb->rgb.b);
				//_note->beat
				//play_tone(_note, _note->tone%8, _note->beat, _note->hold, rgb->rgb.r, rgb->rgb.g, rgb->rgb.b);
				//play_tone(_note, _note->tone%8, _note->beat*tone_div_16, _note->hold*tone_div_16, rgb->rgb.r, rgb->rgb.g, rgb->rgb.b);
				play_tone_t((_note->tone%8), _note->beat*tone_div_16, _note->hold*tone_div_16, rgb->rgb.r, rgb->rgb.g, rgb->rgb.b);
				//BrushOutTest(_note, 0, 0, 0);
			}
			count=0;
		}
	}
}
void play_sketch3(void)
{
	note note_buf[20];
	const melody_t *_melody = &melody[5];
	uint16_t len = strlen(_melody->sketch);
	//if(('\0'!=_melody->tone[len]) || ('\0'!=_melody->time[len]) || ('\0'!=_melody->note[len])) return ;
	uint16_t i=0;
	const RGB *rgb=0;
	note *_note=0;
	uint16_t delay = 0;
	//uint16_t count=0;
	len = strlen(_melody->sketch);
	//if(('\0'!=_melody->tone[len]) || ('\0'!=_melody->time[len]) || ('\0'!=_melody->note[len])) return ;
	//for(i=0; i<len; i++)
	i=0;
	while(i<len)
	{
		uint8_t sketch_tmp = 0;
		uint8_t tone_tmp = 0;
		uint8_t time_tmp = 0;
		//uint8_t note_tmp = 0;
		uint8_t count=0;
		memset(note_buf, 0, sizeof(note_buf));
		while(1)
		{
			sketch_tmp = _melody->sketch[i];
			tone_tmp = _melody->tone[i];
			time_tmp = _melody->time[i];
			//note_tmp = _melody->note[i];
			i++;
			if(('0'<=sketch_tmp) && ('7'>=sketch_tmp))
			{
				note_buf[count].tone = sketch_tmp-'0'; // 1234567
				// 1. or 1* or 1
				if('.'==tone_tmp) note_buf[count].toneH = VALUE_LOW;
				else if('-'==tone_tmp) note_buf[count].toneH = VALUE_NOR;
				else if('*'==tone_tmp) note_buf[count].toneH = VALUE_HIGH;
				if(' '==time_tmp) note_buf[count].beat = TICK_DIV_1;
				else if('-'==time_tmp) note_buf[count].beat = TICK_DIV_2;
				else if('='==time_tmp) note_buf[count].beat = TICK_DIV_4;
				else note_buf[count].beat = TICK_DIV_8;
				//if('.'==note_tmp) note_buf[count].beat += note_buf[count].beat>>1;
				sketch_tmp = _melody->sketch[i];
				note_buf[count].hold = 0;
				if('.'==sketch_tmp)
				{
					note_buf[count].beat += note_buf[count].beat>>1;
					i++;
				}
				while('-'==sketch_tmp)
				{
					i++;
					note_buf[count].hold += TICK_DIV_1;
					sketch_tmp = _melody->sketch[i];
				}
				count++;
			}
			else break;
		}
		//if('|'==_melody->sketch[i])
		{
			// play
			uint16_t index=0;
			//count=4;
			for(index=0; index<count; index++)
			{
				_note = &note_buf[index];
				rgb = &rgb_list[_note->toneH][_note->tone];
				//rgb = &rgb_list[_note->toneH][0];
				//BrushOutTest(_note, rgb->rgb.r, rgb->rgb.g, rgb->rgb.b);
				//_note->beat
				play_tone(_note, _note->tone, _note->beat, _note->hold, rgb->rgb.r, rgb->rgb.g, rgb->rgb.b);
				//BrushOutTest(_note, 0, 0, 0);
			}
			count=0;
		}
	}
}
void play_sketch(void)
{
	note note_buf[20];
	const melody_t *_melody = &melody[3];
	uint16_t len = strlen(_melody->sketch);
	//if(('\0'!=_melody->tone[len]) || ('\0'!=_melody->time[len]) || ('\0'!=_melody->note[len])) return ;
	uint16_t i=0;
	const RGB *rgb=0;
	note *_note=0;
	uint16_t delay = 0;
	//uint16_t count=0;
	len = strlen(_melody->sketch);
	//if(('\0'!=_melody->tone[len]) || ('\0'!=_melody->time[len]) || ('\0'!=_melody->note[len])) return ;
	//for(i=0; i<len; i++)
	i=0;
	while(i<len)
	{
		uint8_t sketch_tmp = 0;
		uint8_t tone_tmp = 0;
		uint8_t time_tmp = 0;
		uint8_t note_tmp = 0;
		uint16_t count=0;
		memset(note_buf, 0, sizeof(note_buf));
		while(1)
		{
			sketch_tmp = _melody->sketch[i];
			tone_tmp = _melody->tone[i];
			time_tmp = _melody->time[i];
			note_tmp = _melody->note[i];
			i++;
			if(('0'<=sketch_tmp) && ('7'>=sketch_tmp))
			{
				note_buf[count].tone = sketch_tmp-'0'; // 1234567
				// 1. or 1* or 1
				if('.'==tone_tmp) note_buf[count].toneH = VALUE_LOW;
				else if('-'==tone_tmp) note_buf[count].toneH = VALUE_NOR;
				else if('*'==tone_tmp) note_buf[count].toneH = VALUE_HIGH;
				if(' '==time_tmp) note_buf[count].beat = TICK_DIV_1;
				else if('-'==time_tmp) note_buf[count].beat = TICK_DIV_2;
				else if('='==time_tmp) note_buf[count].beat = TICK_DIV_4;
				else note_buf[count].beat = TICK_DIV_8;
				if('.'==note_tmp) note_buf[count].beat += note_buf[count].beat>>1;
				sketch_tmp = _melody->sketch[i];
				note_buf[count].hold = 0;
				while('-'==sketch_tmp)
				{
					i++;
					note_buf[count].hold += TICK_DIV_1;
					sketch_tmp = _melody->sketch[i];
				}
				count++;
			}
			else break;
		}
		//if('|'==_melody->sketch[i])
		{
			// play
			uint16_t index=0;
			//count=4;
			for(index=0; index<count; index++)
			{
				_note = &note_buf[index];
				rgb = &rgb_list[_note->toneH][_note->tone];
				//rgb = &rgb_list[_note->toneH][0];
				BrushOutTest(_note, rgb->rgb.r, rgb->rgb.g, rgb->rgb.b);
				//BrushOutTest(_note, 0, 0, 0);
			}
			count=0;
		}
	}
}

static note buf[]={
	{VALUE_LOW, 5, TICK_DIV_1, TICK__NULL}, // |
	{VALUE_NOR, 1, TICK_DIV_1, TICK_DIV_2},
	{VALUE_NOR, 1, TICK_DIV_2, TICK__NULL}, // |
	{VALUE_NOR, 1, TICK_DIV_1, TICK__NULL}, 
	{VALUE_NOR, 3, TICK_DIV_1, TICK__NULL}, // |
	{VALUE_NOR, 2, TICK_DIV_1, TICK_DIV_2},
	{VALUE_NOR, 1, TICK_DIV_2, TICK__NULL}, // |
	{VALUE_NOR, 2, TICK_DIV_1, TICK__NULL},
	{VALUE_NOR, 3, TICK_DIV_1, TICK__NULL}, // |
	{VALUE_NOR, 1, TICK_DIV_1, TICK_DIV_2},
	{VALUE_NOR, 1, TICK_DIV_2, TICK__NULL}, // |
	{VALUE_NOR, 3, TICK_DIV_1, TICK__NULL},
	{VALUE_NOR, 5, TICK_DIV_1, TICK__NULL},
	{VALUE_NOR, 6, TICK_DIV_1, TICK_MUL_2},
	//{VALUE_NOR, 6, TICK_DIV_1, TICK__NULL},
	//{VALUE_NOR, 0, TICK_DIV_1, TICK__NULL},
	{VALUE_NOR, 6, TICK_DIV_1, TICK__NULL},
	{VALUE_NOR, 5, TICK_DIV_1, TICK_DIV_2},
	{VALUE_NOR, 3, TICK_DIV_2, TICK__NULL},
	{VALUE_NOR, 3, TICK_DIV_1, TICK__NULL},
	{VALUE_NOR, 1, TICK_DIV_1, TICK__NULL},
	{VALUE_NOR, 2, TICK_DIV_1, TICK_DIV_2},
	{VALUE_NOR, 1, TICK_DIV_2, TICK__NULL},
	{VALUE_NOR, 2, TICK_DIV_1, TICK__NULL},
	{VALUE_NOR, 3, TICK_DIV_1, TICK__NULL},
	{VALUE_NOR, 1, TICK_DIV_1, TICK_DIV_2},
	{VALUE_LOW, 6, TICK_DIV_2, TICK__NULL},
	{VALUE_LOW, 6, TICK_DIV_1, TICK__NULL},
	{VALUE_LOW, 5, TICK_DIV_1, TICK__NULL},
	{VALUE_NOR, 1, TICK_DIV_1, TICK_MUL_2},
	//{VALUE_NOR, 1, TICK_DIV_1, TICK_DIV_1},
	{VALUE_NOR, 0, TICK_DIV_1, TICK_MUL_2},
	{VALUE_NOR, 0, TICK_DIV_1, TICK_MUL_2},
	{VALUE_NOR, 0, TICK_DIV_1, TICK_MUL_2},
	{VALUE_NOR, 0, TICK_DIV_1, TICK_MUL_2},
	{VALUE_NOR, 0, TICK_DIV_1, TICK_MUL_2},
};

static const uint16_t tick = 1024;//500; //ms
//const float tick = 1000.0f/TICK_DIV_1; //ms
//static float brightness = 50.0f;
//const uint16_t brigh_low = 10;
static void play_tone_t(const uint8_t _tone, const uint16_t _beat, const uint16_t _hold, const uint16_t R, const uint16_t G, const uint16_t B)
{
		uint16_t delay = 0;
		uint16_t i=0;
		uint8_t r;
		uint8_t g;
		uint8_t b;
		//float brigh=1.0f;
		//float scale = 1.0f;
		//float _tick = (float)tick/TICK_DIV_1; //ms
		//scale = brightness/256.0f;
		//float R=200.0f,G=100.0f,B=1.0f;
		//delay = 128;
		delay = _beat*2/3;//+20; //(tick*_note->beat)/TICK_DIV_1/2;
		r = 200;
		g = 100;
		b = 0;
		delay += brigh_low;
		// +++
		for(i=brigh_low; i<delay; i++)
		{
				//brigh = 2.0f*i/delay;
//				r = R*i*scale/delay;
//				g = G*i*scale/delay;
//				b = B*i*scale/delay;
				r = ((R*i)>>8)/delay;
				g = ((G*i)>>8)/delay;
				b = ((B*i)>>8)/delay;
				//BrushOut(20, r, g, b);
				BrushOut(_tone%8, r, g, b);
				HAL_Delay(1);//osDelay(1);
				//HAL_Delay(2);
		}
		// ---
		delay = _beat/3+_hold; //(uint16_t)(tick*(_note->beat/2+_note->hold))+1;
		delay += brigh_low;
		for(i=delay; i>brigh_low; i--)
		{
				r = ((R*i)>>8)/delay;
				g = ((G*i)>>8)/delay;
				b = ((B*i)>>8)/delay;
				BrushOut(_tone%8, r, g, b);
				HAL_Delay(1);//osDelay(1);
				//HAL_Delay(4);
		}
}
static void play_tone(const note *_note_tmp, const uint8_t _tone, const uint16_t _beat, const uint16_t _hold, const float R, const float G, const float B)
{
		uint16_t delay = 0;
		uint16_t i=0;
		uint8_t r;
		uint8_t g;
		uint8_t b;
		float brigh=1.0f;
		float scale = 1.0f;
		float _tick = (float)tick/TICK_DIV_1; //ms
		scale = brightness/256.0f;
		//float R=200.0f,G=100.0f,B=1.0f;
		//delay = 128;
		delay = _tick*_beat*2/3;//+20; //(tick*_note->beat)/TICK_DIV_1/2;
		r = 200;
		g = 100;
		b = 0;
		delay += brigh_low;
		// +++
		for(i=brigh_low; i<delay; i++)
		{
				//brigh = 2.0f*i/delay;
				r = R*i*scale/delay;
				g = G*i*scale/delay;
				b = B*i*scale/delay;
				//BrushOut(20, r, g, b);
				BrushOut(_tone, r, g, b);
				HAL_Delay(1);//osDelay(1);
				//HAL_Delay(2);
		}
		// ---
		//delay = 256;
		//delay = (tick*(_note->beat+_note->hold))/TICK_DIV_1;
		delay = _tick*_beat/3+_tick*_hold; //(uint16_t)(tick*(_note->beat/2+_note->hold))+1;
		delay += brigh_low;
		for(i=delay; i>brigh_low; i--)
		{
				r = R*i*scale/delay;
				g = G*i*scale/delay;
				b = B*i*scale/delay;
				//BrushOut(20, r, g, b);
				BrushOut(_tone, r, g, b);
				HAL_Delay(1);//osDelay(1);
				//HAL_Delay(4);
		}
}
static void BrushOutTest(const note *_note, const float R, const float G, const float B)
{
		uint16_t delay = 0;
		uint16_t i=0;
		uint8_t r;
		uint8_t g;
		uint8_t b;
		float brigh=1.0f;
		float scale = 1.0f;
		float _tick = (float)tick/TICK_DIV_1; //ms
		scale = brightness/256.0f;
		//float R=200.0f,G=100.0f,B=1.0f;
		//delay = 128;
		delay = _tick*_note->beat*2/3;//+20; //(tick*_note->beat)/TICK_DIV_1/2;
		r = 200;
		g = 100;
		b = 0;
		delay += brigh_low;
		// +++
		for(i=brigh_low; i<delay; i++)
		{
				//brigh = 2.0f*i/delay;
				r = R*i*scale/delay;
				g = G*i*scale/delay;
				b = B*i*scale/delay;
				//BrushOut(20, r, g, b);
				BrushOut(_note->tone, r, g, b);
				HAL_Delay(1);//osDelay(1);
				//HAL_Delay(2);
		}
		// ---
		//delay = 256;
		//delay = (tick*(_note->beat+_note->hold))/TICK_DIV_1;
		delay = _tick*_note->beat/3+_tick*_note->hold; //(uint16_t)(tick*(_note->beat/2+_note->hold))+1;
		delay += brigh_low;
		for(i=delay; i>brigh_low; i--)
		{
				r = R*i*scale/delay;
				g = G*i*scale/delay;
				b = B*i*scale/delay;
				//BrushOut(20, r, g, b);
				BrushOut(_note->tone, r, g, b);
				//osDelay(1);
				HAL_Delay(1);
		}
}
//typedef void (*send)(uint8_t data[3]);
//void my_play(send _send)
//void my_play(void (*_Outone)(uint8_t data[3]))
void play_sketch1(void)
{
		uint16_t len = sizeof(buf)/sizeof(note);
		uint16_t i=0;
		const RGB *rgb=0;
		note *_note=0;
		uint16_t delay = 0;
		for(i=0; i<len; i++)
		{
				_note = &buf[i];
				//_note->tone = 6;
				rgb = &rgb_list[_note->toneH][_note->tone];
				//rgb = &rgb_list_base[_note->tone];
#if 0
				//_Outone(rgb->buf);
				BrushOut(20, rgb->rgb.r, rgb->rgb.g, rgb->rgb.b);
				delay = (tick*(_note->beat+_note->hold))/TICK_DIV_1;
				osDelay(delay);
#else
			//while(1)BrushOutTone(_note, rgb->rgb.r, rgb->rgb.g, rgb->rgb.b);
			//while(1) BrushOutTest(_note, rgb->rgb.r, rgb->rgb.g, rgb->rgb.b);
			//BrushOutTest(_note, 0, 000, 200);
			//printf("\r\n%s  %02X %02X %02X\n", __func__, rgb->rgb.r, rgb->rgb.g, rgb->rgb.b);
			BrushOutTest(_note, rgb->rgb.r, rgb->rgb.g, rgb->rgb.b);
#endif
		}
}


