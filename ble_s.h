#ifndef ble_s_h
#define ble_s_h

#include <inttypes.h>
#include "LGATTSUart.h"
#include "LGATTSABC.h"
#include "LGATTUUID.h"

class ble_s 
{
  private:
    static LGATTSUart uart;
    static LGATTUT abc;
    static bool ble_s_init_flag;
  public:
    bool ble_s_init();
    void user_info_get();
};


#endif


