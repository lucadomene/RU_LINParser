#ifndef LIN_H
#define LIN_H
#include <stdint.h>

#define LIN_SYNC_BYTE 0x55
#define LIN_MAX_DATA 8

#define LIN_ENHANCED_CHKSUM 1

typedef enum {
	LIN_OK,
	LIN_SYNC_ERR,
	LIN_PID_ERR,
	LIN_CHKSUM_ERR,
	LIN_FORMAT_ERR,
} LINStatus;

typedef struct {
	uint8_t pid;
	uint8_t data[LIN_MAX_DATA];
	uint8_t checksum;
} LINFrame;

typedef void (*LINFrameHandler)(const LINFrame *frame);
typedef void (*LINErrorHandler)(const uint8_t *raw, LINStatus status);

LINStatus lin_chk_sync(uint8_t sync_byte);

uint8_t lin_pid_addparity(uint8_t pid);

LINStatus lin_val_pid(uint8_t pid);

uint8_t lin_chksum(const uint8_t *data, uint8_t pid);

LINStatus lin_val_chksum(const LINFrame *frame);

LINStatus lin_parse_frame(const uint8_t *raw, LINFrame *out);

void lin_set_handler(LINFrameHandler handler);

void lin_set_errhandler(LINErrorHandler handler);

#endif
