#ifndef LIN_DIAGNOSTIC
#define LIN_DIAGNOSTIC

#include "lin.h"

#define LIN_DIAG_STAT 0x01
#define LIN_DIAG_WARN 0x02
#define LIN_DIAG_CRIT 0x03
#define LIN_DIAG_FAIL 0x04

#define LIN_DIAG_TIRE 0x0A
#define LIN_DIAG_SPEED 0x0B
#define LIN_DIAG_RPM 0x0C
#define LIN_DIAG_STEER 0x0D

void lin_diagnostic_handler(const LINFrame *frame);

#endif
