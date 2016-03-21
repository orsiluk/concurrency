#include "terminal.h"
#include <string.h>
/*
For now, the aim of the terminal is to read in a massage which has to  be a
command, for which the terminal would react by forking and starting to
execute the chosen process. When finished it will return to the terminal.
*/
int findProcess(char* x, char* command ) {
	int c = 1;
	for ( uint32_t i = 0; i < strlen(x) ; i += 1 ) {
		if (x[i] != command[i]) c = 0;
	}
	return c;
}

void chooseAction(char* x) {
	if (findProcess(x, "run P0") == 1) printS(x);
	else if (findProcess(x, "run P1") == 1) printS(x);
	else if (findProcess(x, "run P2") == 1) printS(x);
	else printS("This is not a valid command! ");

}

void terminal() {
	char x[50];

	while (1) {
		//printInt(i);
		printS("shelly$ ");
		read( x );
		printS("\n");
		chooseAction(x);
	}
}
void (*entry_terminal)() = &terminal;