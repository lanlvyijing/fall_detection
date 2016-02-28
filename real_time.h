#ifndef real_time_h
#define real_time_h

#include <inttypes.h>
#include "LDateTime.h"

class real_time
{
  private:
    static datetimeInfo rel_time;
    
    static int set_time_flag;
  public:
    static datetimeInfo preset_time;
    datetimeInfo get_time(void);
    int set_time(datetimeInfo preset);
};


#endif


