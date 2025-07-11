#include <stdint.h>

typedef uint32_t u32;
#define regmap(addr) (*(volatile u32 *)(addr))

#define PINCM regmap(0x40428004)
#define SYSCTL regmap(0x400af000)
#define GPIOA_DOE regmap(0x400a12c0)
#define GPIOA_OUT regmap(0x400a1280)
#define GPIOA_TGL regmap(0x400a12b0)

extern "C" void reset() {
  PINCM = (1 << 7);

  GPIOA_DOE = 0xffffffff;
  GPIOA_OUT = 0xffffffff;
  while (1) {
  GPIOA_TGL = 0xffffffff;
  }
}

extern "C" void stack_top();
[[gnu::section(".text.ivt"), gnu::used]]
void (*const ivt[])(void) = {stack_top, reset};
