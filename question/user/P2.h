#ifndef __P2_H
#define __P2_H

#include <stddef.h>
#include <stdint.h>

#include "libc.h"
extern void (*entry_P2)();
extern uint32_t tos_P2;
void P2();

#endif
