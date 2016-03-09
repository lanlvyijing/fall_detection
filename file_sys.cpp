/*
  Copyright (c) 2014 MediaTek Inc.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License..

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
   See the GNU Lesser General Public License for more details.
*/

#include "Arduino.h"
#include "vmgps.h"
#include "vmlog.h"
#include "file_sys.h"

LFile myFile;

boolean file_sys::check_for_file()
{
  Serial.print("Initializing Drv card...");
    // On the Ethernet Shield, CS is pin 4. It's set as an output by default.
    // Note that even if it's not used as the CS pin, the hardware SS pin
    // (10 on most Arduino boards, 53 on the Mega) must be left as an output
    // or the Drv library functions will not work.
  pinMode(10, OUTPUT);
  Drv.begin();
  if (Drv.exists("AccGyo.txt")) 
    {
        Serial.println("AccGyo.txt exists.");
    }
    else 
    {
        Serial.println("AccGyo.txt doesn't exist.");
        // open a new file and immediately close it:
        Serial.println("Creating AccGyo.txt...");
        myFile = Drv.open("AccGyo.txt", FILE_WRITE);
        myFile.close();
    }
    delay(1000);
    return true;
}

boolean file_sys::reinitial_file()
{
  Serial.print("reinitialing AccGyo.txt...");
    // On the Ethernet Shield, CS is pin 4. It's set as an output by default.
    // Note that even if it's not used as the CS pin, the hardware SS pin
    // (10 on most Arduino boards, 53 on the Mega) must be left as an output
    // or the Drv library functions will not work.
  pinMode(10, OUTPUT);
  Drv.begin();
  if (Drv.exists("AccGyo.txt")) 
    {
        Serial.println("AccGyo.txt exists.");
        Serial.println("Removing AccGyo.txt...");
        Drv.remove("AccGyo.txt");
        myFile = Drv.open("AccGyo.txt", FILE_WRITE);
        myFile.close();
    }
    else 
    {
        Serial.println("AccGyo.txt doesn't exist.");
        // open a new file and immediately close it:
        Serial.println("Creating AccGyo.txt...");
        myFile = Drv.open("AccGyo.txt", FILE_WRITE);
        myFile.close();
    }
    delay(1000);
    return true;
}

boolean file_sys::save_accgyo_file(float * data)
{
  Serial.print("save acc and gyo data to card...");
  
  pinMode(10, OUTPUT);
  Drv.begin();
  if (Drv.exists("AccGyo.txt")) 
    {
        Serial.println("AccGyo.txt exists.");
        myFile = Drv.open("AccGyo.txt", FILE_WRITE);
    }
    else 
    {
        Serial.println("AccGyo.txt doesn't exist.");
        // open a new file and immediately close it:
        Serial.println("Creating AccGyo.txt...");
        myFile = Drv.open("AccGyo.txt", FILE_WRITE);
    }
    myFile.println(String(data[0])+"\t"+String(data[1])+"\t"+String(data[2])+"\t"+String(data[3])+"\t"+String(data[4])+"\t"+String(data[5])+"\r");
    myFile.close();  
    return true;
}


