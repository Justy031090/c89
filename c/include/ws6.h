#ifndef __WS6_H__
#define __WS6_H__

#include <stdio.h>

typedef void (*func_ptr)(void);

static func_ptr table[256];
void PressKeyLut();
void PressKeyIf();
void PressKeySwitch();

#endif /* __WS6_H__ */
