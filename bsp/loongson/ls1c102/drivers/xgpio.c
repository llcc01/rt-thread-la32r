#include <rtthread.h>

#define XGPIO_BASE 0xbf020000

#define XGPIO_REG(offset) (*(volatile uint32_t *)(XGPIO_BASE + offset))

#define XGPIO_REG_DATA XGPIO_REG(0)
#define XGPIO_REG_TRI XGPIO_REG(4)

void xgpio_init() {
    XGPIO_REG_TRI = 0;
}

void xgpio_set(uint32_t data) {
    XGPIO_REG_DATA = data;
}
