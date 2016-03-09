
extern "C" {
  #include <stdlib.h>
  #include <string.h>
  #include <inttypes.h>
}

#include "set_init.h"
#include "Arduino.h"
#include "real_time.h"
#include "user_info.h"
#include "gsm_gprs.h"
#include "ble_s.h"
#include "ble_c.h"
#include "sensors.h"
#include "bat_monitor.h"


// Initialize Class Variables 

uint8_t set_init::check_flag = 0;
uint8_t set_init::check_wrong = 0;
/*************时间**************************/
datetimeInfo set_init::rel_time;
real_time Real_Time;
/*************用户信息***********************/
user_info User_Info;
/***************GSM*************************/
gsm_gprs GSM_GPRS;
phone_call phonecall;
/**************BlueTooth***********************/
ble_c Ble_C;
ble_s Ble_S;
/***************sensor*************************/
sensors Sensors;
/***************battery************************/
bat_monitor Bat_Monitor;
/*********************************************/

/*********************************************/
// Public Methods //////////////////////////////////////////////////////////////

uint8_t set_init::set_init_check(void)
{
    Serial.println("*********************led_blink************************");
    led_blink();   
   /*****************check time*****************/  
   Serial.println("*********************Real_Time.get_time************************");
    rel_time=Real_Time.get_time();
   if(rel_time.year!=2016)
   {
     check_flag=1;
     Serial.println("time is wrong");
     /**********reset time***************/
     Real_Time.set_time(Real_Time.preset_time);
     return check_flag;
   }
   /*****************check user info*****************/
   Serial.println("*********************User_Info.read_user_info************************");
   if(!User_Info.read_user_info())
   {
     Serial.println("lack of user info");
     check_flag=2;
     /************ notic that there is no info****************/
     User_Info.notic_no_info();
     User_Info.save_user_info(User_Info.test_user_name, User_Info.test_user_age, User_Info.test_user_1st_contacts_name, 
                            User_Info.test_user_1st_contacts_num, User_Info.test_user_2nd_contacts_name, User_Info.test_user_2nd_contacts_num, User_Info.test_user_medicine_time);
     return check_flag;
   }
  
   /*****************check GSM*****************/
   Serial.println("*********************GSM_GPRS.gsm_start************************");
   if(!GSM_GPRS.gsm_start())
   {
     Serial.println("gsm not working");
     check_flag=3;
     return check_flag;
   }
   /*****************check BT*****************/
   Serial.println("*********************Ble_S.bt_init************************");
  /*
   if(!Ble_S.ble_s_init())
   {
     Serial.println("bt test wrong");
     check_flag=4;
     return check_flag;
   }*/
   /*****************check sensor*****************/
   Serial.println("*********************Sensors.init_sensors************************");

   if(!Sensors.init_sensors())
   {
     Serial.println("sensor got wrong");
     check_flag=5;
     return check_flag;
   }
    /*****************check battery*****************/
     Serial.println("*********************Bat_Monitor.bat_check************************");
   if(10>=Bat_Monitor.bat_check())
   {
     Serial.println("lack of wrong battery");
     check_flag=6;
     return check_flag;
   } 
    /*****************check SIM card*****************/
   Serial.println("*********************Bat_Monitor.bat_check************************");
   if(phonecall.SIM_init())
   {
       Serial.println("check your SIM card");
       check_flag=7;
       return check_flag;
   }
   return check_flag;
}



void set_init::led_blink(void)
{
    pinMode(13, OUTPUT);
    for(char i = 0;i < 10;i++)
    {
      digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
      delay(300);              // wait for a second
      digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
      delay(300);              // wait for a second
      Serial.println("blinking");
    }   
}




