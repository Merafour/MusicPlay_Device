/************************ (C) COPYLEFT 2018 Merafour *************************

* File Name          : onlinc.h
* Author             : Merafour
* Last Modified Date : 08/11/2018
* Description        : This file provides the onlinc library.
********************************************************************************
* https://merafour.blog.163.com
* merafour@163.com
* https://github.com/merafour
******************************************************************************/
#ifndef __ONLINE_H__
#define __ONLINE_H__

#include <stdint.h>
#include <inttypes.h>


#ifdef __cplusplus
 extern "C" {
#endif
extern void delay_ms(uint16_t ms);
extern uint32_t sys_tick(void);


/* generic receive buffer for async reads */
extern void rx_buf_put(uint8_t b);
extern int rx_buf_get(void);
extern void tx_buf_put(uint8_t b);
extern int tx_buf_get(void);
extern uint8_t online_update(void);
extern const uint8_t* get_music(void);
// Õ®—∂√¸¡Ó
#define PROTO_INSYNC				0x12    // 'in sync' byte sent before status
#define PROTO_EOC					0x50    // end of command
// Reply bytes
#define PROTO_OK					0x10    // INSYNC/OK      - 'ok' response
#define PROTO_FAILED				0x11    // INSYNC/FAILED  - 'fail' response
#define PROTO_INVALID				0x13	// INSYNC/INVALID - 'invalid' response for bad commands
#define PROTO_BAD_SILICON_REV 		0x14 	// On the F4 series there is an issue with < Rev 3 silicon
// Command bytes
#define PROTO_GET_SYNC				0x21    // NOP for re-establishing sync
#define PROTO_GET_DEVICE			0x22    // get device ID bytes
#define PROTO_DEBUG					0x31    // emit debug information - format not defined
#define PROTO_STANDBY				0x32    // boot the application
#define PROTO_CHIP_ERASE			0x23    // erase program area and reset program address
#define PROTO_PROG_MULTI			0x27    // write bytes at program address and increment
#define PROTO_GET_CRC				0x29	// compute & return a CRC
#define PROTO_MUSIC 				0x60    // music data
#define PROTO_PLAY   				0x64    // play
#define PROTO_PAUSE 				0x65    // pause
#define PROTO_NEXT   				0x66    // next play
#define PROTO_LAST   				0x67    // last play

#ifdef __cplusplus
}
#endif

#endif // __LOUDSPEAKER_H__

