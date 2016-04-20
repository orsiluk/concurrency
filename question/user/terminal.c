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


int findProcess(char* x, char* command , int l) {
	// printS("in findprocess \n");
	int c = 1;
	for ( uint32_t i = 0; i < l ; i += 1 ) {
		if (x[i] != command[i]) c = 0;
	}
	return c;
}

void chooseAction(char* x, int l) {
	// printS(" in chooseaction \n");
	if (findProcess(x, "run P0", l) == 1) {
		//fork() returns a zero to the newly created child process.
		//fork() returns a positive value, the process ID of the child process, to the parent.
		int f = fork();
		//printInt(f);
		if (f == 0) {
			P0();
			//system_exit();
			printS("why here P00");
		} /*else {
			printS("why here P0");
			yield();
			printS("yielded");
			return;

		}*/
	}

	else if (findProcess(x, "run P1") == 1)	{	//printS(x);
		int f = fork();
		// printS(x);
		if (f == 0) {
			P1();
			//system_exit();
		} /*else {
			printS("why here P1");
			yield();

		}*/
	}

	else if (findProcess(x, "run P2") == 1)	{
		int f = fork();
		// printS(x);
		if (f == 0) {
			P2();
			// system_exit();
		} /*else {
			yield();

		}*/
	}
	else if (findProcess(x, "run philo") == 1)	{
		int f = fork();
		// printS(x);
		if (f == 0) {
			philosophers();
			//system_exit();
		}/* else {
			yield();

		}*/
	} else if (findProcess(x, "blocknr") == 1) {

		int i = blockNum();
		printS("Number of blocks on disk: ");
		printInt(i);
		printS("\n");
	}
	else if (findProcess(x, "blockln") == 1) {

		int i = blockLen();
		printS("Length of blocks on disk: ");
		printInt(i);
		printS("\n");
	}
	else if (findProcess(x, "wrtd") == 1) {
		char x[50];
		int len = read( x );
		wrtDisk(x, len);
	}

	else {
		printS("This is not a valid command!\n");
		//system_exit();
	}
}

void terminal() {
	char x[50];

	//int chan = create_c(1, 2);
	while (1) {
		//printInt(i);
		printS("shelly$ ");
		int l = read( x );
		//printS("\n");
		chooseAction(x, l);
	}
}
void (*entry_terminal)() = &terminal;