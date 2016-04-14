#include "philosophers.h"
/*
The attempt is to implement Chandy/Misra solution for Dining Philosophers problem
*/

typedef struct {
	int id = 0;			// Philosopher ID
	int lstick = 0;		// Left stick ID
	int rstick = 0;		// Right stick ID
	int lc = 0;			// Left clean -> -1 if dirty 0 if not there 1 if clean
	int rc = 0;			// Right clean -> -1 if dirty 0 if not there 1 if clean
	// int lnb = 0;		//
	// int rnb = 0;		//
	int hungry = 0;		//

} philosopher;


philosopher ph[5];

setupPh() {
	for (i = 0, i < 5, i++) {
		ph[i].id = i;
		ph[i].hungry = 1;
		if (i == 0) {
			ph[i].lstick = 4;
			ph[i].rstick = i;
		} else if (i == 4) {
			ph[i].lstick = 0;
			ph[i].rstick = i;
		} else {
			ph[i].lstick = i - 1;
			ph[i].rstick = i;
		}
	}
}