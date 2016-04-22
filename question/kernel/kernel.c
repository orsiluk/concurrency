#include "kernel.h"


void ipcArray();
/* Define a type pcb_t that captures a Process Control Block (PCB), instances of which form
entries in the process table: given the limited remit here, such entry simply includes a PID and execution
context. */
pcb_t pcb[ 10 ], *current = NULL;

// Array of channels
ipc_t ipc[ 10 ];

// Struct created to manage the memory
typedef struct {
	char name[4];
	int address;
	int size;
} filelist;
// List of files on disk
filelist files[10];

//Current number of files on disk
int nrfile = 0;

int chan; //Channel to current process
uint32_t all =  10; // all represents the overall number of processes which we can allocate (can be changed if you allocate more space at pcb_t pcb[ 10 ] )
int pipes = 0; // The number of open channels
int nrprocess = 0;
uint32_t stack = (uint32_t) &tos_terminal + 0x00001000; //pointer to the top of the stack

// Ageing a process ( incrementing priority actually means that we substract 1 from the prioroty of the current process)
/*void incPrority() {
	for (int i = 0; i < all; i++) {
		if (pcb[i].priority > 0) {
			pcb[i].priority += 1;
		}
	}
}*/

// Returns the next process to be executed
int nextP() {
	uint32_t found = -1;
	uint32_t highest = 11;
	int i = 0;
	while (i < nrprocess) {
		if (i != current->pid || current->pid == 0 ) {
			if (pcb[i].priority < highest && pcb[i].priority != -1 ) {
				found = i;
				highest = pcb[i].priority;
			}
		}
		i++;
	}
//	incPrority();

	if (found != 0 && found != -1) {
		pcb[found].priority++;
	}

	return found;
}

// Priority based scheduler
void scheduler(ctx_t* ctx) {

	// Priority based scheduler with hopefully aging implemented as well
	int next = nextP();
	if (nrprocess == current->pid ) return;
	else {
		memcpy( &pcb[ current->pid ].ctx, ctx, sizeof( ctx_t ) );
		memcpy( ctx, &pcb[ next ].ctx, sizeof( ctx_t ) );
		current = &pcb[ next ];
	}
}

// Round Robin schedular
void schedulerR(ctx_t* ctx) {
	// It is the round-robin scheduler, it can be changed so it would return to the parent process if needed

	/*
		// If I uncomment this it will go back to parent after it was executed

		if (current -> pid != current -> parent) {
			memcpy( &pcb[ current -> pid ].ctx, ctx, sizeof( ctx_t ) );
			memcpy( ctx, &pcb[ current -> parent ].ctx, sizeof( ctx_t ) );
			current = &pcb[ current -> parent ];
		}
		else*/
	if (nrprocess == 1) return;
	else if ( current != &pcb[ nrprocess - 1 ]) {// -1 because I started counting form 0
		memcpy( &pcb[ current -> pid].ctx, ctx, sizeof( ctx_t ) );
		memcpy( ctx, &pcb[ current -> pid + 1 ].ctx, sizeof( ctx_t ) );
		current = &pcb[ current -> pid + 1 ];
	} else {
		memcpy( &pcb[ nrprocess ].ctx, ctx, sizeof( ctx_t ) );
		memcpy( ctx, &pcb[ 0 ].ctx, sizeof( ctx_t ) );
		current = &pcb[ 0 ];
	}

	if (pcb[current->pid].priority == -1) {
		schedulerR(ctx);
	}
}

// Kill process
void killProcess(ctx_t* ctx , int p) {
	pcb[p].priority = -1;
	schedulerR(ctx);
}

// Timer
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

// Creates pre-defined processes
int createProcess(uint32_t pc, uint32_t cpsr, uint32_t priority  ) {
	pid_t pid = findSlot();
	memset( &pcb[ pid ], 0, sizeof( pcb_t ) );
	pcb[ pid ].priority   = priority;
	pcb[ pid ].pid      = pid;
	pcb[ pid ].parent   = pid;
	pcb[ pid ].ctx.cpsr = cpsr;
	pcb[ pid ].ctx.pc   = pc;
	pcb[ pid ].ctx.sp   = stack + pid * 0x00001000;
	return pid;
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
	ipcArray();

	int i = createProcess(( uint32_t )( entry_terminal ), 0x50, 5);

	current = &pcb[ 0 ]; memcpy( ctx, &current->ctx, sizeof( ctx_t ) );
	timer();
	return;
}

void kernel_handler_irq(ctx_t* ctx) {
	// Step 2: read  the interrupt identifier so we know the source.

	uint32_t id = GICC0->IAR;

	// Step 4: handle the interrupt, then clear (or reset) the source.

	if ( id == GIC_SOURCE_TIMER0 ) {
		//PL011_putc( UART0, 'T' );
		/*		// if () {
				int this = current->pid;
				if ( this > 1 && this < chan + 1 ) schedulerR(ctx);
				else if (this == chan + 1) execute(2);
				//}*/
		scheduler(ctx);
		TIMER0->Timer1IntClr = 0x01;

	}

	// Step 5: write the interrupt identifier to signal we're done.

	GICC0->EOIR = id;
}

