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
 * Relationship between priority value & priority
 *
 *   ADC : priority 5    -> NVIC <-> CPU
 * Timer : priority 4    -> NVIC <-> CPU
 *
 * . In this case, Timer peripheral priority value is lower than the priority value of ADC;
 *   hence TIMER interrupt is more URGENT than ADC interrupt.
 * . So, we say TIMER priority is HIGHER than ADC priority.
 * . If both interrupt hits the NVIC at the same time, NVIC allows TIMER interrupt first, so,
 *   TIMER ISR will be executed first by the processor.
 *
 *   For ARM Cortex MX: lower priority value = higher priority (urgency)
 *
 *	 STM32F4x MCU has 16 different priority values.
 *	 Interrupt Priority register is part of NVIC register set.
 *	 The NVIC_IPR[0:59] registers provide 8 bit priority field for each interrupt and each register
 *	 holds 4 priority fields. These registers are byte-accessible.
 *
 *	 Bits      Name                       Function
 *	 [31:24]   Priority, byte offset 3    Each implementation-defined priority field can hold a priority value, 0-255.
 *	 [23:16]   Priority, byte offset 2    The lower the value, the greater the priority of the interrupt. Register priority
 *	 [15:8]    Priority, byte offset 1    value fields are 8 bits wide, and non-implemented low-order bits read as zero
 *	 [7:0]     Priority, byte offset 0    and ignore writes.
 *
 *	                                            Real implementation for STM32F4xx (vendor dependent).
 *	Interrupt Priority Register implementation  Bit 7-4: implemented (16 priority levels)
 *	                                            Bit 3-0: not implemented (write has no effect)
 *
 *          31       24 23      16 15       8 7       0
 *   IPR59: | PRI_239  | PRI_238  | PRI_237  | PRI_236 |
 *    ...
 *	 IPRn:  | PRI_4n+3 | PRI_4n+2 | PRI_4n+1 | PRI_4n  |
 *    ...
 *	 IPR0:  |  PRI_3   |  PRI_2   |  PRI_1   |  PRI_0  |
 *	        <--8bits--><--8bits--><--8bits--><--8bits-->
 *	           IRQ3       IRQ2       IRQ1       IRQ0
 *
 *	 Priority levels (16): 0x00 (highest), 0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70, 0x80, 0x90
 *	                       0xa0, 0xb0, 0xc0, 0xd0, 0xe0, 0xf0 (lowest)
 *
 *	 To change priority for exceptions, look for the SCB (system control block) registers
 *	 instead of the NVIC registers (SHPR1, SHPR2, SHPR3 -- System Handler Priority register 1-3)
 *
 *	 	__Priority Grouping__
 *
 *	 Pre-Empt Priority: When the processor is running interrupt handler, and another interrupt appears, then the pre-empty priority values will be compared,
 *	                    and interrupt with higher pre-empt priority (less in value) will be allowed to run.
 *
 *	 Sub Priority:      This value is used only when two interrupts with the same pre-empt priority values occur at the same time. In this case, the interrupt
 *	                    with higher sub-priority (less in value) will be handled first.
 *
 *	 Priority Group       Pre-empt priority field      sub-priority field
 *	 0 (default)		  Bit[7:1]                     Bit[0]
 *	 1 					  Bit[7:2]                     Bit[1:0]
 *	 2 					  Bit[7:3]                     Bit[2:0]
 *	 3 					  Bit[7:4]                     Bit[3:0]
 *	 4 					  Bit[7:5]                     Bit[4:0]
 *	 5 					  Bit[7:6]                     Bit[5:0]
 *	 6 					  Bit[7:7]                     Bit[6:0]
 *	 7 					  None                     	   Bit[7:0]
 *
 *	 			Interrupt Priority Register (STM32Fxx)
 *   [                       P                       ][  S  ]
 *	 [Bit 7][Bit 6][Bit 5][Bit 4][Bit 3][Bit 2][Bit 1][Bit 0]
 *	 [       Implemented        ][      Not implemented     ]
 *
 *	To change the Priority Grouping, modify the "Application Interrupt and Reset Control Register" (PRIGROUP)
 *
 *	Case 1:
 *	When the priority group = 0,
 *	As per the table we have,
 *	pre-empt priority width = 7 bits (128 programmable priority levels)
 *	But only 3 bits are implemented, so, 8 programmable priority levels
 *	sub-priority width = 1 (2 programmable sub-priority levels)
 *	Bit 0 is not implemented so no sub-priority levels
 *
 *	Case2:
 *	when the priority group = 5,
 *	pre-empt priority width = 2 bits (4 programmable priority levels)
 *	sub-priority width = 6 (64 programmable sub priority levels)
 *	since only 1 bit is implemented, only 2 programmable sub-priority levels
 *
 *	Q. What if two interrupts of the same pre-empty priority and sub priority hit the processor at the same time?
 *	Interrupt with the lowest IRQ number will be allowed first.
 *
 *	Exercise:
 *
 *	Generate the below peripheral interrupts using NVIC interrupt pending register and observe the execution of ISRs
 *	when priorities are the same and different.
 *	TIM2 global interrupt
 *	I2C1 event interrupt
 */

