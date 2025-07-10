#include <stdint.h>

typedef uint32_t u32;
#define regmap(addr) (*(volatile u32 *)(addr))
#define GPIOA regmap(0x400a120c)

extern "C" void reset() {
  GPIOA = 0x01010101;
  while (1)
    ;
}
extern "C" void stack_top();
[[gnu::section(".text.ivt"), gnu::used]]
void (*const ivt[256])(void) = {stack_top, reset};
