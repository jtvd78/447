.globl _start
_start:
    b res_handler		// RESET                 runs in SVC mode
    b hang				// UNDEF INSTR           runs in UND mode
    b svc_handler		// SWI (TRAP)            runs in SVC mode
    b hang				// PREFETCH ABORT        runs in ABT mode
    b hang				// DATA ABORT            runs in ABT mode
    b hang				// HYP MODE              runs in HYP mode
    b irq_handler		// IRQ INTERRUPT         runs in IRQ mode
	// FIQ can simply be written here, since this is the end of the table:
    b hang		// FIQ INTERRUPT         runs in FIQ mode

.equ    FIQSTACK3 , 0x2f000
.equ    IRQSTACK3 , 0x2e000
.equ    SVCSTACK3 , 0x2d000
.equ    KSTACK3   , 0x2c000
.equ    FIQSTACK2 , 0x2b000
.equ    IRQSTACK2 , 0x2a000
.equ    SVCSTACK2 , 0x29000
.equ    KSTACK2   , 0x28000
.equ    FIQSTACK1 , 0x27000
.equ    IRQSTACK1 , 0x26000
.equ    SVCSTACK1 , 0x25000
.equ    KSTACK1   , 0x24000
.equ    FIQSTACK0 , 0x23000
.equ    IRQSTACK0 , 0x22000
.equ    SVCSTACK0 , 0x21000
.equ    KSTACK0   , 0x20000

.equ	USR_mode,	0x10
.equ	FIQ_mode,	0x11
.equ	IRQ_mode,	0x12
.equ	SVC_mode,	0x13
.equ	HYP_mode,	0x1A
.equ	SYS_mode,	0x1F
.equ	No_Int,		0xC0


res_handler:
    mrc p15, 0, r0, c1, c0, 0 @ Read System Control Register
@   orr r0, r0, #(1<<2)       @ dcache enable
    orr r0, r0, #(1<<12)      @ icache enable
    and r0, r0, #0xFFFFDFFF   @ turn on vector table at 0x0000000 (bit 12)
    mcr p15, 0, r0, c1, c0, 0 @ Write System Control Register

	// divide up by core, and set up each stack separately
	mrc     p15, 0, r0, c0, c0, 5
	ubfx    r0, r0, #0, #2
	cmp     r0, #0					@ is it core 0?
	beq     core0
	cmp     r0, #1					@ is it core 1?
	beq     core1
	cmp     r0, #2					@ is it core 2?
	beq     core2
	cmp     r0, #3					@ is it core 3?
	beq     core3

	@ CPU ID is not 0..3 - wth?
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

	// set up stacks (only need IRQ SVC and K at this point, but what the heck)
	cps		#IRQ_mode
	mov		sp, # IRQSTACK0
	cps		#FIQ_mode
	mov		sp, # FIQSTACK0
	cps		#SVC_mode
	mov		sp, # SVCSTACK0
	cps		#SYS_mode
	mov		sp, # KSTACK0
	bl		init_kernel

	// start the timer-interrupt going
	bl		set_timer	// must be BEFORE going to user mode

	// set up user stack and jump to shell
	cps		#USR_mode
	ldr		sp, stack_address_runningthread
	ldr		r0, start_address_runningthread
	mov	pc, r0
	b hang

core1:
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

	// set up stacks (only need IRQ SVC and K at this point, but what the heck)
	cps		#IRQ_mode
	mov		sp, # IRQSTACK1
	cps		#FIQ_mode
	mov		sp, # FIQSTACK1
	cps		#SVC_mode
	mov		sp, # SVCSTACK1
	cps		#SYS_mode
	mov		sp, # KSTACK1
	bl		init_kernel

	// start the timer-interrupt going
	bl		set_timer	// must be BEFORE going to user mode

	// set up user stack and jump to shell
	cps		#USR_mode
	ldr		sp, stack_address_runningthread
	ldr		r0, start_address_runningthread
	mov	pc, r0
	b hang

