#ifndef LIN_LOGGER
#define LIN_LOGGER

#define LIN_LOG_STDOUT 1

int lin_logger_init(const char* filename);

void print_log(const char* module, char* log);

#endif
