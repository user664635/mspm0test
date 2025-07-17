#include <stdint.h>

typedef uint32_t u32;
typedef uint8_t u8;

#define sfr(addr) ((volatile u32 *)(addr))

#define pincm sfr(0x40428000)
#define gpioa_power sfr(0x400a0800)
#define gpioa sfr(0x400a1280)
#define sysctl sfr(0x400af000)

#define syst sfr(0xe000e010)

static u32 dir;
void reset() {
  extern u32 ldata[], sdata[], edata[];
  for (u32 *l = ldata, *s = sdata, *e = edata; s < e; *s++ = *l++)
    ;
  sysctl[0x4c2] = 0;
  gpioa_power[1] = 0xb1000003;
  gpioa_power[0] = 0x26000001;
  pincm[36] = 0x81;

  gpioa[16] = -1;
  gpioa[0] = -1;

  syst[1] = -1;
  syst[0] = 7;

  while (1) {
    u32 tmp = syst[2];
    gpioa[0] = dir ^ -(tmp >> 12 < (tmp & 0xfff));
  }
}
void systick() { dir ^= -1; }

void stack_top();
[[gnu::section(".text.ivt"), gnu::used]]
void (*const ivt[])(void) = {stack_top, reset, [15] = systick};
