//YWROBOT
//last updated on 21/12/2011
//Tim Starling Fix the reset bug (Thanks Tim)
//wiki doc http://www.dfrobot.com/wiki/index.php?title=I2C/TWI_LCD1602_Module_(SKU:_DFR0063)
//Support Forum: http://www.dfrobot.com/forum/
//Compatible with the Arduino IDE 1.0
//Library version:1.1


#include "BMI055_I2C.h"
#include <inttypes.h>
//#if defined(ARDUINO) && ARDUINO >= 100

#include "Arduino.h"


int BMI055_I2C::ii = 0;
const int BMI055_I2C::nValCnt = 6; //一次读取寄存器的数量
const int BMI055_I2C::nCalibTimes = 30; //校准时读数的次数
//short BMI055_I2C::calibData[nValCnt]; //校准数据

void BMI055_I2C::bmi055_init(void)
{
  I2C_WriteByte(BMI055_ACC_ADDR, ACC_SFRSET, 0xb6); //acc soft reset, check
  delay(100);
  I2C_WriteByte(BMI055_ACC_ADDR, ACC_RANGE, 0x05); //4g  0x03:2g 0x05:4g 0x08:8g 0x0a:16g
  delay(10);
  I2C_WriteByte(BMI055_ACC_ADDR, ACC_BW, 0x0c); //1000HZ 0x0c:250HZ 0x0d:500HZ 0x0f:2000HZ
  delay(10);
  //	I2C_WriteByte(BMI055_ACC_ADDR, ACC_OFC_CTRL, 0x07); //enable acc slow offset compensation
  //	delay(10);

  I2C_WriteByte(BMI055_GYRO_ADDR, GYRO_RANGE, 0x00); //2000 deg/s  0x00:2000 0x01:1000 0x02:500 0x03:250 0x04:125
  delay(10);
  I2C_WriteByte(BMI055_GYRO_ADDR, GYRO_BW, 0x03); //1000HZ  0x01:2000 0x02:1000 0x03:400 0x04:200 0x05:100
  delay(10);
  I2C_WriteByte(BMI055_GYRO_ADDR, GYRO_INT_EN0, 0xa0); //new data int enable and auto-offset compensation
  delay(10);
  //	I2C_WriteByte(BMI055_GYRO_ADDR, GYRO_SOC, 0x67); //enables gyro slow offset compensation for x/y/z
  //	delay();
  //	I2C_WriteByte(BMI055_GYRO_ADDR, GYRO_FOC, 0xc7); //enables fast offset compensation for x/y/z
  //	delay();
  //    I2C_WriteByte(BMI055_ACC_ADDR, ACC_SFRSET, 0xb6); //acc soft reset, check
  DisplayBMI055Log();
}

void BMI055_I2C::DisplayBMI055Log()
{
  Serial.println("start to display");
  uint8_t tmpreg[16];
  I2C_ReadnByte(BMI055_ACC_ADDR, ACC_CHIPID, 1, &tmpreg[0]);
  Serial.println("acc chip id: 0x" + String(tmpreg[0]));
  delay(10);
  I2C_ReadnByte(BMI055_ACC_ADDR, ACC_RANGE, 1, &tmpreg[1]);
  Serial.println("acc range: 0x" + String(tmpreg[1]));
  delay(10);
  I2C_ReadnByte(BMI055_ACC_ADDR, ACC_BW, 1, &tmpreg[2]);
  Serial.println("acc bw: 0x" + String(tmpreg[2]));
  delay(10);
  I2C_ReadnByte(BMI055_ACC_ADDR, ACC_OFC_CTRL, 1, &tmpreg[3]);
  Serial.println("acc ofc: 0x" + String(tmpreg[3]));
  delay(10);

  I2C_ReadnByte(BMI055_GYRO_ADDR, GYRO_RANGE, 1, &tmpreg[4]);
  Serial.println("gyro range: 0x" + String(tmpreg[4]));
  I2C_ReadnByte(BMI055_GYRO_ADDR, GYRO_BW, 1, &tmpreg[5]);
  Serial.println("gyro bw: 0x" + String(tmpreg[5]));
  I2C_ReadnByte(BMI055_GYRO_ADDR, GYRO_INT_EN0, 1, &tmpreg[6]);
  Serial.println("gyro int en0: 0x" + String(tmpreg[6]));
  I2C_ReadnByte(BMI055_GYRO_ADDR, GYRO_SOC, 1, &tmpreg[7]);
  Serial.println("gyro soc: 0x" + String(tmpreg[7]));
  I2C_ReadnByte(BMI055_GYRO_ADDR, GYRO_FOC, 1, &tmpreg[8]);
  Serial.println("gyro foc: 0x" + String(tmpreg[8]));
  Serial.println("end display");

}

