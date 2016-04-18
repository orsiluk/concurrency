#ifndef __talk_H
#define __talk_H

#include <stddef.h>
#include <stdint.h>


#include "libc.h"
#include "talk.h"



extern void (*entry_talk)();
extern uint32_t tos_talk;
void talk1();
void talk2();

#endif