#include<stdio.h>
#include<stdint.h>

#define IRQNO_TIMER2	28
#define IRQNO_I2C1		31

uint32_t *pNVIC_IPRBase = (uint32_t*)0xE000E400;
uint32_t *pNVIC_ISERBase = (uint32_t*)0xE000E100;
uint32_t *pNVIC_ISPRBase = (uint32_t*)0xE000E200;

void configure_priority_for_irqs(uint8_t irq_no, uint8_t priority_value)
{
	//1. find out iprx
	uint8_t iprx = irq_no / 4;
	uint32_t *ipr = pNVIC_IPRBase + iprx;

	//2. position in iprx
	uint8_t pos = (irq_no % 4) * 8;

	//3. configure the priority
	*ipr &= ~(0xFF << pos); 	     //clear
	*ipr |= (priority_value << pos); //set
}

int main(void)
{
	//1. Lets configure the priority for the peripherals
	configure_priority_for_irqs(IRQNO_TIMER2, 0x80);
	configure_priority_for_irqs(IRQNO_I2C1, 0x80); //0x70

	//2. Set the interrupt pending bit in the NVIC PR
	*pNVIC_ISPRBase |= (1 << IRQNO_TIMER2);

	//3. Enable the IRQs in NVIC ISER
	*pNVIC_ISERBase |= (1 << IRQNO_I2C1);
	*pNVIC_ISERBase |= (1 << IRQNO_TIMER2);

}

//isrs

void TIM2_IRQHandler(void)
{
	printf("[TIM2_IRQHandler]\n");
	*pNVIC_ISPRBase |= (1 << IRQNO_I2C1); //interrupt nesting: if priority of nested interrupt is higher and preempts the current interrupt
	while(1);
}

void I2C1_EV_IRQHandler(void)
{
	printf("[I2C1_EV_IRQHandler]\n");
}

/*
 *         __Pending Interrupt Behavior__
 *
 *   Case 1: Single Pended Interrupt
 *   	. Processor is operating in thread mode & interrupt request occurs (interrupt assertion - low to high transition) (Thread)
 *   		. Pending bit is set (Interrupt pending status)
 *   			. Processor accepts the interrupt and changes the mode to Handler Mode (stacking and vector fetch)
 *   				. When the processor is executing the ISR, it sets the responding bit in the Interrupt Active Status register to 1
 *   				  to indicate that the processor is busy servicing the interrupt, and it clears the pending bit inside the ISR. (Interrupt Handler)
 *   					. When the processor is done with the ISR, it exits (exception return) and the Interrupt Active Status bit is cleared
 *   					  meaning that the processor is no longer servicing interrupts.
 *   						. (Unstacking) begins and processor resumes to (Thread) mode.
 *
 *   Case 2: Double Pended Interrupt
 *		. Same steps as before when the first interrupt is asserted: Interrupt issued, ISR entered (IPS bit set), Processor goes from Thread mode to Handler mode,
 *		  Interrupt active status bit is set, interrupt pending status (IPS) bit is cleared...  THEN
 *			. A new interrupt is issued
 *				. Interrupt pending bit set again
 *					. Processor completes the ISR of the first interrupt and returns to Thread mode, Interrupt Active Status bit is cleared
 *						. Since the Interrupt Pending Status bit is still asserted/active/set, the processor immediately reenters Handler mode
 *							. Interrupt Pending Status bit is cleared once the processor starts executing the ISR and the Interrupt Active Status bit is once again set/asserted/active.
 *
 */
