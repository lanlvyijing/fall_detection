
#include "arduino.h"

#include "GPS_location.h"

gpsSentenceInfoStruct GPS_location::info;
char GPS_location::buff[256];
char GPS_location::sat_num = 0;

unsigned char GPS_location::getComma(unsigned char num,const char *str)
{
  unsigned char i,j = 0;
  int len=strlen(str);
  for(i = 0;i < len;i ++)
  {
     if(str[i] == ',')
     j++;
     if(j == num)
      return i + 1; 
  }
  return 0; 
}

double GPS_location::getDoubleNumber(const char *s)
{
  char buf[10];
  unsigned char i;
  double rev;
  
  i=getComma(1, s);
  i = i - 1;
  strncpy(buf, s, i);
  buf[i] = 0;
  rev=atof(buf);
  return rev; 
}

double GPS_location::getIntNumber(const char *s)
{
  char buf[10];
  unsigned char i;
  double rev;
  
  i=getComma(1, s);
  i = i - 1;
  strncpy(buf, s, i);
  buf[i] = 0;
  rev=atoi(buf);
  return rev; 
}

void GPS_location::parseGPGGA(const char* GPGGAstr)
{
  /* Refer to http://www.gpsinformation.org/dale/nmea.htm#GGA
   * Sample data: $GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47
   * Where:
   *GPGGA 语句包括17个字段：语句标识头，世界时间，纬度，纬度半球，经度，经度半球，定位质量指示，使用卫星数量，水平精确度，海拔高度，高度单位，大地水准面高度，高度单位，差分GPS数据期限，差分参考基站标号，校验和结束标记(用回车符<CR>和换行符<LF>)，分别用14个逗号进行分隔。该数据帧的结构及各字段释义如下：
   *$GPGGA,<1>,<2>,<3>,<4>,<5>,<6>,<7>,<8>,<9>,M,<10>,M,<11>,<12>*xx<CR><LF>
   *$GPGGA：起始引导符及语句格式说明(本句为GPS定位数据)；
   *<1> UTC时间，格式为hhmmss.sss；
   *<2> 纬度，格式为ddmm.mmmm(第一位是零也将传送)；
   *<3> 纬度半球，N或S(北纬或南纬)
   *<4> 经度，格式为dddmm.mmmm(第一位零也将传送)；
   *<5> 经度半球，E或W(东经或西经)
   *<6> GPS状态， 0初始化， 1单点定位， 2码差分， 3无效PPS， 4固定解， 5浮点解， 6正在估算 7，人工输入固定值， 8模拟模式， 9WAAS查分
   *<7> 使用卫星数量，从00到12(第一个零也将传送)
   *<8> 水平精度因子，0.5到99.9
   *<9> 天线离海平面的高度，-9999.9到9999.9米  M 指单位米
   *<10> 大地水准面高度，-9999.9到9999.9米     M 指单位米
   *<11> 差分GPS数据期限(RTCM SC-104)，最后设立RTCM传送的秒数量，如不是差分定位则为空
   *<12> 差分参考基站标号，从0000到1023(首位0也将传送)。
   *  * 语句结束标志符
   *  xx 从$开始到*之间的所有ASCII码的异或校验和
   *  <CR> 回车
   *  <LF> 换行
   *************************************************
   *  GGA          Global Positioning System Fix Data
   *  123519       Fix taken at 12:35:19 UTC
   *  4807.038,N   Latitude 48 deg 07.038' N
   *  01131.000,E  Longitude 11 deg 31.000' E
   *  1            Fix quality: 0 = invalid
   *                            1 = GPS fix (SPS)
   *                            2 = DGPS fix
   *                            3 = PPS fix
   *                            4 = Real Time Kinematic
   *                            5 = Float RTK
   *                            6 = estimated (dead reckoning) (2.3 feature)
   *                            7 = Manual input mode
   *                            8 = Simulation mode
   *  08           Number of satellites being tracked
   *  0.9          Horizontal dilution of position
   *  545.4,M      Altitude, Meters, above mean sea level
   *  46.9,M       Height of geoid (mean sea level) above WGS84
   *                   ellipsoid
   *  (empty field) time in seconds since last DGPS update
   *  (empty field) DGPS station ID number
   *  *47          the checksum data, always begins with *
   */
  double latitude;
  double longitude;
  int tmp, hour, minute, second, num ;
  if(GPGGAstr[0] == '$')
  {
    tmp = getComma(1, GPGGAstr);
    hour     = (GPGGAstr[tmp + 0] - '0') * 10 + (GPGGAstr[tmp + 1] - '0');
    minute   = (GPGGAstr[tmp + 2] - '0') * 10 + (GPGGAstr[tmp + 3] - '0');
    second    = (GPGGAstr[tmp + 4] - '0') * 10 + (GPGGAstr[tmp + 5] - '0');
    
    sprintf(buff, "UTC timer %2d-%2d-%2d", hour, minute, second);
    Serial.println(buff);
    
    tmp = getComma(2, GPGGAstr);
    latitude = getDoubleNumber(&GPGGAstr[tmp]);
    tmp = getComma(4, GPGGAstr);
    longitude = getDoubleNumber(&GPGGAstr[tmp]);
    sprintf(buff, "latitude = %10.4f, longitude = %10.4f", latitude, longitude);
    Serial.println(buff); 
    
    tmp = getComma(7, GPGGAstr);
    num = getIntNumber(&GPGGAstr[tmp]);    
    sat_num = num;
    sprintf(buff, "satellites number = %d", num);
    Serial.println(buff); 
  }
  else
  {
    Serial.println("Not get data"); 
  }
}

void GPS_location::GPS_init() {
  // put your setup code here, to run once:

  LGPS.powerOn();
  Serial.println("LGPS Power on, and waiting ..."); 
  delay(3000);
}

void GPS_location::get_gps_info() {
  // put your main code here, to run repeatedly:
  Serial.println("LGPS loop"); 
  LGPS.getData(&info);
  Serial.println((char*)info.GPGGA); 
  parseGPGGA((const char*)info.GPGGA);
  delay(2000);
}
