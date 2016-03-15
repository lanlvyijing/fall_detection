#ifndef _fall_queue_H_
#define _fall_queue_H_
//#include "sensor_process.h"
//#include "behave_queue.h"
//#include "pedometer.h"
#include "arduino.h"
#define fall_queue_Length 120
#define Rear_Length  60
#define G 9.8
#define SCALE 0x7FFF
#define Multiple_SCALE 4
#define HZ 20

#define G_F                               ((uint32_t)(SCALE*9.8/(Multiple_SCALE*G)))
#define Gyrm_Static_F                     ((uint32_t)(SCALE*1.5/(Multiple_SCALE*G)))
#define AccmPeak_Fall_Thredhold           ((uint32_t)(SCALE*35/(Multiple_SCALE*G)))

#define Accmfall_velocity_Thredhold_D     ((uint32_t)(SCALE*20/(Multiple_SCALE*G)))
#define Accmfall_velocity_Thredhold_U     ((uint32_t)(SCALE*50/(Multiple_SCALE*G)))

#define Gyrm_max_Thredhold_D              ((uint32_t)(SCALE*10/(Multiple_SCALE*G)))
#define Gyrm_max_Thredhold_U              ((uint32_t)(SCALE*40/(Multiple_SCALE*G)))

#define Gyrm_fall_angleThredhold_D        ((uint32_t)(SCALE*60/(Multiple_SCALE*G)))
#define Gyrm_fall_angleThredhold_U        ((uint32_t)(SCALE*120/(Multiple_SCALE*G)))
#define accm_Rear_Stitic_Thredhold        ((uint32_t)(SCALE*0.5/(Multiple_SCALE*G)))
#define accx_Rear_Lie_Thredhold        	  ((uint32_t)(SCALE*3/(Multiple_SCALE*G)))
#define accx_Rear_Stand_Thredhold         ((uint32_t)(SCALE*5/(Multiple_SCALE*G)))

#ifdef __cplusplus
extern "C" {
#endif

typedef struct 
{	
	short data_frame[fall_queue_Length];
	uint8_t rear;
	uint8_t front;
	uint8_t count;
	uint8_t len;	
}fall_queue_t;


  void fall_queue_init(void *pp_fall_queue, uint8_t len);

  void fall_queue_insert(void *p_fall_queue,short p_data );
  void fall_queue_out(void *p_fall_queue, short *p_data);

  void fill_fall_queue(void *p_fall_queue,short p_data);

  uint8_t judge_falldown(short fall_featrue[],uint8_t Is_fall[]);
  void get_fall_AxisRear_featrue(short fall_featrue[],void *accx_queue,void *accm_queue,void *gyrm_queue);

  void get_fall_AxisFront_featrue(short behave_featrue[],void *accx_queue,void *accm_queue,void *gyrm_queue);

  void get_fall_featrue(short raw_data[],short fall_featrue[],void *fall_accx_queue,void *fall_accm_queue,void *fall_gyrm_queue);

  void init_fall_queue(short *data_des, void *fall_accx_queue, void *fall_accm_queue, void *fall_gyrm_queue);

#ifdef __cplusplus
}
#endif
#endif
