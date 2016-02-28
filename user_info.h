
#ifndef user_info_h
#define user_info_h

#include <inttypes.h>
#include "LEEPROM.h"
#include "LTask.h"
#include "commondata.h"  
class user_info 
{
  private:
    static uint8_t read_info_flag;
    static uint8_t no_info_flag;
    static String  user_info_str;
    static String  user_packet_header;
    static String  user_name;
    static String  user_age;
    static String  user_1st_contacts_name ;   //6
    static String  user_1st_contacts_num  ;   //11
    static String  user_2nd_contacts_name ;   //6
    static String  user_2nd_contacts_num  ;   //11
    static String  user_medicine_time     ;   //6
    
   
  public:
    static String  test_user_name;
    static String  test_user_age;
    static String  test_user_1st_contacts_name ;   //6
    static String  test_user_1st_contacts_num  ;   //11
    static String  test_user_2nd_contacts_name ;   //6
    static String  test_user_2nd_contacts_num  ;   //11
    static String  test_user_medicine_time     ;   //6
  
    uint8_t read_user_info();
    uint8_t notic_no_info(void);
    void extract_info(String info);
    void save_user_info(String username, String userage, String user_1stcontacts_name, String user_1stcontacts_num, String user_2ndcontacts_name, String user_2ndcontacts_num, String user_medicinetime);

};


#endif


