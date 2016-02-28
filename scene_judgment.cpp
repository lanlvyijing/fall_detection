
extern "C" {
  #include <stdlib.h>
  #include <string.h>
  #include <inttypes.h>

}

#include "scene_judgment.h"
#include "Arduino.h"

wifi_client scene_judgment::Wifi_Client;

enum home_place{parlour=1,
                kitchen,
                bedroom,
                Toilet};

// Initialize Class Variables 

uint8_t scene_judgment::which_scence = 0;
uint8_t scene_judgment::place = 0;
uint8_t scene_judgment::in_home_flag = 0;

GPS_location gps_locat;
// Public Methods //////////////////////////////////////////////////////////////

uint8_t scene_judgment::scene_check(void)
{
   Serial.println("scene_check(void)");
   
   if(!check_in_home())
   {
     check_outside();
   }
   
   Serial.print("which scence: ");
   Serial.println(which_scence);
   return which_scence;
}

uint8_t scene_judgment::check_in_home(void)
{
   Serial.println("check_in_home(void)");
   in_home_flag=Wifi_Client.wifi_check_home();
   if(in_home_flag)
   {
     Serial.println("user is in home now");
       place = BT_check_place();
   }
   Serial.print("at the room: ");
   Serial.println(in_home_flag);
   return place;
}

uint8_t scene_judgment::BT_check_place(void)
{
   home_place H_place=home_place(1);
   Serial.println("BT_check_place(void)");
   switch (H_place)
  {
    case parlour: 
      Serial.println("user is in the parlour");
      break;
    case kitchen: 
      Serial.println("user is in the kitchen");
      break;
    case bedroom:
      Serial.println("user is in the bedroom");
      break ;
    case Toilet:
      Serial.println("user is in the Toilet"); 
      break ;
    
    default : break ;
  } 
   return int(H_place);
}

uint8_t scene_judgment::check_outside(void)
{
   Serial.println("check_outside(void)");
   gps_locat.GPS_init();
  
  gps_locat.get_gps_info();
  if(gps_locat.sat_num==0)
  {
    Serial.println("no sat is visibel because userdev is indoor now");
    return 1;
  }
  return 0;
}



