#include "kernel.h"

pcb_t pcb[ 6 ], *current = NULL;
/* Define a type pcb_t that captures a Process Control Block (PCB), instances of which form
entries in the process table: given the limited remit here, such entry simply includes a PID and execution
context. */

pc_t ipc[ 6 ];
/*
Define a list of pipes the same way as I dedined pcbs
*/

//uint32_t nr = 4; //contains the number of processes we currently have
uint32_t all =  6; // all represents the overall number of processes which we can allocate (can be changed if you allocate more space at pcb_t pcb[ 5 ] )
int nrprocess = 0;
//int nr = 4;
uint32_t stack = (uint32_t) &tos_terminal; //pointer to the top of the stack
timer();
ipcArray();
/*
	// This scheduler won't work now becuse the implementation changed, but with a few
	// changes it can be fixed

	void scheduler( ctx_t* ctx ) {
	// Round Robin scheduler

	//Change this so it would return to priority process if it was a child
	if (current -> pid != current -> priority) {
		memcpy( &pcb[ current -> pid ].ctx, ctx, sizeof( ctx_t ) );
		memcpy( ctx, &pcb[ current -> priority ].ctx, sizeof( ctx_t ) );
		current = &pcb[ current -> priority ];
	}
	else if ( current != &pcb[ nrprocess - 1 ] ) {// -1 because I started counting form 0
		memcpy( &pcb[ current -> pid].ctx, ctx, sizeof( ctx_t ) );
		memcpy( ctx, &pcb[ current -> pid + 1 ].ctx, sizeof( ctx_t ) );
		current = &pcb[ current -> pid + 1 ];
	} else {
		memcpy( &pcb[ nrprocess ].ctx, ctx, sizeof( ctx_t ) );
		memcpy( ctx, &pcb[ 0 ].ctx, sizeof( ctx_t ) );
		current = &pcb[ 0 ];
	}

}*/

void incPrority() {
	for (int i = 0; i < all; i++) {
		if (pcb[i].priority > 0) {
			pcb[i].priority -= 1;
		}
	}
}

int nextP() {
	uint32_t found = -1;
	uint32_t highest = 7;
	int i = 0;
	while (i < nrprocess) {
		if (i != current->pid) {
			if (pcb[i].priority < highest && pcb[i].priority != -1 ) {
				found = i;
				highest = pcb[i].priority;
			}
		}
		i++;
	}
	incPrority();

	if (found != 0) {
		pcb[found].priority++;
	}
	return found;
}

void scheduler(ctx_t* ctx) {
	// Priority based scheduler with hopefully aging implemented as well
	// priority will be changed to be priority and will work such as: priority process will have higher
	// prority than child, and ( Is this an auomated process or do I have a terminal from where I call processes?)

	// For now I will desing it such that there is a terminal and there will be an execute function which
	// will execute the next process in the priority queue, but this means that the processes are already
	// predefined before execution, but you are still able to add processes (figure a way to do this)

	// Question: Is it enough if it always retuns to the terminal or does it have to retun t te parent process? Like go back to P1 if we forked from P1?


	// The way it works right now is that Termnal is the first element in the Q and it has priority 0
	// so it will always be chosen when another process terminates. Also the process when it terminates
	// exits, which is not great, because eventually will exit a process which I need in the Q.
	// I am not sure what we are expected to do. :|
	// If it shouldn't exit when it's not child process I can put an if in exit, and only exit if the
	// process number is > than the predefined nr of processes. ( It will enevr exit predefined processes
	// not even when I want to!)
	int next = nextP();

	memcpy( &pcb[ current->pid ].ctx, ctx, sizeof( ctx_t ) );
	memcpy( ctx, &pcb[ next ].ctx, sizeof( ctx_t ) );
	current = &pcb[ next ];
}

void schedulerR(ctx_t* ctx) {
	// It is the round-robin scheduler, it can be changed so it would return to the parent process if needed
	if ( current != &pcb[ nrprocess - 1 ] ) {// -1 because I started counting form 0
		memcpy( &pcb[ current -> pid].ctx, ctx, sizeof( ctx_t ) );
		memcpy( ctx, &pcb[ current -> pid + 1 ].ctx, sizeof( ctx_t ) );
		current = &pcb[ current -> pid + 1 ];
	} else {
		memcpy( &pcb[ nrprocess ].ctx, ctx, sizeof( ctx_t ) );
		memcpy( ctx, &pcb[ 0 ].ctx, sizeof( ctx_t ) );
		current = &pcb[ 0 ];
	}
}

