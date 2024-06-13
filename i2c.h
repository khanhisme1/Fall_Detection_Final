#include "MKL46Z4.h"

void I2C_Init(void);
void I2C_WriteRegister(uint8_t device_address, uint8_t register_address, uint8_t data);
uint8_t I2C_ReadRegister(uint8_t device_address, uint8_t register_address);
