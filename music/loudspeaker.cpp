// -*- Mode: C++; c-basic-offset: 4; indent-tabs-mode: nil -*-
/************************ (C) COPYLEFT 2018 Merafour *************************

* File Name          : loudspeaker.cpp
* Author             : Merafour
* Last Modified Date : 08/11/2018
* Description        : This file provides the loudspeaker library.
********************************************************************************
* https://merafour.blog.163.com
* merafour@163.com
* https://github.com/merafour
******************************************************************************/


#include "loudspeaker.h"

#include <stdarg.h>
#include <string.h>

#include "main.h"
#include "stm32f1xx_hal.h"
#include "tim.h"
#include "gpio.h"

void loudspeaker::Init(const uint16_t notes[], const uint16_t notes_len)
{
	int i=0;
	frequency[0] = 0;
	volume[0] = 0;
	for(i=1; i<notes_len; i++)
	{
		 frequency[i]=sampling_rate/notes[i];
		 volume[i]=sampling_rate/notes[i]/100;
	}
}

void beep_speaker::Init(const uint16_t notes[], const uint16_t notes_len)
{
    //sampling_rate = 1000000; 1M
    sampling_rate = 48000000; // 48M
    loudspeaker::Init(notes, notes_len);
    MX_TIM3_Init();
}

void beep_speaker::start(void)
{
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);
}
void beep_speaker::stop(void)
{
    HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_4);  
}
void beep_speaker::note_update(const uint16_t _note, const float _volume)
{
    uint16_t note_Period=frequency[_note];
    uint16_t volume_Pulse=volume[_note]*_volume;
#if 0
		__HAL_TIM_SET_AUTORELOAD(&htim3, 1000000/note_table[note][0]-1);
		__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, 10000/note_table[note][0]); 
#else
	//HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_4);
	//__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, note_table[note][1]*volume/100); 
		__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, 0);
		HAL_Delay(1);
		__HAL_TIM_SET_AUTORELOAD(&htim3, note_Period);
		__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, volume_Pulse);
	//HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);  	
#endif
    HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
}
void beep_speaker::volume_update(const uint16_t _note, const float _volume)
{
    uint16_t volume_Pulse=volume[_note]*_volume;
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, volume_Pulse);
}