void BMI055_I2C::BMI055_ReadSensor(float *ax, float *ay, float *az, float *gx, float *gy, float *gz)
{
  uint8_t buf[6];
  VMUINT16 a_x = 0, a_y = 0, a_z = 0;
  VMUINT16 g_x = 0, g_y = 0, g_z = 0;
  /****************陀螺仪****************************/
  I2C_ReadnByte(BMI055_GYRO_ADDR, GYRO_INT_STATUS1, 1, &buf[0]);

  while (!(buf[0] & 0x80))
  {
    //Serial.println("buf[0] & 0x80 != 1");
    I2C_ReadnByte(BMI055_GYRO_ADDR, GYRO_INT_STATUS1, 1, &buf[0]);
    delay(10);
  }

  I2C_ReadnByte(BMI055_GYRO_ADDR, GYRO_X_LSB, 6, &buf[0]);
  g_x = (buf[1] << 8) + buf[0];
  g_y = (buf[3] << 8) + buf[2];
  g_z = (buf[5] << 8) + buf[4];
  gyo_x = g_x;
  gyo_y = g_y;
  gyo_z = g_z;
  Serial.println("gx=" + String(g_x));
  Serial.println("gy=" + String(g_y));
  Serial.println("gz=" + String(g_z));
  /****************加速度计****************************/
  I2C_ReadnByte(BMI055_ACC_ADDR, ACC_X_LSB, 6, &buf[0]);
  while (!((buf[0] & 0x01) && (buf[2] & 0x01) && (buf[4] & 0x01)))
  {
    I2C_ReadnByte(BMI055_ACC_ADDR, ACC_X_LSB, 6, &buf[0]);
    delay(10);
  }

  a_x = ((buf[1] << 4) + (buf[0] >> 4));
  a_y = ((buf[3] << 4) + (buf[2] >> 4));
  a_z = ((buf[5] << 4) + (buf[4] >> 4));
  acc_x = a_x;
  acc_y = a_y;
  acc_z = a_z;
  Serial.println("ax=" + String(a_x));
  Serial.println("ay=" + String(a_y));
  Serial.println("az=" + String(a_z));
}

void BMI055_I2C::ReadAccGyr(short *pVals)
{
  uint8_t buf[6];
  Serial.println("ReadAccGyr");
  /****************加速度计****************************/
  I2C_ReadnByte(BMI055_ACC_ADDR, ACC_X_LSB, 6, &buf[0]);
  while (!((buf[0] & 0x01) && (buf[2] & 0x01) && (buf[4] & 0x01)))
  {
    I2C_ReadnByte(BMI055_ACC_ADDR, ACC_X_LSB, 6, &buf[0]);
    delay(10);
  }

  pVals[0] =  ((buf[1] << 8) | buf[0]);
  pVals[0] = pVals[0] >> 4;
  pVals[1] =  ((buf[3] << 8) | buf[2]);
  pVals[1] = pVals[1] >> 4;
  pVals[2] =  ((buf[5] << 8) | buf[4]);
  pVals[2] = pVals[2] >> 4;


  /****************陀螺仪****************************/
  I2C_ReadnByte(BMI055_GYRO_ADDR, GYRO_INT_STATUS1, 1, &buf[0]);

  while (!(buf[0] & 0x80))
  {
    //Serial.println("buf[0] & 0x80 != 1");
    I2C_ReadnByte(BMI055_GYRO_ADDR, GYRO_INT_STATUS1, 1, &buf[0]);
    delay(10);
  }

  I2C_ReadnByte(BMI055_GYRO_ADDR, GYRO_X_LSB, 6, &buf[0]);

  pVals[3] =  ((buf[1] << 8) | buf[0]);
  pVals[4] =  ((buf[3] << 8) | buf[2]);
  pVals[5] =  ((buf[5] << 8) | buf[4]);

}
void BMI055_I2C::ReadAccGyrforqueue(short *pVals)
{
  short buf[6];
  ReadAccGyr(buf);
  Rectify(buf, pVals, calibData);
  Serial.println("ReadAccGyrforqueue");

  pVals[3] = sqrt(buf[0]*buf[0]+buf[1]*buf[1]+buf[2]*buf[2]);
 
  pVals[7] = sqrt(buf[3]*buf[3]+buf[4]*buf[4]+buf[5]*buf[5]);
  Serial.println("[pVals] pVals ACC_X: " + String(pVals[0]));
  Serial.println("[pVals] pVals ACC_Y: " + String(pVals[1]));
  Serial.println("[pVals] pVals ACC_Z: " + String(pVals[2]));
  Serial.println("[pVals] pVals GYO_X: " + String(pVals[4]));
  Serial.println("[pVals] pVals GYO_Y: " + String(pVals[5]));
  Serial.println("[pVals] pVals GYO_Z: " + String(pVals[6]));

}

