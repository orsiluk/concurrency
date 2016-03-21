#ifndef __TERMINAL_H
#define __TERMINAL_H

#include <stddef.h>
#include <stdint.h>

#include "libc.h"
extern void (*entry_terminal)();
extern uint32_t tos_terminal;

#endif