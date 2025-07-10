#include <stdint.h>

typedef uint32_t u32;
#define regmap(addr) (*(volatile u32 *)(addr))
#define GPIOA_D15_12 regmap(0x400a120c)
#define GPIOASET regmap(0x400a1290)
#define GPIOACLR regmap(0x400a12a0)

extern "C" void reset() {
      PINCM14 = (1 << 7);

    GPIOA_DOE31_0 |= (1 << 14);
  while (1) {
    GPIOA_D15_12 = 0x01010101;
  }
}

extern "C" void stack_top();
[[gnu::section(".text.ivt"), gnu::used]]
void (*const ivt[])(void) = {stack_top, reset};
