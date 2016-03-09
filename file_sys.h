
#ifndef _file_sys_H
#define _file_sys_H

#include "LTask.h"
#include "LFlash.h"
#include "LSD.h"
#include "LStorage.h"

#define Drv LFlash          // use Internal 10M Flash
// #define Drv LSD           // use SD card

class file_sys 
{

// Constructor / Destructor

public:
      boolean check_for_file();
      boolean reinitial_file();
      boolean save_accgyo_file(float * data);
private:
     // static gpsSentenceInfoStruct info;
        
};

#endif
