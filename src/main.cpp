#include <stdint.h>

typedef uint32_t u32;
typedef uint8_t u8;

#define regmap(addr) ((volatile u32 *)(addr))
#define sfr(addr) (*(volatile u32 *)(addr))

#define pincm regmap(0x40428000)
#define gpioa_power regmap(0x400a0800)
#define gpioa sfr(0x400a1280)
#define gpioa_en sfr(0x400a12c0)

extern "C" void reset() {
  gpioa_power[1] = 0xb1000003;
  gpioa_power[0] = 0x26000001;
  pincm[36] = 0x81;

  gpioa_en = 0xffffffff;
  gpioa = 0xffffffff;

  while (1) {
  }
}

extern "C" void stack_top();
[[gnu::section(".text.ivt"), gnu::used]]
void (*const ivt[])(void) = {stack_top, reset};
