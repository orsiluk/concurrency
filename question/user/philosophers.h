#ifndef __philosophers_H
#define __philosophers_H

#include <stddef.h>
#include <stdint.h>


#include "libc.h"

extern void (*entry_philosophers)();
extern uint32_t tos_philosophers;
void philosophers();

#endif
