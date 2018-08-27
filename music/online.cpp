// -*- Mode: C++; c-basic-offset: 4; indent-tabs-mode: nil -*-
/************************ (C) COPYLEFT 2018 Merafour *************************

* File Name          : online.c
* Author             : Merafour
* Last Modified Date : 08/11/2018
* Description        : This file provides the online library.
********************************************************************************
* https://merafour.blog.163.com
* merafour@163.com
* https://github.com/merafour
******************************************************************************/


#include "online.h"
#include "music.h"

#include <stdarg.h>
#include <string.h>

#define PROTO_PROG_MULTI_MAX    64	// maximum PROG_MULTI size
#define PROTO_READ_MULTI_MAX    255	// size of the size field

/* argument values for PROTO_GET_DEVICE */
#define PROTO_DEVICE_BL_REV	1	// bootloader revision
#define PROTO_DEVICE_BOARD_ID	2	// board ID
#define PROTO_DEVICE_BOARD_REV	3	// board revision
#define PROTO_DEVICE_FW_SIZE	4	// size of flashable area
#define PROTO_DEVICE_VEC_AREA	5	// contents of reserved vectors 7-10
#define PROTO_DEVICE_HW_BL_REV 	15	// hw bootloader revision
#define _BL_REVR							0x00010000
#define _BL_REVX							0x00000400
#define _BL_REVY							0x00000004
#define HW_BOARD_DEFAULT   0x01000000

#define HW_BOARD_REV					HW_BOARD_DEFAULT
#define BL_REVR							_BL_REVR
#define BL_REVX							_BL_REVX
#define BL_REVY							_BL_REVY
#define HW_NAME							"MusicPlsy"
/* board info forwarded from board-specific code to booloader */
struct boardinfo {
	uint32_t	board_type;
	uint32_t	board_rev;
	uint32_t	fw_size;
	// board | bl_revr | bl_revx | bl_revy |
	//uint32_t	bl_rev;   
	uint32_t	hw_bl_rev;   
	const uint8_t     hw_name[16];

} __attribute__((packed));
/* board definition */
struct boardinfo board_info = {
	.board_type	= 0,
	.board_rev	= 0,
	.fw_size	= 4096,
	.hw_bl_rev     = (HW_BOARD_REV & 0xFF000000) | (BL_REVR & 0x00FF0000) | (BL_REVX & 0x0000FF00) | (BL_REVY & 0x000000FF),
	.hw_name    = HW_NAME,
};

static uint32_t rx_head=0, rx_tail=0;
static uint8_t rx_buf[512];

void rx_buf_put(uint8_t b)
{
	unsigned next = (rx_head + 1) % sizeof(rx_buf);

	if (next != rx_tail) {
		rx_buf[rx_head] = b;
		rx_head = next;
	}
}

int rx_buf_get(void)
{
	int	ret = -1;

	if (rx_tail != rx_head) {
		ret = rx_buf[rx_tail];
		rx_tail = (rx_tail + 1) % sizeof(rx_buf);
	}

	return ret;
}
static uint32_t tx_head=0, tx_tail=0;
static uint8_t tx_buf[512];


void tx_buf_put(uint8_t b)
{
	unsigned next = (tx_head + 1) % sizeof(tx_buf);

	if (next != tx_tail) {
		tx_buf[tx_head] = b;
		tx_head = next;
	}
}

int tx_buf_get(void)
{
	int	ret = -1;

	if (tx_tail != tx_head) {
		ret = tx_buf[tx_tail];
		tx_tail = (tx_tail + 1) % sizeof(tx_buf);
	}

	return ret;
}
static volatile unsigned cin_count;
inline void cout_debug(uint8_t buf)
{
    //CDC_Transmit_FS(&buf, 1);
    tx_buf_put(buf);
}
static int ch = -1;
static int cin_wait(unsigned timeout)
{
	//int ch = -1;

	/* start the timeout */
    uint32_t timer=0;
    ch = -1;
    timer = sys_tick() + timeout;

	do {
		ch = rx_buf_get();

		if (ch >= 0) {
			cin_count++;
            //cout_debug(ch);
            return ch;
			break;
		}

	} while (sys_tick() < timer);

	return ch;
}
inline void cout(uint8_t *buf, unsigned len)
{
    //CDC_Transmit_FS(buf, len);
    int i=0;
    for(i=0; i<len; i++)
    {
        tx_buf_put(buf[i]);
    }
}
/**
 * Function to wait for EOC
 *
 * @param timeout length of time in ms to wait for the EOC to be received
 * @return true if the EOC is returned within the timeout perio, else false
 */
inline static bool wait_for_eoc(unsigned timeout)
{
	return cin_wait(timeout) == PROTO_EOC;
}
static void cout_word(uint32_t val)
{
	cout((uint8_t *)&val, 4);
}

