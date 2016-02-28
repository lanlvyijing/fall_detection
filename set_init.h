
#ifndef set_init_h
#define set_init_h

#include <inttypes.h>
#include "LDateTime.h"

class set_init 
{
  private:
    static uint8_t check_flag;
    static uint8_t check_wrong;
    static datetimeInfo rel_time;
  public:
    uint8_t set_init_check();
    uint8_t check_any_wrong(uint8_t check_wrong);
    void led_blink(void);
};


#endif


