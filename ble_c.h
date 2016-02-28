#ifndef ble_c_h
#define ble_c_h

#include <inttypes.h>
#include "LGATT.h"
#include "LGATTClient.h"
#include "LGATTServer.h"
class ble_c 
{
  private:
    static uint8_t bt_work_flag;
    static LGATTUUID uuidService; 
    static LGATTClient c;
  public:
    uint8_t bt_init();
    boolean descInProcessing(const LGATTUUID &serviceUUID,boolean isPrimary,const LGATTUUID&characteristicUUID);
    void charInProcessing(const LGATTUUID &serviceUUID,boolean isPrimary,const LGATTUUID& characteristicUUID,LGATT_CHAR_PROPERTIES properties);
    void serviceInProcessing(const LGATTUUID &inputServiceUUID);
    boolean hasFoundSpencifiedBleDev(const LGATTDeviceInfo &info);
    void scan_bt_dev(void);
};


#endif


