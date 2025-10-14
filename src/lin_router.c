#include <stdio.h>
#include "lin_router.h"

static void lin_print_frame(const LINFrame *frame) {
	printf("Received PID = 0x%02X\n", frame->pid);

	printf("Data: ");
	for (size_t i = 0; i < LIN_MAX_DATA; i++)
		printf("0x%02X", frame->data[i]);

	printf("\nChecksum: 0x%02X\n", frame->checksum);
}

void lin_set_print_handler() {
	lin_set_handler(lin_print_frame);
}

static void lin_diagnostic_handler(const LINFrame *frame) {
	printf("--- LIN DIAGNOSTIC ---\n");
	printf("PID: 0x%02X - Data: ", frame->pid);
	for (size_t i = 0; i < LIN_MAX_DATA; i++)
		printf("0x%02X", frame->data[i]);
	printf("\n----------------------\n");
}

static void lin_tuning_handler(const LINFrame *frame) {
	printf("--- LIN TUNING ---\n");
	printf("PID: 0x%02X - Data: ", frame->pid);
	for (size_t i = 0; i < LIN_MAX_DATA; i++)
		printf("0x%02X", frame->data[i]);
	printf("\n----------------------\n");
}

static void lin_frame_router(const LINFrame *frame) {
	uint8_t id = frame->pid & 0x3F // 0x00111111
	switch (id) {
		case 0x3C:
			lin_diagnostic_handler(frame);
			break;
		case 0x3D:
			lin_diagnostic_handler(frame);
			break;
		case 0x10:
			lin_tuning_handler(frame);
			break;
		default:9
			lin_print_frame(frame);
	}
}

void lin_set_router_handler() {
	lin_set_handler(lin_frame_router);
}