void killProcess(ctx_t* ctx , int p) {
	pcb[p].priority = -1;
	scheduler(ctx);
}



void timer() {

	TIMER0->Timer1Load     = 0x00100000; // select period = 2^20 ticks ~= 1 sec
	TIMER0->Timer1Ctrl     = 0x00000002; // select 32-bit   timer
	TIMER0->Timer1Ctrl    |= 0x00000040; // select periodic timer
	TIMER0->Timer1Ctrl    |= 0x00000020; // enable          timer interrupt
	TIMER0->Timer1Ctrl    |= 0x00000080; // enable          timer

	GICC0->PMR             = 0x000000F0; // unmask all            interrupts
	GICD0->ISENABLER[ 1 ] |= 0x00000010; // enable timer          interrupt
	GICC0->CTLR            = 0x00000001; // enable GIC interface
	GICD0->CTLR            = 0x00000001; // enable GIC distributor

	irq_enable();
}

// Looks up if there are any processes deleted if not returns the top of the stack If no mor stack space can be allocated it retunrns -1
int findSlot() {
	int place = -1;
	for (int i = 0; i < nrprocess; i++) {
		if (pcb[i].priority == -1) {
			place = i;
		}
	}
	if (place == -1) {
		if (nrprocess < all) {
			stack = stack + 0x00001000;
			nrprocess++;
			place = nrprocess - 1;
		} else place = -1;
	}
	return (place);
}

void createProcess(uint32_t pc, uint32_t cpsr, uint32_t priority  ) {
	pid_t pid = findSlot();
	memset( &pcb[ pid ], 0, sizeof( pcb_t ) );
	pcb[ pid ].priority   = priority;
	pcb[ pid ].pid      = pid;
	pcb[ pid ].ctx.cpsr = cpsr;
	pcb[ pid ].ctx.pc   = pc;
	pcb[ pid ].ctx.sp   = stack + pid * 0x00001000;
	printS("Created a process! Yeyy! \n");
}

void kernel_handler_rst(ctx_t* ctx) {
	/* Configure the mechanism for interrupt handling by
	 *
	 * - configuring timer st. it raises a (periodic) interrupt for each
	 *   timer tick,
	 * - configuring GIC st. the selected interrupts are forwarded to the
	 *   processor via the IRQ interrupt signal, then
	 * - enabling IRQ interrupts.
	 */

	//timer();
// For some megical reason all Ps work even dough I don't define them. HOW?


	createProcess(( uint32_t )( entry_terminal ), 0x50, 0);
	/*	createProcess(( uint32_t )( entry_P0 ), 0x50, 2);
		createProcess(( uint32_t )( entry_P1 ), 0x50, 3);
		createProcess(( uint32_t )( entry_P2 ), 0x50, 1);*/

	current = &pcb[ 0 ]; memcpy( ctx, &current->ctx, sizeof( ctx_t ) );

	return;
}

void kernel_handler_irq(ctx_t* ctx) {
	// Step 2: read  the interrupt identifier so we know the source.

	uint32_t id = GICC0->IAR;

	// Step 4: handle the interrupt, then clear (or reset) the source.

	if ( id == GIC_SOURCE_TIMER0 ) {
		//PL011_putc( UART0, 'T' );
		TIMER0->Timer1IntClr = 0x01;
		if (current -> pid != 3) {
			//scheduler(ctx);
		}
	}

	// Step 5: write the interrupt identifier to signal we're done.

	GICC0->EOIR = id;
}

void addPCB(pid_t cp, pid_t pp, ctx_t* ctx) {

	memset (&pcb[ cp ], 0, sizeof(pcb_t));

	pcb[ cp ].pid      = cp;
	pcb[ cp ].priority   = 3; // I set it to 3 for now
	pcb[ cp ].ctx.pc   = pcb[ pp ].ctx.pc;
	pcb[ cp ].ctx.cpsr = pcb[ pp ].ctx.cpsr;
	pcb[ cp ].ctx.sp   = pcb[ pp ].ctx.sp + (cp - pp) * 0x00001000;
	memcpy( &pcb[ cp ].ctx, ctx, sizeof(ctx_t));
	/*	printS("Added ");
		printInt(cp);
		printS("\n");*/
}

void ipcArray() {
	for ( int i = 0; i < 6; i++ ) {
		memset( &ipc[ i ], -1, sizeof( ipc ) );
	}
}
int createPipe(int c_start, int c_end) {
	int slot = -1;
	for ( int i = 0; i < 6; i++ ) {
		if ( ipc[ i ].c_end == -1 ) slot = i;
	}
	if (slot == -1) printS("No more space for new pipe");
	else {
		ipc[ slot ].c_start = c_start;
		ipc[ slot ].c_end = c_end;
	}

	return slot;
}

