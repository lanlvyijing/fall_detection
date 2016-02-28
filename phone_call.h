#ifndef phone_call_h
#define phone_call_h

#include <inttypes.h>
#include "LGSM.h"

class phone_call
{
  private:
      static String remoteNumber;  // the number you will call
      static char charbuffer[20];
  public:
    void hang_call();
    void make_call_to(String Number);
    bool SIM_init();
    bool SIM_send(char *sms_num,char *sms_context);
};

#endif


