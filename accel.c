#include "MKL46Z4.h"
#include "accel.h"
#include "delay.h"
#include "i2c.h"
#include <math.h>

void MMA8451_Init(void) {
	// Unactive MMA8451Q for configuration
    I2C_WriteRegister(MMA8451Q_ADDRESS, CTRL_REG1, 0x00);
	delay(1);
	
	//4g sensitivity
	I2C_WriteRegister(MMA8451Q_ADDRESS, XYZ_DATA_CFG, 0x01);
	delay(1);
	
	//Active MMA8451Q
	I2C_WriteRegister(MMA8451Q_ADDRESS, CTRL_REG1, 0x01);
	delay(1);
}

//For polling check
AccelData MMA8451_Read(void) {
	// Read 6 register corresponding to 3 Axis value
    uint8_t x_msb = I2C_ReadRegister(MMA8451Q_ADDRESS, OUT_X_MSB);
	delay(1);
    uint8_t x_lsb = I2C_ReadRegister(MMA8451Q_ADDRESS, OUT_X_LSB);
	delay(1);
    uint8_t y_msb = I2C_ReadRegister(MMA8451Q_ADDRESS, OUT_Y_MSB);
	delay(1);
    uint8_t y_lsb = I2C_ReadRegister(MMA8451Q_ADDRESS, OUT_Y_LSB);
	delay(1);
    uint8_t z_msb = I2C_ReadRegister(MMA8451Q_ADDRESS, OUT_Z_MSB);
	delay(1);
    uint8_t z_lsb = I2C_ReadRegister(MMA8451Q_ADDRESS, OUT_Z_LSB);
    delay(1);
	
	// Get raw data with 14 bit
    int16_t raw_x_data = ((int16_t)(x_msb << 8 | x_lsb)) >> 2;
    int16_t raw_y_data = ((int16_t)(y_msb << 8 | y_lsb)) >> 2;
    int16_t raw_z_data = ((int16_t)(z_msb << 8 | z_lsb)) >> 2;
	
	// Real data (Converted with 4 counts/g sensivity, 4g = 8192)
	AccelData data;
	data.x = (float)raw_x_data / (1 << 13) * 4;
    data.y = (float)raw_y_data / (1 << 13) * 4;
	data.z = (float)raw_z_data / (1 << 13) * 4;
	
    return data;
}

float Get_Accel_Value(AccelData data) {
	return sqrt((data.x * data.x) + (data.y * data.y) + (data.z * data.z));
}
