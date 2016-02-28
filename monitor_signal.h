#ifndef monitor_signal_h
#define monitor_signal_h

#include <inttypes.h>
#include "real_time.h"
#include "commondata.h"
 
class monitor_signal 
{
  private:
    static uint8_t signal_flag;
    static real_time Real_Time;
    static datetimeInfo rel_time;
  public:

    uint8_t signal_checking();

};


#endif


