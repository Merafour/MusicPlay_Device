// -*- Mode: C++; c-basic-offset: 4; indent-tabs-mode: nil -*-
/************************ (C) COPYLEFT 2018 Merafour *************************

* File Name          : Music.cpp
* Author             : Merafour
* Last Modified Date : 08/11/2018
* Description        : This file provides the Music library.
********************************************************************************
* https://merafour.blog.163.com
* merafour@163.com
* https://github.com/merafour
******************************************************************************/

#include "music.h"
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#if 0
#include "main.h"
#include "stm32f1xx_hal.h"
#include "tim.h"
#include "gpio.h"
#endif
#define isdigit(n) (n >= '0' && n <= '9')
// 十二度音阶,含半度
static uint16_t notes[NOTES_LEN] = { 0,
	//   1,        2,       3,        4,       5,       6,        7,       8,        9,       10,      11,       12
    NOTE_C3, NOTE_CS3, NOTE_D3, NOTE_DS3, NOTE_E3, NOTE_F3, NOTE_FS3, NOTE_G3, NOTE_GS3, NOTE_A3, NOTE_AS3, NOTE_B3,
	NOTE_C4, NOTE_CS4, NOTE_D4, NOTE_DS4, NOTE_E4, NOTE_F4, NOTE_FS4, NOTE_G4, NOTE_GS4, NOTE_A4, NOTE_AS4, NOTE_B4,
    NOTE_C5, NOTE_CS5, NOTE_D5, NOTE_DS5, NOTE_E5, NOTE_F5, NOTE_FS5, NOTE_G5, NOTE_GS5, NOTE_A5, NOTE_AS5, NOTE_B5,
    NOTE_C6, NOTE_CS6, NOTE_D6, NOTE_DS6, NOTE_E6, NOTE_F6, NOTE_FS6, NOTE_G6, NOTE_GS6, NOTE_A6, NOTE_AS6, NOTE_B6,
    NOTE_C7, NOTE_CS7, NOTE_D7, NOTE_DS7, NOTE_E7, NOTE_F7, NOTE_FS7, NOTE_G7, NOTE_GS7, NOTE_A7, NOTE_AS7, NOTE_B7,
    NOTE_C8, NOTE_CS8, NOTE_D8, NOTE_DS8, NOTE_E7, NOTE_F7, NOTE_FS7, NOTE_G7, NOTE_GS7, NOTE_A7, NOTE_AS7, NOTE_B7, // 不能支持的音域,使用重复
    NOTE_C7, NOTE_CS7, NOTE_D7, NOTE_DS7, NOTE_E7, NOTE_F7, NOTE_FS7, NOTE_G7, NOTE_GS7, NOTE_A7, NOTE_AS7, NOTE_B7,
    NOTE_C7, NOTE_CS7, NOTE_D7, NOTE_DS7, NOTE_E7, NOTE_F7, NOTE_FS7, NOTE_G7, NOTE_GS7, NOTE_A7, NOTE_AS7, NOTE_B7,
	NOTE_C8, NOTE_CS8, NOTE_D8, NOTE_DS8, 
};

