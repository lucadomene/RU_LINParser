#ifndef LIN_FIFO_INTERFACE
#define LIN_FIFO_INTERFACE

#include "lin.h"

int lin_init_fifo();

int lin_send_frame(int fd, LINFrame* frame);

int lin_receive_raw(int fd, uint8_t* raw);

#endif
