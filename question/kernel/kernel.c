#include "kernel.h"

pcb_t pcb[ 5 ], *current = NULL; /* Define a type pcb_t that captures a Process Control Block (PCB), instances of which form
entries in the process table: given the limited remit here, such entry simply includes a PID and execution
context. */
uint32_t nr = 3; //contains the number of processes we currently have
uint32_t all =  5; // all represents the overall number of processes which we can allocate (can be changed if you allocate more space at pcb_t pcb[ 5 ] )
uint32_t stack = (uint32_t) &tos_P2; //pointer to the top of the stack

void scheduler( ctx_t* ctx ) {

	// For now, when I have a  new process (when I fork, for example I will increment nr_used so when the programm is running, printing it's name and the new process is
	// the child of process two it will print 12321232....

	//Change this so it would return to parent process if it was a child

	if ( current != &pcb[ nr - 1 ] ) {// -1 because I started counting form 0
		//printS(" not in last    ");
		memcpy( &pcb[ current -> pid].ctx, ctx, sizeof( ctx_t ) );
		memcpy( ctx, &pcb[ current -> pid + 1 ].ctx, sizeof( ctx_t ) );
		current = &pcb[ current -> pid + 1 ];
	} else {
		//printS(" IN last    ");
		//printInt(nr - 1);
		memcpy( &pcb[ nr ].ctx, ctx, sizeof( ctx_t ) );
		memcpy( ctx, &pcb[ 0 ].ctx, sizeof( ctx_t ) );
		current = &pcb[ 0 ];
	}

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

void kernel_handler_rst(ctx_t* ctx) {
	/* Configure the mechanism for interrupt handling by
	 *
	 * - configuring timer st. it raises a (periodic) interrupt for each
	 *   timer tick,
	 * - configuring GIC st. the selected interrupts are forwarded to the
	 *   processor via the IRQ interrupt signal, then
	 * - enabling IRQ interrupts.
	 */

	memset( &pcb[ 0 ], 0, sizeof( pcb_t ) );
	pcb[ 0 ].pid      = 0;
	pcb[ 0 ].parent   = 0;
	pcb[ 0 ].ctx.cpsr = 0x50;
	pcb[ 0 ].ctx.pc   = ( uint32_t )( entry_P0 );
	pcb[ 0 ].ctx.sp   = ( uint32_t )(  &tos_P0 );

	memset( &pcb[ 1 ], 0, sizeof( pcb_t ) );
	pcb[ 1 ].pid      = 1;
	pcb[ 1 ].parent   = 1;
	pcb[ 1 ].ctx.cpsr = 0x50;
	pcb[ 1 ].ctx.pc   = ( uint32_t )( entry_P1 );
	pcb[ 1 ].ctx.sp   = ( uint32_t )(  &tos_P1 );

	memset( &pcb[ 2 ], 0, sizeof( pcb_t ) );
	pcb[ 2 ].pid      = 2;
	pcb[ 2 ].parent   = 2;
	pcb[ 2 ].ctx.cpsr = 0x50;
	pcb[ 2 ].ctx.pc   = ( uint32_t )( entry_P2 );
	pcb[ 2 ].ctx.sp   = ( uint32_t )(  &tos_P2 );




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
		//scheduler(ctx);
	}

	// Step 5: write the interrupt identifier to signal we're done.

	GICC0->EOIR = id;
}

void addPCB(pid_t cp, pid_t pp, ctx_t* ctx) {

	memset (&pcb[ cp ], 0, sizeof(pcb_t));

	//pcb[ cpid ].parent   = ppid;
	pcb[ cp ].pid      = cp;
	pcb[ cp ].parent   = pp;
	pcb[ cp ].ctx.pc   = pcb[ pp ].ctx.pc;
	pcb[ cp ].ctx.cpsr = pcb[ pp ].ctx.cpsr;
	pcb[ cp ].ctx.sp   = pcb[ pp ].ctx.sp + (cp - pp) * 0x00001000;
	memcpy( &pcb[ cp ].ctx, ctx, sizeof(ctx_t));
	/*	printS("Added ");
		printInt(cp);
		printS("\n");*/
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
		pid_t cp  = nr;

		if (nr < all) {
			stack += 0x00001000;
			nr ++;
			addPCB(cp, pp, ctx);
		} else {
			printS("No more space for new processes!");
		}


		/*printInt(nr);

		memset( &pcb[ nr - 1 ], 0, sizeof( pcb_t ) ); //clears the space for new things and puts 0s
		pcb[ nr - 1 ].pid      = nr - 1;
		pcb[ nr - 1 ].ctx.cpsr = pcb[current -> pid].ctx.cpsr; //current -> pid --- gets the pid
		pcb[ nr - 1 ].ctx.pc   = pcb[current -> pid].ctx.pc;
		pcb[ nr - 1 ].ctx.sp   = pcb[current -> pid].ctx.sp + 0x00001000 * (nr - 1 - current -> pid);

		memcpy( &pcb[nr - 1].ctx, ctx, sizeof( ctx_t ) );*/

		//current = &pcb[ nr - 1 ];
		//ctx -> gpr[ 0 ] = cp;
		scheduler(cp);

		break;
	}

	case 0x03 : { // read( fd, x, n )
		//int   fd = ( int   )( ctx->gpr[ 0 ] );
		char*  x = ( char* )( ctx->gpr[ 0 ] );
		//int    n = ( int   )( ctx->gpr[ 2 ] );

		/*		for ( int i = 0; i < n - 1; i++ ) {
					x[i] = PL011_getc( UART0);
					PL011_putc(UART0, x[i]);
				}*/

		int i = 0;
		x[0] = PL011_getc( UART0);
		PL011_putc(UART0, x[0]);
		//while (x[i] != 	'\x0D') {
		while (x[i] != '\x0D') {
			i++;
			x[i] = PL011_getc( UART0);
			PL011_putc(UART0, x[i]);
		}
		printS("\n");
		ctx->gpr[ 0 ] = i - 1;

		// ctx->gpr[ 0 ] = n;
		break;
	}

	case 0x04 : { // system_exit --> exit terminates a process by deleting it form the list of processes and call the scheduler to decide where to go
		if (nr > 0) {
			int cp = current->pid;
			int pp = pcb[ cp ].parent;
			//printInt(004);
			//printInt(nr);
			memcpy( &pcb[ cp ].ctx, ctx, sizeof( ctx_t ) );
			memcpy( ctx, &pcb[ pp ].ctx, sizeof( ctx_t ) );
			memset (&pcb[ cp ], 0, sizeof(pcb_t));

			stack -= 0x00001000;
			current = &pcb[ pp ];
			/*		if (pcb[ current -> pid].pid == nr - 1) {
						current = &pcb[ 0 ];
					} else {
						current = &pcb[ current -> pid + 1];
					}*/

			//memset( &pcb[ nr - 1 ], 0, sizeof( pcb_t ) ); //clears the space for new things and puts 0s
			ctx -> gpr[ 0 ] = 0;
			nr --;
		} else {
			write(0, "no process to be deleted \n", 26);
		}
		//current = &pcb[ nr - 1 ]; // this is not good becasue this way you can only delete things frm the top of the stack

		//scheduler(&current->ctx);

	}
	default   : { // unknown
		break;
	}

	}
	return;
}