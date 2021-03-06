/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Auto-generated by STM32CubeIDE
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

/*
 * Write a program to add, subtract, multiply, and divide 2 operands using SVC handler and return the resuilt to the thread mode code
 * and print the result. Thread mode code should pass 2 operands via the stack frame.
 * Use SVC number to decide the operation:
 *
 * Service Number			Operation
 * ---------------------------------------
 * 36						Addition
 * 37						Subtraction
 * 38						Multiplication
 * 39						Division
 *
 * 		__PendSV Exception__
 * 	. It is an exception type 14 and has a programmable priority level.
 * 	. This exception is triggered by setting its pending status by writing to the "Interrupt Control and State Register" of processor.
 * 	. Triggering a PendSV system exception is a way of invoking the preemptive kernel to carry out the context switch in an OS environment.
 * 	. In an OS environment, PendSV handler is set to the lowest priority level, and the PendSV handler carries out the context switch operation.
 *
 * 		__Typical Use of PendSV__
 * 	. Typically this exception is triggered inside a higher priority exception handler, and it gets executed when the higher priority handler finishes.
 * 	. Using this characteristic, we can schedule the PendSV exception handler to be executed after all the other interrupt processing tasks are done.
 * 	. This is very useful for a context switching operation, which is a crucial operation in various OS design.
 * 	. Using PendSV in context switching will be more efficient in an interrupt noisy environment.
 * 	. In an interrupt noisy environment, we need to delay the context switching until all IRQ are executed.
 *
 * 		__Context Switching__
 * 	  It means saving the context of an old task and retrieving the context of the next task which needs to be scheduled on the processor.
 * 	  Task A -> Task B -> Task A    each have a time slot and are "context switched" based on the scheduler every SysTick Timer Exception.
 *
 *		__Scenario of PendSV in context switching__
 *	1) Task A running in Thread Mode and a SysTick Timeout happens.
 *	2) Scheduler runs which is also called a SysTick Handler. The Scheduler pends the PendSV (doesn't carry out context switch).
 *	3) When SysTick Handler exits, and there are no interrupts in the system, then PendSV handler runs (lowest possible priority).
 *	   Context Switch happens in PendSV handler. Context of Task A will be saved in the stack, and the context of the next task will
 *	   be retrieved from the stack. The PendSV handler also makes the next task run on the CPU.
 *	4) Task B executes.
 *	5) Interrupt occurs during Task B -> ISR executes. ISR executes in Handler mode so it will always preempt Thread mode code.
 *	6) During ISR execution, suddenly a SysTick Timeout happens. (SysTick will preempt the ISR only if its priority is higher than
 *	   the currently active ISR. Let's assume SysTick interrupt is highest priority). This process is called interrupt nesting.
 *	7) Once again, scheduler runs and the scheduler pends the PendSV and exits.
 *	8) After the scheduler exits, the ISR continues where it left off. ISR finishes and exits. (You can consider this an interrupt noisy environment)
 *	9) After the ISR exits, the PendSV handler will be invoked and it will carry out the context switch.
 *	10) Task A returns.
 *
 *	Typically you wouldn't do context switching in the SysTick handler, you would just pend the PendSV. You do context switching in the PendSV handler.
 *
 *	An unfinished ISR returning to thread mode, keeping unfinished handler code (ISR) will cause a Usage Fault. This could happen if you do context switching
 *	from the SysTick handler (Scheduler) and it attempts to transition to thread mode after preempting an ISR.
 *
 *	Correct way:  Task -> SysTick Handler (Scheduler) *pend PendSV* -> PendSV Handler *context switch* -> Task
 *		or 		  Task -> ISR -> SysTick Handler (Scheduler) *pend PendSV* -> ISR -> PendSV Handler *context switch* -> Task
 *
 *  Priority Scheme (low to high): Task (Thread mode), PendSV handler (Handler mode), ISR (Handler mode), SysTick Handler (assume highest) (Handler mode)
 *
 *  		__PendSV other use cases__
 *  . Offloading Interrupt processing
 *  . If a higher priority handler is doing time-consuming work, then the other lower priority interrupts will suffer, and systems responsiveness may reduce.
 *    This can be solved using a combination of ISR and PendSV handler.
 *
 *    		__Offloading Interrupt Processing Using PendSV__
 *  Interrupts may be serviced in 2 halves.
 *  1) The first half is the time critical part that needs to be executed as a part of ISR.
 *  2) The second half is called 'bottom half', is basically delayed execution where rest of the time-consuming work will be done.
 *
 *  So, PendSV can be used in these cases, to handle the second half execution by triggering it in the first half.
 *
 *  		__Scenario of using PendSV in offloading interrupt processing__
 *  1) Task A running
 *  2) Interrupt triggers (IRQ #0) and ISR of IRQ #0 (ISR 0) is entered which is supposed to do time-consuming work which could delay
 *     other lower priority interrupts (bad). Instead, it does the first half of processing (time critical portion). It then pends the
 *     PendSV and exits.
 *  3) When there are no other active interrupts, or active ISR's executing in the system, the PendSV handler will run. The PendSV handler
 *     can carry out the non-time critical (time consuming) bottom half of processing. This makes the ISR very short.
 *  4) If another interrupt occurs (IRQ #1) during the bottom half of processing, it can preempt this PendSV handler. PendSV is always configured for
 *     the lowest priority.
 *  5) ISR 1 will now run (no longer delayed by the time consuming bottom-half processing of ISR 0).
 *  6) When ISR 1 finishes, PendSV (PendSV Handler) resumes bottom half processing.
 *  7) When PendSV finishes, it can return to the Thread mode code of Task A.
 *
 */