core2:
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

	// set up stacks (only need IRQ SVC and K at this point, but what the heck)
	cps		#IRQ_mode
	mov		sp, # IRQSTACK2
	cps		#FIQ_mode
	mov		sp, # FIQSTACK2
	cps		#SVC_mode
	mov		sp, # SVCSTACK2
	cps		#SYS_mode
	mov		sp, # KSTACK2
	bl		init_kernel

	// start the timer-interrupt going
	bl		set_timer	// must be BEFORE going to user mode

	// set up user stack and jump to shell
	cps		#USR_mode
	ldr		sp, stack_address_runningthread
	ldr		r0, start_address_runningthread
	mov	pc, r0
	b hang

core3:
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

	// set up stacks (only need IRQ SVC and K at this point, but what the heck)
	cps		#IRQ_mode
	mov		sp, # IRQSTACK3
	cps		#FIQ_mode
	mov		sp, # FIQSTACK3
	cps		#SVC_mode
	mov		sp, # SVCSTACK3
	cps		#SYS_mode
	mov		sp, # KSTACK3
	bl		init_kernel

	// start the timer-interrupt going
	bl		set_timer	// must be BEFORE going to user mode

	// set up user stack and jump to shell
	cps		#USR_mode
	ldr		sp, stack_address_runningthread
	ldr		r0, start_address_runningthread
	mov	pc, r0
	b hang



.globl hang
hang: 	wfi
		b hang


.globl runningthreadid
runningthreadid:	.word 0

.globl start_address_runningthread
start_address_runningthread:	.word 0

.globl stack_address_runningthread
stack_address_runningthread:	.word 0

.globl tcb_address_runningthread
tcb_address_runningthread:	.word 0


save_lr_irq: .word 0

// courtesy of Prof Vince Weaver, U Maine
irq_handler:

    ldr     sp, tcb_address_runningthread            @ load the now-destroyed r13 w TCB pointer
    stmia   sp,{r0-lr}^     @ Save all user registers r0-lr
						@ (the ^ means user registers)

//	subs	lr, lr, #4
    str lr,[sp,#60]     @ store saved PC on stack

    str     lr, save_lr_irq           @ save the SVC lr
    mrs lr, SPSR        @ load SPSR (assume ip not a swi arg)
    str lr,[sp,#64]     @ store on stack
    ldr     lr, save_lr_irq           @ save the SVC lr

    @ Call the C version of the handler
	mov		sp, #SVCSTACK0
	bl		clear_timer_interrupt
	bl		periodic_timer
	bl		set_timer

    ldr     sp, tcb_address_runningthread            @ load the now-destroyed r13 w TCB pointer
    ldr r0,[sp,#64]     @ pop saved CPSR
    msr SPSR_cxsf, r0       @ move it into place

    ldr lr,[sp,#60]     @ restore address to return to

    @ Restore saved values.  The ^ means to restore the userspace registers
    ldmia   sp, {r0-lr}^
//	mov	pc, lr					@ return from exception
	subs	pc, lr, #4					@ return from exception


save_r0_svc: .word 0

// courtesy of Prof Vince Weaver, U Maine
svc_handler:

    ldr     sp, tcb_address_runningthread            @ load the now-destroyed r13 w TCB pointer
    stmia   sp,{r0-lr}^     @ Save all user registers r0-lr
						@ (the ^ means user registers)

//	str lr,[sp,#60]     @ store saved PC on stack
	add ip, lr, #4
    str ip,[sp,#60]     @ store saved PC on stack

    mrs ip, SPSR        @ load SPSR (assume ip not a swi arg)
    str ip,[sp,#64]     @ store on stack

    @ Call the C version of the handler
	mov		sp, #SVCSTACK0
    bl  trap_handler

	// only because we know trap handler won't task switch
	str	r0, save_r0_svc

    ldr     sp, tcb_address_runningthread            @ load the now-destroyed r13 w TCB pointer
    ldr r0,[sp,#64]     @ pop saved CPSR
    msr SPSR_cxsf, r0       @ move it into place

    ldr lr,[sp,#60]     @ restore address to return to

    @ Restore saved values.  The ^ means to restore the userspace registers
    ldmia   sp, {r0-lr}^
//	movs    pc, lr
	ldr	r0, save_r0_svc
	subs	pc, lr, #4					@ return from exception

