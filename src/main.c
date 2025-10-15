#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include "lin.h"
#include "lin_diagnostic.h"
#include "lin_router.h"
#include "lin_logger.h"
#include "lin_fifo_interface.h"

static volatile sig_atomic_t stop = 0;

static void handle_sigint(int sig) {
	(void)sig;
	stop = 1;
}

int main (int argc, char* argv[]) {
	printf("Setting up custom diagnostic handler...\n");
	lin_set_router_handler();
	printf("Successfully set handler!\n");

	printf("Setting up logger...\n");
	if (lin_logger_init("lin.log") != 0) {
		fprintf(stderr, "Error setting up logger!\n");
		return 1;
	}
	printf("Successfully set logger!\n");

	signal(SIGINT, handle_sigint);
	signal(SIGTERM, handle_sigint);

	int fd;
	if ((fd = lin_init_fifo()) == -1)
		return 2;

	uint8_t* raw_frame = (uint8_t*)malloc(2 + LIN_MAX_DATA + 1);

	LINFrame frame;
	LINStatus status;

	while (!stop) {
		if (lin_receive_raw(fd, raw_frame) == -1)
			return 3;

		status = lin_parse_frame(raw_frame, &frame);
		if (status != LIN_OK) {
			printf("Parsing error %d\n", status);
		}
	}

	free(raw_frame);
	close(fd);

	printf("Exiting...\n");

	return 0;
}
