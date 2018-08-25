/************************ (C) COPYLEFT 2018 Merafour *************************

* File Name          : loudspeaker.h
* Author             : Merafour
* Last Modified Date : 08/11/2018
* Description        : This file provides the LED Controller library.
********************************************************************************
* https://merafour.blog.163.com
* merafour@163.com
* https://github.com/merafour
******************************************************************************/
#ifndef __LOUDSPEAKER_H__
#define __LOUDSPEAKER_H__

#include <stdint.h>
#include <inttypes.h>

/**
#define OCTAVE_START   3
//#define NOTES_LEN  (12*4+1+4)
#define NOTES_LEN  (12*(8-OCTAVE_START)+1+4)
*/

//#define LOUD_NOTES_LEN  (12*(8-3)+1+4)
#define LOUD_NOTES_LEN  (12*(8-3)+1+4+12*3)

class loudspeaker {
private:

protected:
	uint32_t sampling_rate;  // ²ÉÑùÂÊ
    uint16_t frequency[LOUD_NOTES_LEN];
    uint16_t volume[LOUD_NOTES_LEN];
public:
    loudspeaker() 
    {
        sampling_rate=1000000;
    }
    virtual void Test(void) {}
    virtual void Init(const uint16_t notes[], const uint16_t notes_len);
    virtual void start(void)=0;
    virtual void stop(void)=0;
    virtual void note_update(const uint16_t _note, const float _volume)=0;
    virtual void volume_update(const uint16_t _note, const float _volume)=0;
};

class beep_speaker : public loudspeaker{
private:

public:
    beep_speaker() {}
	virtual void Test(void) {}
	virtual void Init(const uint16_t notes[], const uint16_t notes_len);
    virtual void start(void);
    virtual void stop(void);
	virtual void note_update(const uint16_t _note, const float _volume);
	virtual void volume_update(const uint16_t _note, const float _volume);
};



#endif // __LOUDSPEAKER_H__