#include <stdio.h>
#include <stdint.h>

void SVC_Handler_c(uint32_t*);

int32_t add_numbers(int32_t x, int32_t y)
{
	int32_t res;
	__asm volatile("SVC #0x36");
	__asm volatile("MOV %0, R0": "=r"(res) ::);
	return res;
}

int32_t sub_numbers(int32_t x, int32_t y)
{
	int32_t res;
	__asm volatile("SVC #0x37");
	__asm volatile("MOV %0, R0": "=r"(res) ::);
	return res;
}

int32_t mul_numbers(int32_t x, int32_t y)
{
	int32_t res;
	__asm volatile("SVC #0x38");
	__asm volatile("MOV %0, R0": "=r"(res) ::);
	return res;
}

int32_t div_numbers(int32_t x, int32_t y)
{
	int32_t res;
	__asm volatile("SVC #0x39");
	__asm volatile("MOV %0, R0": "=r"(res) ::);
	return res;
}

int main(void)
{
	int32_t res;

	res = add_numbers(40,-90);
	printf("Add result = %ld\n", res);

	res = sub_numbers(25,150);
	printf("Subtraction result = %ld\n", res);

	res = mul_numbers(374,890);
	printf("Multiply result = %ld\n", res);

	res = div_numbers(67,-3);
	printf("Division result = %ld\n", res);

	for(;;);
}

__attribute__((naked)) void SVC_Handler(void)
{
	//1. Get the value of the MSP
	__asm("MRS R0, MSP"); //get MSP value without prologue/epilogue instructions modifying it
	__asm("B SVC_Handler_c"); //branch to c function
}

//Handler Mode code
void SVC_Handler_c(uint32_t *pBaseOfStackFrame)
{
	printf("in SVC handler\n");

	int32_t arg0, arg1, res;

	//__asm("MRS R0, MSP"); //executing this while in a c function can give a corrupted MSP value because of prologue/epilogue instructions included with a C function. (use naked func)
	uint8_t *pReturn_addr = (uint8_t*)pBaseOfStackFrame[6];

	//2. decrement the return address by 2 to point to opcode of the SVC instruction in the program memory
	pReturn_addr -= 2;

	//3. Extract the SVC number
	uint8_t svc_number = *pReturn_addr;
	printf("SVC number is : %d\n", svc_number);

	//4. Increment the SVC number by 4 and return it to thread mode
	svc_number += 4;

	arg0 = pBaseOfStackFrame[0];
	arg1 = pBaseOfStackFrame[1];

	switch(svc_number)
	{
		case 36:
			res = arg0 + arg1;
			break;
		case 37:
			res = arg0 - arg1;
			break;
		case 38:
			res = arg0 * arg1;
			break;
		case 39:
			res = arg0 / arg1;
			break;
		default:
			printf("Invalid SVC code");
	}

	pBaseOfStackFrame[0] = res;
}
