
extern "C" {
  #include <stdlib.h>
  #include <string.h>
  #include <inttypes.h>
  
}

#include "real_time.h"
#include "Arduino.h"


// Initialize Class Variables 

datetimeInfo real_time::rel_time;
datetimeInfo real_time::preset_time = {2016,1,18,14,25,40};
int real_time::set_time_flag  = 0;
// Public Methods //////////////////////////////////////////////////////////////

datetimeInfo real_time::get_time(void)
{
   Serial.println("get_time(void)");
   LDateTime.getTime(&rel_time);
   
   Serial.println("the year is :"+String(rel_time.year));
   Serial.println("the month is :"+String(rel_time.mon));
   Serial.println("the day is :"+String(rel_time.day));
   Serial.println("the hour is :"+String(rel_time.hour));
   Serial.println("the min is :"+String(rel_time.min));
   Serial.println("the sec is :"+String(rel_time.sec));
  
   return rel_time;
}

int real_time::set_time(datetimeInfo preset)
{
   Serial.println("set_time(void)");
   if(LDateTime.setTime(&preset))
   {
     set_time_flag=1;
   }
   else
   {
      set_time_flag=0;
   }
   return set_time_flag;
}




