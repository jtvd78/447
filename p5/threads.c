
#include "os.h"

extern	long stack_address_runningthread;
extern	long start_address_runningthread;
extern	long * tcb_address_runningthread;
extern	long runningthreadid;
extern int strcpyN(char *to, char *from, int bufsiz);

#define NAMESIZE 16

struct tcb {
	LL_PTRS;
	char	name[NAMESIZE];
	long	threadid;
	long	stack;
	long	regs[17];	// 17th reg is the SPSR saved by context switch
} tcbs[ NUM_THREADS ];

llobject_t TF;
struct tcb *tfree;

struct tcb *active_thread;
struct tcb *null_thread;

llobject_t RQ;
struct tcb *runq;

llobject_t SQ;
struct tcb *sleepq;


void
dump_tcb(long threadid)
{
	if (threadid < 0 || threadid > NUM_THREADS) {
		log("dump_tcb: invalid threadid", threadid);
		return;
	}
	if (threadid == NUM_THREADS) {
		threadid = active_thread->threadid;
	}
	log("Dumping TCB for thread", threadid);
	struct tcb *tp = &tcbs[threadid];
	log(tp->name, tp->threadid);
	log(" stack", tp->stack);
	log(" tcb @", (long)(tp->regs));
	log("  r0  ", tp->regs[REG_r0]);
	log("  r1  ", tp->regs[REG_r1]);
	log("  r2  ", tp->regs[REG_r2]);
	log("  r3  ", tp->regs[REG_r3]);
	log("  r4  ", tp->regs[REG_r4]);
	log("  r5  ", tp->regs[REG_r5]);
	log("  r6  ", tp->regs[REG_r6]);
	log("  r7  ", tp->regs[REG_r7]);
	log("  r8  ", tp->regs[REG_r8]);
	log("  r9  ", tp->regs[REG_r9]);
	log("  r10 ", tp->regs[REG_r10]);
	log("  r11 ", tp->regs[REG_r11]);
	log("  r12 ", tp->regs[REG_r12]);
	log("  sp  ", tp->regs[REG_sp]);
	log("  lr  ", tp->regs[REG_lr]);
	log("  pc  ", tp->regs[REG_pc]-4);
	log("  spsr", tp->regs[REG_spsr]);

	return;
}

void
create_thread(char *name, void *text)
{
	struct tcb *tp;

	if (name == (char *)NULL) {
		tp = &tcbs[0];
		tp->regs[REG_sp] = tp->stack;
		tp->regs[REG_pc] = (long)text+4;
		strcpyN(tp->name, "NULL thread", NAMESIZE);
		tp->name[NAMESIZE-1] = '\0';
	} else {
		tp = (struct tcb *)LL_POP(tfree);
		if (tp == (struct tcb *)NULL) {
			log("WARNING: out of processes in create_thread", NOVAL);
			return ;
		}

		tp->regs[REG_sp] = tp->stack;
		tp->regs[REG_pc] = (long)text+4;
		strcpyN(tp->name, name, NAMESIZE);
		tp->name[NAMESIZE-1] = '\0';

		LL_PUSH(runq, tp);
	}

	log("create_thread:",NOVAL);
	log(tp->name, tp->threadid);
	log(" stack =", tp->regs[REG_sp]);
	log(" start =", tp->regs[REG_pc] - 4);
	log(" tcb   =", (long)tp->regs);

	return;
}

void
scheduler(int new_state)
{

// your code goes here

	debug(DEBUG_HI, "scheduling out:",NOVAL);
	debug(DEBUG_HI, out->name, out->threadid);
	debug(DEBUG_HI, " stack =", out->regs[REG_sp]);
	debug(DEBUG_HI, " start =", out->regs[REG_pc]-4);
	debug(DEBUG_HI, " tcb   =", (long)out->regs);
	debug(DEBUG_HI, " spsr  =", out->regs[REG_spsr]);

	// you will find this useful
	if (in->regs[REG_spsr] == 0 && out != (struct tcb *)NULL) {
		in->regs[REG_spsr] = out->regs[REG_spsr] & 0xFFF;
	}

	debug(DEBUG_HI, "scheduling in:",NOVAL);
	debug(DEBUG_HI, in->name, in->threadid);
	debug(DEBUG_HI, " stack =", in->regs[REG_sp]);
	debug(DEBUG_HI, " start =", in->regs[REG_pc]-4);
	debug(DEBUG_HI, " tcb   =", (long)in->regs);
	debug(DEBUG_HI, " spsr  =", in->regs[REG_spsr]);

	return;
}

void
wake_thread(long threadid, long returnval)
{
	if (threadid < 0 || threadid >= NUM_THREADS) {
		log("ERROR: wake_thread: invalid threadid", threadid);
		return;
	}
	struct tcb *tp = &tcbs[threadid];

	// your code goes here ... get it off the sleepq, put it on the runq
	// and set it up so that the user process receives the return val in r0

	return;
}

void
init_threads()
{
	int i,j;

	tfree = (struct tcb *)&TF;
	LL_INIT(tfree);

	runq = (struct tcb *)&RQ;
	LL_INIT(runq);

	sleepq = (struct tcb *)&SQ;
	LL_INIT(sleepq);

	for (i=0; i<NUM_THREADS; i++) {
		strcpyN(tcbs[i].name, "[uninitialized]", NAMESIZE);
		tcbs[i].name[NAMESIZE-1] = '\0';
		tcbs[i].threadid = i;
		tcbs[i].stack = 0x20000 + ((i+1) << 12);	// hack for now
		for (j=0; j<17; j++) {
			tcbs[i].regs[j] = 0;
		}
		if (i > 0) {
			struct tcb *tp = &tcbs[i];
			LL_APPEND(tfree, tp);
		}
	}

	active_thread = (struct tcb *)NULL;
	null_thread = &tcbs[0];

	create_thread((char *)NULL, hang);
}

void
dump_processes()
{
	struct tcb *tp;
	log("PS: Active processes ...", NOVAL);
	dump_tcb(active_thread->threadid);
	LL_EACH(runq, tp, struct tcb) {
		dump_tcb(tp->threadid);
	}
}
