
extern "C" {
  #include <stdlib.h>
  #include <string.h>
  #include <inttypes.h>

}

#include "ble_c.h"
#include "Arduino.h"

// Initialize Class Variables 

uint8_t ble_c::bt_work_flag= 0;

int i = 0;
int j = 0;

static LGATTUUID test_uuid("B4B4B4B4-B4B4-B4B4-B4B4-B4B4B4B4B4B4");
LGATTUUID ble_c::uuidService = 0x180F; 
LGATTClient ble_c::c;
// Public Methods //////////////////////////////////////////////////////////////

uint8_t ble_c::bt_init(void)
{
    Serial.println("bt_init()");
    delay(1000);
    Serial.println("[LGATTC ino]setup.");
    
    // GATT central begin to start
    if (c.begin(test_uuid))
    {
        Serial.println("[LGATTC ino]begin() success to register.");
        bt_work_flag = 1;
    }
    else
    {
        Serial.println("[LGATTC ino]begin() failed to register.");
        delay(0xffffffff);
    }
   return bt_work_flag;
}

boolean ble_c::descInProcessing(const LGATTUUID &serviceUUID,boolean isPrimary,const LGATTUUID& characteristicUUID)
{
    boolean ret1;
    LGATTUUID descriptorUUID;

    // get descriptor for current characteristic
    ret1 = c.getDescriptorInfo(serviceUUID, isPrimary, characteristicUUID, descriptorUUID);
    if (ret1)
    {
        Serial.print("[LGATTC ino]descriptor uuid :");
        Serial.print(descriptorUUID);
        Serial.println();
    }
    else
        return false;
    return true;
}

void ble_c::charInProcessing(const LGATTUUID &serviceUUID,boolean isPrimary,const LGATTUUID& characteristicUUID,LGATT_CHAR_PROPERTIES properties)
{
    LGATTAttributeValue attrValue = {0};
    // notification or indication
    if ((properties & LGATT_CHAR_PROP_NOTIFY) == LGATT_CHAR_PROP_NOTIFY||(properties & LGATT_CHAR_PROP_INDICATE) == LGATT_CHAR_PROP_INDICATE)
    {
        // enable notification first
        if (!c.setupNotification(true, serviceUUID, isPrimary, characteristicUUID))
        {
            Serial.println("[LGATTC ino]notification already enabled");
        }
          
        // query if the notification has come.
        LGATTAttributeValue value = {0};
        Serial.println("[LGATTC ino]query notification data");
        if (c.queryNotification(serviceUUID, isPrimary, characteristicUUID, value))
        {
            Serial.printf("[LGATTC ino]notification data[%s][%d]", value.value, value.len);
            Serial.println();
        }
        
    }
    // read characteristic
    if ((properties & LGATT_CHAR_PROP_READ) == LGATT_CHAR_PROP_READ)  //read or write
    {
        memset(&attrValue, 0, sizeof(attrValue));
        if (c.readCharacteristic(serviceUUID, isPrimary, characteristicUUID, attrValue))
        {
            Serial.print("[LGATTC ino]characteristicUUID is:");
            Serial.println(characteristicUUID);
            Serial.print("[LGATTC ino]read char :");
            Serial.print((char*)attrValue.value);
            Serial.println();
        }
        else
        {
            Serial.print("[LGATTC ino]read char [FAILED]");
            Serial.println();
        }
    }
    
    // write characteristic
    if ((properties & LGATT_CHAR_PROP_WRITE) == LGATT_CHAR_PROP_WRITE)     //read or write
    {
        /* you can prepare data for the specified characteristic with write property here */
        char szbuf[] = "im a central"; // somedata will be sent to prepherial
        memset(&attrValue, 0, sizeof(attrValue));
        memcpy(attrValue.value, szbuf, strlen(szbuf));
        attrValue.len = strlen(szbuf);
        if (c.writeCharacteristic(serviceUUID, isPrimary, characteristicUUID, attrValue))
        {
            Serial.print("[LGATTC ino]written data :");
            Serial.print((char*)attrValue.value);
            Serial.println();
        }
        else
        {
            Serial.print("[LGATTC ino]write data [FAILED]");
            Serial.println();
        }
    }
 
}

