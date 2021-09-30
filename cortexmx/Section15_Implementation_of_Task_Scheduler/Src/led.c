
#include<stdint.h>
#include "led.h"



void delay(uint32_t count)
{
  for(uint32_t i = 0 ; i < count ; i++);
}

// RED1 = PB5, GREEN = PB10, RED2 = PB9 , YELLOW = PB8
void led_init_all(void)
{
	uint32_t *pRccAhb1enr = (uint32_t*)0x40023830;
	uint32_t *pGpiobModeReg = (uint32_t*)0x40020400;

	/* 											  -- AHB1PERIPH_BASE --
	 *  GPIOB_BASE = AHB1PERIPH_BASE + 0x0400 = (PERIPH_BASE + 0x0002_0000) + 0x0400 = 0x4000_0000 + 0x0002_0000 + 0x0000_0400 = 0x4002_0400
	 *
	 *	MODER   0x00
	 *	OTYPER	0x04
	 *	OSPEEDR	0x08
	 *	PUPDR	0x0C
	 *	IDR		0x10
	 *	ODR		0x14
	 *	BSRR	0x18
	 *	LCKR	0x1C
	 *	AFR[2]	0x20-0x24
	 *
	 */

	*pRccAhb1enr |= ( 1 << 1); //GPIOB enable

	*pGpiobModeReg |= ( 1 << (2 * LED_RED1));
	*pGpiobModeReg |= ( 1 << (2 * LED_GREEN));
	*pGpiobModeReg |= ( 1 << (2 * LED_RED2));
	*pGpiobModeReg |= ( 1 << (2 * LED_YELLOW));

#if 0
	//configure the outputtype
	*pGpioOpTypeReg |= ( 1 << (2 * LED_RED1));
	*pGpioOpTypeReg |= ( 1 << (2 * LED_GREEN));
	*pGpioOpTypeReg |= ( 1 << (2 * LED_RED2));
	*pGpioOpTypeReg |= ( 1 << (2 * LED_YELLOW));
#endif

    led_off(LED_RED1);
    led_off(LED_GREEN);
    led_off(LED_RED2);
    led_off(LED_YELLOW);



}

void led_on(uint8_t led_no)
{
  uint32_t *pGpiobDataReg = (uint32_t*)0x40020414;
  *pGpiobDataReg |= ( 1 << led_no);

}

void led_off(uint8_t led_no)
{
	  uint32_t *pGpiobDataReg = (uint32_t*)0x40020414;
	  *pGpiobDataReg &= ~( 1 << led_no);

}


