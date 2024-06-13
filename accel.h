#include "MKL46Z4.h"

#define MMA8451Q_ADDRESS 0x1D

#define OUT_X_MSB 0x01
#define OUT_X_LSB 0x02
#define OUT_Y_MSB 0x03
#define OUT_Y_LSB 0x04
#define OUT_Z_MSB 0x05
#define OUT_Z_LSB 0x06

#define XYZ_DATA_CFG 0x0E

#define FF_MT_CFG 0x15
#define FF_MT_THS 0x17
#define FF_MT_COUNT 0x18

#define CTRL_REG1 0x2A
#define CTRL_REG4 0x2D
#define CTRL_REG5 0x2E

#define INT1_PIN 5

typedef struct {
    float x;
    float y;
    float z;
} AccelData;

void MMA8451_Init(void);
AccelData MMA8451_Read(void);
float Get_Accel_Value(AccelData data);