void kernel_handler_svc(ctx_t* ctx, uint32_t id ) {

	switch ( id ) {
	case 0x00 : { // yield()

		scheduler( ctx );
		break;
	}
	case 0x01 : { // write( fd, x, n )
		int   fd = ( int   )( ctx->gpr[ 0 ] );
		char*  x = ( char* )( ctx->gpr[ 1 ] );
		int    n = ( int   )( ctx->gpr[ 2 ] );

		for ( int i = 0; i < n; i++ ) {
			PL011_putc( UART0, *x++ );
		}
		//scheduler(ctx);
		ctx->gpr[ 0 ] = n;
		break;
	}
	case 0x02 : { // fork()
		//The way fork works is that while running the process where it is called it forks the process exactly where it left of, creates a new process and continues form the
		//place where the process where it was called left off.
		//I have to increase stack size and than copy the currently running processes information
		pid_t pp  = current->pid;
		pid_t cp  = findSlot(pp);
		//fork() returns a zero to the newly created child process.
		//fork() returns a positive value, the process ID of the child process, to the parent.
		if (cp != -1) {
			addPCB(cp, pp, ctx);
			memcpy( &pcb[ cp ].ctx, ctx, sizeof(ctx_t));
			//pcb[ cp ].ctx.sp   = pcb[ pp ].ctx.sp + (cp - pp) * 0x00001000;
			memcpy( &pcb[ pp ].ctx, ctx, sizeof( ctx_t ) );
			memcpy( ctx, &pcb[ cp ].ctx, sizeof( ctx_t ) );
			pcb[pp].ctx.gpr[0] = cp;
			// if i change ctx -> gpr[ 0 ] = 0; to the line below it has the right stuff in pcb gpr [0], but doesn't work, like this it doesn't but it works ??
			//pcb[cp].ctx.gpr[0] = 0;
			current = &pcb[ cp ];
			ctx -> gpr[ 0 ] = 0;

		} else {
			printS("No more space for new processes!\n");
		}

		break;
	}

	case 0x03 : { // read( fd, x, n )

		char*  x = ( char* )( ctx->gpr[ 0 ] );
		int i = 0;
		x[0] = PL011_getc( UART0);
		PL011_putc(UART0, x[0]);

		while (x[i] != '\x0D') {
			i++;
			x[i] = PL011_getc( UART0);
			PL011_putc(UART0, x[i]);
		}
		printS("\n");
		ctx->gpr[ 0 ] = i - 1;

		break;
	}

	case 0x04 : { // system_exit --> exit terminates a process by deleting it form the list of processes and call the scheduler to decide where to go

		int cp = current->pid;
		pcb[ cp ].priority = -1;
		//stack -= 0x00001000;
		scheduler(ctx);
		break;
	}

	case 0x05 : {//kill
		killProcess(ctx, ctx->gpr[0]);
		break;
	}
	// case 0x06 : {//share? DO I need this?
//      int pid = ctx->gpr[0];
//      int dat = ctx->gpr[1];
//      do_share(pid, dat);
//      ctx->gpr[0] = 0;
//      break;
//    }

	// case 0x06 : { // exec()

	// 	int cp = current->pid;
	// 	int pp = pcb[ cp ].priority;

	// 	pcb[ pp ].ctx.pc   = pcb[ cp ].ctx.pc;
	// 	pcb[ pp ].ctx.cpsr = pcb[ cp ].ctx.cpsr;
	// 	//pcb[ pp ].ctx.sp   = pcb[ cp ].ctx.sp + (cp - pp) * 0x00001000;

	// 	// memcpy( &pcb[ cp ].ctx, ctx, sizeof(ctx_t));
	// 	// memcpy( &pcb[ pp ].ctx, ctx, sizeof( ctx_t ) );
	// 	// memcpy( ctx, &pcb[ cp ].ctx, sizeof( ctx_t ) );
	// 	current = &pcb[ pp ];
	// 	break;

	// }

	case 0x07 : {
		int c_start  = ( int )(ctx -> gpr[0]);
		int c_end  = ( int )(ctx -> gpr[1]);
		int ipc = createPipe(c_start, c_end);

		ctx -> gpr[0] = ipc;
		break;
	}

	default   : { // unknown
		printS(" Something went wrong! \n");
		break;
	}

	}
	return;
}