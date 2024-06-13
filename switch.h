#include "MKL46Z4.h"

#define GPIO_MUX 8
#define IRQ_FALLING_EDGE 0x0A
#define SW1 3
#define SW2 12
#define IRQ_FLAG 24
#define SHUTDOWN 0
#define NORMAL 1
#define FALL 2
extern volatile uint8_t state;

void Switch_Init(void);
void INT1_Init(void);