// Add a new element to pcb (process control block)
void addPCB(pid_t cp, pid_t pp, ctx_t* ctx, int prior) {

	memset (&pcb[ cp ], 0, sizeof(pcb_t));

	pcb[ cp ].pid      = cp;
	pcb[ cp ].parent   = pp;
	pcb[ cp ].priority = prior; // I set it to 3 for now
	pcb[ cp ].ctx.pc   = pcb[ pp ].ctx.pc;
	pcb[ cp ].ctx.cpsr = pcb[ pp ].ctx.cpsr;
	pcb[ cp ].ctx.sp   = pcb[ pp ].ctx.sp + (cp - pp) * 0x00001000;
	memcpy( &pcb[ cp ].ctx, ctx, sizeof(ctx_t));
}

// Sets up the IPC array at the beggening
void ipcArray() {

	for ( int i = 0; i < 10; i++ ) {
		memset( &ipc[ i ], -1, sizeof( ipc ) );
	}
}

// Finds a slot for the new channel
int getIpcSlot() {
	for ( int i = 0; i < 10; i++ ) {
		if ( ipc[ i ].c_end == -1 ) return i;
	}

	return -1;
}

// Creates a channel
int createPipe(int c_start, int c_end) {
	int slot = getIpcSlot();

	// Find slot for next pcb
	if (slot > -1) {
		ipc[ slot ].c_start = c_start;
		ipc[ slot ].c_end = c_end;
		ipc[ slot ].buff[0] = -1;
		ipc[ slot ].buff[1] = -1;
	} else printS("No more space for pipes");

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
		ctx->gpr[ 0 ] = n;
		break;
	}
	case 0x02 : { // fork()
		//fork() returns a zero to the newly created child process.
		//fork() returns a positive value, the process ID of the child process, to the parent.
		pid_t pp  = current->pid;
		pid_t cp  = findSlot(pp);

		uint32_t sp = ctx->sp;
		if (cp != -1) {
			addPCB(cp, pp, ctx, 3);

			uint32_t sp2 = sp + (cp - pp) * 0x00001000;
			uint32_t stackSpaceA  = stack + (cp - 1) * 0x00001000;
			uint32_t stackSpaceB  = stack + (pp - 1) * 0x00001000;

			memcpy((void*)stackSpaceA, (void*)stackSpaceB, 0x00001000);
			ctx->gpr[0] = cp;
			pcb[cp].ctx.gpr[0] = 0;
			pcb[cp].ctx.sp = sp2;

			memcpy( &pcb[ pp ].ctx, ctx, sizeof( ctx_t ) );
			memcpy( ctx, &pcb[ cp ].ctx, sizeof( ctx_t ) );
			current = &pcb[ cp ];
			pcb[pp].ctx.sp = sp;
		} else {
			printS("No more space for new processes!\n");
		}

		break;
	}

	case 0x03 : { // read( x )

		char*  x = ( char* )( ctx->gpr[ 0 ] );
		int i = 0;
		x[0] = PL011_getc( UART0);
		PL011_putc(UART0, x[0]);

		while (x[i] != '\r') {
			i++;
			x[i] = PL011_getc( UART0);
			PL011_putc(UART0, x[i]);
		}
		printS("\n");
		ctx->gpr[ 0 ] = i;

		break;
	}

	case 0x04 : { // system_exit --> exit terminates a process by deleting it form the list of processes and call the scheduler to decide where to go

		int cp = current->pid;
		pcb[ cp ].priority = -1;
		scheduler(ctx);
		break;
	}

	case 0x05 : {//kill

		killProcess(ctx, ctx->gpr[0]);
		break;
	}

	case 0x06 : { // execute(pid) // I want this function to execute a process with a certain pid fead into it

		uint32_t execp = ( uint32_t )( ctx->gpr[ 0 ] );
		if (pcb[execp].pid != -1) {
			memcpy( &pcb[ current->pid ].ctx, ctx, sizeof( ctx_t ) );
			memcpy( ctx, &pcb[ execp ].ctx, sizeof( ctx_t ) );
			current = &pcb[ execp ];
		} else printS("No a real process");
		break;

	}

	case 0x07 : { // create_c( int c_start, int c_end)
		int c_start  = ( int )(ctx -> gpr[0]);
		int c_end  = ( int )(ctx -> gpr[1]);
		int ipc = createPipe(c_start, c_end);
		chan ++;

		ctx -> gpr[0] = ipc; // ipc is the value which shows where is this channel in ipc list
		break;
	}
	case 0x08 : { // get_id()
		ctx -> gpr[0] = current -> pid;
		break;
	}
	case 0x09 : { // writeC(chanid, cstick, who) - cstick is 0 if asking 1 if giving
		int chanid = ( int )(ctx -> gpr[0]);
		int cstick = ( int )(ctx -> gpr[1]);
		int who = ( int )(ctx -> gpr[2]);

		if (ipc[chanid].buff[who] == -1) ipc[chanid].buff[who] = cstick;
		//else printS(" You first need to read from channel");

		break;
	}
	case 10 : { // int readC(int chanid,int who)
		int chanid = ( int )(ctx -> gpr[0]);
		int who = ( int )(ctx -> gpr[1]);

		if (ipc[chanid].buff[who] != -1) {
			ctx -> gpr[0] = ipc[chanid].buff[who];
			ipc[chanid].buff[who] = -1;
		} else {
			//printS(" Nothing to read.");
			ctx -> gpr[0] = 0;
		}

		break;
	}
	case 11: { //runT() run timer
		int i = 0;
		while (i < 300000000) {
			i++;
		}
		/*		uint32_t t = GICC0->IAR;
						if ( id == GIC_SOURCE_TIMER0 ) {

						}
				TIMER0->Timer1IntClr = 0x01;

				// Step 5: write the interrupt identifier to signal we're done.

				GICC0->EOIR = id;*/
		break;
	}
	case 12 : { //blockNum()
		int i = disk_get_block_num();
		ctx -> gpr[0] = i;
		break;
	}
	case 13 : { //blockLen()
		int i = disk_get_block_len();
		ctx -> gpr[0] = i;
		break;
	}
	case 14 : { // wrtDisk(int where, char* x, int l)

		// For now it only writes in the first line. Find a way to find you the next free space
		int where = ( int )(ctx -> gpr[0]);
		char* text = ( char* )(ctx -> gpr[1]);
		int len = ( int )(ctx -> gpr[2]);

		disk_wr(0, text, len);
		break;
	}
	case 15 : { //rdDisk(int where, char* text,int len)
		int    where = ( int )(ctx -> gpr[0]);
		char*  text =  ( char* )( ctx->gpr[ 1 ] );
		int    len =   ( int )(ctx -> gpr[2]);
		disk_rd(where, text, len);
		break;
	}
	case 16 : {//createfile(char fname, char text, int fsize)
		char*  fname =  ( char* )( ctx->gpr[ 0 ] );
		char*  text =  ( char* )( ctx->gpr[ 1 ] );
		int    fsize = ( int )(ctx -> gpr[2]);

		scopy(fname, files[nrfile].name);
		int spot = -1;
		for (int i = 0; i <= nrfile; i++) {
			if (files[i].address == -1) {
				spot = i;
				break;
			}
		}

		if (nrfile == 0) {
			disk_wr(16, text, fsize); //I start at 12, I wanted to leave space to just write random stuff to disk
			files[nrfile].address = 16;
			files[nrfile].size = fsize;
			nrfile++;
		} else {
			if (spot < 0) spot = nrfile;
			if (spot = nrfile) nrfile++;
			files[spot].address = 16 + files[spot - 1].address;
			files[spot].size = fsize;
			disk_wr(files[spot].address, text, fsize);
		}
		break;
	}
	case 17 : { //openfile(int read, char* fname) //returns the address of a file
		int i = 0;
		int    read = ( int )(ctx -> gpr[0]);
		char*  fname =  ( char* )( ctx->gpr[ 1 ] );

		int eq;
		while (i < nrfile) {
			eq = compare(fname, files[i].name);
			if (eq == 1 && files[i].address != -1) {
				if (read == 1) {
					char content[files[i].size];
					int adr = files[i].address;
					disk_rd(files[i].address, content, files[i].size);

					printS(content);
					ctx->gpr[ 0 ] = files[i].size;

					break;
				} else
					ctx->gpr[ 0 ] = files[i].address;
				break;
			}
			i++;
		}
		break;
		if (eq == 0) {
			printS("No such file\n" );
			ctx->gpr[ 0 ] = 0;
			break;
		}

	}
	case 18 : { //closeFile(char* fname)
		int i = 0;
		int eq;
		char*  fname =  ( char* )( ctx->gpr[ 0 ] );
		while (i < nrfile) {
			eq = compare(fname, files[i].name);
			if (eq == 1 && files[i].address != -1) {
				/*				printS("Found!\n");
								printS("Address was: ");
								printInt(files[i].address);
								printS("\n");*/
				files[i].address = -1;
				files[i].size = 0;
				printS("File closed! \n");
				//printInt(files[i].address);
				//printS("\n");
				break;
			}
			i++;
		}
		if ( i == nrfile) printS("File not found");

		break;
	}
	// case 19: { //pbs()
	// 	psched = 1;

	// }
	default   : {
		printS(" Something went wrong in kernel! \n");
		break;
	}

	}
	return;
}


/*
creat ------------- create a file
open -------------- open a file
close-------------- close a file
unlink ------------ delete a file
write ------------- write to a file
read -------------- read from a file
*/