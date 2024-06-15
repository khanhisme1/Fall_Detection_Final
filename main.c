#include "MKL46Z4.h"
#include "accel.h"
#include "delay.h"
#include "led.h"
#include "lcd.h"
#include "i2c.h"
#include "switch.h"

volatile uint8_t state;
volatile uint8_t interrupt;

int main(void)
{
	LED_Init();
	Switch_Init();
	Systick_Init();
	LCD_Init();
	I2C_Init();
	INT1_Init();
	MMA8451_Init();
	
	while (1)
	{
		if (state == NORMAL) {
			AccelData raw_data = MMA8451_Read();
			float accel = Get_Accel_Value(raw_data);
			if (accel > 4.5 && interrupt == FALL)
			{
				state = FALL;
				LCD_WriteChar('1', 0);
				while(state == FALL);
			}
		}
	}
}
