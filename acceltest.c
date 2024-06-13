#include "fsl_i2c.h"
#include "fsl_port.h"
#include "fsl_gpio.h"
#include "board.h"

#define ACCEL_I2C_ADDRESS 0x1D // Ð?a ch? I2C c?a MMA8451Q

void I2C_Init(void)
{
    i2c_master_config_t masterConfig;

    /* C?u hình chân I2C */
    PORT_SetPinMux(PORTE, 24u, kPORT_MuxAlt5);
    PORT_SetPinMux(PORTE, 25u, kPORT_MuxAlt5);

    I2C_MasterGetDefaultConfig(&masterConfig);
    masterConfig.baudRate_Bps = 100000U;
    I2C_MasterInit(I2C0, &masterConfig, CLOCK_GetFreq(kCLOCK_BusClk));
}

void I2C_WriteReg(uint8_t reg, uint8_t data)
{
    i2c_master_transfer_t masterXfer;
    memset(&masterXfer, 0, sizeof(masterXfer));

    masterXfer.slaveAddress = ACCEL_I2C_ADDRESS;
    masterXfer.direction = kI2C_Write;
    masterXfer.subaddress = reg;
    masterXfer.subaddressSize = 1;
    masterXfer.data = &data;
    masterXfer.dataSize = 1;
    masterXfer.flags = kI2C_TransferDefaultFlag;

    I2C_MasterTransferBlocking(I2C0, &masterXfer);
}

uint8_t I2C_ReadReg(uint8_t reg)
{
    uint8_t data;
    i2c_master_transfer_t masterXfer;
    memset(&masterXfer, 0, sizeof(masterXfer));

    masterXfer.slaveAddress = ACCEL_I2C_ADDRESS;
    masterXfer.direction = kI2C_Read;
    masterXfer.subaddress = reg;
    masterXfer.subaddressSize = 1;
    masterXfer.data = &data;
    masterXfer.dataSize = 1;
    masterXfer.flags = kI2C_TransferDefaultFlag;

    I2C_MasterTransferBlocking(I2C0, &masterXfer);

    return data;
}