static int cin_word(uint32_t *wp, unsigned timeout)
{
	union {
		uint32_t w;
		uint8_t b[4];
	} u;
	for (unsigned i = 0; i < 4; i++) {
		int c = cin_wait(timeout);

		if (c < 0) {
			return c;
		}
		u.b[i] = c & 0xff;
	}
	*wp = u.w;
	return 0;
}
static int cin_half_word(uint16_t *wp, unsigned timeout)
{
	union {
		uint16_t w;
		uint8_t b[2];
	} u;
	for (unsigned i = 0; i < 2; i++) {
		int c = cin_wait(timeout);

		if (c < 0) {
			return c;
		}
		u.b[i] = c & 0xff;
	}

	*wp = u.w;
	return 0;
}

static void sync_response(void)
{
	uint8_t data[] = {
		PROTO_INSYNC,	// "in sync"
		PROTO_OK	// "OK"
	};

	cout(data, sizeof(data));
}
static void
invalid_response(void)
{
	uint8_t data[] = {
		PROTO_INSYNC,	// "in sync"
		PROTO_INVALID	// "invalid command"
	};

	cout(data, sizeof(data));
}

static void
failure_response(void)
{
	uint8_t data[] = {
		PROTO_INSYNC,	// "in sync"
		PROTO_FAILED	// "command failed"
	};

	cout(data, sizeof(data));
}
static uint32_t crc32(const uint8_t *src, const uint32_t len)
{
	uint32_t sum=0;
    uint32_t data=0;
    uint32_t i=0;
    const uint8_t left[4]={0, 8, 16, 24};

    sum=0;
	for (unsigned i = 0; i < len; i++) {
		data = src[i]<<left[i&0x3];
        sum += data;
	}

	return sum;
}
static uint8_t music_buffer[2048];
const uint8_t* get_music(void)
{
    return music_buffer;
}