uint16_t Music::frequency[NOTES_LEN] = { 0,
	//   1,        2,       3,        4,       5,       6,        7,       8,        9,       10,      11,       12
	NOTE_C3, NOTE_CS3, NOTE_D3, NOTE_DS3, NOTE_E3, NOTE_F3, NOTE_FS3, NOTE_G3, NOTE_GS3, NOTE_A3, NOTE_AS3, NOTE_B3,
    NOTE_C4, NOTE_CS4, NOTE_D4, NOTE_DS4, NOTE_E4, NOTE_F4, NOTE_FS4, NOTE_G4, NOTE_GS4, NOTE_A4, NOTE_AS4, NOTE_B4,
    NOTE_C5, NOTE_CS5, NOTE_D5, NOTE_DS5, NOTE_E5, NOTE_F5, NOTE_FS5, NOTE_G5, NOTE_GS5, NOTE_A5, NOTE_AS5, NOTE_B5,
    NOTE_C6, NOTE_CS6, NOTE_D6, NOTE_DS6, NOTE_E6, NOTE_F6, NOTE_FS6, NOTE_G6, NOTE_GS6, NOTE_A6, NOTE_AS6, NOTE_B6,
    NOTE_C7, NOTE_CS7, NOTE_D7, NOTE_DS7, NOTE_E7, NOTE_F7, NOTE_FS7, NOTE_G7, NOTE_GS7, NOTE_A7, NOTE_AS7, NOTE_B7,
	NOTE_C8, NOTE_CS8, NOTE_D8, NOTE_DS8, 
};
uint16_t Music::volume[NOTES_LEN] = { 0,
	//   1,        2,       3,        4,       5,       6,        7,       8,        9,       10,      11,       12
	NOTE_C3, NOTE_CS3, NOTE_D3, NOTE_DS3, NOTE_E3, NOTE_F3, NOTE_FS3, NOTE_G3, NOTE_GS3, NOTE_A3, NOTE_AS3, NOTE_B3,
    NOTE_C4, NOTE_CS4, NOTE_D4, NOTE_DS4, NOTE_E4, NOTE_F4, NOTE_FS4, NOTE_G4, NOTE_GS4, NOTE_A4, NOTE_AS4, NOTE_B4,
    NOTE_C5, NOTE_CS5, NOTE_D5, NOTE_DS5, NOTE_E5, NOTE_F5, NOTE_FS5, NOTE_G5, NOTE_GS5, NOTE_A5, NOTE_AS5, NOTE_B5,
    NOTE_C6, NOTE_CS6, NOTE_D6, NOTE_DS6, NOTE_E6, NOTE_F6, NOTE_FS6, NOTE_G6, NOTE_GS6, NOTE_A6, NOTE_AS6, NOTE_B6,
    NOTE_C7, NOTE_CS7, NOTE_D7, NOTE_DS7, NOTE_E7, NOTE_F7, NOTE_FS7, NOTE_G7, NOTE_GS7, NOTE_A7, NOTE_AS7, NOTE_B7,
	NOTE_C8, NOTE_CS8, NOTE_D8, NOTE_DS8, 
};

