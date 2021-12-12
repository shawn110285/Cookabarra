


#include "../common/util.h"
#include "../common/xprintf.h"


void  timer_callback(void)
{
  uint64_t curr_time = timer_read();
  uint32_t curr_time_high = curr_time >> 32;
  uint32_t curr_time_low = curr_time &0xFFFFFFFF;
  xprintf("Timer interrupt!, high =%D, low=%D \n", curr_time_high, curr_time_low);    
}

int main(int argc, char **argv) 
{

  xprintf("Hello simple system\n");
  put_char('\n');
  put_char('\n');

  // Enable periodic timer interrupt
  // (the actual timebase is a bit meaningless in simulation)
  timer_enable(200000, timer_callback);
  put_str("Enabled the timer\n");

/*
  uint64_t last_elapsed_time = get_elapsed_time();

  while (last_elapsed_time <= 4) 
  {
    uint64_t cur_time = get_elapsed_time();
    if (cur_time != last_elapsed_time) 
    {
      last_elapsed_time = cur_time;

      if (last_elapsed_time & 1) 
      {
        put_str("Tick!\n");
      } 
      else 
      {
        put_str("Tock!\n");
      }
    }
    asm volatile("wfi");
  }

*/
  while(1);
  
  return 0;
}
