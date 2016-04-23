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
			//yield();
			//system_exit();
			// printS("why here P00");
		} /*else {
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
		}
		// else {
		// 	// printS("why here P1");
		// 	yield();

		// }
	}

	else if (findProcess(x, "run P2", l) == 1)	{
		int f = fork();
		// printS(x);
		if (f == 0) {
			P2();
			// system_exit();
		} else {
			yield();

		}
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
		printS("What would you like to write to disk? \n");
		char x[16];
		int len = read( x );
		printInt(len);
		wrtDisk(0, x);
	}
	else if (findProcess(x, "rddisk", l) == 1) {
		//int len = read( x );

		// So it seems like reading integers is not perfect ((Only works for strings))
		int len = 16;
		char x[len];
		rdDisk(0, x);
		printS("The forst 16 bites contain: ");
		write(0, x, 16);
		printS("\n");
	}
	else if (findProcess(x, "rdfile", l) == 1) {
		//int len = read( x );

		// So it seems like reading integers is not perfect ((Only works for strings))
		char x[16];
		printS("File name you want to read: ");
		char name[16];
		read(name);
		openfile(1, name);
		printS("\n");
	}
	else if (findProcess(x, "ofile", l) == 1) {
		printS("Open file and replace content. File name: \n");
		char name[16];
		read(name);
		int i = openfile(0, name);
		//int i = openfile(0, "dogs");

		if (i > 0) {
			printS("Found file, location is: ");
			printInt(i);
			printS("\n");
			printS("What do you wnat to replace the content with? \n");
			char t[16];
			int len;
			len = read(t);

			wrtDisk(i, t);
		} else printS("This file doesn't exist! \n");

	}
	else if (findProcess(x, "unlink", l) == 1) {
		printS("Which file do you want to close? \n");
		char name[4];
		read(name);

		closefile(name);
	}
	else if (findProcess(x, "createfiles", l) == 1) {
		//printS("In create file process");
		printS("I create 3 files dogs, cats and bird \n");
		char name[16];
		char t0[16];
		char t1[16];
		char t2[16];
		int len;

		scopy("dogs", name);
		scopy("fluffy", t0);
		createfile(name, t0, 16);

		scopy("cats", name);
		scopy("scratch", t1);
		createfile(name, t1, 16);

		scopy("bird", name);
		scopy("flie away", t2);
		createfile(name, t2, 16);

	}
	else if (findProcess(x, "inputfile", l) == 1) {
		char name[4];
		char t[16];
		printS("Read file name. Name must be 4 characters! \n");
		read(name);

		printS("Read file content. \n");
		read(t);
		createfile(name, t, 16);

	}
	else if (findProcess(x, "test", l) == 1) {
		chooseAction("blocknr", slen("blocknr"));
		chooseAction("blockln", slen("blockln"));
		chooseAction("wrtdisk", slen("wrtdisk"));
		chooseAction("createfiles", slen("createfiles"));
		chooseAction("rddisk", slen("rddisk"));
		chooseAction("rdfile", slen("rdfile"));
		chooseAction("ofile", slen("ofile"));
		chooseAction("unlink", slen("closefile"));
		chooseAction("inputfile", slen("inputfile"));

		printS("Filesystem test over.");
	}
	/*	else {
			printS("This is not a valid command!\n");
			//system_exit();
		}*/
}

void terminal() {
	char x[50];

	//int chan = create_c(1, 2);
	while (1) {
		//printInt(i);
		printS("shelly$ ");
		int l = read( x );
		/*		while (l < 1) {
					printS("shelly$ ");
					l = read( x );
				}*/
		//printS("\n");
		chooseAction(x, l);
	}
}
void (*entry_terminal)() = &terminal;