const uint32_t Music::pwm_frequency=48000000; // pwm 采用 48M时钟
uint16_t Music::g_volume=10;
const musical_t Music::musicals[musical_play_size] = 
{
	{   //0 虫儿飞
	  .rtttl     = "ChongErFei:d=4,o=5,b=103,c=F:", // 歌曲名:<ChongErFei>, 4(d=4)/4拍, 音阶5, 每分钟节拍数 103, 调式: F调
		.melody    = "33345|3.22-|11123|3.77-|632-|632-|632.1|1---|00032|5--43|2--54|345.3|2--01|632.1|521--|43431-|43431.2|1---|0000|",
		.octave    = "-----|-----|-----|--..-|.---|.---|.----|----|-----|-----|-----|-----|-----|.----|.----|------|-------|----|----|",
		.durations = " --  |  -  | --  |  -  |    |    |    -|    |   --|   - |-  --| -   |   --|    -| --  |----  |----  -|    |    |",
	},
	{   //1 虫儿飞
	  .rtttl     = "ChongErFei:d=4,o=5,b=103,c=F:", // 歌曲名:<ChongErFei>, 4(d=4)/4拍, 音阶5, 每分钟节拍数 103, 调式: F调
		.melody    = "33345|3.22-|11123|3.77-|632-|632-|632.1|1---|33345|3.22-|11123|3.77-|632-|632-|632.1|1--32|5--43|2--54|345.3|2--01|632.1|521--|43431-|43431.2|1---|0000|",
		.octave    = "-----|-----|-----|--..-|.---|.---|.----|----|-----|-----|-----|--..-|.---|.---|.----|-----|-----|-----|-----|-----|.----|.----|------|-------|----|----|",
		.durations = " --  |  -  | --  |  -  |    |    |    -|    | --  |  -  | --  |  -  |    |    |    -|   --|   - |-  --| -   |   --|    -| --  |----  |----  -|    |    |",
	},
	{   // 2月亮代表我的心
#if 0
		//.rtttl     = "YueLiangDaiBiaoWoDeXin:d=4,o=5,b=78,c=F#:", // 歌曲名:<YueLiangDaiBiaoWoDeXin>, 4(d=4)/4拍, 音阶5, 每分钟节拍数 78, 调式: F升调
		.rtttl     = "YueLiangDaiBiaoWoDeXin:d=4,o=5,b=78,c=F#:",
		.melody    = "05|1.35.1|7.35.5|6.71.6|5--32|1.1132|1.1123|2.1623|2--05|1.35.1|7.35.5|6.71.6|5--32|1.1132|1.1123|2.6712|1--35|3.215|7--67|6.765|3--5|3.215|7--67|1.11.23|2--0|0000|",
		.octave    = "-.|------|..----|------|-----|------|------|---.--|----.|------|..----|---**-|-----|------|------|--..--|-----|-----|.....|.....|----|-----|.....|-------|----|----|",
		.durations = "--|  -  -|  -  -|  -  -|   --|  - --|  - --|  - --|   --|  -  -|  -  -|  -  -|   --|  - --|  - --|  - --|   --|  -  |   --|  -  |    |  -  |   --|  -  ==|    |    |"
#else
        // http://www.qupu123.com/jipu/p288319.html
		//.rtttl     = "YueLiangDaiBiaoWoDeXin:d=4,o=5,b=78,c=F#:", // 歌曲名:<YueLiangDaiBiaoWoDeXin>, 4(d=4)/4拍, 音阶5, 每分钟节拍数 78, 调式: F升调
		.rtttl     = "YueLiangDaiBiaoWoDeXin:d=4,o=5,b=78,c=F#:",
		.melody    = "05|1.35.1|7.35.5|6.71.6|5--32|1.1132|1.1123|2.1623|2--05|1.35.1|7.35.5|6.71.6|5--32|1.1132|1.1123|2.6712|1--35|3.215|7--67|6.765|3--5|3.215|7--67|1.11.23|2--05|1.35.1|7.35.5|6.71.6|5--32|1.1132|1.1123|2.6712|1--0|0000|",
		.octave    = "-.|------|..----|---**-|-----|------|------|---.--|----.|------|..----|---**-|-----|------|------|--..--|-----|-----|.....|.....|----|-----|.....|-------|----.|------|..----|---**-|-----|------|------|--..--|----|----|",
		.durations = "--|  -  -|  -  -|  -  -|   --|  - --|  - --|  - --|   --|  -  -|  -  -|  -  -|   --|  - --|  - --|  - --|   --|  -  |   --|  -  |    |  -  |   --|  -  ==|   --|  -  -|  -  -|  -  -|   --|  - --|  - --|  - --|    |    |"
#endif
	},
	{  //3 故乡的原风景
		//.rtttl     = "GuXiangDeYuanFengJing:d=4,o=5,b=103,c=F:", // 歌曲名:<GuXiangDeYuanFengJing>, 4(d=4)/4拍, 音阶5, 每分钟节拍数 103, 调式: F调
		.rtttl     = "GuXiangDeYuanFengJing:d=4,o=5,b=60,c=C:",
		.melody    = "5612----321.6|3--5612----|321553|3-5.612.----|32163---|5612----|3216---0|",
		.octave    = "..----------.|---..------|------|--...-------|---.----|..------|---....0|",
		.durations = "----    ==  =|   ----    |== - -|  --==--    |== -    |----    |== -    |",
	},
	{   //4 知道不知道
		.rtttl     = "ZhiDaoBuZhiDao:d=4,o=5,b=82,c=B#:", // 歌曲名:<ZhiDaoBuZhiDao>, 4(d=4)/4拍, 音阶5, 每分钟节拍数 82, 调式: D调
		//.melody    = "33212|35323-|332153|32200|35165.6|16120|226165|5000|52-3|22165|332153|32200|35165.6|1612-|2.6165|5-00|",
		.melody    = "33212|35323-|332153|322.--0|35165.6|1612.-0|226165|5.---0|52-3|22165|332153|322--|35165.6|1612-|2.6165|5-00|",
		.octave    = "-----|------|------|------=|---....|-.----=|--.-..|.----=|----|---..|------|-----|---....|-.---|--.-..|.---|",
		.durations = " --  |----  | -- --|--     |----  -| --    | -- --|      |    | --  | -- --|--   |----  -| --  |  - --|    |",
	},
	{   // you yi tian chang di jiu 友谊天长地久
		.rtttl     = "YouYiDiJiuTianChang:d=2,o=5,b=50,c=F:", // 歌曲名:<YouYiDiJiuTianChang>, 2(d=2)/4拍, 音阶5, 每分钟节拍数 50, 调式: F调
		.melody    = "5|1.113|2.123|1.135|6.6|5.331|2.123|1.665|1.6|5.331|2.126|5.335|6.1|5.331|2.123|1.665|1.0|",
		.octave    = ".|-----|-----|-----|---|-----|-----|--...|--.|-----|-----|-----|--*|-----|-----|--...|---|",
		.durations = "-|--=--|--=--|--=--|  -|--=--|--=--|--=--|  -|--=--|--=--|--=--|  -|--=--|--=--|--=--|   |",
	},
	{
		// 天空之城
		.rtttl     = "TianKongZhiCheng:d=4,o=5,b=103,c=D:", // 歌曲名:<TianKongZhiCheng>, 4(d=4)/4拍, 音阶5, 每分钟节拍数 103, 调式: D调
		.melody    = "67|1.713|7--3|6.561|5--3|4.3411|3--1|7.447|7-067|1.713|7--33|6.561|5--3|41771|231-0|17675|6-012|3.2345|2--5|21711123|3---|671712|1.55-|4321|3--67|1.713|7--3|6.561|5--3|41771|2311-|17675|6---|0000|",
		.octave    = "..|--.--|.--.|....-|.--.|....--|.---|.....|.--..|--.--|.--..|....-|.--.|.-..-|-----|-....|.----|------|---.|--.-----|----|..-.--|--..-|----|---..|--.--|.--.|....-|.--.|.-..-|-----|-....|....|----|",
		.durations = "--|  -  |    |  -  |    |  --- |    |  -  |   --|  -  |   --|  -  |    | --  | -   |--   |   --|     -|    |==== - -|    |-- -- |  -  |    |   --|  -  |    |  -  |    | --  | --  |--   |    |    |",
	},
	{ // 7
		// 天空之城-赵海洋[质谱]
		// https://www.zhaogepu.com/jianpu/314882.html
		.rtttl     = "TianKongZhiCheng:d=4,o=5,b=100,c=D:", // 歌曲名:<TianKongZhiCheng>, 4(d=4)/4拍, 音阶5, 每分钟节拍数 100, 调式: D调
		.melody    = "00067|1.713|7--33|6.561|5--3|4.341|3-0111|7.4#47|7-067|1.713|7--33|6.561|5--23|417711|22311-|176675#|6--12|3.235|2--55|1.713|3---|671722|1.55-|4321|3--3|6-55|321-01|21225|3--3|6-5-|321-01|21227|6--0|",
		.octave    = "-----|**-**|-----|----*|----|----*|---***|------|-----|**-**|-----|----*|-----|-*--**|******|*------|---**|*****|***--|**-**|****|--*-**|**---|****|****|****|****-*|*****|****|****|**** *|**** |----|",
		.durations = "   --|  -  |   --|  -  |    |  -  |  ----|  --  |   --|  -  |   --|  -  |   --| ---- |----  |----   |   --|  -  |   --|  -  |    |--  --|  -  |    |    |    |--  --| --  |    |    |--  --| --  |    |",
	},
	{ // 8
		// 泪的物语
		// http://www.everyonepiano.cn/Number-2709.html
		//.rtttl     = "LeiDeWuYu:d=4,o=5,b=93,c=D:", // 歌曲名:<LeiDeWuYu>, 4(d=4)/4拍, 音阶5, 每分钟节拍数 93, 调式: G调
        .rtttl     = "LeiDeWuYu:d=4,o=5,b=100,c=D:",
        //           (1)                        (5)                            (9)                    (13)                         (17)                      (21)                            (25)                  (29)                                   (33)                          (37)                            (41)                      (45)                        (49)                         (53)                      (57)                                    
		.melody    = "171753|6545-|43236|4.#45#-|17175|6545.3|43213727|16146146|7---|37135|6545-|43236|4.#45#-|37135|6545.3|4321323|1---|135353|36313.3|43226|74#763-|135353|36313.3|43213232|65612|3513|2571|6---|6---|351351136136|2572572573571571|6--66|5351155#-|135353|36313.3|43236|4#445#-|135353|36313.3|43213232|1561235|3---|3--33|363-35|531175|2-3236|61335.|62.23.|1235612|1611111|371753|6545-|43236|4.#45#-|17175|6545.3|43213727|16146146|6---|0---|",
		.octave    = "H*H***|*****|*****|*******|H*H**|******|*******-|*-***HHH|HHHH|*-*--|-----|-----|-------|*-*--|------|-------|----|*-----|-.-----|-----|.....--|*-----|-.-----|--------|.--**|----|---*|----|----|-.-----..--.|-..--.------*--*|-----|---**----|*-----|-.-----|-----|-------|*-----|-.-----|--------|---****|****|*****|******|****--|--****|*HHH**|******|-------|-------|**H***|*****|*****|*******|H*H**|******|*******-|*-***HHH|H---|----|",
		.durations = " -- --| --  | --  |   -   | --  | --  -|--------|--------|    | --  | --  | --  |   -   | --  | --  -|---- --|    |----  |----  -| --  |-----  |----  |----  -|---- -==| --  |    |    |    |    |====--====--|================|   --|====--   |----  |----  -| --  |---    |----  |----   |---- -==| ------|    |   --|--  --|- -- -|  ----|- --  |-  -  | ------| ------| -- --| --  | --  |   -   | --  | --  -|--------|--------|    |    |",
	},
    {   //9 
        // 雨的印记 
        // http://www.jianpu.cn/pu/20/205626.htm
#if 0
	    .rtttl     = "YuDeYinJi:d=4,o=4,b=60,c=A#:", // 歌曲名:<YuDeYinJi>, 4(d=4)/4拍, 音阶5, 每分钟节拍数 100, 调式: A调
        //.rtttl     = "YuDeYinJi:d=4,o=5,b=60,c=F:",
		.melody    = "0512|233-0123|2557-0567|71102321|7-0175|566-0654|45511-012|34444-54|3-2512|233-0123|255770567|71102321|7-0175|566-054|4551-012|34617|1-0135|6-0716|6550715|544332221|2330135|6-07765|50345443|4556.771|3.42512|3.22512|1-023|346171|1--0|0000|",
		.octave    = "-.--|--------|---..----|-**-****|---*--|--------|---------|--------|------|--------|---..----|-**-****|---*--|-------|--------|--.-.|------|---.--|----.--|---------|-------|-------|--------|-------*|****.--|****.--|-----|--.-.-|----|----|",
		.durations = "====|--  ====|---- ====|-- ---==|  ----|--  ====|---== -==|---== --|  ----|--  ====|---- ====|-- ---==|  ----|--  -==|---- -==|- -  |  ----|  ----|-- ----|-----==--|-- ----|  ---==| -==----| ----=--|  =----|  -----|   --|- - --|    |    |",
#else
	    //.rtttl     = "YuDeYinJi:d=4,o=5,b=60,c=A#:", // 歌曲名:<YuDeYinJi>, 4(d=4)/4拍, 音阶5, 每分钟节拍数 100, 调式: A调
        .rtttl     = "YuDeYinJi:d=4,o=5,b=60,c=A#:",
		.melody    = "512|233-0123|2557-0567|71102321|7-0175|566-0654|45511-012|34444-54|3-2512|233-0123|2557-0567|71102321|7-0175|566-054|4551-012|34617|1-023|346171|1--0|0000|",
		.octave    = ".--|--------|---..----|-**-****|---*--|--------|---------|--------|------|--------|---..----|-**-****|---*--|-------|--------|--.-.|-----|--.-.-|----|----|",
		.durations = "===|--  ====|---- ====|-- ---==|  ----|--  ====|---== -==|---== --|  ----|--  ====|---- ====|-- ---==|  ----|--  -==|---- -==|- -  |   --|- - --|    |    |",
#endif
	},
    {   //10 新鸳鸯蝴蝶梦
        // http://www.qupu123.com/jipu/p109532.html
        // http://www.jianpu.cn/pu/84/84532.htm
        .rtttl     = "XinYuanYangHuDieMeng:d=4,o=5,b=100,c=G:", // 歌曲名:<XinYuanYangHuDieMeng>, 4(d=4)/4拍, 音阶5, 每分钟节拍数 100, 调式: G调
        //            %3               %2                            %2              %3               %2                            %2                   %2                              %3                               %0                                                   
//		.melody    = "35|6653321|23216-|23216123|2--35|6653321|23216-|2321656|1----0035|6653321|23216-|23216123|2--35|6653321|23216-|23216556|1--0|000321|2232.12|1656012|35565321|21612-|35563.2|1236.765|0535356|0161612|0323556|1--0|0000|",
//		.octave    = "--|-------|----..|----.---|-----|-------|----..|----...|---------|-------|----..|----.---|-----|-------|----..|----....|----|------|-------|-...---|--------|--.---|-------|---.....|-......|--.-.--|-----..|----|----|",
//		.durations = "--|---- --|----  |---- -==|   --|---- --|----  |-------|       --|---- --|----  |---- -==|   --|---- --|----  |--------|    |  ----|---  --|---- --|- ---==-|----  |- ==  -|- ---OO |------ |------ |---- --|    |    |",
//		.melody    = "35|6653321|23216-|23216123|2--35|6653321|23216-|2321656|1----0035|6653321|23216-|23216123|2--35|6653321|23216-|2321656|1--0|000321|2232.12|1656012|35565321|21612-|35563.2|1236-|0535356|0161612|0323556|1--0|0000|",
//		.octave    = "--|-------|----..|----.---|-----|-------|----..|----...|---------|-------|----..|----.---|-----|-------|----..|----...|----|------|-------|-...---|--------|--.---|-------|---..|-......|--.-.--|-----..|----|----|",
//		.durations = "--|---- --|----  |---- -==|   --|---- --|----  |---- --|       --|---- --|----  |---- -==|   --|---- --|----  |---- --|    |  ----|---  --|---- --|- ---==-|----  |- ==  -|- -  |------ |------ |---- --|    |    |",
		.melody    = "35|6653321|23216-|2321613|2--35|6655321|23216-|2321656|1----0035|6655321|23216-|2321613|2--35|6653321|23216-|2321656|1---|000321|2232.12|1656-12|3566532|21612-|3563.2|1236765|0535356|0161612|0323556|1--0|0000|",
		.octave    = "--|-------|----..|----.--|-----|-------|----..|----...|---------|-------|----..|----.--|-----|-------|----..|----...|----|------|-------|-...---|-------|--.---|------|---...-|-......|--.-.--|-----..|----|----|",
		.durations = "--|---- --|----  |---- --|   --|---- --|----  |---- --|       --|---- --|----  |---- --|   --|---- --|----  |---- --|    |  ----|---  --|---- --|- -----|----  |- -  -|- --== |------ |------ |---- --|    |    |",
	},
};

