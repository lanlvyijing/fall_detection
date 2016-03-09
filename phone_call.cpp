
extern "C" {
  #include <stdlib.h>
  #include <string.h>
  #include <inttypes.h>
  
}

#include "phone_call.h"
#include "Arduino.h"

// Initialize Class Variables 

// Public Methods //////////////////////////////////////////////////////////////
String phone_call::remoteNumber = "13882137772";  // the number you will call
char phone_call::charbuffer[20];


void phone_call::make_call_to(String Number)
{
  /****************Make Voice Call*********************/
  Serial.println("Make Voice Call");
  Number.toCharArray(charbuffer, 20);
  if(LVoiceCall.voiceCall(charbuffer))
  {
      Serial.println("Call Established. Enter line to end");
   }
}

void phone_call::hang_call()
{
  /****************Make Voice Call*********************/
    Serial.println("Call Finished");
    LVoiceCall.hangCall();
}

bool phone_call::SIM_init()
{
  /****************Make Voice Call*********************/
  Serial.println("initing sim");
  if(LSMS.available())
  { 
      Serial.println("no sim card on board ");
      return 1; 
   }
  while(!LSMS.ready())
  {
    Serial.println("waiting for the sim to be init ");
    delay(800);
  }
  return 0;
}

bool phone_call::SIM_send(String sms_num,char *sms_context)
{
  /****************Make Voice Call*********************/
  char *number;
  sms_num.toCharArray(number, 11);
  const char * rnumber=number;
  
  
  LSMS.beginSMS(rnumber);
  LSMS.print(sms_context);
  return LSMS.endSMS();
}
/*
int phone_call::answerCall()
{


}*/
