#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdint.h>

#define DEV_WRITE(addr, val) (*((volatile uint32_t *)(addr)) = val)
#define DEV_READ(addr, val) (*((volatile uint32_t *)(addr)))

#define read_csr(reg) ({ unsigned long __tmp; \
  asm volatile ("csrr %0, " #reg : "=r"(__tmp)); \
  __tmp; })

#define write_csr(reg, val) ({ \
  if (__builtin_constant_p(val) && (unsigned long)(val) < 32) \
    asm volatile ("csrw " #reg ", %0" :: "i"(val)); \
  else \
    asm volatile ("csrw " #reg ", %0" :: "r"(val)); })


uint64_t get_cycle_value();


typedef void (*callback)(void);

uint64_t timer_read(void);
void timecmp_update(uint64_t new_time);
void timer_enable(uint64_t time_base, callback timer_cb);
void timer_disable(void);



/**
 * Immediately halts the simulation
 */
void sim_halt();

#endif