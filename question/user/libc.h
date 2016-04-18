#ifndef __LIBC_H
#define __LIBC_H

#include <stddef.h>
#include <stdint.h>

int write( int fd, void* x, size_t n );

int read( void* x );

//int read( void* text );

void printInt(int i);

int fork();

int system_exit();

void printS( char* text);

void execute(int pid);

void kill(int p);

int get_id();

int create_c( int c_start, int c_end);
//void readS(char* text);

void writeC(int chanid, int cstick);

int readC(int chanid);

void yield();

// int createP();

#endif
