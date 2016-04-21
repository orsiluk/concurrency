# concurrency
COncurrent COmputing COursework 2

So far implemented:

Stage 1. 
This stage is designed to match the task in the lab. worksheet from week #16: you should implement
a simple, functioning operating system kernel that supports pre-emptive multi-tasking. All similar
simplifications made there can also be made here. In particular, note that at this point you can and
should assume a) each user program is statically compiled into the kernel image, and b) there is no
need to deal with dynamic process creation or termination.
Success criteria. Demonstrate concurrent execution of all the provided user programs (i.e., those in
the user directory) under control of the kernel. Note that ideally, the programs would be updated
so as to provide suitable output: each produce a sequence of integers step-by-step, so writing those
integers (plus some form of process identifier) to the emulator terminal should help demonstrate their
progress.


Stage 2. 
This stage involves the design and implementation of some standard improvements to the kernel.

(a) The kernel developed in the lab. worksheet(s) assumed two user processes exist at all times
(once executed). Improve on this by supporting dynamic creation and termination of processes
via fork and exit system calls. Since you design the semantics of these system calls, any
reasoned simplifications are allowed provided they support the desired functionality.
Success criteria. Demonstrate a command-line shell 2 (however simple) that allows the user to
enter commands and thus interactively control program execution (i.e., process creation and
termination); in UNIX parlance, this interaction is often termed job control 3 .

(b) The kernel developed in the lab. worksheet(s) used a special-purpose scheduling algorithm: it
could do so as the result of the assumption about two user processes existing at all times. Improve
on this by implementing an alternative; you can select or design an algorithm, but it must be able
to capitalise on the concept of priorities somehow.
Success criteria. Demonstrate the differing behaviour of your implementation vs. round-robin
scheduling (as implemented in the same kernel), plus explain when and why this represents an
improvement.

(c) The kernel developed in the lab. worksheet(s) lacked any mechanism for Inter-Process Commu-
nication (IPC). The first half of the unit introduced various ways to support the concept of IPC:
implement one of them in the kernel.
Success criteria. Consider a problem from first half of the unit that motivated study of IPC (e.g.,
dining philosophers), and demonstrate a a solution for it using your implemented mechanism.


Stage 3.
This stage has several diverse options, each representing a more ambitious improvement to the kernel.
The marking scheme clearly indicates that one option should be submitted; there is nothing to prevent
you attempting and submitting more than one, but only the best mark among those you submit will
count.
(a) An alternative to considering one of the various real, albeit emulated devices supported by the
PB-A8 is a compromise: for certain cases we could consider a simplified device instead, and
therefore (legitimately) focus on higher-level use rather than the device itself. Appendix A outlines the source code provided in order to support such a case. The goal is to use
a simplified disk, which offers block-based storage of data, to implement a file system: ideally
this will a) implement a UNIX-like, inode-based data structure, and b) support a suite of system
calls such as creat, unlink, open, close, read and write, with semantics of your own design,
which, in turn, demand management of file descriptors.
Success criteria. Demonstrate either a) an example user program that reads and writes data
to the disk in a non-trivial way, and/or b) the kernel dynamically loading a user program from
the disk then executing it (vs. using one of the statically compiled user programs, as assumed
above).



