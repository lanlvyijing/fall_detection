#ifndef bat_monitor_h
#define bat_monitor_h

#include <inttypes.h>

class bat_monitor
{
  private:
    static char buff[256];
    static char bat_value;
    static char charge_value;
  public:

    char bat_check();
    char charge_check();
};

#endif


