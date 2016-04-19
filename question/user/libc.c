#include "libc.h"
#include <string.h>

// Find the next process to be executed
void yield() {
	asm volatile( "svc #0     \n"  );
}

// Write to screen
int write( int fd, void* x, size_t n ) {
	int r;

	asm volatile( "mov r0, %1 \n"
	              "mov r1, %2 \n"
	              "mov r2, %3 \n"
	              "svc #1     \n"
	              "mov %0, r0 \n"
	              : "=r" (r)
	              : "r" (fd), "r" (x), "r" (n)
	              : "r0", "r1", "r2" );

	return r;
}

//Fork current process
int fork() {
	int r;
	asm volatile(
	    "svc #2     \n"
	    "mov %0, r0 \n"
	    : "=r" (r));
	return r;
}
// Read from keyboard
int read(void* x ) {
	int r;

	asm volatile( "mov r0, %1 \n"
	              "svc #3     \n"
	              "mov %0, r0 \n"
	              : "=r" (r)
	              : "r" (x)
	              : "r0");
	return r;
}

// exit currently running process
int system_exit() {
	int r;
	asm volatile("svc #4     \n"
	             "mov %0, r0 \n"
	             : "=r" (r));
	return r;

}

// kill process p
void kill(int p) {
	int r;

	asm volatile( "svc #5     \n"
	              "mov r0, %0 \n"
	              : "=r" (r)
	              : "p" (p)
	              : "r0");
}

// Execute process pid
void execute(int pid) {
	//replace current process image with new process image,
	int r;
	asm volatile( "mov r0, %1 \n"
	              "svc #6     \n"
	              "mov %0, r0 \n"
	              : "=r" (r)
	              : "r" (pid)
	              : "r0");
	//return r;
}

// Create a channel
int create_c( int c_start, int c_end) {
	int r;
	asm volatile( "mov r0, %1 \n"
	              "mov r1, %2 \n"
	              "svc #7     \n"
	              "mov %0, r0 \n"
	              : "=r" (r)
	              : "r" (c_start), "r" (c_end)
	              : "r0", "r1");
	return r;
}

// Get the id of current process
int get_id() {
	int r;
	asm volatile( "svc #8     \n"
	              "mov %0, r0 \n"
	              : "=r" (r));
	return r;
}

void writeC(int chanid, int cstick) {
	int r;
	asm volatile( "mov r0, %1 \n"
	              "mov r1, %2 \n"
	              "svc #9     \n"
	              "mov %0, r0 \n"
	              : "=r" (r)
	              : "r" (chanid), "r" (cstick)
	              : "r0", "r1");
}

int readC(int chanid) {
	int r;
	asm volatile( "mov r0, %1 \n"
	              "svc #10     \n"
	              "mov %0, r0 \n"
	              : "=r" (r)
	              : "r" (chanid)
	              : "r0");
	//return r;
}
/*int createP() {
		int r;
		asm volatile( "svc #11     \n"
		              "mov %0, r0 \n"
		              : "=r" (r));
		return r;
	}*/

void runT() {
	int r;
	asm volatile( "svc #11     \n"
	              "mov %0, r0 \n"
	              : "=r" (r));
}
// Print integer
void printInt(int i) {
	if ( i > 10) {
		printInt(i / 10);
	}
	char digit = 0x30 + i % 10;
	write(0, &digit, 1);
}

//Print string - Implemented to make it simpler to print


void printS(char* text) {
	//int n = strlen(text);
	int i = 0;
	while (*text != '\0' ) {
		write(0, text, 1);

		text++;
	}
	return;

}
