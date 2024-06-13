#include "MKL46Z4.h"
#include "i2c.h"

void I2C_Init(void) {
    //  Enable clock
    SIM->SCGC4 |= SIM_SCGC4_I2C0_MASK;	// 1 << 5
    SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;	// 1 << 13
    
    //  PTE24 (SCL) and PTE25 (SDA) - Alternative function 5
    PORTE->PCR[24] |= PORT_PCR_MUX(5);	//  1 << 8 | 1 << 10 
    PORTE->PCR[25] |= PORT_PCR_MUX(5);
	
	//  Pull up resistor
	PORTE->PCR[24] |= PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;	// 1 << 1 | 1 << 0 
    PORTE->PCR[25] |= PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;
    
    //  Enable I2C0
    I2C0->C1 |= I2C_C1_IICEN_MASK;	// 1 << 7
}

void I2C_WriteRegister(uint8_t device_address, uint8_t register_address, uint8_t data) {
	//  Set to transmit mode
    I2C0->C1 |= I2C_C1_TX_MASK;		// 1 << 4
	
	//  Send start signal
    I2C0->C1 |= I2C_C1_MST_MASK;	// 1 << 5
	
	//  Send device address with write bit
    I2C0->D = device_address << 1;	// 0x1C
    while (!(I2C0->S & I2C_S_IICIF_MASK));	// 1 << 1
    I2C0->S |= I2C_S_IICIF_MASK;
	
    //  Send register address
    I2C0->D = register_address;
    while (!(I2C0->S & I2C_S_IICIF_MASK));
    I2C0->S |= I2C_S_IICIF_MASK;
	
    //  Send data
    I2C0->D = data;
    while (!(I2C0->S & I2C_S_IICIF_MASK));
    I2C0->S |= I2C_S_IICIF_MASK;
	
	//  Send stop signal
    I2C0->C1 &= ~I2C_C1_MST_MASK;
}

uint8_t I2C_ReadRegister(uint8_t device_address, uint8_t register_address) {
    uint8_t result;
    //  Set to transmit mode
    I2C0->C1 |= I2C_C1_TX_MASK;
	
	//  Send START signal
    I2C0->C1 |= I2C_C1_MST_MASK;
	
    //  Send device address with write bit
    I2C0->D = device_address << 1;
    while (!(I2C0->S & I2C_S_IICIF_MASK));
    I2C0->S |= I2C_S_IICIF_MASK;
	
    //  Send register address
    I2C0->D = register_address;
    while (!(I2C0->S & I2C_S_IICIF_MASK));
    I2C0->S |= I2C_S_IICIF_MASK;
    
    //  Repeated start to change to read mode
    I2C0->C1 |= I2C_C1_RSTA_MASK;	// 1 << 2
	
    //  Send device address with read bit
    I2C0->D = (device_address << 1) | 0x01;
    while (!(I2C0->S & I2C_S_IICIF_MASK));
    I2C0->S |= I2C_S_IICIF_MASK;
	
    //  Recieve mode
    I2C0->C1 &= ~I2C_C1_TX_MASK;	// 1 << 4
	
	//  Send NACK
    I2C0->C1 |= I2C_C1_TXAK_MASK;	// 1 << 8
	
    //  Fake read
    result = I2C0->D;
    while (!(I2C0->S & I2C_S_IICIF_MASK));
    I2C0->S |= I2C_S_IICIF_MASK;
	
    //  Read data
    result = I2C0->D;
	while (!(I2C0->S & I2C_S_IICIF_MASK));
    I2C0->S |= I2C_S_IICIF_MASK;
	
	//  Send stop signal
    I2C0->C1 &= ~I2C_C1_MST_MASK;
    
    return result;
}
