#include "terminal.h"
#include <string.h>
/*
For now, the aim of the terminal is to read in a massage which has to  be a
command, for which the terminal would react by forking and starting to
execute the chosen process. When finished it will return to the terminal.
*/

/*
Questions:

1. Do we want to exit the process or just go bact to terminal where we left off?
2. Is it ok if the terminal is a program?
*/
int findProcess(char* x, char* command ) {
	// printS("in findprocess \n");
	int c = 1;
	for ( uint32_t i = 0; i < strlen(command) ; i += 1 ) {
		if (x[i] != command[i]) c = 0;
	}
	return c;
}

void chooseAction(char* x) {
	// printS(" in chooseaction \n");
	if (findProcess(x, "run P0") == 1) {
		//fork() returns a zero to the newly created child process.
		//fork() returns a positive value, the process ID of the child process, to the parent.
		int f = fork();
		printInt(f);
		if (f == 0) {
			P0();
			//system_exit();
		}
	}

	else if (findProcess(x, "run P1") == 1)	{	//printS(x);
		int f = fork();
		// printS(x);
		if (f == 0) {
			P1();
			// system_exit();
		}
	}

	else if (findProcess(x, "run P2") == 1)	{
		int f = fork();
		// printS(x);
		if (f == 0) {
			P2();
			// system_exit();
		}
	}
	else {
		printS("This is not a valid command!\n");
		//system_exit();
	}


}

void terminal() {
	char x[50];

	while (1) {
		//printInt(i);
		printS("shelly$ ");
		read( x );
		//printS("\n");
		chooseAction(x);
	}
}
void (*entry_terminal)() = &terminal;