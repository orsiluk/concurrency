#ifndef __LIBC_H
#define __LIBC_H

#include <stddef.h>
#include <stdint.h>
#include "disk.h"

// Get string legth
int slen( char* text);

// Copy string
char* scopy(char* from, char* to);

// Write to screen
int write( int fd, void* x, size_t n );

// Read from keyboard
int read( void* x );

//Print string
void printS( char* text);

// Print integer
void printInt(int i);

// Compare two strings
int compare(char* one, char* two);

char* emptyCharArray();


// Find the next process to be executed
void yield();

//Fork current process
int fork();

// exit currently running process
int system_exit();

// Execute process pid
void execute(int pid);

// kill process p
void kill(int p);

// Get the id of current process
int get_id();

// Create a channel
int create_c( int c_start, int c_end);

// Write to channel
void writeC(int chanid, int cstick, int who);

// Read form channel
int readC(int chanid, int who);

// Run timer
void runT();

// Get number of blocks on disk
int blockNum();

// Get legth of blocks on disk
int blockLen();

// Write to disk
void wrtDisk(int where, char* text);

// Read from disk
void rdDisk(int where, char* text);

// Create file to disk
void createfile(char* name, char* text, int len);

// Open file from disk
int openfile(int i, char* file);

// Close file from disk
void closefile(char* x);

#endif
