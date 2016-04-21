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
			// printS("why here P00");
		} /*else {
			printS("why here P0");
			yield();
			printS("yielded");
			return;

		}*/
	}

	else if (findProcess(x, "run P1", l) == 1)	{	//printS(x);
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

	else if (findProcess(x, "run P2", l) == 1)	{
		int f = fork();
		// printS(x);
		if (f == 0) {
			P2();
			// system_exit();
		} /*else {
			yield();

		}*/
	}
	else if (findProcess(x, "run philo", l) == 1)	{
		int f = fork();
		// printS(x);
		if (f == 0) {
			philosophers();
			//system_exit();
		}/* else {
			yield();

		}*/
	} else if (findProcess(x, "blocknr", l) == 1) {

		int i = blockNum();
		printS("Number of blocks on disk: ");
		printInt(i);
		printS("\n");
	}
	else if (findProcess(x, "blockln", l) == 1) {

		int i = blockLen();
		printS("Length of blocks on disk: ");
		printInt(i);
		printS("\n");
	}
	else if (findProcess(x, "wrtdisk", l) == 1) {
		char x[50];
		int len = read( x );
		printInt(len);
		wrtDisk(0, x, len);
	}
	else if (findProcess(x, "rddisk", l) == 1) {
		//int len = read( x );

		// So it seems like reading integers is not perfect ((Only works for strings))
		int len = 10;
		char x[len];
		rdDisk(0, x, len);
		printS("I've read: ");
		write(0, x, 10);
		//printS(x);
	}
	else if (findProcess(x, "rdfile", l) == 1) {
		//int len = read( x );

		// So it seems like reading integers is not perfect ((Only works for strings))
		char x[20];
		printS("I've read: ");
		int len = openfile(1, "cats");
		//write(0, x, len);
		printS("\n");
	}
	else if (findProcess(x, "ofile", l) == 1) {
		int i = openfile(0, "dogs");

		if (i > 0) {
			printS("Found file, location is: ");
			printInt(i);
		} else printS("This file doesn't exist!");
	}
	else if (findProcess(x, "closefile", l) == 1) {
		char name[4];
		scopy("dogs", name);
		closefile(name);
	}
	else if (findProcess(x, "cfile", l) == 1) {
		//printS("In create file process");
		char name[4];
		char t[19];
		int len;

		scopy("dogs", name);
		scopy("fluffy :D ", t);
		len = slen(t);
		createfile(name, t, len);
		scopy("cats", name);
		scopy("scratch", t);
		len = slen(t);
		createfile(name, t, len);
		scopy("bird", name);
		scopy("I don't like birds", t);
		len = slen(t);
		createfile(name, t, len);

	}
	else if (findProcess(x, "len", l) == 1) {
		int i = slen("aaaa\0");
		printInt(i);

		char x[10];
		int j = read(x);
		i = slen(x);

		printS("X len :" );
		printInt(i);
		printInt(j);
		printS("new line?");
		printS(x);
		printS("Am I in new line?");

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
		while (l < 1) {
			printS("shelly$ ");
			l = read( x );
		}
		//printS("\n");
		chooseAction(x, l);
	}
}
void (*entry_terminal)() = &terminal;