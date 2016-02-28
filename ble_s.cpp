extern "C" {
  #include <stdlib.h>
  #include <string.h>
  #include <inttypes.h>

}

#include "Arduino.h"

#include "ble_s.h"

#ifdef APP_LOG
#undef APP_LOG
#endif

#define APP_LOG(...) Serial.printf(__VA_ARGS__); \
    Serial.println();

LGATTSUart ble_s::uart;
LGATTUT ble_s::abc;
bool ble_s::ble_s_init_flag=0;

bool ble_s::ble_s_init(){
  // put your setup code here, to run once:
  Serial.println("[ble_s::ble_s_init] ble_s is inited");
  delay(1000);
     if (!LGATTServer.begin(2, &uart, &abc))
    {
        APP_LOG("[ble_s::ble_s_init] GATTS begin FAILED");       
    }
    else
    {
        APP_LOG("[ble_s::ble_s_init] GATTS begin");
        ble_s_init_flag = 1;
    }
    
    return ble_s_init_flag;
}

void ble_s::user_info_get(){
  // put your main code here, to run repeatedly:
  while(!leave_bt_set)
  {
      delay(1000);
      LGATTServer.handleEvents();
      if (uart.isConnected())                                //boolean isConnected(){ return _connected; };
      {
        APP_LOG("[ble_s::user_info_get] phone is connected");   
      }
   }
   leave_bt_set=0;
}

