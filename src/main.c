#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include "lin.h"
#include "lin_diagnostic.h"
#include "lin_router.h"

int main (int argc, char* argv[]) {
	printf("Setting up custom diagnostic handler...\n");
	lin_set_router_handler();
	printf("Successfully set!\n");

	// uint8_t raw_frame[] = {
	// 	0x55, // sync
	// 	0x3C, // pid
	// 	LIN_CMD_UPDATE, // command + data (tot 8 bytes)
	// 	0xAA,
	// 	0x00,
	// 	0x00,
	// 	0x00,
	// 	0x00,
	// 	0x00,
	// 	0x00,
	// 	0x00 // checksum
	// };
	
	uint8_t read_buffer[11];

	ssize_t n_read = read(STDIN_FILENO, read_buffer, 11);
	if (n_read == -1 || n_read != 11) {
		fprintf(stderr, "error on input reading\n");
		return 2;
	}

	uint8_t* raw_frame = read_buffer;

	uint8_t chksum = lin_chksum(&raw_frame[2], raw_frame[1]);
	raw_frame[10] = chksum;

	LINFrame frame;
	LINStatus status = lin_parse_frame(raw_frame, &frame);

	if (status != LIN_OK) {
		printf("Parsing error %d\n", status);
	}
	
	return 0;
}