void ble_c::serviceInProcessing(const LGATTUUID &inputServiceUUID)
{
    // query all services, to find if your specified service exist or not. 
    int num = c.getServiceCount();
    // enum all of the services
    for (i = 0; i < num; i++)
    {
        LGATTUUID serviceUUID; 
        boolean isPrimary = false;
        
        Serial.print("[LGATTC ino]serviceInProcessing service uuid :");
        Serial.print(inputServiceUUID);
        Serial.println();

        // service uuid matched "E20A39F4-73F5-4BC4-A12F-17D1AD07A961" or not?
        bool get_SerInfo_OK=c.getServiceInfo(i, serviceUUID, isPrimary);
        Serial.print("[LGATTC ino]get service uuid :");
        Serial.print(serviceUUID);
        Serial.println();
        
        if (get_SerInfo_OK && inputServiceUUID == serviceUUID)
        {
            Serial.print("[LGATTC ino]found service uuid :");
            Serial.print(serviceUUID);   
            Serial.println();
            boolean ret;

            while (1)
            {
                delay(50);    
                LGATTUUID characteristicUUID;
                LGATT_CHAR_PROPERTIES properties = 0;
                
                // polling all characteristics of current service
                ret = c.getCharacteristicInfo(serviceUUID, isPrimary, characteristicUUID, properties);
                if (ret)//get Characteristic Info success
                {
                    // characteristic processing here
                    charInProcessing(serviceUUID, isPrimary, characteristicUUID, properties);
                    while (1)
                    {
                        // polling all descriptors for current characteristic
                        if (!descInProcessing(serviceUUID, isPrimary, characteristicUUID))
                            break;
                    }
                }
                else
                {
                    break;
                }

            }
            break;
        }
        delay(50);    
    }

}

boolean ble_c::hasFoundSpencifiedBleDev(const LGATTDeviceInfo &info)
{
    // you can check your known remote device address here, or else the first descovered ble device will be connected.
    {
       return true; 
    }
    return false;
}
void ble_c::scan_bt_dev() 
{
    // scan device
    Serial.println("[LGATTC ino]loop::start to scan.");
    int num = 0;
    LGATTDeviceInfo info = {0};
    while (1)
    {
        // scan ble devices
        num = c.scan(6);
        int found = 0;
        Serial.printf("scan num [%d]\n", num);
   

        // polling all found devices
        for (i = 0; i < num; i++)
        {
            c.getScanResult(i, info);
            Serial.printf("[LGATTC ino]dev address %d: [%x:%x:%x:%x:%x:%x] rssi [%d]\n",i, 
            info.bd_addr.addr[5], info.bd_addr.addr[4], info.bd_addr.addr[3], info.bd_addr.addr[2], info.bd_addr.addr[1], info.bd_addr.addr[0],info.rssi);

            // check if the current ble device is the one you prefered.
            //you can check your known remote device address here, or else the first descovered ble device will be connected.
            if (hasFoundSpencifiedBleDev(info))
            {
                found = 1;
                break;
            }
            Serial.printf("[LGATTC ino]dev not found yet\n");
            
        }
        if (found)
        {
            break;
        }
        delay(500);
    }
    
    Serial.println("[LGATTC ino]loop::start to connect.");

    if (!c.connect(info.bd_addr)) // search all services till timeout or searching done.
    {
        Serial.println("[LGATTC ino]begin() failed to connect.");
        delay(0xffffffff);
    }
    else
        Serial.println("[LGATTC ino]begin() success to connect.");

    // central working here, serviceInProcessing is a function query all characteristics. 
    // it make a chance for you to do some actions to the one or more characteristics. 
    // such as read / write them.
    int times = 3;
    while (times--)
    {
        serviceInProcessing(uuidService);    //  128bit format for your cared service uuid
        delay(1000);
    }

    // disconnect the remote
    c.disconnect(info.bd_addr);
    Serial.println("[LGATTC ino] Disconnected");

    // finished all
    if (!c.end())
    {
        Serial.println("[LGATTC ino] failed to end");
    }
    Serial.println("[LGATTC ino] ended");
    
    delay(1000);

    // re-start again
    if (!c.begin(test_uuid))
    {
        Serial.println("[LGATTC ino] failed to begin");
    }    
    Serial.println("[LGATTC ino] Begined");
    //delay(0xffffffff);
}


