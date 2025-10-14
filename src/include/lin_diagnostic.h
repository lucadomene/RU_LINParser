#ifndef LIN_DIAGNOSTIC
#define LIN_DIAGNOSTIC

#include "lin.h"

#define LIN_CMD_READ 0x01
#define LIN_CMD_UPDATE 0x02
#define LIN_CMD_PRINT 0x03

void lin_diagnostic_handler(const LINFrame *frame);

#endif
