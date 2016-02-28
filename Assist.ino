#include "set_init.h"
#include "user_info.h"
#include "scene_judgment.h"
#include "monitor_signal.h"
#include "ble_s.h"
#include "phone_call.h"
#include "GY85_I2C.h"
/****************************初始化**************************************/
uint8_t check_flag = 0;
set_init Set_Init;
/****************************场景检测************************************/
uint8_t which_scence = 0;
scene_judgment Scene_Judgment;
/****************************信号监测************************************/
uint8_t signal_flag = 0;
monitor_signal signal_check;
/*****************************循环*************************************/
uint8_t counting = 0;
/****************************按键相关**************************************/
const char buttonPin1 = 2;     // the number of the pushbutton pin1
const char buttonPin2 = 3;   // the number of the pushbutton pin2
// variables will change:
bool buttonState1 = 0;         // variable for reading the pushbutton status
bool buttonState2 = 0;         // variable for reading the pushbutton status
char pushtime = 0;
/************************************************************************/

ble_s Ble_SS;
GY85_I2C gy85_i2c;
/******************************按键处理函数******************************/

void Two_button_pushed();
void button1_push();
void button2_push();
void check_botton_stat();
void check_exit_call();
/************************************************************************/
void BLE_phone();
phone_call Phone_Call;
/************************************************************************/
void setup() {
  // put your setup code here, to run once:
   
  char check_flag=0;
  Serial.begin(9600);
  delay(3000);
  gy85_i2c.Init_ITG3205();
  gy85_i2c.Init_ADXL345();
 /*
  Serial.println("*************************set_init_check(void)************************************");
  check_flag = Set_Init.set_init_check();
  Ble_SS.ble_s_init();
  while(check_flag)
  {
    Serial.println("*************************set_init_check(void)************************************");
    check_flag = Set_Init.set_init_check();
    Serial.println("set init check flag:"+String(check_flag));
  }  
  //init button pin
  pinMode(buttonPin1, INPUT_PULLUP);
  pinMode(buttonPin2, INPUT_PULLUP);
 */
}

void loop() {
  delay(1000);
  gy85_i2c.READ_ITG3205();
 
  gy85_i2c.read_ADXL345();
  gy85_i2c.Send_ADXL345();
  gy85_i2c.Q=gy85_i2c.Q/(float)1000;
  gy85_i2c.T=gy85_i2c.T/(float)1000;
  gy85_i2c.K=gy85_i2c.K/(float)1000;
  gy85_i2c.gg = sqrt(gy85_i2c.Q*gy85_i2c.Q+gy85_i2c.T*gy85_i2c.T+gy85_i2c.K*gy85_i2c.K);
  gy85_i2c.conversion(gy85_i2c.gg*1000);          //转换出显示需要的数据
  /*
    if(counting--)
    {
        Serial.println("************************* signal_checking ************************************");
        signal_check.signal_checking();
           
    }
    else
    {
       Serial.println("************************* Scene_Judgment ************************************");
        which_scence = Scene_Judgment.scene_check();
        
        counting=20;
        delay(500);
    }
    
    Serial.println("************************* check_botton_stat ************************************");
    check_botton_stat();  
    delay(10);*/
}


void button1_push()
{ 
  int count=0;
    Serial.println("[button1_push] button1 pushed, call for help");
    stop_call_help = 1;
    while(stop_call_help)
    {
      if(count>200)
      {
        count=0;
        Phone_Call.make_call_to(test_num);
      }
      check_exit_call();
      count++;
      delay(60);
    }
    stop_call_help=0;
}
void button2_push()
{
  int count=0;
  Serial.println("[button2_push] button2 pushed, call for aid");
  
  stop_call_aid = 1;
  while(stop_call_aid)
   {
       if(count>200)
       {
          count=0;
          Phone_Call.make_call_to(test_num);
        }
        check_exit_call();
        count++;
        delay(60);
    }
    stop_call_aid=0;
}

void Two_button_pushed_short()
{
    Serial.println("[Two_button_pushed_short] Two button are pushed SHORTER than 3s");
    
    if(stop_call_help)
    {
      stop_call_help=0;
    }
    if(stop_call_aid)
    {
      stop_call_aid=0;
    }
    delay(500);
}

void Two_button_pushed_long()
{
    Serial.println("[Two_button_pushed_long] Two button are pushed LONGER than 3s ");
    
    Ble_SS.user_info_get();
}

//检测按键并根据不同情况执行求助，求救，设置，解除的功能
void check_botton_stat()
{
     // read the state of the pushbutton value:
             // check if the pushbutton is pressed.if it is, the buttonState is HIGH:
        if (digitalRead(buttonPin1) == LOW) 
        {
          delay(60);
          
          if(digitalRead(buttonPin1) == LOW)//消抖
          {
              while(digitalRead(buttonPin1) == LOW)//松手检测
              {  
                  if(digitalRead(buttonPin2) == LOW)//检测是否同时按下按键2
                  {
                      delay(60);
                      if(digitalRead(buttonPin2) == LOW)
                      {
                          while((digitalRead(buttonPin2) == LOW)||(digitalRead(buttonPin1) == LOW))
                          {
                              pushtime++;  
                              delay(100);
                           }
                           if( pushtime>30)
                           {
                             if(stop_call_help==0)
                              {
                                  Two_button_pushed_long();
                              }
                           }
                           else
                           {
                               Two_button_pushed_short();
                           }
                           pushtime=0;
                      }
                  } 
                 
              }            
              button1_push();
          }
        }
        if (digitalRead(buttonPin2) == LOW) 
        {
          delay(60);
              if(digitalRead(buttonPin2) == LOW)//消抖
              {
                    while(digitalRead(buttonPin2) == LOW)//松手检测
                    {
                            if(digitalRead(buttonPin1) == LOW)//检测是否同时按下按键2
                            {
                                    delay(60);
                                    if(digitalRead(buttonPin1) == LOW)
                                    {
                                        while((digitalRead(buttonPin2) == LOW)||(digitalRead(buttonPin1) == LOW))
                                        {
                                            pushtime++;  
                                            delay(100);
                                         }
                                         if( pushtime>30)
                                         {
                                           if(stop_call_help==0)
                                            {
                                                Two_button_pushed_long();
                                            }
                                         }
                                         else
                                         {
                                             Two_button_pushed_short();
                                         }
                                         pushtime=0;
                                    }
                             }
                     }
                     button2_push();
               }
        }
}

void check_exit_call()
{
    if (digitalRead(buttonPin1) == LOW) 
    {
          delay(60);
          
          if(digitalRead(buttonPin1) == LOW)//消抖
          {
              while(digitalRead(buttonPin1) == LOW)//松手检测
              {  
                  if(digitalRead(buttonPin2) == LOW)//检测是否同时按下按键2
                  {
                      delay(60);
                      if(digitalRead(buttonPin2) == LOW)
                      {
                          while((digitalRead(buttonPin2) == LOW)||(digitalRead(buttonPin1) == LOW))
                          {
                              pushtime++;  
                              delay(100);
                           }
                           if( pushtime<30)
                           {
                               Two_button_pushed_short();
                           }
                           pushtime=0;
                      }
                  }    
              }
          }
     }
}

