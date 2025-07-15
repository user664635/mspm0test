#include <stdint.h>

typedef uint32_t u32;
typedef uint8_t u8;

#define sfr(addr) ((volatile u32 *)(addr))

#define pincm sfr(0x40428000)
#define gpioa_power sfr(0x400a0800)
#define gpioa sfr(0x400a1280)

#define syst sfr(0xe000e010)

void reset() {
  gpioa_power[1] = 0xb1000003;
  gpioa_power[0] = 0x26000001;
  pincm[36] = 0x81;

  gpioa[16] = 0xffffffff;
  gpioa[0] = 0xffffffff;

  syst[1] = 16000000;
  syst[0] = 7;

  while (1) {
  }
}
void systick() { gpioa[12] = 0xffffffff; }

void stack_top();
[[gnu::section(".text.ivt"), gnu::used]]
void (*const ivt[])(void) = {stack_top, reset, [15] = systick};
