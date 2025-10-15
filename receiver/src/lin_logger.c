#include <time.h>
#include <stddef.h>
#include <stdio.h>
#include "lin_logger.h"

static FILE* logfile;

int lin_logger_init(const char* filename) {
	logfile = fopen(filename, "a");
	if (logfile == NULL) {
		perror("fopen");
		return 2;
	}
	return 0;
}

void print_log(const char* module, char* log) {
	time_t raw_time;
	struct tm* time_info;
	char time_buffer[80];

	time(&raw_time);
	time_info = localtime(&raw_time);

	strftime(time_buffer, 80, "%D %T %Z", time_info);
	fprintf(logfile, "%s: [%s] %s\n", time_buffer, module, log);
}
