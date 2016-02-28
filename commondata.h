#ifndef commondata_h
#define commondata_h

#include <inttypes.h>
#include "Arduino.h"

  extern  float acc_x;
  extern  float acc_y;
  extern  float acc_z;
  extern  float gyo_x;
  extern  float gyo_y;
  extern  float gyo_z;
  extern  int medicine_hour;
  extern  int medicine_min;
  extern  bool medicine_take_flage;
  extern  bool medicine_time_flage;
  extern  bool leave_bt_set;
  /*****************************************************************/
  extern  String buff_user_name;            //6
  extern  String buff_user_age;                //2
  extern  String buff_user_1st_contacts_name;  //6
  extern  String buff_user_1st_contacts_num;   //11
  extern  String buff_user_2nd_contacts_name;  //6
  extern  String buff_user_2nd_contacts_num;   //11
  extern  String buff_user_medicine_time;      //6
  extern  String test_num;
  /*****************************************************************/
  extern  bool stop_call_help;
  extern  bool stop_call_aid;
#endif

