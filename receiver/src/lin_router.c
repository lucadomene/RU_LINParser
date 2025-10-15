#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include "lin_router.h"
#include "lin_diagnostic.h"
#include "lin_tuning.h"

// print frame with current time
static void lin_print_frame(const LINFrame *frame) {
	time_t currentTime;
	time(&currentTime);
	printf("%s [debug] ", ctime(&currentTime));

	printf("{ pid = 0x%02X; ", frame->pid);

	printf("data: ");
	for (size_t i = 0; i < LIN_MAX_DATA; i++)
		printf("0x%02X", frame->data[i]);

	printf("; checksum: 0x%02X }\n", frame->checksum);
}

// routing handle, dispatches the packet to different handlers
static void lin_frame_router(const LINFrame *frame) {
	uint8_t id = frame->pid & 0x3F; // 0x00111111
	switch (id) {
		case 0x3C:
			lin_diagnostic_handler(frame);
			break;
		case 0x3D:
			lin_tuning_handler(frame);
			break;
		default:
			lin_print_frame(frame);
	}
}

// set the routing handle
void lin_set_router_handler() {
	lin_set_handler(lin_frame_router);
}
