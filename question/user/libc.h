#ifndef __LIBC_H
#define __LIBC_H

#include <stddef.h>
#include <stdint.h>

int write( int fd, void* x, size_t n );

int read( int fd, void* x, size_t n );

//int read( void* text );

void printInt(int i);

int fork();

void system_exit();

void printS( char* text);

//void readS(char* text);

void yield();

#endif