static uint32_t
crc32(const uint8_t *src, unsigned len, unsigned state)
{
	static uint32_t crctab[256]={0};

	/* check whether we have generated the CRC table yet */
	/* this is much smaller than a static table */
	if (crctab[1] == 0) {
		for (unsigned i = 0; i < 256; i++) {
			uint32_t c = i;

			for (unsigned j = 0; j < 8; j++) {
				if (c & 1) {
					c = 0xedb88320U ^ (c >> 1);

				} else {
					c = c >> 1;
				}
			}

			crctab[i] = c;
		}
	}

	for (unsigned i = 0; i < len; i++) {
		state = crctab[(state ^ src[i]) & 0xff] ^ (state >> 8);
	}

	return state;
}
//static volatile int c=-1;
uint8_t online_update(void)
{
    uint32_t	address = 0;
    uint16_t len;
    uint32_t crc=0;
    board_info.fw_size = sizeof(music_buffer);
	while (true) 
    {
		volatile int c=-1;
        int arg;
		static union {
			uint8_t		c[256];
			uint32_t	w[64];
		} flash_buffer;
        c=-1;

		// Wait for a command byte
		delay_ms(10);

		do {
			/* try to get a byte from the host */
			c = cin_wait(500);

		} while (c < 0);
        //cout_word(c);

		// handle the command byte
		switch (c) {

		// sync
		//
		// command:		GET_SYNC/EOC
		// reply:		INSYNC/OK
		//
		case PROTO_GET_SYNC:

			/* expect EOC */
			if (!wait_for_eoc(2)) {
				goto cmd_bad;
			}

			break;
		// command:		PROTO_MUSIC/<crc32:4>/<len:2>/<data:len>/EOC
		// success reply:	INSYNC/OK
		// invalid reply:	INSYNC/INVALID
		// readback failure:	INSYNC/FAILURE
        case PROTO_MUSIC:
			/* expect EOC */
			if (!wait_for_eoc(2)) {
				goto cmd_bad;
			}
            sync_response();
            return PROTO_MUSIC;
			break;
		// command:		PROTO_PLAY/EOC
		// reply:		INSYNC/OK
		case PROTO_PLAY:
			/* expect EOC */
			if (!wait_for_eoc(2)) {
				goto cmd_bad;
			}
            sync_response();
            return PROTO_PLAY;
			break;
		case PROTO_PAUSE:
			/* expect EOC */
			if (!wait_for_eoc(2)) {
				goto cmd_bad;
			}
            sync_response();
            return PROTO_PAUSE;
			break;
		case PROTO_NEXT:
			/* expect EOC */
			if (!wait_for_eoc(2)) {
				goto cmd_bad;
			}
            sync_response();
            return PROTO_NEXT;
			break;
		case PROTO_LAST:
			/* expect EOC */
			if (!wait_for_eoc(2)) {
				goto cmd_bad;
			}
            sync_response();
            return PROTO_LAST;
			break;
        
		// get device info
		//
		// command:		GET_DEVICE/<arg:1>/EOC
		// BL_REV reply:	<revision:4>/INSYNC/EOC
		// BOARD_ID reply:	<board type:4>/INSYNC/EOC
		// BOARD_REV reply:	<board rev:4>/INSYNC/EOC
		// FW_SIZE reply:	<firmware size:4>/INSYNC/EOC
		// VEC_AREA reply	<vectors 7-10:16>/INSYNC/EOC
		// bad arg reply:	INSYNC/INVALID
		//
		case PROTO_GET_DEVICE:
			/* expect arg then EOC */
			arg = cin_wait(1000);

			if (arg < 0) {
				goto cmd_bad;
			}

			if (!wait_for_eoc(2)) {
				goto cmd_bad;
			}

			switch (arg) {

			case PROTO_DEVICE_BOARD_ID:
				cout((uint8_t *)&board_info.board_type, sizeof(board_info.board_type));
				break;

			case PROTO_DEVICE_BOARD_REV:
				cout((uint8_t *)&board_info.board_rev, sizeof(board_info.board_rev));
				break;

			case PROTO_DEVICE_FW_SIZE:
				cout((uint8_t *)&board_info.fw_size, sizeof(board_info.fw_size));
				break;

			case PROTO_DEVICE_HW_BL_REV:
				cout((uint8_t *)&board_info.hw_bl_rev, sizeof(board_info.hw_bl_rev));
				break;

			default:
				goto cmd_bad;
			}

			break;
		// erase and prepare for programming
		//
		// command:		ERASE/EOC
		// success reply:	INSYNC/OK
		// erase failure:	INSYNC/FAILURE
		//
		case PROTO_CHIP_ERASE:

			/* expect EOC */
			if (!wait_for_eoc(2)) {
				goto cmd_bad;
			}
			memset(music_buffer, 0, sizeof(music_buffer));
			address = 0;
			break;
		// program bytes at current address
		//
		// command:		PROG_MULTI/<len:1>/<data:len>/EOC
		// success reply:	INSYNC/OK
		// invalid reply:	INSYNC/INVALID
		// readback failure:	INSYNC/FAILURE
		case PROTO_PROG_MULTI:		// program bytes
			// expect count
			arg = cin_wait(50);
			//c=0x01; if(bl_type == USART) usb_cout((uint8_t *)&c, 1);
			if (arg < 0) {
				goto cmd_bad;
			}
			//c=0x02; if(bl_type == USART) usb_cout((uint8_t *)&c, 1);
			// sanity-check arguments
			if (arg % 4) {
				goto cmd_bad;
			}
			//c=0x03; if(bl_type == USART) usb_cout((uint8_t *)&c, 1);
			if ((address + arg) > sizeof(music_buffer)) {
				goto cmd_bad;
			}

			if (arg > sizeof(flash_buffer.c)) {
				goto cmd_bad;
			}
			//c=0x05; if(bl_type == USART) usb_cout((uint8_t *)&c, 1);
			for (int i = 0; i < arg; i++) {
				c = cin_wait(1000);

				if (c < 0) {
					goto cmd_bad;
				}

				flash_buffer.c[i] = c;
			}
			if (!wait_for_eoc(200)) {
				goto cmd_bad;
			}
            memcpy(&music_buffer[address], flash_buffer.c, arg);
            address += arg;
			break;

		// fetch CRC of the entire flash area
		//
		// command:			GET_CRC/EOC
		// reply:			<crc:4>/INSYNC/OK
		//
		case PROTO_GET_CRC:

			// expect EOC
			if (!wait_for_eoc(2)) {
				goto cmd_bad;
			}

			// compute CRC of the programmed area
			uint32_t sum = 0;

			for (unsigned p = 0; p < board_info.fw_size; p += 4) 
            {
                sum = crc32(&music_buffer[p], 4, sum);
			}

			cout_word(sum);
			break;
		// fetch CRC of the entire flash area
		//
		// command:			PROTO_VOL_GET/EOC
		// reply:			<crc:4>/INSYNC/OK
		//
		case PROTO_VOL_GET:
			// expect EOC
			if (!wait_for_eoc(2)) {
				goto cmd_bad;
			}
            uint8_t _vol=0;
            _vol = Music::get_volume();
            cout(&_vol, 1);
			break;
		case PROTO_VOL_SET:

            c = cin_wait(1000);
            if (c < 0) {
                goto cmd_bad;
            }
			// expect EOC
			if (!wait_for_eoc(2)) {
				goto cmd_bad;
			}
            Music::set_volume(c);
			break;
        case PROTO_STANDBY:
            for (unsigned p = 0; p < 512; p += 128) 
            {
				cout(&music_buffer[p], 128);
                delay_ms(10);
			}
            break;
		case PROTO_DEBUG:
			// XXX reserved for ad-hoc debugging as required
			break;

		default:
			continue;
		}

		// send the sync response for this command
		sync_response();
		continue;
cmd_bad:
		// send an 'invalid' response but don't kill the timeout - could be garbage
		invalid_response();
		continue;

cmd_fail:
		// send a 'command failed' response but don't kill the timeout - could be garbage
		failure_response();
		continue;
	}
    return 0;
}




