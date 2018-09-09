// -*- Mode: C++; c-basic-offset: 4; indent-tabs-mode: nil -*-
/************************ (C) COPYLEFT 2018 Merafour *************************

* File Name          : RTTTL.cpp
* Author             : Merafour
* Last Modified Date : 08/11/2018
* Description        : This file provides the RTTTL library.
********************************************************************************
* https://merafour.blog.163.com
* merafour@163.com
* https://github.com/merafour
******************************************************************************/


#include "RTTTL.h"

#include <stdarg.h>
#include <string.h>
#include <stdio.h>

#define isdigit(n) (n >= '0' && n <= '9')

const char* tune[] = {
					 "Indiana Jones:d=4,o=5,b=250:e,8p,8f,8g,8p,1c6,8p.,d,8p,8e,1f,p.,g,8p,8a,8b,8p,1f6,p,a,8p,8b,2c6,2d6,2e6,e,8p,8f,8g,8p,1c6,p,d6,8p,8e6,1f.6,g,8p,8g,e.6,8p,d6,8p,8g,e.6,8p,d6,8p,8g,f.6,8p,e6,8p,8d6,2c6",
                     "TheSimpsons:d=4,o=5,b=160:c.6,e6,f#6,8a6,g.6,e6,c6,8a,8f#,8f#,8f#,2g,8p,8p,8f#,8f#,8f#,8g,a#.,8c6,8c6,8c6,c6",
                     "Entertainer:d=4,o=5,b=140:8d,8d#,8e,c6,8e,c6,8e,2c.6,8c6,8d6,8d#6,8e6,8c6,8d6,e6,8b,d6,2c6,p,8d,8d#,8e,c6,8e,c6,8e,2c.6,8p,8a,8g,8f#,8a,8c6,e6,8d6,8c6,8a,2d6",
                     "Muppets:d=4,o=5,b=250:c6,c6,a,b,8a,b,g,p,c6,c6,a,8b,8a,8p,g.,p,e,e,g,f,8e,f,8c6,8c,8d,e,8e,8e,8p,8e,g,2p,c6,c6,a,b,8a,b,g,p,c6,c6,a,8b,a,g.,p,e,e,g,f,8e,f,8c6,8c,8d,e,8e,d,8d,c",
                     "Bond:d=4,o=5,b=80:32p,16c#6,32d#6,32d#6,16d#6,8d#6,16c#6,16c#6,16c#6,16c#6,32e6,32e6,16e6,8e6,16d#6,16d#6,16d#6,16c#6,32d#6,32d#6,16d#6,8d#6,16c#6,16c#6,16c#6,16c#6,32e6,32e6,16e6,8e6,16d#6,16d6,16c#6,16c#7,c.7,16g#6,16f#6,g#.6",

                     "StarWars:d=4,o=5,b=45:32p,32f#,32f#,32f#,8b.,8f#.6,32e6,32d#6,32c#6,8b.6,16f#.6,32e6,32d#6,32c#6,8b.6,16f#.6,32e6,32d#6,32e6,8c#.6,32f#,32f#,32f#,8b.,8f#.6,32e6,32d#6,32c#6,8b.6,16f#.6,32e6,32d#6,32c#6,8b.6,16f#.6,32e6,32d#6,32e6,8c#6",
                     "GoodBad:d=4,o=5,b=56:32p,32a#,32d#6,32a#,32d#6,8a#.,16f#.,16g#.,d#,32a#,32d#6,32a#,32d#6,8a#.,16f#.,16g#.,c#6,32a#,32d#6,32a#,32d#6,8a#.,16f#.,32f.,32d#.,c#,32a#,32d#6,32a#,32d#6,8a#.,16g#.,d#",
                     "A-Team:d=8,o=5,b=120:4d#6,a#,2d#6,16p,g#,4a#,4d#.,p,16g,16a#,d#6,a#,f6,2d#6,16p,c#.6,16c6,16a#,g#.,2a#,d#6,a#,2d#6,16p,g#,4a#,4d#.,p,16g,16a#,d#6,a#,f6,2d#6,16p,c#.6,16c6,16a#,g#.,2a#",
                     "Flinstones:d=4,o=5,b=40:32p,16f6,16a#,16a#6,32g6,16f6,16a#.,16f6,32d#6,32d6,32d6,32d#6,32f6,16a#,16c6,d6,16f6,16a#.,16a#6,32g6,16f6,16a#.,32f6,32f6,32d#6,32d6,32d6,32d#6,32f6,16a#,16c6,a#,16a6,16d.6,16a#6,32a6,32a6,32g6,32f#6,32a6,8g6,16g6,16c.6,32a6,32a6,32g6,32g6,32f6,32e6,32g6,8f6,16f6,16a#.,16a#6,32g6,16f6,16a#.,16f6,32d#6,32d6,32d6,32d#6,32f6,16a#,16c.6,32d6,32d#6,32f6,16a#,16c.6,32d6,32d#6,32f6,16a#6,16c7,8a#.6",
                     "Jeopardy:d=4,o=6,b=125:c,f,c,f5,c,f,2c,c,f,c,f,a.,8g,8f,8e,8d,8c#,c,f,c,f5,c,f,2c,f.,8d,c,a#5,a5,g5,f5,p,d#,g#,d#,g#5,d#,g#,2d#,d#,g#,d#,g#,c.7,8a#,8g#,8g,8f,8e,d#,g#,d#,g#5,d#,g#,2d#,g#.,8f,d#,c#,c,p,a#5,p,g#.5,d#,g#",

                     "Gadget:d=16,o=5,b=40:32d#,32f,32f#,32g#,a#,f#,a,f,g#,f#,32d#,32f,32f#,32g#,a#,d#6,4d6,32d#,32f,32f#,32g#,a#,f#,a,f,g#,f#,8d#",
                     "MissionImp:d=16,o=6,b=75:32d,32d#,32d,32d#,32d,32d#,32d,32d#,32d,32d,32d#,32e,32f,32f#,32g,g,8p,g,8p,a#,p,c7,p,g,8p,g,8p,f,p,f#,p,g,8p,g,8p,a#,p,c7,p,g,8p,g,8p,f,p,f#,p,a#,g,2d,32p,a#,g,2c#,32p,a#,g,2c,a#5,8c,2p,32p,a#5,g5,2f#,32p,a#5,g5,2f,32p,a#5,g5,2e,d#,8d",
                     "Airwolf:d=4,o=5,b=100:e,16a,16b,16d6,e6,16g6,16f#6,16d6,e6,16g6,16f#6,16d6,e6,8d6,16f#6,b,a,8g,16a,8f#,16d,g,16c6,16d6,16f6,g,16c6,16b6,16f6,g6,16c6,16b6,16f6,g6,8f6,16a,d6,c6,8b,16d6,8a,16f,g6,16c6,16d6,16f6,g6,16c6,16b6,16f6",
                     "StarWars:d=4,o=5,b=112:8d.,16p,8d.,16p,8d.,16p,8a#4,16p,16f,8d.,16p,8a#4,16p,16f,d.,8p,8a.,16p,8a.,16p,8a.,16p,8a#,16p,16f,8c#.,16p,8a#4,16p,16f,d.,8p,8d.6,16p,8d,16p,16d,8d6,8p,8c#6,16p,16c6,16b,16a#,8b,8p,16d#,16p,8g#,8p,8g,16p,16f#,16f,16e,8f,8p,16a#4,16p,8c#,8p,8a#4,16p,16c#,8f.,16p,8d,16p,16f,a.,8p,8d.6,16p,8d,16p,16d,8d6,8p,8c#6,16p,16c6,16b,16a#,8b,8p,16d#,16p,8g#,8p,8g,16p,16f#,16f,16e,8f,8p,16a#4,16p,8c#"
};


