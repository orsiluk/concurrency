#include "terminal.h"
#include <string.h>
/*
For now, the aim of the terminal is to read in a massage which has to  be a
command, for which the terminal would react by forking and starting to
execute the chosen process. When finished it will return to the terminal.
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
		// printS(x);
		fork();
		P0();
		system_exit();
	}

	else if (findProcess(x, "run P1") == 1)	{	//printS(x);
		fork();
		P1();
		system_exit();
	}

	else if (findProcess(x, "run P2") == 1)	{
		// printS(x);
		fork();
		P2();
		system_exit();
	}
	else printS("This is not a valid command!\n");


}

void terminal() {
	char x[50];

	while (1) {
		//printInt(i);
		// Have to do somting with print sometimes it starts from the 0th position and doensn't display
		printS("shelly$ ");
		read( x );
		//printS("\n");
		chooseAction(x);
	}
}
void (*entry_terminal)() = &terminal;