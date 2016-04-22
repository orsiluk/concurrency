#include "philosophers.h"

/*
The attempt is to implement Chandy/Misra solution for Dining Philosophers problem
*/

/*
When a Philo needs a fork, he asks his neighbor for his fork. When his neighbor gets the request:

~ if his fork is clean, he keeps it.
~ if his fork is dirty, he cleans it and sends it over.
When a Philo has eaten, all his forks become dirty.

*/

// go back to clean and dirty forks
typedef struct {
	int id;			// Philosopher ID
	int pid;		// Process ID
	int lstick ;	// Left stick ID 		// Ask for it from process i-1
	int rstick ;	// Right stick ID 		// Ask for it from process i+1

	// These two: I want to use them but for now it's too much to handle
	int lc ;		// Left clean -> -1 if dirty 0 if not there 1 if clean
	int rc ;		// Right clean -> -1 if dirty 0 if not there 1 if clean
	int lnb ;		// Left sid neighbour
	int rnb ;		// Right side neighbour
	int hungry ;	// Is he houngry

} philosopher;

philosopher philo[5];
int p[5]; // This array contains the pid of the ith philosopher -> Now I know which to exeute if needed
int chan_circ[5]; // Channel circle -> chan_circ[0] for 0-1 , chan_circ[1] for 1-2 ... chan_circ[4] for 4-0
//int stick[5]; //-1 if dirty 0 if not there 1 if clean

