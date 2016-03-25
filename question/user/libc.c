#include "libc.h"
#include <string.h>

void yield() {
	asm volatile( "svc #0     \n"  );
}

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


/*int read(void *text) {
	int r;
	asm volatile( "mov r0, %1 \n"
	              "svc #3     \n"
	              "mov %0, r0 \n"
	              : "=r" (r)
	              : "r" (text)
	              : "r0");
	return r;
}*/

int fork() {
	int r;
	asm volatile(
	    "svc #2     \n"
	    "mov %0, r0 \n"
	    : "=r" (r));
	//printInt(r);
	return r;

}

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


int system_exit() {
	int r;
	asm volatile("svc #4     \n"
	             "mov %0, r0 \n"
	             : "=r" (r));
	return r;

}

/*void exec() {
	//replace current process image with new process image,
	//no return, since call point no longer exists (!)
	asm volatile("svc #5     \n");

}*/
void kill(int p) {
	int r;

	asm volatile( "svc #5     \n"
	              "mov r0, %0 \n"
	              : "=r" (r)
	              : "p" (p)
	              : "r0");
}

void printInt(int i) {
	if ( i > 10) {
		printInt(i / 10);
	}
	char digit = 0x30 + i % 10;
	write(0, &digit, 1);
}

void printS(char* text) {
	int n = strlen(text);
	write(0, text, n);
}

/*void readS(char* text) {
	int n = strlen(text);
	read(0, text, n);
}*/
//write printf for strings :)