#include "philosophers.h"

/*
The attempt is to implement Chandy/Misra solution for Dining Philosophers problem
*/

typedef struct {
	int id;			// Philosopher ID
	int pid;		// Process ID
	int lstick ;		// Left stick ID
	int rstick ;		// Right stick ID
	int lc ;			// Left clean -> -1 if dirty 0 if not there 1 if clean
	int rc ;			// Right clean -> -1 if dirty 0 if not there 1 if clean
	int lnb ;		// Left sid neighbour
	int rnb ;		// Right side neighbour
	int hungry ;		// Is he houngry

} philosopher;

philosopher philo;
int p[5]; // This array contains the pid of the ith philosopher -> Now I know which to exeute if needed

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
	int ppID = get_id(); // parent process ID

	for (int i = 0; i < 5; i++) {
		int f = fork(); // fork returns 0 the child and childID to the parent
		int cid;

		if (f == 0) { // If it is a child
			cid = get_id();
			philo.id = i;
			philo.pid = cid;
			philo.hungry = 1;
			philo.lc = -1;
			philo.rc = -1;
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
			printInt(philo.lc);
			printS("  ");
			printInt(philo.rc);
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
			p[i] = f;
			printS("In parent, p[i]=");
			printInt(p[i]);
			printS("\n");
		}
		execute(ppID); // After setting up one philosopher in the fork return to parent process
	}
	printS("For loop is over -- Table setup over");
	printS("\n");
	// With this command I can go back to process 1 but it doesn't know that I already forked ad it keeps creating forks how to fix dis? :D
}

void philosophers() {
	// Set up the table with philosophers and sticks
	// Every fork has one philosopher in it and they don't know anything about the others
	setupTable();

	system_exit();
}

