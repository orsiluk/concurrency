#include "philosophers.h"

/*
The attempt is to implement Chandy/Misra solution for Dining Philosophers problem
*/

typedef struct {
	int id;			// Philosopher ID
	int pid;		// Process ID
	int lstick ;	// Left stick ID 		// Ask for it from process i-1
	int rstick ;	// Right stick ID 		// Ask for it from process i+1

	// These two: I want to use them but for now it's too much to handle
	// int lc ;		// Left clean -> -1 if dirty 0 if not there 1 if clean
	// int rc ;		// Right clean -> -1 if dirty 0 if not there 1 if clean
	int lnb ;		// Left sid neighbour
	int rnb ;		// Right side neighbour
	int hungry ;	// Is he houngry

} philosopher;

philosopher philo;
int p[5]; // This array contains the pid of the ith philosopher -> Now I know which to exeute if needed
int chan_circ[5]; // Channel circle -> chan_circ[0] for 0-1 , chan_circ[1] for 1-2 ... chan_circ[4] for 4-0
int ppID = 1;  // parent process ID
int setup = 0; // I turn it int 1 when setup is over

// If I write a function in kernel.c get_id with which I can found out the ID of the current process
// I fork 5 times and I create only one philosopher in each fork, and have the parent process handle the table and procedures
/*
I don't know how to imagine it
If I have a list of philosophers and have all the info inside than I don't even need channels so there is no point

What if I creat a new file, called individual and call it as I call P0/P1/.. and this file would be the "shell" for it
where they return and trough the channels the tell this file if hey ahve a for or not, if they put down the fork, if
they are hugry ...
This might work.
*/
// Have to find a way to switch between processes (ik, yield, but I want to spacify where to go, need to write another yield)
void setupTable() {
	// ppID = get_id();
	for (int i = 0; i < 5; i++) {

		if (setup == 0) {
			int f = fork(); // fork returns 0 the child and childID to the parent
			int cid;

			if (f == 0) { // If it is a child
				//setup = 1;
				cid = get_id();
				philo.id = i;
				philo.pid = cid;
				philo.hungry = 1;
				// philo.lc = -1;
				// philo.rc = -1;
				if (i == 0) {
					philo.lstick = 4;
					philo.rstick = i;
					philo.lnb = 4;
					philo.rnb = 1;
				} else if (i == 4) {
					philo.lstick = 0;
					philo.rstick = i;
					philo.lnb = 3;
					philo.rnb = 0;
				} else {
					philo.lstick = i - 1;
					philo.rstick = i;
					philo.lnb = i - 1;
					philo.rnb = i + 1;
				}

				printS("It is child ");
				printInt(cid);
				printS("\n");
				printS("Info id pid hungry lc rc lstick rstick lnb rnb");
				printS("\n");
				printInt(philo.id);
				printS("  ");
				printInt(philo.pid);
				printS("  ");
				printInt(philo.hungry);
				printS("  ");
				printInt(philo.lstick);
				printS("  ");
				printInt(philo.rstick);
				printS("  ");
				printInt(philo.lnb);
				printS("  ");
				printInt(philo.rnb);
				printS("\n");
				printS("\n");

			} else {
				// printS("P[i] = ");
				p[i] = f; // If in parent, save the id of child
				// printInt(p[i]);
			}
			execute(ppID); // After setting up one philosopher in the fork return to parent process
		}
	}
	if (setup == 0) {
		printS("For loop is over -- Table setup over");
		printS("\n");
	}
	// With this command I can go back to process 1 but it doesn't know that I already forked ad it keeps creating forks how to fix dis? :D
}

void setupChan() {

	for (int i = 0; i < 5; i++) {
		if (i < 4) chan_circ[i] = create_c(p[i], p[i + 1]);
		else chan_circ[i] = create_c(p[i], p[0]);
	}
}

/* What do I want to write to a channel?
	channel -- Have to spcify trough which channel I send the stuff
	stick :
	0. asking for stick
	1. giving stick
*/

/* What do I want to read from a channel?
	channel -- Have to spcify from which channel I read from
	stick :
	1. getting stick
*/

void philosophers() {
	// Set up the table with philosophers and sticks
	// Every fork has one philosopher in it and they don't know anything about the others

	// BUUUG : get_id() doesn't return the right thing and fucks up everything

	/*
		It is not working because of one simple reason: When you fork, you fork a process ( FUNCTION!! ) not a file
		So even dough I call execute in philosophers, it will try to execute wherever it was left off when I left it the last time
	*/



	printS("Start philosophers! \n");
	int current = get_id();
	// printS("current=");
	// printInt(current);
	printS("\n");
	if (current == 1 && setup == 0) {
		setupTable();
		setupChan();
		printS("Channel setup done. \n");
		setup = 1;
	}
	if (setup == 1) {
		printS("Want to execute p[0]");
		execute(p[0]);
		// printInt(p[0]);
		int thisP = get_id();
		printInt(thisP);
		if (thisP == p[0] ) {
			printS("\n Writing to channel 0 \n");
			writeC(chan_circ[0], 9);
			execute(p[1]);
			// printInt(p[1]);
			int thisP = get_id();
			// printInt(thisP);
		}
		if (thisP == p[1] ) {
			printS("\n I want to read : ");
			int rd = readC(chan_circ[0]);
			printS("I read from channel 0 : ");
			printInt(rd);
			printS("\n ");
		}
	}
	execute(0);
	//system_exit();
}