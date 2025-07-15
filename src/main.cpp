#include <stdint.h>

typedef uint32_t u32;
typedef uint8_t u8;
#define regmap(addr) (*(volatile u32 *)(addr))

struct gpio {
  u8 r0[0x800];
  u32 pwren, rstctl;
};
#define gpioa (*(volatile gpio *)(0x400a0000))
#define pincm ((volatile u32 *)(0x400a0000))

#define SYSCTL regmap(0x400af000)
#define GPIOA_DOE regmap(0x400a12c0)
#define GPIOA_OUT regmap(0x400a1280)
#define GPIOA_TGL regmap(0x400a12b0)

extern "C" void reset() {

  gpioa.rstctl = 0xb1000003;
  gpioa.rstctl = 0xb1000003;
  gpioa.pwren = 0x26000001;

  for (u32 i = 251; i--; pincm[i] = 0x81)
    ;

  GPIOA_OUT = 0xffffffff;
  GPIOA_DOE = 0xffffffff;
  while (1) {
  }
}

extern "C" void stack_top();
[[gnu::section(".text.ivt"), gnu::used]]
void (*const ivt[])(void) = {stack_top, reset};
