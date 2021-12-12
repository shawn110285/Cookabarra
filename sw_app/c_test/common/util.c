
#include <stdint.h>
#include "util.h"
#include "xprintf.h"
#include "soc_reg.h"



uint64_t get_cycle_value()
{
    uint64_t cycle;

    cycle = read_csr(cycle);
    cycle += (uint64_t)(read_csr(cycleh)) << 32;

    return cycle;
}

unsigned int get_mepc() 
{
  uint32_t result;
  __asm__ volatile("csrr %0, mepc;" : "=r"(result));
  return result;
}

unsigned int get_mcause() 
{
  uint32_t result;
  __asm__ volatile("csrr %0, mcause;" : "=r"(result));
  return result;
}

unsigned int get_mtval() 
{
  uint32_t result;
  __asm__ volatile("csrr %0, mtval;" : "=r"(result));
  return result;
}

void sim_halt() 
{ 
  DEV_WRITE(SIM_CTRL_BASE + SIM_CTRL_CTRL, 1); 
}


void simple_exc_handler(void) 
{
  put_str("EXCEPTION!!!\n");
  put_str("============\n");
  put_str("MEPC:   0x");
  put_hex(get_mepc());
  put_str("\nMCAUSE: 0x");
  put_hex(get_mcause());
  put_str("\nMTVAL:  0x");
  put_hex(get_mtval());
  put_char('\n');
  sim_halt();

  while(1);
}


uint64_t timer_read(void) 
{
  uint32_t current_timeh;
  uint32_t current_time;
  // check if time overflowed while reading and try again
  do 
  {
    current_timeh = DEV_READ(TIMER_BASE + TIMER_MTIMEH, 0);
    current_time = DEV_READ(TIMER_BASE + TIMER_MTIME, 0);
  } while (current_timeh != DEV_READ(TIMER_BASE + TIMER_MTIMEH, 0));
  
  uint64_t final_time = ((uint64_t)current_timeh << 32) | current_time;
  return final_time;
}


void timecmp_update(uint64_t new_time) 
{
  DEV_WRITE(TIMER_BASE + TIMER_MTIMECMP, -1);
  DEV_WRITE(TIMER_BASE + TIMER_MTIMECMPH, new_time >> 32);
  DEV_WRITE(TIMER_BASE + TIMER_MTIMECMP, new_time);
}

inline static void increment_timecmp(uint64_t time_base) 
{
  uint64_t current_time = timer_read();
  current_time += time_base;
  timecmp_update(current_time);
}



uint64_t time_increment;
callback sgfTimerCallback;

void timer_enable(uint64_t time_base, callback timer_cb) 
{
  time_increment = time_base;
  sgfTimerCallback = timer_cb;
  // Set timer values
  increment_timecmp(time_base);
  // enable timer interrupt
  asm volatile("csrs  mie, %0\n" : : "r"(0x80));
  // enable global interrupt
  asm volatile("csrs  mstatus, %0\n" : : "r"(0x8));
}


void simple_timer_handler(void) __attribute__((interrupt));
void simple_timer_handler(void) 
{
  increment_timecmp(time_increment);
  sgfTimerCallback();
}

void timer_disable(void) { asm volatile("csrc  mie, %0\n" : : "r"(0x80)); }