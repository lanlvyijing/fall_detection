#ifndef sensors_h
#define sensors_h

#include <inttypes.h>
#include "BMI055_I2C.h"

#ifdef APP_LOG
#undef APP_LOG
#endif

#define APP_LOG(...) Serial.printf(__VA_ARGS__); \
    Serial.println();
#define sensor_bmi055 1

class sensors
{
  private:
    static char sensor_work_flag;
  public:

    bool check_sensors(int sensor_num);
    char init_sensors(void);
};


#endif


