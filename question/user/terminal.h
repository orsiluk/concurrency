#ifndef __TERMINAL_H
#define __TERMINAL_H

#include <stddef.h>
#include <stdint.h>

#include "libc.h"
#include "P0.h"
#include "P1.h"
#include "P2.h"
#include "philosophers.h"

extern void (*entry_terminal)();
extern uint32_t tos_terminal;

#endif