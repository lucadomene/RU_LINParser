#ifndef LIN_TUNING
#define LIN_TUNING

#include "lin.h"
#include <stdbool.h>

#define DRS 0x0A
#define SUS_STIFF 0x0B
#define BRK_FORCE 0x0C
#define LIGHTS 0x0D

void lin_tuning_handler(const LINFrame *frame);

#endif
