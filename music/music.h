/************************ (C) COPYLEFT 2018 Merafour *************************

* File Name          : music.h
* Author             : Merafour
* Last Modified Date : 08/11/2018
* Description        : This file provides the Player library.
********************************************************************************
* https://merafour.blog.163.com
* merafour@163.com
* https://github.com/merafour
******************************************************************************/
#ifndef __MUSIC_H__
#define __MUSIC_H__

#include <stdint.h>
#include <inttypes.h>
#include "loudspeaker.h"


#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978

#define NOTE_C    1
#define NOTE_D    3
#define NOTE_E    5
#define NOTE_F    6
#define NOTE_G    8
#define NOTE_A    10
#define NOTE_B    12

#define note_do   NOTE_C
#define note_rai  NOTE_D
#define note_me   NOTE_E
#define note_fa   NOTE_F
#define note_suo  NOTE_G
#define note_la   NOTE_A
#define note_xi   NOTE_B

#define OCTAVE_START   3
//#define NOTES_LEN  (12*4+1+4)
#define NOTES_LEN  (12*(8-OCTAVE_START)+1+4+12*3)
#define musical_play_size   19

/**
	{   // 虫儿飞
	  .rtttl     = "ChongErFei:d=4,o=5,b=103,c=F:" // 歌曲名:<ChongErFei>, 4(d=4)/4拍, 音阶5, 每分钟节拍数 103, 调式: F调
		.melody    = "33345|3.22-|11123|3.77-|632-|632-|632.1|1---|00032|5--43|2--54|345.3|2--01|632.1|521--|43431-|43431.2|1---|0000|",
		.octave    = "-----|-----|-----|--..-|.---|.---|.----|----|-----|-----|-----|-----|-----|.----|.----|------|-------|----|----|",
		.durations = " --  |  -  | --  |  -  |    |    |    -|    |   --|   - |-  --| -   |   --|    -| --  |----  |----  -|    |    |",
	},
*/
typedef struct musical{  // 简谱,音节
	const char* rtttl;    // 歌曲信息, 采用 RTTTL格式,eg: "Indiana Jones:d=4,o=5,b=250:",并增加调式:"c=F"
	const char* melody;   // 旋律, 01234567
	const char* octave;   // 八度, 高中低音 '.':低音, '-'/' ':中音, '*':高音, 'H':高两个八度, 'L':低两个八度
	const char* durations;// 持续时间,全音符,二分音符,四分音符,八分音符... ' ':全音符, '-':二分音符, '=':四分音符, 'O':octal, 八分音符, 'H':hexadecimal,十六分音符
}musical_t; //用于描述一首音乐的数据结构

typedef struct{         // 音符
//	uint8_t recive:2;   // 
//	uint8_t tone:6;     // 01234567, 2^5=32  4个八度 // 
	uint8_t tone;     // 01234567, 2^5=32  4个八度
	uint16_t beat;       // tone, tone/2, tone/4, tone/8, tone/16, 音符时间
	uint16_t hold;       // tone, tone*2, tone*3, 延音
	/**
	sizeof(note_t) = 16bit = 2byte ==> buf512 > decode 256tone
	define tone=16 ==> tone/16=1, tone.=24, beat:[1-24], hold:[16-48]
	*/
}tone_t;

class Music {
private:
	static const uint32_t pwm_frequency;
	
protected:
	uint8_t default_oct;
	uint8_t default_dur;
    uint16_t bpm;
    uint16_t wholenote;
	uint8_t default_mode; // 调式

    loudspeaker &speaker;

	static uint16_t frequency[NOTES_LEN]; // 频率
	static uint16_t volume[NOTES_LEN];    // 音量, 1%
	tone_t note_buf[256];  // buffer,歌曲解析后的音符
	static const musical_t musicals[musical_play_size];
    static uint16_t g_volume;
    static musical_t musical;
    static uint8_t _pause;

	uint16_t rtttl(const char* _rtttl);
	virtual void play_tone(const uint8_t _tone, const uint16_t _beat, const uint16_t _hold, const uint16_t _volume);
public:
    Music(loudspeaker &_speaker) : speaker(_speaker) 
    {
        musical.rtttl = _rtttl_buf;
        musical.melody = _melody_buf;
        musical.octave = _octave_buf;
        musical.durations = _durations_buf;
    }
	void delay_ms(const uint16_t ms);
	virtual void Init(void);
	virtual void play(void);
	virtual void play(const musical_t *_melody);
	virtual void test(void);
    void pause(void)
    {
        _pause = 1;
    }
    
#define BUFFER_SIZE      512
    static char _rtttl_buf[128];
    static char _melody_buf[BUFFER_SIZE];
    static char _octave_buf[BUFFER_SIZE];
    static char _durations_buf[BUFFER_SIZE];
    
};

#endif // __MUSIC_H__

