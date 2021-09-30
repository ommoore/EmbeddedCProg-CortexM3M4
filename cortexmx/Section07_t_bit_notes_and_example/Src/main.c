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

#include<stdio.h>
#include<stdint.h>

/* This function executes in THREAD MODE of the processor */
void generate_interrupt()
{
	//these control register addresses cannot be changed in unprivileged mode.
	//any attempt to change contents in unpriv access level will result in processor fault exception
	uint32_t *pSTIR  = (uint32_t*)0xE000EF00;
	uint32_t *pISER0 = (uint32_t*)0xE000E100;

	//enable IRQ3 interrupt
	*pISER0 |= ( 1 << 3);   //ISER = interrupt set-enable register

	//generate an interrupt from software for IRQ3
	*pSTIR = (3 & 0x1FF); //STIR = Software Trigger Interrupt Register

}

void change_access_level_unpriv(void)
{
	//read
	__asm volatile ("MRS R0,CONTROL");
	//modify
	__asm volatile ("ORR R0,R0,#0x01");
	//write
	__asm volatile ("MSR CONTROL,R0");
}


int main(void)
{
	/*
	 * 1) Various ARM processors support ARM-Thumb inter-working, ie. the ability to switch between ARM and Thumb states.
	 *
	 * 2) The processor must be in ARM state to execute instructions from ARM ISA. The processor must be in Thumb state to execute instructions
	 *    of Thumb ISA.
	 *
	 * 3) If "T" bit of the EPSR is set(1), processor thinks the next instruction it is about to execute is from Thumb ISA.
	 *
	 * 4) If "T" bit of the EPSR is reset(0), processor thinks that the next instruction which it is about to execute is from ARM ISA.
	 *
	 * 5) The Cortex-MX processor does not support the "ARM" state.
	 *    Hence, the value of "T" bit must always be 1.
	 *    Failing to maintain this is illegal and will result in the "Usage fault" Exception
	 *
	 * 6) The LSB (bit 0) of the program counter (PC) is linked to this "T" bit.
	 *    When you load a value or an address in to PC the Bit[0] of the value is loaded into "T" bit.
	 *
	 * 7) Hence, any address you place in the PC must have its 0th bit as 1.
	 *    This is usually taken care of by the compiler and programmers don't need to worry most of the time.
	 *
	 * 8) This is the reason why you see all vector addresses are incremented by 1 in the vector table.
	 */

	printf("In thread mode : before interrupt\n");

	void (*func_ptr)(void);

	func_ptr = change_access_level_unpriv; //initialized pointer to the address of this function

	func_ptr(); //call function by dereferencing function pointer and jump to function

	//compiler changes address by incrementing 1 (to set T-bit to 1)
	//true address is even, not odd.

	printf("In thread mode : after interrupt\n");

	for(;;);
}

/* This function(ISR) executes in HANDLER MODE of the processor */
void RTC_WKUP_IRQHandler(void)
{
	printf("In handler mode : ISR\n");
}

/* fault handler catches exception */
void HardFault_Handler(void)
{
	printf("Hard fault detected\n");
	while(1);
}
