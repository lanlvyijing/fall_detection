
extern "C" {
  #include <stdlib.h>
  #include <string.h>
  #include <inttypes.h>

}

#include "gsm_gprs.h"
#include "Arduino.h"

// Initialize Class Variables 

uint8_t gsm_gprs::gsm_work_flag = 0;


// Public Methods //////////////////////////////////////////////////////////////

uint8_t gsm_gprs::gsm_start(void)
{
   Serial.println("gsm_start");
   gsm_work_flag = 1;
   return gsm_work_flag;
}




