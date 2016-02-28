//YWROBOT
#ifndef GY85_I2C_h
#define GY85_I2C_h

#include <inttypes.h>
#include "Print.h" 
#include "Wire.h"
#include  <math.h>    //Keil library  
#include "commondata.h"
/***************************************/
#define uint8 unsigned char
#define uint16 unsigned short int
#define uint32 unsigned long int
#define int8	signed char
#define int16 signed short int
#define int32 signed long int
#define uint64 unsigned long long int
#define int64 signed long long int
#define u8 unsigned char

#define SCL 19
#define SDA 18

#define	ADXL345_Addr  0xA6	//加速度传感器器件地址

//定义ITG3205内部地址********************
#define WHO	0x00
#define	SMPL	0x15
#define DLPF	0x16
#define INT_C	0x17
#define INT_S	0x1A
#define	TMP_H	0x1B
#define	TMP_L	0x1C
#define	GX_H	0x1D
#define	GX_L	0x1E
#define	GY_H	0x1F
#define	GY_L	0x20
#define GZ_H	0x21
#define GZ_L	0x22
#define PWR_M	0x3E
#define	ITG3205_Addr   0xD0	  //定义器件在IIC总线中的从地址,根据ALT  ADDRESS地址引脚不同修改
//****************************

class GY85_I2C{

// Constructor / Destructor

// Method
public:
	void I2C_WriteByte(uint8_t slv_add, uint8_t res_add,uint8_t w_data);
        void I2C_ReadnByte(uint8_t slv_add, uint8_t res_add, uint8_t num, uint8_t *buf);
        void conversion(long temp_data);
        void Send_ADXL345_data(int dis_data);
        void Send_ADXL345();
        void read_ADXL345(void);
        void Init_ADXL345();

        void READ_ITG3205(void);
        void Init_ITG3205(void);
        void adxl345_angle(void);
        
        static float Q;
	static float T;
	static float K;
        static float gg;
private:
	static int ii;
	static char ge;
	static char shi;
 	static char bai;
        static char qian;
        static char wan;
        static char shiwan;           //显示变量
	static uint8_t BUF[10];       //接收数据缓存区
	static int A_X;
	static int A_Y;
	static int A_Z;
	static int16 data_des[9];
	static uint8_t num[10];
	static char a;

	static short T_X;
	static short T_Y;
	static short T_Z;
	static short T_T;		 //X,Y,Z轴，温度
	static float Roll;
	static float Pitch;
	static float yaw;
	static float angle;
	static short data_xyz[3];


};


#endif
