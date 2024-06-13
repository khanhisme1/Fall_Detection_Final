#include "MKL46Z4.h"
#include "delay.h"
#include "switch.h"
#include "led.h"

volatile uint32_t msTick;
volatile uint32_t LED_ACTIVE_TICK;
volatile uint32_t LED_FALL_TICK;

void Systick_Init(void) {
	// Enable Systick Interupt
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;	// 1 << 1
	
	// Choose clock source for Systick (processor clock)
	SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;	// 1 << 2
	
	// LOAD the count value (start)
	SysTick->LOAD = SystemCoreClock/1000 - 1;	// SystemCoreClock is approximately 21MHz by default
	
	// Set priority
	NVIC_SetPriority(SysTick_IRQn, 2);	//-1
	
	LED_ACTIVE_TICK = 500;
	LED_FALL_TICK = 0;
}

void Systick_Shutdown(void) {
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}

void Systick_Enable(void) {
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
}

void SysTick_Handler(void) {
	msTick++;
	// 1 Hz
	if (LED_ACTIVE_TICK == 500) {
		PTD->PTOR |= 1 << GREEN_LED;
		LED_ACTIVE_TICK = 0;
	}
	LED_ACTIVE_TICK++;
	if (state == FALL) {
		//2 Hz
		if (LED_FALL_TICK == 250) {
			PTE->PTOR |= 1 << RED_LED;
			LED_FALL_TICK = 0;
		}
		LED_FALL_TICK++;
	}
}

void delay(uint32_t TICK) {
	while (msTick < TICK);
	msTick = 0;
}
