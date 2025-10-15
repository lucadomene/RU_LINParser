#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "lin_diagnostic.h"
#include "lin_logger.h"

// #define LIN_DIAG_STAT 0x01
// #define LIN_DIAG_WARN 0x02
// #define LIN_DIAG_CRIT 0x03
// #define LIN_DIAG_FAIL 0x04
//
// #define LIN_DIAG_TIRE 0x0A
// #define LIN_DIAG_SPEED 0x0B
// #define LIN_DIAG_RPM 0x0C
// #define LIN_DIAG_STEER 0x0D

#define LIN_DIAG_MAX_LOG 40

// set diagnostic log level
static void print_diag_level(uint8_t level, char* log) {
	switch (level) {
		case LIN_DIAG_STAT:
			sprintf(log, "(status) ");
			break;
		case LIN_DIAG_WARN:
			sprintf(log, "(warning) ");
			break;
		case LIN_DIAG_CRIT:
			sprintf(log, "(CRITICAL) ");
			break;
		case LIN_DIAG_FAIL:
			sprintf(log, "(FAILURE) ");
			break;
		default:
			sprintf(log, "(unknown) ");
	}
}

// print diagnostic log with (dummy) car sensor readings
static void print_diagnostic(uint8_t category, uint8_t measure, char* log) {
	switch (category) {
		case LIN_DIAG_TIRE:
			sprintf(log, "tire pressure: %d psi\n", measure);
			break;
		case LIN_DIAG_SPEED:
			sprintf(log, "speed: %d km/h\n", measure);
			break;
		case LIN_DIAG_RPM:
			sprintf(log, "rpm: %d\n", measure);
			break;
		case LIN_DIAG_STEER:
			sprintf(log, "steer: %d deg\n", measure);
			break;
		default:
			sprintf(log, "unknown measure %d: %d\n", category, measure);
	}
}

// diagnostic handler: receive frame, print diagnostic log
void lin_diagnostic_handler(const LINFrame *frame) {
	uint8_t level = frame->data[0];
	uint8_t category = frame->data[1];
	uint8_t measure = frame->data[2];

	char* log = (char*)malloc(LIN_DIAG_MAX_LOG);

	print_diag_level(level, log);
	print_diagnostic(category, measure, log + strlen(log));
	print_log("diagnostic", log);

	free(log);
}
