// NOT USEDED RIGHT NOW

#ifndef __phil_H
#define __phil_H

#include <stddef.h>
#include <stdint.h>


#include "libc.h"

extern void (*entry_phil)();
extern uint32_t tos_phil;
void phil();

#endif
