#include "lin_diagnostic.h"

// LIN_CMD_READ 0x01
// LIN_CMD_UPDATE 0x02
// LIN_CMD_PRINT 0x03

static uint8_t counter = 100;

static void diagnostic_handler(const LINFrame *frame) {
	uint8_t cmd = frame->data[0];

	switch (cmd) {
		case LIN_CMD_READ:
			printf("--- DIAGNOSTIC ---\n");
			printf("value of counter: %d\n", counter);
			printf("------------------\n");
			break;
		case LIN_CMD_UPDATE:
			printf("--- DIAGNOSTIC ---\n");
			counter = frame->data[2];
			printf("value of counter updated: %d\n", counter);
			printf("------------------\n");
			break;
		case LIN_CMD_PRINT:
			printf("--- DIAGNOSTIC ---\n");
			printf("data: ");
			for (size_t i = 1; i < LIN_MAX_DATA; i++) {
				printf("%c", frame->data[i]);
			}
			printf("\n------------------\n");
			break;
		default:
			printf("--- DIAGNOSTIC ---\n");
			printf("unknown command: 0x%02X\n", cmd);
			printf("------------------\n");
	}
}
