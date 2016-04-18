// NOT USEDED RIGHT NOW

#include "phil.h"

typedef struct {
	int id;			// Philosopher ID
	int pid;		// Process ID
	int lstick ;	// Left stick ID 		// Ask for it from process i-1
	int rstick ;	// Right stick ID 		// Ask for it from process i+1

	// These two: I want to use them but for now it's too much to handle
	// int lc ;		// Left clean -> -1 if dirty 0 if not there 1 if clean
	// int rc ;		// Right clean -> -1 if dirty 0 if not there 1 if clean
	int cl ;		// channel left
	int cr ;		// channel right
	int hungry ;	// Is he houngry

} philosopher;
int done = 0;
philosopher philo;

void setUp(int ppID, int pid) {
	int cid;
	cid = get_id();
	philo.id = pid;
	philo.pid = cid;
	philo.hungry = 1;
	// philo.lc = -1;
	// philo.rc = -1;
	if (pid == 0) {
		philo.lstick = 4;
		philo.rstick = pid;
		// philo.lnb = 4;
		// philo.rnb = 1;
	} else if (pid == 4) {
		philo.lstick = 0;
		philo.rstick = pid;
		// philo.lnb = 3;
		// philo.rnb = 0;
	} else {
		philo.lstick = pid - 1;
		philo.rstick = pid;
		// philo.lnb = pid - 1;
		// philo.rnb = pid + 1;
	}
	/*			printS("It is child ");
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
				printS("\n");*/
}

void phil(int ppID, int pid) {
	if (done < 5 ) {
		setUp(ppID, pid);
		execute(ppID);
		done++;
	} else {
		int thisP = get_id();
		// printInt(thisP);
		if (thisP == 2 ) {
			printS("\n Writing to channel 0 \n");
			writeC(0, 9);
			execute(3);
			// printInt(p[1]);
			int thisP = get_id();
			// printInt(thisP);
		} if (thisP == 3 ) {
			printS("\n I want to read : ");
			int rd = readC(0);
			printS("I read from channel 0 : ");
			printInt(rd);
			printS("\n ");
		}
		execute(ppID);
	}
}