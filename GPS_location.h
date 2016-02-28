
#ifndef _GPS_location_H
#define _GPS_location_H

#include "LGPS.h"

// LBattery class interface, With this library, you can get battery infomation like charging status and battery level
class GPS_location 
{

// Constructor / Destructor

public:
     static char sat_num;
     static char buff[256];
      unsigned char getComma(unsigned char num,const char *str);
      double getDoubleNumber(const char *s);
      double getIntNumber(const char *s);
      void parseGPGGA(const char* GPGGAstr);
      void GPS_init();
      void get_gps_info();
private:
      static gpsSentenceInfoStruct info;
      
      
};

#endif
