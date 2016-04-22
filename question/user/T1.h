#ifndef __T1_H
#define __T1_H

#include <stddef.h>
#include <stdint.h>


#include "libc.h"
#include "P0.h"
#include "P1.h"
#include "P2.h"

extern void (*entry_T1)();
extern uint32_t tos_T1;
void priority();

#endif