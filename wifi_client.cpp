
extern "C" {
  #include <stdlib.h>
  #include <string.h>
  #include <inttypes.h>
}

#include "wifi_client.h"
#include "Arduino.h"

// Initialize Class Variables 

#define WIFI_AP "Tenda_262028"
#define WIFI_PASSWORD "xgy30787"
#define WIFI_AUTH LWIFI_WPA  // choose from LWIFI_OPEN, LWIFI_WPA, or LWIFI_WEP.
#define SITE_URL "www.baidu.com"

void wifi_client::wifi_scan()
{
  delay(1000);
  LWiFi.begin();
       int numOfAP = LWiFi.scanNetworks();
      Serial.print("number of WiFi AP found:");
      Serial.println(numOfAP);
      char *ssid_list;
      for(char i=0;i<numOfAP;i++)
      {
        ssid_list = LWiFi.SSID(i);
        Serial.println(ssid_list);
        delay(1000);
      }
  LWiFi.end();
}
uint8_t wifi_client::wifi_check_home()
{
  uint8_t check_flag=0;
  LWiFi.begin();
      int numOfAP = LWiFi.scanNetworks();
      Serial.print("number of WiFi AP found:");
      Serial.println(numOfAP);
      char *ssid_list;
      for(char i=0;i<numOfAP;i++)
      {
        ssid_list = LWiFi.SSID(i);
        Serial.println(ssid_list);
        if(strcmp(ssid_list,WIFI_AP))
        {
            check_flag=1;
        }  
    }
  LWiFi.end();
  return check_flag;
}


