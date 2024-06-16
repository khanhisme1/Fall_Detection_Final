#include "MKL46Z4.h"
#include "delay.h"
#include "switch.h"
#include "lcd.h"
#include "led.h"
#include "accel.h"
#include "i2c.h"

void Switch_Init(void) {
	// Shutdown mode
	state = SHUTDOWN;
	
	// Clock
	SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK;	// 1 << 11
	
	// SW1 GPIO Function
	PORTC->PCR[3] |= 1 << GPIO_MUX;
	
	// SW1 Pull up
	PORTC->PCR[SW1] |= PORT_PCR_PS_MASK;	// 1 << 0
	PORTC->PCR[SW1] |= PORT_PCR_PE_MASK;	// 1 << 1
	
	// SW1 Interupt on falling edge
	PORTC->PCR[SW1] |= PORT_PCR_IRQC(IRQ_FALLING_EDGE);	// 0xA << 16
	
	// SW2 GPIO Function
	PORTC->PCR[SW2] |= 1 << GPIO_MUX;
	
	// SW2 Pull up
	PORTC->PCR[SW2] |= PORT_PCR_PS_MASK;
	PORTC->PCR[SW2] |= PORT_PCR_PE_MASK;
	
	// SW2 Interupt on falling edge
	PORTC->PCR[SW2] |= PORT_PCR_IRQC(IRQ_FALLING_EDGE);	// (0xA) << 16
	
	NVIC_ClearPendingIRQ(PORTC_PORTD_IRQn);	// 31
	NVIC_EnableIRQ(PORTC_PORTD_IRQn);
	NVIC_SetPriority(PORTC_PORTD_IRQn, 1);
	
	// Input mode
	PTC->PDDR &= (~(uint32_t)(1 << SW1));
	PTC->PDDR &= (~(uint32_t)(1 << SW2));
}

void INT1_Init(void) {
	// Clock
    SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK;
	
	// Pull up
	PORTC->PCR[INT1_PIN] |= PORT_PCR_PS_MASK;
	PORTC->PCR[INT1_PIN] |= PORT_PCR_PE_MASK;
	
	// Interupt on falling edge
    PORTC->PCR[INT1_PIN] |= PORT_PCR_IRQC(0xA);
	
	// GPIO mode
	PORTC->PCR[INT1_PIN] |= 1 << GPIO_MUX;
	
	// Input
    PTC->PDDR &= ~(1 << INT1_PIN);
}

void PORTC_PORTD_IRQHandler(void) {
	// Switch 1 pressed
	if ((PTC->PDIR & (1 << SW1)) == 0) {
		if (state != FALL) {
			// Invert the state
			if (state == SHUTDOWN ) {
				state = NORMAL;
				PTD->PCOR |= 1 << GREEN_LED;
				Systick_Enable();
				LCD_WriteChar('0', 0);
			} else if (state == NORMAL) {
				state = SHUTDOWN;
				PTD->PSOR |= 1 << GREEN_LED;
				Systick_Shutdown();
				LCD_WriteChar(':', 0);
			}
		}
		// Clear interupt flag
		PORTC->PCR[SW1] |= 1 << IRQ_FLAG;
	}
	
	// Switch 2 pressed
	if ((PTC->PDIR & (1 << SW2)) == 0) {
		if (state != SHUTDOWN) {
			state = NORMAL;
			interrupt = NORMAL;
			PTE->PSOR |= 1 << RED_LED;
			LCD_WriteChar('0', 0);
	
			//Clear Free fall interupt flag
			uint8_t temp = I2C_ReadRegister(MMA8451Q_ADDRESS, 0x16);
		}
		// Clear interupt flag
		PORTC->PCR[SW2] |= 1 << IRQ_FLAG;
	}
	
	// Interrupt from MMA8451Q
	if (PORTC->ISFR & (1 << INT1_PIN)) {
		if (state != SHUTDOWN) {
			interrupt = FALL;
		} else {
			interrupt = NORMAL;
			uint8_t temp = I2C_ReadRegister(MMA8451Q_ADDRESS, 0x16);
		}
		PORTC->ISFR |= (1 << INT1_PIN);
		PORTC->PCR[INT1_PIN] |= 1 << IRQ_FLAG;
	}
}