void BMI055_I2C::I2C_WriteByte(uint8_t slv_add, uint8_t res_add, uint8_t w_data)
{
  Wire.beginTransmission(slv_add >> 1); //开启MPU6050的传输
  Wire.write(res_add); //指定寄存器地址
  Wire.write(w_data); //写入一个字节的数据
  Wire.endTransmission(true); //结束传输，true表示释放总线
}

void BMI055_I2C::I2C_ReadnByte(uint8_t slv_add, uint8_t res_add, uint8_t num, uint8_t *buf)
{
  int ii = 0;
  Wire.beginTransmission(slv_add >> 1);
  Wire.write(res_add);
  Wire.requestFrom(slv_add >> 1, num, true);
  while ((Wire.available()))
  {
    buf[ii++] = char(Wire.read());
  }
  Wire.endTransmission(true);
}

void BMI055_I2C::Calibration(short* calibData)
{
  short calibDataa[nValCnt];
  float valSums[6] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0};
  //先求和
  Serial.println("[Calibration] ");

  for (int i = 0; i < nCalibTimes; ++i)
  {
    short mpuVals[nValCnt];
    ReadAccGyr(mpuVals);
    for (int j = 0; j < nValCnt; ++j)
    {
      valSums[j] += mpuVals[j];
    }
    delay(10);
  }

  //再求平均
  for (int i = 0; i < nValCnt; ++i)
  {
    calibData[i] = short(valSums[i] / nCalibTimes);
    Serial.println("[Calibration] ");
  }

  calibData[2] -= 512; //设芯片Z轴竖直向下，设定静态工作点。
  Serial.println("[Calibration] calibData ACC_X: " + String(calibData[0]));
  Serial.println("[Calibration] calibData ACC_Y: " + String(calibData[1]));
  Serial.println("[Calibration] calibData ACC_Z: " + String(calibData[2]));
  Serial.println("[Calibration] calibData GYO_X: " + String(calibData[3]));
  Serial.println("[Calibration] calibData GYO_Y: " + String(calibData[4]));
  Serial.println("[Calibration] calibData GYO_Z: " + String(calibData[5]));
  delay(3000);
}

void BMI055_I2C::Rectify(short *pReadout,short *pRealVals, short *calibData) {
  for (int i = 0; i < 3; ++i) {
    //pRealVals[i] = (float)(pReadout[i] - calibData[i]) / 16384.0f;
    pRealVals[i] = pReadout[i] - calibData[i];
  }
  // pRealVals[3] = pReadout[3] / 340.0f + 36.53;
  for (int i = 3; i < 6; ++i) {
    pRealVals[i+1] = pReadout[i] - calibData[i];
  }
}

/* this is the real data of the acc and gyr
void BMI055_I2C::Rectify(short *pReadout, float *pRealVals, short *calibData) {
  for (int i = 0; i < 3; ++i) {
    //pRealVals[i] = (float)(pReadout[i] - calibData[i]) / 16384.0f;
    pRealVals[i] = ((float)(pReadout[i] - calibData[i])) / 512.0f;
  }
  // pRealVals[3] = pReadout[3] / 340.0f + 36.53;
  for (int i = 3; i < 6; ++i) {
    pRealVals[i] = (float)(pReadout[i] - calibData[i]) / 16.4f;
  }
}*/

