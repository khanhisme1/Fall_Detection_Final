#include "MKL46Z4.h"
#include "accel.h"
#include "delay.h"
#include "led.h"
#include "lcd.h"
#include "i2c.h"
#include "switch.h"

volatile uint8_t state;

int main(void) {
	LED_Init();
	Switch_Init();
	Systick_Init();
	LCD_Init();
	I2C_Init();
	INT1_Init();
	MMA8451_Init();
	
    while (1) {

    }
}