void RTTTL::test(void)
{
    speaker.note_update(0, 0);
    speaker.start();
    g_volume = 50;
	while(1)
	{
#if 0
		//play(&musicals[1]); 
        //play(&musicals[2]);
        //play(&musicals[7]);
        play(&musicals[10]);
        //play(tune[13]); 
		delay_ms(2000);
#else
		play(&musicals[2]); // 2月亮代表我的心
		if(1==_pause) return; // pause
		delay_ms(2000);
		play(&musicals[7]); // 天空之城-赵海洋[质谱]
		if(1==_pause) return; // pause
		delay_ms(2000);
		play(&musicals[5]); // you yi tian chang di jiu 友谊天长地久
		if(1==_pause) return; // pause
		delay_ms(2000);
		play(&musicals[4]); // 知道不知道
		if(1==_pause) return; // pause
		delay_ms(2000);
		play(&musicals[1]); //1 虫儿飞
		if(1==_pause) return; // pause
		delay_ms(2000);
#endif
	}
}

void RTTTL::Init(void)
{
	Music::Init();
}
void RTTTL::play(void)
{
    _pause = 0;
//    play(&musical);
    Music::play();
}
uint16_t RTTTL::decode_rtttl(const char *_rtttl)
{
	uint16_t len = 0;
	uint16_t _pos=0;
	//uint16_t _music=0;
	uint16_t scale,note,num =0;
	uint16_t duration=0;
	uint16_t count=0;
	uint16_t cur_note;
	// rtttl
	//_music=rtttl(_rtttl);
    rtttl(_rtttl);
    wholenote *=4;
	len = strlen(_rtttl);
	memset(note_buf, 0, sizeof(note_buf));
	_pos = 0;
	// "TheSimpsons:d=4,o=5,b=160:c.6,e6,f#6,8a6,g.6,e6,c6,8a,8f#,8f#,8f#,2g,8p,8p,8f#,8f#,8f#,8g,a#.,8c6,8c6,8c6,c6",
	for(_pos = 0; ':'!=_rtttl[_pos]; _pos++)
	{
			if('\0'==_rtttl[_pos]) break;
	}
	_pos++;
	// "c.6,e6,f#6,8a6,g.6,e6,c6,8a,8f#,8f#,8f#,2g,8p,8p,8f#,8f#,8f#,8g,a#.,8c6,8c6,8c6,c6",
	while(_pos<len)
	{
			num =0;
			//  计算时值
			while(isdigit(_rtttl[_pos])){                   //this is a safe while loop as it can't go further than
																																	//the length of the rtttl tone string
					num = (num * 10) + (_rtttl[_pos++] - '0');
			}
			if(num){
					duration = wholenote / num;
			} else{
					duration = wholenote / default_dur;  // we will need to check if we are a dotted note after
			}
			// now get the note
			note = 0;
			// 计算音符
			switch(_rtttl[_pos]){
					case 'c':
					note = 1;
					break;
					case 'd':
					note = 3;
					break;
					case 'e':
					note = 5;
					break;
					case 'f':
					note = 6;
					break;
					case 'g':
					note = 8;
					break;
					case 'a':
					note = 10;
					break;
					case 'b':
					note = 12;
					break;
					case 'p':
					default:
					note = 0;
			}
			_pos++;
			// 升调
			// now, get optional '#' sharp
			if(_rtttl[_pos] == '#'){
					note++;
					_pos++;
			}
			// now, get optional '.' dotted note
			// 附点音符
			if(_rtttl[_pos] == '.'){
					duration += duration/2;
					_pos++;
			}
			// now, get scale
			// 设置音阶
			if(isdigit(_rtttl[_pos])){
					scale = _rtttl[_pos] - '0';  //
					_pos++;
			} else{
					scale = default_oct;  // 默认音阶
			}
			//scale += OCTAVE_OFFSET;
            // "Startup:d=8,o=6,b=480:a,d7,c7,a,d7,c7,a,d7,16d7,16c7,16d7,16c7,16d7,16c7,16d7,16c7",
			if(_rtttl[_pos] == ','){
					_pos++;       // skip comma for next note (or we may be at the end)
			}
			// 一个音符结束
			// now play the note
			if(note){
					cur_note = (scale - OCTAVE_START) * 12 + note;
			} else{
					cur_note = 0;
			}
				note_buf[count].tone = cur_note;
				note_buf[count].beat = duration;
				note_buf[count].hold = 0;
				count++;
	}
		return count;
}
void RTTTL::play(const musical_t *_musical)
{
	Music::play(_musical);
}
void RTTTL::play(const char *_rtttl)
{
	uint16_t count=0;
	uint16_t index=0;
	tone_t _note;
	_pause = 0;
	count=decode_rtttl(_rtttl);
	// play
	for(index=0; index<count; index++)
	{
		_note.tone = note_buf[index].tone;
		_note.beat = note_buf[index].beat;
		_note.hold = note_buf[index].hold;
		play_tone(_note.tone, _note.beat, _note.hold*wholenote, 10);
	}
	count=0;
}
void RTTTL::play_tone(const uint8_t _tone, const uint16_t _beat, const uint16_t _hold, const uint16_t _volume)
{
    int16_t delay = 0;
    int16_t i=0;
    delay = (_beat-10)/2;//+20; //(tick*_note->beat)/TICK_DIV_1/2;
    delay = delay - (delay&0x1);
    {
        speaker.note_update(_tone, 0);
        for(i=0; i<delay; i+=2)
        {
            speaker.volume_update(_tone, ((float)_volume*i)/delay);
            delay_ms(2);
        }
        speaker.volume_update(_tone, _volume);
        delay = _beat+_hold-i;
        for(i=delay; i>=6; i-=2)
        {
            speaker.volume_update(_tone, ((float)_volume*i)/delay);
            delay_ms(2);
        }
        speaker.volume_update(_tone, 0);
        //delay_ms(10+i);
		delay_ms(i);
    }
}

