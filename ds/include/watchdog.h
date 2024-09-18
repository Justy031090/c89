#ifndef __WD_H__
#define __WD_H__

#include <stddef.h>

int WDStart(size_t threshold, size_t interval, int argc, char **argv);
void WDStop(void);

#endif