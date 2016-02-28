
#include "Arduino.h"
#include "GY85_I2C.h"

int GY85_I2C::ii;
char GY85_I2C::ge;
char GY85_I2C::shi;
char GY85_I2C::bai;
char GY85_I2C::qian;
char GY85_I2C::wan;
char GY85_I2C::shiwan;           //显示变量
uint8_t GY85_I2C::BUF[10];       //接收数据缓存区
int GY85_I2C::A_X;
int GY85_I2C::A_Y;
int GY85_I2C::A_Z;
int16 GY85_I2C::data_des[9];
uint8_t GY85_I2C::num[10];
float GY85_I2C::gg=0;
char GY85_I2C::a=0;
short GY85_I2C::T_X;
short GY85_I2C::T_Y;
short GY85_I2C::T_Z;
short GY85_I2C::T_T;		 //X,Y,Z轴，温度
float GY85_I2C::Roll;
float GY85_I2C::Pitch;
float GY85_I2C::yaw;
float GY85_I2C::Q;
float GY85_I2C::T;
float GY85_I2C::K;
float GY85_I2C::angle;
short GY85_I2C::data_xyz[3];

void GY85_I2C::Init_ADXL345()
{
        Wire.begin();
        I2C_WriteByte(ADXL345_Addr,0x31,0x2B);   //测量范围,正负16g，13位模式
        I2C_WriteByte(ADXL345_Addr,0x2C,0x0e);   //速率设定为100hz 参考pdf13页
	I2C_WriteByte(ADXL345_Addr,0x2D,0x08);   //选择电源模式   参考pdf24页  
	
	I2C_WriteByte(ADXL345_Addr,0x24,0x07);   //设置Activity的门限为2g
	I2C_WriteByte(ADXL345_Addr,0x2F,0x10);   //中断影射到Int1管脚
	I2C_WriteByte(ADXL345_Addr,0x27,0x70);   //使能X、Y、Z三轴的Activity
	I2C_WriteByte(ADXL345_Addr,0x2E,0x10);   //使能 Activity 中断
}
void GY85_I2C::read_ADXL345(void)
{
       I2C_ReadnByte(ADXL345_Addr,0x32,1,&BUF[0]);//OUT_X_L_A
       I2C_ReadnByte(ADXL345_Addr,0x33,1,&BUF[1]);//OUT_X_H_A
        

       I2C_ReadnByte(ADXL345_Addr,0x34,1,&BUF[2]);//OUT_Y_L_A
       I2C_ReadnByte(ADXL345_Addr,0x35,1,&BUF[3]);//OUT_Y_H_A

       I2C_ReadnByte(ADXL345_Addr,0x36,1,&BUF[4]);//OUT_Z_L_A
       I2C_ReadnByte(ADXL345_Addr,0x37,1,&BUF[5]);//OUT_Z_H_A
	
	  A_X=(BUF[1]<<8)+BUF[0];  //合成数据  
	   A_Y=(BUF[3]<<8)+BUF[2];  //合成数据
	   A_Z=(BUF[5]<<8)+BUF[4];  //合成数据
		data_des[0]=A_X;
		data_des[1]=A_Y;
		data_des[2]=A_Z;
}

void  GY85_I2C::Send_ADXL345()
{
	    Serial.print("ADXL345:X=");
	    Send_ADXL345_data(A_X);

	    Serial.print("   Y=");
	    Send_ADXL345_data(A_Y);

	    Serial.print("   Z=");
	    Send_ADXL345_data(A_Z);
	    
            Serial.print('\n');
            adxl345_angle();
		
}

void GY85_I2C::Send_ADXL345_data(int dis_data)
{ 
    float temp ;
    if(dis_data>0x7fff)dis_data-=0xffff;
    if(dis_data<0){
  	     dis_data=-dis_data;
	     Serial.print('-');
		 delay(2);
	}
	else
	  { 
	   Serial.print('+');
	   delay(2);
	   }
        temp=(float)dis_data*3.9;  //计算数据和显示,查考ADXL345快速入门第4页
        conversion(temp);          //转换出显示需要的数据	 
	    Serial.print(qian);
	  Serial.print('.');
	    Serial.print(bai); 
	    Serial.print(shi); 
	    Serial.print(ge); 
	Serial.print('g'); 
}

