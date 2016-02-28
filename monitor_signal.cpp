
extern "C" {
  #include <stdlib.h>
  #include <string.h>
  #include <inttypes.h>

}

#include "monitor_signal.h"
#include "Arduino.h"

datetimeInfo monitor_signal::rel_time;
real_time monitor_signal::Real_Time;
// Initialize Class Variables 

uint8_t monitor_signal::signal_flag = 0;


// Public Methods //////////////////////////////////////////////////////////////

uint8_t monitor_signal::signal_checking(void)
{
   Serial.println("signal_check(void)");
   rel_time=Real_Time.get_time();
   Serial.println("It's "+String(rel_time.hour)+":"+String(rel_time.min)+":"+String(rel_time.sec)+"now");
   if(medicine_take_flage==0)
   {
       if((rel_time.hour == medicine_hour)&&(rel_time.min == medicine_min))
       {
         Serial.println("It's time to take medicine");
          if(medicine_time_flage!=1)
          {
            medicine_time_flage = 1;
          }
       }
   }
   return signal_flag;
}




