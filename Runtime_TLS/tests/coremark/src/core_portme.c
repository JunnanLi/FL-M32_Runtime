#include <stdio.h>
// #include <stdlib.h>
#include "coremark.h"
#include "firmware.h"
// #include "../include/utils.h"
// #include "../include/uart.h"
// #include "../include/xprintf.h"

#if VALIDATION_RUN
	volatile ee_s32 seed1_volatile=0x3415;
	volatile ee_s32 seed2_volatile=0x3415;
	volatile ee_s32 seed3_volatile=0x66;
#endif

#if PERFORMANCE_RUN
	volatile ee_s32 seed1_volatile=0x0;
	volatile ee_s32 seed2_volatile=0x0;
	volatile ee_s32 seed3_volatile=0x66;
#endif

#if PROFILE_RUN
	volatile ee_s32 seed1_volatile=0x8;
	volatile ee_s32 seed2_volatile=0x8;
	volatile ee_s32 seed3_volatile=0x8;
#endif

volatile ee_s32 seed4_volatile=ITERATIONS;
volatile ee_s32 seed5_volatile=0;

CORE_TICKS t0_high, t0_low, t1_high, t1_low;


void start_time(void)
{
  uint32_t *addr = (uint32_t *) CSR_CYCLE_HIGH_ADDR;
  t0_high = *((volatile uint32_t*)addr);
  *addr = (uint32_t *) CSR_CYCLE_LOW_ADDR;
  t0_low = *((volatile uint32_t*)addr-1);
}

void stop_time(void)
{
  uint32_t *addr = (uint32_t *) CSR_CYCLE_HIGH_ADDR;
  t1_high = *((volatile uint32_t*)addr);
  *addr = (uint32_t *) CSR_CYCLE_LOW_ADDR;
  t1_low = *((volatile uint32_t*)addr-1);
  printf("t0: %d, %d\n\r", t0_high, t0_low);
  printf("t1: %d, %d\n\r", t1_high, t1_low);
}

CORE_TICKS get_time(void)
{
  CORE_TICKS t_div_scale;
  if(t1_low > t0_low){
    t_div_scale = (t1_low - t0_low) >> SHIFT_TO_SCALE;
    t_div_scale += (t1_high - t0_high) << (32 - SHIFT_TO_SCALE);
  }
  else {
  ee_printf("cycle_low: %08x\n",(CORE_TICKS)(t0_low - t1_low));
    t_div_scale = (t0_low - t1_low) >> SHIFT_TO_SCALE;
  ee_printf("cycle_low: %d,%d\n",t0_low,t1_low);
  ee_printf("cycle_low: %d\n",t_div_scale);
    t_div_scale += (t1_high - t0_high - 1) << (32 - SHIFT_TO_SCALE);
  ee_printf("cycle_high: %d\n",t_div_scale);
  }
  return t_div_scale;
}

secs_ret time_in_secs(CORE_TICKS ticks)
{
  // scale timer down to avoid uint64_t -> double conversion in RV32
  // int scale = 256;
  // uint32_t delta = ticks / scale;
  uint32_t delta = ticks;
  uint32_t freq = CPU_FREQ_HZ >> SHIFT_TO_SCALE;
  return delta / (double)freq;
}

void portable_init(core_portable *p, int *argc, char *argv[])
{
    // uart_init();
}