int ppID = 1;  // parent process ID
int setup = 0; // I turn it int 1 when setup is over
int c = 0;
//int id = 0;
int s = 0;

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
void setupTable(int f, int i) {
	// ppID = get_id();

	int cid;

//For every Philosopher, give the fork to the guy with the smaller id

	if (f == 0) { // If it is a child
		//setup = 1;
		cid = get_id();
		philo[i].id = i;
		philo[i].pid = cid;
		philo[i].hungry = 1;
		// philo[i].lc = -1;
		// philo[i].rc = -1;
		if (i == 0) {
			philo[i].lstick = 1; // Has stick form left but dirty
			philo[i].rstick = 1; // Has stick form right but dirty
			philo[i].lnb = 4;
			philo[i].rnb = 1;
		} else if (i == 4) {
			philo[i].lstick = 0; // Has no stick
			philo[i].rstick = 0; // Has no stick
			philo[i].lnb = 3;
			philo[i].rnb = 0;
		} else {
			philo[i].lstick = 0;
			philo[i].rstick = 1;
			philo[i].lnb = i - 1;
			philo[i].rnb = i + 1;
		}

		/*		printS("It is child ");
				printInt(cid);
				printS("\n");
				printS("Info id pid hungry lc rc lstick rstick lnb rnb");
				printS("\n");
				printInt(philo[i].id);
				printS("  ");
				printInt(philo[i].pid);
				printS("  ");
				printInt(philo[i].hungry);
				printS("  ");
				printInt(philo[i].lstick);
				printS("  ");
				printInt(philo[i].rstick);
				printS("  ");
				printInt(philo[i].lnb);
				printS("  ");
				printInt(philo[i].rnb);
				printS("\n");
		*/		//printS("\n");

	} else {
		// printS("P[i] = ");
		p[i] = f; // If in parent, save the id of child
		// printInt(p[i]);
	}
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

/*
I don't ever have to go in the forks again
*/
void think(int th) {
	printInt(th);
	printS("th philosopher is thinking \n");
	//runT();
	// int t = 0;
	// while (t < 300000000) {
	// 	t++;
	// }
	printInt(th);
	printS(" Done thinking \n");
}

void sticks(int me) {
	//printS("p1\n");
	/*if (philo[id].lstick == 1 && philo[id].rstick == 1) {
		//printS("In sticks\n");
		printS("Philosopher "); printInt(id); printS(" is eating\n");
		runT();
		printS("Done eating \n");
		writeC(id, 1); //id == with chanelid to right neighbour
		philo[id].rstick = 0;
		writeC(philo[id].lnb, 1); //lnb == with chanelid to left neighbour
		philo[id].lstick = 0;

		philo[id].hungry = 0;
	}

	else if (philo[id].rstick == 0) {
		//printS("p2\n");
		philo[id].rstick = readC(id);
	}
	else if (philo[id].lstick == 0) {
		//printS("p3\n");
		philo[id].lstick = readC(philo[id].lnb);
	}
	return;*/
	int left = philo[me].lstick;
	int right = philo[me].rstick;
	//int mess = -1;

	printInt(me);
	printS (" Philo lstick: ");
	printInt(philo[me].lstick);
	printS("\n");
	printInt(me);
	printS (" Philo rstick: ");
	printInt(philo[me].rstick);
	printS("\n");
	while (left == 0) {
		if (me == 0) {
			left = readC(philo[me].lnb, 1);
		} else {

			left = readC(philo[me].lnb, 0);
		}
		philo[me].lstick = left;
	}
	while (right == 0) {
		if (me == 0) {
			right = readC(me, 0);
		} else {
			right = readC(me, 1);
		}
		philo[me].rstick = right;
	}

	printS("Philosopher "); printInt(me); printS(" is eating\n");
	int t = 0;
	/*	while (t < 300000000) {
			t++;
		}*/
	//runT();
	printS("Done eating "); printInt(me); printS("\n");

	philo[me].hungry = 0;

	philo[me].rstick = 0;
	philo[me].lstick = 0;
	writeC(me, 1, 0); //id == with chanelid to right neighbour 3rd ar. 1 is left 0 is right
	writeC(philo[me].lnb, 1, 1); //lnb == with chanelid to left neighbour

	printS("Stick sent");
}
/*void sticks(int id) {
	//printS("p1\n");
	if (philo[id].lstick == 1 && philo[id].rstick == 1) {
		//printS("In sticks\n");
		printS("Philosopher "); printInt(id); printS(" is eating\n");
		runT();
		printS("Done eating \n");
		philo[id].rstick = -1;
		philo[id].lstick = -1;
		//writeC(id, 1); //id == with chanelid to right neighbour
		//philo[id].rstick = 0;
		//writeC(philo[id].lnb, 1); //lnb == with chanelid to left neighbour
		//philo[id].lstick = 0;

		philo[id].hungry = 0;
	} else if (philo[id].rstick == -1) {
		writeC(id, 1); //id == with chanelid to right neighbour
		philo[id].rstick = 0;
	}
	else if (philo[id].lstick == -1) {
		writeC(philo[id].lnb, 1); //lnb == with chanelid to left neighbour
		philo[id].lstick = 0;
	}
	else if (philo[id].rstick == 0) {
		//printS("p2\n");
		philo[id].rstick = readC(id);
	}
	else if (philo[id].lstick == 0) {
		//printS("p3\n");
		philo[id].lstick = readC(philo[id].lnb);
	}
	return;
}*/


void start() {
	int i = 0;
//	while (i < 50) {
	// if (id < 5) {

	int philoid = get_id() - 2;

	/*	printS("Info id pid hungry lc rc lstick rstick lnb rnb");
		printS("\n");
		printInt(philo[philoid].id);
		printS("  ");
		printInt(philo[philoid].pid);
		printS("  ");
		printInt(philo[philoid].hungry);
		printS("  ");
		printInt(philo[philoid].lstick);
		printS("  ");
		printInt(philo[philoid].rstick);
		printS("  ");
		printInt(philo[philoid].lnb);
		printS("  ");
		printInt(philo[philoid].rnb);
		printS("\n");

		printInt(philoid);
	*/
	if (philo[philoid].hungry == 0) {
		think(philoid);
		philo[philoid].hungry = 1;
	}
	else {

		printS("Philosopher "); printInt(philoid); printS(" is hungry\n");

		sticks(philoid);


		//	}
		i++;
		//id++;
		//} else {

		//	id = 0;
		//}
	}
//}
}

void philosophers() {
	// Set up the table with philosophers and sticks
	// Every fork has one philosopher in it and they don't know anything about the others
	/*
		It is not working because of one simple reason: When you fork, you fork a process ( FUNCTION!! ) not a file
		So even dough I call execute in philosophers, it will try to execute wherever it was left off when I left it the last time
	*/



	printS("Start philosophers! \n");
//	int current = get_id();
	// printS("current=");
	// printInt(current);

	if (setup == 0) {
		for (int i = 0; i < 5; i++) {

			if (setup == 0) {
				int f = fork(); // fork returns 0 the child and childID to the parent
				setupTable(f, i);
				execute(ppID);
			}
		}
		setup = 1;
	}
	if (c == 0) {
		printS("For loop is over -- Table setup over\n");
		setupChan();
		printS("Channel setup done. \n");
		c = 1;

		execute(p[0]);

	}
	start();


	printS("philosophers over\n");
	/*if (setup == 1) {
		if (id == 0) {
			id++;
			printS("Want to execute p[0]\n");
			execute(p[id - 1]);
		}
		// printInt(p[0]);
		int thisP = get_id();
		printInt(thisP);
		if (thisP == p[0] ) {
			printS("Writing to channel 0 \n");
			writeC(chan_circ[0], 9);
			execute(p[1]);
			// printInt(p[1]);
			int thisP = get_id();
			// printInt(thisP);
		}
		if (thisP == p[1] ) {
			printS("\n I want to read : \n");
			int rd = readC(chan_circ[0]);
			printS("I read from channel 0 : ");
			printInt(rd);
			printS("\n ");
		}
	}
	execute(0);*/
	system_exit();
}
void (*entry_philosphers)() = &philosophers;