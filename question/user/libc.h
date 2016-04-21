#ifndef __LIBC_H
#define __LIBC_H

#include <stddef.h>
#include <stdint.h>
#include "disk.h"

int slen( char* text);

char* scopy(char* from, char* to);


int write( int fd, void* x, size_t n );

int read( void* x );

void printS( char* text);

void printInt(int i);

int compare(char* one, char* two);


void yield();

int fork();

int system_exit();

void execute(int pid);

void kill(int p);

int get_id();

int create_c( int c_start, int c_end);

void writeC(int chanid, int cstick);

int readC(int chanid);

void runT();


int blockNum();

int blockLen();

void wrtDisk(int where, char* text, int len);

void rdDisk(int where, char* text, int len);

void createfile(char* name, char* text, int len);

int openfile(int i, char* file);

void closefile(char* x);

#endif
