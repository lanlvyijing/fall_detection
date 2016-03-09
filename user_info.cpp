
extern "C" {
  #include <stdlib.h>
  #include <string.h>
  #include <inttypes.h>
}

#include "user_info.h"
#include "Arduino.h"



// Initialize Class Variables 

uint8_t user_info::read_info_flag = 0;
uint8_t user_info::no_info_flag = 0;
String  user_info::user_info_str = "";
String  user_info::user_packet_header = "PH";    //2
String  user_info::user_name = "";               //6
String  user_info::user_age = "";                //2
String  user_info::user_1st_contacts_name = "";  //6
String  user_info::user_1st_contacts_num = "";   //11
String  user_info::user_2nd_contacts_name = "";  //6
String  user_info::user_2nd_contacts_num = "";   //11
String  user_info::user_medicine_time = "";      //6

String  user_info::test_user_name = "shaooo";            //6
String  user_info::test_user_age = "22";                //2
String  user_info::test_user_1st_contacts_name = "momo 1";  //6
String  user_info::test_user_1st_contacts_num = "13882137772";   //11
String  user_info::test_user_2nd_contacts_name = "momo 2";  //6
String  user_info::test_user_2nd_contacts_num = "13882137773";   //11
String  user_info::test_user_medicine_time = "123456";      //6

#define user_info_length 50

// Public Methods //////

uint8_t user_info::read_user_info(void)
{
    Serial.println("read_user_info(void)");
    user_info_str = "";// clear the string:
    delay(100);
    for(int i=0;i<user_info_length;i++)
    {
      user_info_str += char(EEPROM.read(i));
    }
    Serial.print("user_info_str:");
    Serial.println(user_info_str);
    if((user_info_str[0]=='P')&&(user_info_str[1]=='H'))
    {
      read_info_flag = 1;
   //   set_user_info(test_user_name, test_user_age, test_user_1st_contacts_name, test_user_1st_contacts_num, test_user_2nd_contacts_name, test_user_2nd_contacts_num, test_user_medicine_time);
      extract_info(user_info_str);
    }
    else
    {
      read_info_flag = 0; 
      save_user_info(test_user_name, test_user_age, test_user_1st_contacts_name, test_user_1st_contacts_num, test_user_2nd_contacts_name, test_user_2nd_contacts_num, test_user_medicine_time);
    }
    
    return read_info_flag;
}

uint8_t user_info::notic_no_info(void)
{
     Serial.println("notic_no_info");
     Serial.println("please open the bluetooth and down load the user info");
     return no_info_flag;
}

void user_info::extract_info(String info)
{
     Serial.print("extract_info: ");
     buff_user_name               = info.substring(2,8);
     buff_user_age                = info.substring(8,10);
     buff_user_1st_contacts_name  = info.substring(10,16);
     buff_user_1st_contacts_num   = info.substring(16,27);
     buff_user_2nd_contacts_name  = info.substring(27,33);
     buff_user_2nd_contacts_num   = info.substring(33,44);
     user_medicine_time      = info.substring(44,50);
     
     char hour_buf[2];
     char min_buf[2];
     hour_buf[0]=user_medicine_time[0];
     hour_buf[1]=user_medicine_time[1];
     min_buf[0]=user_medicine_time[2];
     min_buf[1]=user_medicine_time[3];
     medicine_hour = atoi(hour_buf);
     medicine_min = atoi(min_buf);
     Serial.println("user name is:"+buff_user_name);
     Serial.println("user age is:"+buff_user_age);
     Serial.println("user first contacts name is:"+buff_user_1st_contacts_name);
     Serial.println("user first contacts num is:"+buff_user_1st_contacts_num);
     Serial.println("user second contacts name is:"+buff_user_2nd_contacts_name);
     Serial.println("user second contacts num is:"+buff_user_2nd_contacts_num);    
     Serial.println("medicine_hour is:"+medicine_hour); 
     Serial.println("medicine_min is:"+medicine_min);
}

void user_info::save_user_info(String username,String userage,String user_1stcontacts_name,String user_1stcontacts_num, String user_2ndcontacts_name,String user_2ndcontacts_num,String user_medicinetime)
{
     String info = user_packet_header + username + userage +  user_1stcontacts_name + user_1stcontacts_num + user_2ndcontacts_name + user_2ndcontacts_num + user_medicinetime;
     //String info;
     Serial.println("set_user_info()");
     int inputlength=info.length();
     for(int i=0;i<inputlength;i++)
     {
       EEPROM.write(i, info[i]);
     }
     Serial.println("written OK\n");
}




