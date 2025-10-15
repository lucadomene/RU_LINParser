#define _POSIX_C_SOURCE 200809L
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include "lin_fifo_interface.h"
#include "lin.h"

static const char *FIFO_PATH = "/tmp/lin_fifo";

int lin_init_fifo() {
	if (mkfifo(FIFO_PATH, 0666) != 0) {
		if (errno != EEXIST) {
			perror("mkfifo");
			return -1;
		}
	}

	int fd = open(FIFO_PATH, O_RDWR);
	if (fd == -1) {
		perror("open fifo");
		return -1;
	}
	return fd;
}

int lin_send_frame(int fd, LINFrame* frame) {
	uint8_t pid = lin_pid_addparity(frame->pid);
	frame->checksum = lin_chksum(frame->data, pid);

	size_t total_bytes = 2 + LIN_MAX_DATA + 1;

	uint8_t raw[total_bytes];
	raw[0] = LIN_SYNC_BYTE;
	raw[1] = pid;
	for (size_t i = 0; i < LIN_MAX_DATA; i++)
		raw[2 + i] = frame->data[i];
	raw[total_bytes - 1] = frame->checksum;

	if (write(fd, raw, total_bytes) == -1) {
		perror("write");
		return -1;
	}

	return 0;
}

int lin_receive_raw(int fd, uint8_t* raw) {
	size_t total_bytes = 2 + LIN_MAX_DATA + 1;
	if (read(fd, raw, total_bytes) <= 0) {
		perror("read");
		return -1;
	}
	return 0;
}
