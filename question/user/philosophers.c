#include "philosophers.h"


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

	int lnb ;		// Left sid neighbour
	int rnb ;		// Right side neighbour
	int hungry ;	// Is he houngry

} philosopher;

philosopher philo[5];
int p[5]; // This array contains the pid of the ith philosopher -> Now I know which to exeute if needed
int chan_circ[5]; // Channel circle -> chan_circ[0] for 0-1 , chan_circ[1] for 1-2 ... chan_circ[4] for 4-0

int ppID = 1;  // parent process ID
int setup = 0; // I turn it int 1 when setup is over
int c = 0;
//int id = 0;
int s = 0;


void setupTable(int f, int i) {
	int cid;

//For every Philosopher, give the fork to the one with the smaller id

	if (f == 0) { // If it is a child
		cid = get_id();
		philo[i].id = i;
		philo[i].pid = cid;
		philo[i].hungry = 1;
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

	} else {
		p[i] = f; // If in parent, save the id of child
	}
}

void setupChan() {

	for (int i = 0; i < 5; i++) {
		if (i < 4) chan_circ[i] = create_c(p[i], p[i + 1]);
		else chan_circ[i] = create_c(p[i], p[0]);
	}
}

/*

The channel has a buffer with two posotions. When I want to send to the right neigbour I put the message on the 0th position,
when I want to talk to the left neigbor I  put the message on the 1st position. If I am reading from the channel I read from the right from
the 0th position and from the left from the 1st position.

*/

void think(int th) {
	printInt(th);
	printS("th philosopher is thinking \n");
	runT(); // Function which makes the philosopher wait.
	printInt(th);
	printS(" Done thinking \n");
}

void sticks(int me) {

	int left = philo[me].lstick;
	int right = philo[me].rstick;

	while (left == 0) {

		left = readC(philo[me].lnb, 1);
		philo[me].lstick = left;
	}
	while (right == 0) {

		right = readC(me, 0);
		philo[me].rstick = right;
	}

	printS("Philosopher "); printInt(me); printS(" is eating\n");
	runT();
	printS("Done eating "); printInt(me); printS("\n");

	philo[me].hungry = 0;
	philo[me].rstick = 0;
	philo[me].lstick = 0;
	writeC(me, 1, 1); //id == with chanelid to right neighbour 3rd ar. 1 is left 0 is right
	writeC(philo[me].lnb, 1, 0); //lnb == with chanelid to left neighbour
}

void start() {
	int i = 0;
	while (i < 50) {
		int philoid = get_id() - 2;
		if (philoid == -1) system_exit();

		if (philo[philoid].hungry == 0) {
			think(philoid);
			philo[philoid].hungry = 1;
		}
		else {
			printS("Philosopher "); printInt(philoid); printS(" is hungry\n");
			sticks(philoid);
			i++;
		}
	}
}

void philosophers() {
	// Set up the table with philosophers and sticks
	// Every fork has one philosopher in it and they don't know anything about the others, they only talk through channels
	printS("Start philosophers! \n");
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


	printS("Philosophers over\n");
	system_exit();
}
void (*entry_philosphers)() = &philosophers;