char Music::_rtttl_buf[128];
char Music::_melody_buf[BUFFER_SIZE];
char Music::_octave_buf[BUFFER_SIZE];
char Music::_durations_buf[BUFFER_SIZE];
musical_t Music::musical;
uint8_t Music::_pause=0;

void Music::test(void)
{
//	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);  
//	delay_ms(500);
//	HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_4);
#if 0
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);
#endif
    speaker.note_update(0, 0);
    speaker.start();
	while(1)
	{
#if 0
		play(&musicals[1]); 
		delay_ms(2000);
#else
		play(&musicals[2]); // 2月亮代表我的心
		delay_ms(2000);
		play(&musicals[7]); // 天空之城-赵海洋[质谱]
		delay_ms(2000);
		play(&musicals[5]); // you yi tian chang di jiu 友谊天长地久
		delay_ms(2000);
		play(&musicals[4]); // 知道不知道
		delay_ms(2000);
		play(&musicals[1]); //1 虫儿飞
		delay_ms(2000);
#endif
	}
}

void Music::Init(void)
{
	int i=0;
	frequency[0] = 0;
	volume[0] = 0;
	for(i=1; i<NOTES_LEN; i++)
	{
		 frequency[i]=pwm_frequency/notes[i];
		 volume[i]=pwm_frequency/notes[i]/100;
	}
    speaker.Init(notes, NOTES_LEN);
}

