#include "lin.h"

static uint8_t lin_bit_parity(uint8_t);

static LINFrameHandler lin_handler = NULL;

static LINErrorHandler lin_errhandler = NULL;

void lin_set_handler(LINFrameHandler handler) {
	lin_handler = handler;
}

void lin_set_errhandler(LINErrorHandler handler) {
	lin_errhandler = handler;
}


LINStatus lin_chk_sync(uint8_t sync) {
	if (sync == LIN_SYNC_BYTE)
		return LIN_OK;
	else return LIN_SYNC_ERR;
}

static uint8_t lin_pid_addparity(uint8_t pid) {
	id = pid & 0x3F; // mask id with 0b0011111
	
	// p0 = id0 ^ id1 ^ id2 ^ id4
	uint8_t p0 = (id & 0x01) ^ ((id >> 1) & 0x01) ^ ((id >> 2) & 0x01) ^ ((id >> 4) & 0x01); 

	// p1 = !(id1 ^ id3 ^ id4 ^ id5)
	uint8_t p1 = !( ((id >> 1) & 0x01) ^ ((id >> 3) & 0x01) ^ ((id >> 4) & 0x01) ^ ((id >> 5) & 0x01) );

	return id | (p0 << 6) | (p1 << 7);
}

LINStatus lin_val_pid(uint8_t pid) {
	uint8_t expected_pid = lin_pid_addparity(pid);
	
	if (pid == expected_pid)
		return LIN_OK;
	else return LIN_PID_ERR;
}

uint8_t lin_chksum(const uint8_t *data, uint8_t pid) {
	uint16_t sum = 0; // need for 2 bytes to preserve the carry bit
	//
#ifdef LIN_ENHANCED_CHKSUM
	sum += pid;
#endif
	
	for (size_t i = 0; i < LIN_MAX_DATA; i++) {
		sum += data[i];
		if (sum > 255)
			sum -= 255
	}

	return (uint8_t)(~sum); // inverted sum;
}

LINStatus lin_val_chksum(const LinFrame *frame) {
	uint8_t expected_chksum = lin_chksum(frame->data, frame->pid);

	if (frame->checksum == expected_checksum)
		return LIN_OK;
	else return LIN_CHKSUM_ERR;
}

LINStatus lin_parse_frame(const uint8_t *raw, LINFrame *out) {
	if (raw == NULL || out == NULL) return LIN_FORMAT_ERR;

	if (lin_chk_sync(raw[0]) != LIN_OK && lin_errhandler)
		return LIN_SYNC_ERR;

	out->pid = raw[1];
	if (lin_val_pid(out->pid) != LIN_OK) {
		return LIN_PID_ERR;
	}

	for (size_t i = 0; i < LIN_MAX_DATA; i++) {
		out->data[i] = raw[2 + i];
	}

	out->checksum = raw[2 + LIN_MAX_DATA + 1];

	if (lin_val_chksum(out) != LIN_OK) 
		return LIN_CHKSUM_ERR;

	if (lin_handler != NULL)
		lin_handler(out);

	return LIN_OK;
}
