#ifndef scene_judgment_h
#define scene_judgment_h

#include <inttypes.h>
#include "GPS_location.h"
#include "wifi_client.h"

class scene_judgment 
{
  private:
    static uint8_t which_scence;
    static uint8_t place;
    static uint8_t in_home_flag;
    static wifi_client Wifi_Client;
  public:

    uint8_t scene_check();
    uint8_t check_in_home(void);
    uint8_t BT_check_place(void);
    uint8_t check_outside(void);
};


#endif


