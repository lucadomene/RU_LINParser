#include <stdio.h>
// #include <signal.h>
#include "lin.h"
#include "lin_fifo_interface.h"

// static volatile sig_atomic_t stop = 0;
//
// static void handle_sigint(int sig) {
// 	(void)sig;
// 	stop = 1;
// }

int main(int argc, char* argv[]) {
	// signal(SIGINT, handle_sigint);
	// signal(SIGTERM, handle_sigint);
	if (argc != 10) {
		fprintf(stderr, "usage: %s id data1 ... data8\n", argv[0]);
		return 1;
	}

	int fd = lin_init_fifo();
	if (fd == -1) {
		fprintf(stderr, "error opening FIFO\n");
		return 3;
	}

	uint8_t id;
	uint8_t data[LIN_MAX_DATA];

	if (sscanf(argv[1], "%hhx", &id) == -1) {
		fprintf(stderr, "invalid input: %s\n", argv[1]);
		return 2;
	}

	for (size_t i = 0; i < LIN_MAX_DATA; i++) {
		if (sscanf(argv[2+i], "%hhx", &data[i]) == -1) {
			fprintf(stderr, "invalid input: %s\n", argv[2+i]);
			return 2;
		}
	}

	uint8_t pid = lin_pid_addparity(id);
	uint8_t checksum = lin_chksum(data, pid);

	LINFrame frame;
	frame.pid = pid;
	for (size_t i = 0; i < LIN_MAX_DATA; i++)
		frame.data[i] = data[i];
	frame.checksum = checksum;

	if (lin_send_frame(fd, &frame) == -1) {
		fprintf(stderr, "error sending frame\n");
		return 3;
	}

	return 0;
}
