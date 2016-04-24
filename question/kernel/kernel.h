#ifndef __KERNEL_H
#define __KERNEL_H

#include <stddef.h>
#include <stdint.h>

#include   "GIC.h"
#include "PL011.h"
#include "SP804.h"

#include "interrupt.h"
#include "disk.h"
#include <string.h>

#include "P0.h"
#include "P1.h"
#include "P2.h"
#include "talk.h"
#include "philosophers.h"
#include "terminal.h"

/* The kernel source code is made simpler by three type definitions:
 *
 * - a type that captures each component of an execution context (i.e.,
 *   processor state) in a compatible order wrt. the low-level handler
     preservation and restoration prologue and epilogue,
 * - a type that captures a process identifier, which is basically an
 *   integer, and
 * - a type that captures a process PCB.
 */

typedef struct {
	uint32_t cpsr, pc, gpr[ 13 ], sp, lr;
} ctx_t;

typedef int pid_t;

typedef struct {
	int priority;
	int parent;
	pid_t pid;
	ctx_t ctx;
} pcb_t;

typedef struct {
	int buff[2];
	pid_t c_start;
	pid_t c_end;
} ipc_t;

#endif
