#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>
#include "lin_tuning.h"
#include "lin_logger.h"

#define LIN_TUNING_MAX_LOG 60

struct CarParams {
	bool drs;
	uint8_t suspension_stiff;
	uint8_t brake_force;
	bool lights;
};

static struct CarParams car_params = {
	false,
	100,
	50,
	true
};

void update_setting(uint8_t setting, uint8_t measure, char* log) {
	switch (setting) {
		case DRS:
			car_params.drs = (bool)measure;
			sprintf(log, "(drf) new value: %d", car_params.drs);
			break;
		case SUS_STIFF:
			car_params.suspension_stiff = measure;
			sprintf(log, "(suspension stiffness) new value: %d", car_params.suspension_stiff);
			break;
		case BRK_FORCE:
			car_params.brake_force = measure;
			sprintf(log, "(brake force) new value: %d", car_params.brake_force);
			break;
		case LIGHTS:
			car_params.lights = (bool)measure;
			sprintf(log, "(lights) new value: %d", car_params.lights);
			break;
		default:
			sprintf(log, "unknown setting %d with measure %d", setting, measure);
	}
}

void lin_tuning_handler(const LINFrame *frame) {
	uint8_t setting = frame->data[0];
	uint8_t measure = frame->data[1];

	char* log = (char*)malloc(LIN_TUNING_MAX_LOG);

	update_setting(setting, measure, log);
	printf("i am here!\n");
	print_log("tuning", log);

	free(log);
}
