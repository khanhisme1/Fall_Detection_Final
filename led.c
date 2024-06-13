#include "MKL46Z4.h"
#include "led.h"

void LED_Init(void) {
	// Clock
	SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;		//1 << 12
	SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;		//1 << 13
	
	// GPIO alternative function
	PORTD->PCR[GREEN_LED] |= PORT_PCR_MUX(1);	// 1 << 8
	PORTE->PCR[RED_LED] |= PORT_PCR_MUX(1);	// 1 << 8
	
	// Output mode
	PTD->PDDR |= (1 << GREEN_LED);		// 1 << 5
	PTE->PDDR |= (1 << RED_LED);		// 1 << 29
	
	//Turn off LEDs
	PTE->PSOR |= (1 << RED_LED);
	PTD->PSOR |= (1 << GREEN_LED);
}
