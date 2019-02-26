.globl _start
_start:
    b res_handler	// RESET                 runs in SVC mode
    b hang			// UNDEF INSTR           runs in UND mode
    b svc_handler	// SWI (TRAP)            runs in SVC mode
    b hang			// PREFETCH ABORT        runs in ABT mode
    b hang			// DATA ABORT            runs in ABT mode
    b hang			// HYP MODE              runs in HYP mode
    b hang			// IRQ INTERRUPT         runs in IRQ mode
	// FIQ can simply be written here, since this is the end of the table:
    b hang			// FIQ INTERRUPT         runs in FIQ mode

.equ    FIQSTACK0 , 0xf0000
.equ    IRQSTACK0 , 0xe0000
.equ    SVCSTACK0 , 0xd0000
.equ    KSTACK0   , 0xc0000
.equ    USTACK0   , 0x80000

.equ	USR_mode,	0x10
.equ	FIQ_mode,	0x11
.equ	IRQ_mode,	0x12
.equ	SVC_mode,	0x13
.equ	HYP_mode,	0x1A
.equ	SYS_mode,	0x1F
.equ	No_Int,		0xC0


res_handler:
// check core ID
	mrc     p15, 0, r0, c0, c0, 5
	ubfx    r0, r0, #0, #2
	cmp     r0, #0					// is it core 0?
	beq     core0

	// it is not core0, so do things that are appropriate for SVC level as opposed to HYP
	// like set up separate stacks for each core, etc.

	beq     hang

hang: 	wfi
		b hang

core0:
	// Initialize SPSR in all modes.
	MOV    R0, #0
	MSR    SPSR, R0
	MSR    SPSR_svc, R0
	MSR    SPSR_und, R0
	MSR    SPSR_hyp, R0
	MSR    SPSR_abt, R0
	MSR    SPSR_irq, R0
	MSR    SPSR_fiq, R0

	// Initialize ELR_hyp (necessary?)
	MOV		R0, #0
	MSR		ELR_hyp, R0

	// set up stacks (only need SVC and K at this point, but what the heck)
	cps		#IRQ_mode
	mov		sp, # IRQSTACK0

	cps		#FIQ_mode
	mov		sp, # FIQSTACK0

	cps		#SVC_mode
	mov		sp, # SVCSTACK0

	cps		#SYS_mode
	mov		sp, # KSTACK0
	bl		init_kernel

	// set up user stack and jump to shell
	cps		#USR_mode
	mov		sp, # USTACK0
	bl		run_shell
	b hang


// courtesy of Prof Vince Weaver, U Maine
svc_handler:

    stmia   sp,{r0-lr}^     @ Save all user registers r0-lr
                    @ (the ^ means user registers)

    str lr,[sp,#60]     @ store saved PC on stack

    mrs ip, SPSR        @ load SPSR (assume ip not a swi arg)
    str ip,[sp,#64]     @ store on stack


    add sp,sp,#8192

    @ Call the C version of the handler

    bl  trap_handler

    @ Put our return value of r0 on the stack so it is
    @ restored with the rest of the saved registers

    sub sp,sp,#8192

    str r0,[sp]

    ldr r0,[sp,#64]     @ pop saved CPSR
    msr SPSR_cxsf, r0       @ move it into place

    ldr lr,[sp,#60]     @ restore address to return to

    @ Restore saved values.  The ^ means to restore the userspace registers
    ldmia   sp, {r0-lr}^
    movs    pc, lr