void GY85_I2C::conversion(long temp_data)  
{     
    shiwan=temp_data/100000+0x30 ;
    temp_data=temp_data%100000;   //取余运算 
    wan=temp_data/10000+0x30 ;
    temp_data=temp_data%10000;   //取余运算
    qian=temp_data/1000+0x30 ;
    temp_data=temp_data%1000;    //取余运算
    bai=temp_data/100+0x30   ;
    temp_data=temp_data%100;     //取余运算
    shi=temp_data/10+0x30    ;
    temp_data=temp_data%10;      //取余运算
    ge=temp_data+0x30; 	
}
	

void GY85_I2C::I2C_WriteByte(uint8_t slv_add, uint8_t res_add,uint8_t w_data)
{
  Wire.beginTransmission(slv_add>>1); //开启MPU6050的传输
  Wire.write(res_add); //指定寄存器地址
  Wire.write(w_data); //写入一个字节的数据
  Wire.endTransmission(true); //结束传输，true表示释放总线
}

void GY85_I2C::I2C_ReadnByte(uint8_t slv_add, uint8_t res_add, uint8_t num, uint8_t *buf)
{
  int ii=0;
  Wire.beginTransmission(slv_add>>1);
  Wire.write(res_add);
  Wire.requestFrom(slv_add>>1,num,true);
  while((Wire.available()))
  {
     buf[ii++] = char(Wire.read());
  }
  Wire.endTransmission(true);
}

//******************ADXL345计算倾斜角度************
void GY85_I2C::adxl345_angle(void)
{

  data_xyz[0]=A_X;  //合成数据   
  data_xyz[1]=A_Y;  //合成数据   
  data_xyz[2]=A_Z;  //合成数据   
  
  //分别是加速度X,Y,Z的原始数据，10位的
  Q=(float)data_xyz[0]*3.9;
  T=(float)data_xyz[1]*3.9;
  K=(float)data_xyz[2]*3.9;
  Q=-Q;

  Roll=(float)(((atan2(K,Q)*180)/3.14159265)+180);    //X轴角度值
  Pitch=(float)(((atan2(K,T)*180)/3.14159265)+180);  //Y轴角度值
  yaw=(float)(((atan2(Q,T)*180)/3.14159265)+180);  //z轴角度值
  conversion(Pitch);
  Serial.println("Pitch = "+String(bai)+String(shi)+String(ge)); 	 //需要显示时候请更换
  conversion(Roll);
  Serial.println("Roll = "+String(bai)+String(shi)+String(ge)); 
  conversion(yaw);
  Serial.println("yaw = "+String(bai)+String(shi)+String(ge)); 
}


//初始化ITG3205，根据需要请参考pdf进行修改************************
void GY85_I2C::Init_ITG3205(void)
{
   I2C_WriteByte(ITG3205_Addr,PWR_M, 0x80);   //
   I2C_WriteByte(ITG3205_Addr,SMPL, 0x07);    //
   I2C_WriteByte(ITG3205_Addr,DLPF, 0x1E);    //±2000°
   I2C_WriteByte(ITG3205_Addr,INT_C, 0x00 );  //
   I2C_WriteByte(ITG3205_Addr,PWR_M, 0x00);   //
}
	
//******读取ITG3205数据****************************************
void GY85_I2C::READ_ITG3205(void)
{
  
   I2C_ReadnByte(ITG3205_Addr,GX_L,1,&BUF[0]); 
   I2C_ReadnByte(ITG3205_Addr,GX_H,1,&BUF[1]);
   T_X=	(BUF[1]<<8)|BUF[0];
   T_X/=14.375; 						   //读取计算X轴数据
   Serial.println("T_X:"+String(T_X));
   
   I2C_ReadnByte(ITG3205_Addr,GY_L,1,&BUF[2]);
   I2C_ReadnByte(ITG3205_Addr,GY_H,1,&BUF[3]);
   T_Y=	(BUF[3]<<8)|BUF[2];
   T_Y/=14.375; 	
   Serial.println("T_Y:"+String(T_Y));					   //读取计算Y轴数据
   
   I2C_ReadnByte(ITG3205_Addr,GZ_L,1,&BUF[4]);
   I2C_ReadnByte(ITG3205_Addr,GZ_H,1,&BUF[5]);
   T_Z=	(BUF[5]<<8)|BUF[4];
   T_Z/=14.375; 					       //读取计算Z轴数据
   Serial.println("T_Z:"+String(T_Z));	
  
   I2C_ReadnByte(ITG3205_Addr,TMP_L,1,&BUF[6]); 
   I2C_ReadnByte(ITG3205_Addr,TMP_H,1,&BUF[7]); 
   T_T=(BUF[7]<<8)|BUF[6];
   T_T = 35+ ((double) (T_T + 13200)) / 280;// 读取计算出温度
   Serial.println("T_T:"+String(T_T));	
}
