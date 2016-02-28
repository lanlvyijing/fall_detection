
extern "C" {
  #include <stdlib.h>
  #include <string.h>
  #include <inttypes.h>
  
}

#include "bat_monitor.h"
#include "LBattery.h"
#include "Arduino.h"


// Initialize Class Variables 
char bat_monitor::buff[256];
char bat_monitor::bat_value = 0;
char bat_monitor::charge_value = 0;

// Public Methods //////////////////////////////////////////////////////////////

char bat_monitor::bat_check() 
{
  // put your main code here, to run repeatedly:
  bat_value =  LBattery.level();
  bat_value = 40;
  sprintf(buff,"battery level = %d", bat_value );
  Serial.println(buff);
  delay(100); 
  return bat_value;
}

char bat_monitor::charge_check() 
{
  charge_value = LBattery.isCharging();
  sprintf(buff,"is charging = %d", charge_value );
  Serial.println(buff);
  delay(100);
  return charge_value; 
}




