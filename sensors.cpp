
extern "C" {
  #include <stdlib.h>
  #include <string.h>
  #include <inttypes.h>  
}

#include "sensors.h"
#include "Arduino.h"

BMI055_I2C bmi055;

// Initialize Class Variables 

char sensors::sensor_work_flag = 0;

// Public Methods //////////////////////////////////////////////////////////////

char sensors::init_sensors(void)
{
  Serial.println("init_sensors(void)");
  Wire.begin();

  delay(100);
  bmi055.bmi055_init();
  /**************BT begin***********************/
  sensor_work_flag = check_sensors(sensor_bmi055);
  /************** only for testing ,should be deleted after sensor is onboard ************************/
  sensor_work_flag = 1;
   return sensor_work_flag;
}

bool sensors::check_sensors(int sensor_num)
{
   Serial.println("check_sensors(void)");
   uint8_t tmpreg;
   bmi055.I2C_ReadnByte(BMI055_ACC_ADDR, ACC_CHIPID, 1, &tmpreg);
   if(tmpreg == 0x3E)
   {
     Serial.print("acc chip id: 0x"+ String(tmpreg) + "\n");
     return 1;
   }
   delay(10);
   return 0;
  // bmi055.BMI055_ReadSensor(&acc_x, &acc_y, &acc_z, &gyo_x, &gyo_y, &gyo_z);
}




