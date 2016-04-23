#include "libc.h"

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

// Write to channel
void writeC(int chanid, int cstick, int who) {
	int r;
	asm volatile( "mov r0, %1 \n"
	              "mov r1, %2 \n"
	              "svc #9     \n"
	              "mov %0, r0 \n"
	              : "=r" (r)
	              : "r" (chanid), "r" (cstick), "r" (who)
	              : "r0", "r1");
}

// Read form channel
int readC(int chanid, int who) {
	int r;
	asm volatile( "mov r0, %1 \n"
	              "svc #10     \n"
	              "mov %0, r0 \n"
	              : "=r" (r)
	              : "r" (chanid), "r" (who)
	              : "r0");
	//return r;
}

// Run timer
void runT() {
	int r;
	asm volatile( "svc #11     \n"
	              "mov %0, r0 \n"
	              : "=r" (r));
}

// Get number of blocks on disk
int blockNum() {
	int r;
	asm volatile( "svc #12     \n"
	              "mov %0, r0 \n"
	              : "=r" (r));
	return r;
}

// Get legth of blocks on disk
int blockLen() {
	int r;
	asm volatile( "svc #13     \n"
	              "mov %0, r0 \n"
	              : "=r" (r));
	return r;
}

// Write to disk
void wrtDisk(int where, char* x) {
	int r;
	asm volatile( "mov r0, %1 \n"
	              "svc #14     \n"
	              "mov %0, r0 \n"
	              : "=r" (r)
	              : "r" (where), "r" (x)
	              : "r0");

}

// Read from disk
void rdDisk(int where, char* text) {
	int r;

	asm volatile( "mov r0, %1 \n"
	              "svc #15    \n"
	              "mov %0, r0 \n"
	              : "=r" (r)
	              : "r" (where), "r" (text)
	              : "r0");
	//return r;
}

// Create file to disk
void createfile(char* name, char* text, int len) {
	int r;

	asm volatile( "mov r0, %1 \n"
	              "svc #16    \n"
	              "mov %0, r0 \n"
	              : "=r" (r)
	              : "r" (name), "r" (text), "r" (len)
	              : "r0");
	//return r;
}

// Open file from disk
int openfile(int i, char* x) {
	int r;
	asm volatile( "mov r0, %1 \n"
	              "svc #17     \n"
	              "mov %0, r0 \n"
	              : "=r" (r)
	              : "r" (i), "r" (x)
	              : "r0");
	return r;
}

// Close file from disk
void closefile(char* x) {
	int r;
	asm volatile( "mov r0, %1 \n"
	              "svc #18     \n"
	              "mov %0, r0 \n"
	              : "=r" (r)
	              : "r" (x)
	              : "r0");
}

// Print integer
void printInt(int i) {
	if ( i >= 10) {
		printInt(i / 10);
	}
	char digit = 0x30 + i % 10;
	write(0, &digit, 1);
	return;
}

//Print string - Implemented to make it simpler to print
void printS(char* text) {
	int i = 0;
	while (*text != '\0' ) {
		write(0, text, 1);

		text++;
	}
	return;

}

// Get string legth
int slen(char* text) {
	int i = 0;
	while (text[i] != '\0' ) {
		//write(0, (char*) text[i], 1);
		i++;
	}
	return i;
}

// Copy string
char* scopy(char* from, char* to) {
	int i = 0;
	while (from[i] != '\0' ) {
		to[i] = from[i];
		i++;
	}
	return to;
}
char* emptyCharArray(char* empty) {
	for ( uint32_t i = 0; i < 16 ; i += 1 ) {
		empty[i] = '\0';
	}
	return empty;
}

// Compare two strings
int compare(char* one, char* two) {
	//int c = 1;
	//int l1 = slen(one);
	for ( uint32_t i = 0; i < 4 ; i += 1 ) {
		/*		printS(&one[i]);
				printS(&two[i]);*/
		if (one[i] != two[i]) {
			//printS("character mismatch");
			return 0;
		}
	}
	return 1;
}


