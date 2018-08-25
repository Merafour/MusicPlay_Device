/************************ (C) COPYLEFT 2018 Merafour *************************

* File Name          : RTTTL.h
* Author             : Merafour
* Last Modified Date : 08/11/2018
* Description        : This file provides the RTTTL library.
********************************************************************************
* https://merafour.blog.163.com
* merafour@163.com
* https://github.com/merafour
******************************************************************************/
#ifndef __RTTTL_H__
#define __RTTTL_H__

#include <stdint.h>
#include <inttypes.h>

#include "music.h"

class RTTTL:public Music {
private:
protected:

	virtual void play_tone(const uint8_t _tone, const uint16_t _beat, const uint16_t _hold, const uint16_t _volume);
public:
    RTTTL(loudspeaker &_speaker) : Music(_speaker) {}
	virtual void Init(void);
	virtual void play(const musical_t *_melody);
    void play(const char *_rtttl);
    virtual void play(void);
	uint16_t decode_rtttl(const char *_rtttl);
	virtual void test(void);
};

#endif // __RTTTL_H__