void Music::play(void)
{
    _pause = 0;
    play(&musical);
}

uint16_t Music::rtttl(const char* _rtttl)
{
    uint16_t num;
    uint16_t pos=0;
    default_dur = 4;
    default_oct = 6;
	default_mode = NOTE_C; // 默认C调
    bpm = 63;
    // "ChongErFei:d=4,o=5,b=103,c=F:"
    for(pos=0; ':'!=_rtttl[pos]; pos++)
    {
        if('\0'==_rtttl[pos]) return -1;
    }
    pos++;
	// 计算拍数
    if(_rtttl[pos] == 'd')
    {
        pos+=2;  // skip "d="
        num = 0;
        while(isdigit(_rtttl[pos])){
            num = (num * 10) + (_rtttl[pos++] - '0');
        }
        if(num > 0){
            default_dur = num;
        }
        pos++;                   // skip comma
    }

    // get default octave
    if(_rtttl[pos] == 'o')
    {
        pos+=2;              // skip "o="
        num = _rtttl[pos++] - '0';
        if(num >= 3 && num <=7){
            default_oct = num;
        }
        pos++;                   // skip comma
    }
    default_oct = default_oct-OCTAVE_START;

    // get BPM
    if(_rtttl[pos] == 'b'){
        pos+=2;              // skip "b="
        num = 0;
        while(isdigit(_rtttl[pos])){
            num = (num * 10) + (_rtttl[pos++] - '0');
        }
        bpm = num;
        pos++;                   // skip comma/colon
    }
    // get mode,获取调式
    if(_rtttl[pos] == 'c'){
        pos+=2;              // skip "c="
        switch(_rtttl[pos])
        {
            case 'C':
                default_mode = NOTE_C;
                break;
            case 'D':
                default_mode = NOTE_D;
                break;
            case 'E':
                default_mode = NOTE_E;
                break;
            case 'F':
                default_mode = NOTE_F;
                break;
            case 'G':
                default_mode = NOTE_G;
                break;
            case 'A':
                default_mode = NOTE_A;
                break;
            case 'B':
                default_mode = NOTE_B;
                break;
            default:
                default_mode = NOTE_C;
            break;
        }
        pos++;                   // skip colon
        if('#'==_rtttl[pos]) // 升调
        {
            default_mode++;
            pos++;                   // skip colon
        }
    }
    // BPM usually expresses the number of quarter notes per minute
    //wholenote = (60 * 1000L / bpm) * 4;  // this is the time for whole note (in milliseconds)
    wholenote = (60 * 1000L / bpm);  // this is the time for whole note (in milliseconds)
    return pos;
}
void Music::play(const musical_t *_musical)
{
	uint16_t len = strlen(_musical->melody);
	//if(('\0'!=_melody->tone[len]) || ('\0'!=_melody->time[len]) || ('\0'!=_melody->note[len])) return ;
	uint16_t i=0;
	tone_t _note;
	len = strlen(_musical->melody);
	//if(('\0'!=_melody->tone[len]) || ('\0'!=_melody->time[len]) || ('\0'!=_melody->note[len])) return ;
	i=0;
	// rtttl
	rtttl(_musical->rtttl);
	while(i<len)
	{
		uint8_t _melody = 0;
		uint8_t _octave = 0;
		uint8_t _durations = 0;
		uint16_t count=0;
		uint16_t _beat;
		count=0;
		memset(note_buf, 0, sizeof(note_buf));
		while(i<len) //解析歌曲
		{
			_melody = _musical->melody[i];
			_octave = _musical->octave[i];
			_durations = _musical->durations[i];
			memset(&_note, 0, sizeof(_note));
			i++;
			if(('0'<=_melody) && ('7'>=_melody)) //do, rai, me, fa, suo, la, xi  
			{
				//_note.tone = sketch_tmp-'0'; // 1234567
				switch(_melody)
				{
						case '1':
							_note.tone = 1;
						break;
						case '2':
							_note.tone = 3;
						break;
						case '3':
							_note.tone = 5;
						break;
						case '4':
							_note.tone = 6;
						break;
						case '5':
							_note.tone = 8;
						break;
						case '6':
							_note.tone = 10;
						break;
						case '7':
							_note.tone = 12;
						break;
						case '0':
						default:
							_note.tone = 0;
				}
				// 音阶
				if(_note.tone>0)
				{
#if 0 // 3个音阶
						if('*'==_octave) _note.tone += (12*2);  // 高音
						else if('-'==_octave) _note.tone += (12*1);  // 中音
						else if('.'==_octave) _note.tone += (12*0);  // 低音
						//else if('H'==_octave) _note.tone += (12*3);  // 两个音阶高音
						//else if('L'==_octave) _note.tone -= (12*1);  // 两个音阶低音
						else _note.tone += (12*1);                   // 默认中音
#else // 5个音阶
						if('*'==_octave) _note.tone += (12*(default_oct+2));       // 高音
						else if('-'==_octave) _note.tone += (12*(default_oct+1));  // 中音
						else if('.'==_octave) _note.tone += (12*(default_oct-0));  // 低音
						else if('H'==_octave) _note.tone += (12*(default_oct+3));  // 两个音阶高音
						else if('L'==_octave) _note.tone += (12*(default_oct-1));  // 两个音阶低音
						else _note.tone += (12*(default_oct+1));                   // 默认中音
#endif
				}
				// now, get optional '#' sharp, 升调
				_melody = _musical->melody[i];
				if('#'==_melody){
						_note.tone++;
						i++;
				}
				if(' '==_durations) _beat = 1;     //全音符
				else if('-'==_durations) _beat = 2;//二分音符
				else if('='==_durations) _beat = 4;//四分音符
                else if('O'==_durations) _beat = 8;//八分音符
				else _note.beat = 4;               //四分音符
				_note.beat = wholenote/_beat;
				_melody = _musical->melody[i];
				if('.'==_melody) // 附点音符
				{
					_note.beat += _note.beat>>1;
					i++;
				}
				_note.hold = 0;
				_melody = _musical->melody[i];
				while('-'==_melody)
				{
					i++;
					_note.hold++;
					_melody = _musical->melody[i];
				}
				if(_note.tone>0)
				{
#if 0
						_note.tone += (default_oct-OCTAVE_START)*12;    // 设置默认音阶
#endif
						_note.tone += default_mode;          // 设置调式
				}
				note_buf[count].tone = _note.tone;
				note_buf[count].beat = _note.beat;
				note_buf[count].hold = _note.hold;
				count++;
			}
			//else break;
			if(count >= (sizeof(note_buf)/sizeof(note_buf[0])) ) break;
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
				//play_tone_t((_note->tone%8), _note->beat*tone_div_16, _note->hold*tone_div_16, rgb->rgb.r, rgb->rgb.g, rgb->rgb.b);
				//play_tone_t(_note.tone, _note.beat*tone_div_16, _note.hold*tone_div_16, rgb->rgb.r*brightness, rgb->rgb.g*brightness, rgb->rgb.b*brightness);
				//Pulse_frequency_Value(note_table[_note.tone][0], note_table[_note.tone][0]);
				//Pulse_frequency_Value(note_table[1][0], note_table[_note.tone][0]);
				play_tone(_note.tone, _note.beat, _note.hold*wholenote, g_volume);
                if(1==_pause) return; // pause
			}
			count=0;
		}
	}
}
void Music::play_tone(const uint8_t _tone, const uint16_t _beat, const uint16_t _hold, const uint16_t _volume)
{
    int16_t delay = 0;
    int16_t i=0;
    delay = (_beat-20)/2;//+20; //(tick*_note->beat)/TICK_DIV_1/2;
    delay = delay - (delay&0x1);
    {
        speaker.note_update(_tone, 0);
        for(i=0; i<delay; i+=2)
        {
            speaker.volume_update(_tone, ((float)_volume*i)/delay);
            delay_ms(2);
        }
        speaker.volume_update(_tone, _volume);
        delay = _beat+_hold-20-delay;
        for(i=delay; i>=30; i-=1)
        {
            speaker.volume_update(_tone, ((float)_volume*i)/delay);
            delay_ms(1);
        }
        speaker.volume_update(_tone, 0);
        delay_ms(50);
    }
}


