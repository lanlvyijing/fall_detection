
//#include <stdint.h>
#include <stdlib.h>
#include <inttypes.h>
#include "fall_queue.h"
#include <Math.h>



void init_fall_queue(short *data_des,void *fall_accx_queue,void *fall_accm_queue,void *fall_gyrm_queue)
{
   
  fall_queue_init(fall_accx_queue,fall_queue_Length);
  fall_queue_init(fall_accm_queue,fall_queue_Length);
  fall_queue_init(fall_gyrm_queue,fall_queue_Length);
  fill_fall_queue(fall_accx_queue,data_des[0]);
  fill_fall_queue(fall_accm_queue,data_des[3]);
  fill_fall_queue(fall_gyrm_queue,data_des[7]);

}

uint8_t judge_falldown(short *fall_featrue,uint8_t *Is_fall)
{
  /*
  uint8_t Is_falldown=0;
  uint8_t Is_peak_enough      = fall_featrue[0] > AccmPeak_Fall_Thredhold;
  uint8_t Is_velocity_enough  = fall_featrue[1] > Accmfall_velocity_Thredhold_D && fall_featrue[1] < Accmfall_velocity_Thredhold_U;
  uint8_t Is_gyrmmax_enough   = fall_featrue[2] > Gyrm_max_Thredhold_D          && fall_featrue[2] < Gyrm_max_Thredhold_U;

  uint8_t Is_fallangle_enough = fall_featrue[3] > Gyrm_fall_angleThredhold_D    && fall_featrue[3] < Gyrm_fall_angleThredhold_U;
  uint8_t Is_time_ok          = fall_featrue[4] <= 10;
  //uint8_t Is_barodiff_ok      = fall_featrue[5] >= baro_diff_Thredhold;
  uint8_t Is_rear_static_ok   = fall_featrue[5] < accm_Rear_Stitic_Thredhold;
  uint8_t Is_rear_lie_ok      = fall_featrue[6] < accx_Rear_Lie_Thredhold;
  uint8_t Is_front_stand_ok   = fall_featrue[7] > accx_Rear_Stand_Thredhold;

  if(Is_peak_enough&&Is_velocity_enough&&Is_gyrmmax_enough&&Is_rear_static_ok&&Is_rear_lie_ok&&Is_time_ok&&Is_front_stand_ok)//&&Is_barodiff_ok
  {
    Is_falldown=1;
  }
  Is_fall[0]=Is_peak_enough;
  Is_fall[1]=Is_velocity_enough;
  Is_fall[2]=Is_gyrmmax_enough;
  Is_fall[3]=Is_fallangle_enough;
  Is_fall[4]=Is_rear_static_ok;
  Is_fall[5]=Is_rear_lie_ok;
  return Is_falldown;*/
}

void get_fall_featrue(short *raw_data,short *fall_featrue,void *fall_accx_queue,void *fall_accm_queue,void *fall_gyrm_queue)
{
   short a;
   short accx_front;
   short fall_front_featrue[6];
   short fall_rear_featrue[2];
   fall_queue_insert(fall_accx_queue,raw_data[0]);
   fall_queue_out(fall_accx_queue, &accx_front);

   fall_queue_insert(fall_accm_queue,raw_data[3]);
   fall_queue_out(fall_accm_queue, &a);

   fall_queue_insert(fall_gyrm_queue,raw_data[7]);
   fall_queue_out(fall_gyrm_queue, &a);
   //fall_queue_insert(&fall_baro_queue,raw_data[8]);
   //fall_queue_out(&fall_baro_queue, &a);
   get_fall_AxisFront_featrue(fall_front_featrue,fall_accx_queue,fall_accm_queue,fall_gyrm_queue);
   get_fall_AxisRear_featrue(fall_rear_featrue,fall_accx_queue,fall_accm_queue,fall_gyrm_queue);
   fall_featrue[0]=fall_front_featrue[0];
   fall_featrue[1]=fall_front_featrue[1];
   fall_featrue[2]=fall_front_featrue[2];
   fall_featrue[3]=fall_front_featrue[3];
   fall_featrue[4]=fall_front_featrue[4];
   fall_featrue[5]=fall_front_featrue[5];
   fall_featrue[6]=fall_rear_featrue[0];
   fall_featrue[7]=fall_rear_featrue[1];
  //fall_featrue[8]=fall_front_featrue[6];
}

void get_fall_AxisFront_featrue(short *fall_featrue,void *accx_queue,void *accm_queue,void *gyrm_queue)//0~accm_max��1~fall_velocity��2~gyrm_max��3~fall_angle��4abs(accm_max_inc-gyrm_max_inc)
{
     int i=0;
     fall_queue_t *fall_accx_queue=(fall_queue_t *)accx_queue;
     fall_queue_t *fall_accm_queue=(fall_queue_t *)accm_queue;
     fall_queue_t *fall_gyrm_queue=(fall_queue_t *)gyrm_queue;

     uint8_t subscript=fall_accm_queue->rear;  
     Serial.println("subscript:"+String(subscript));
     short accm_max=-100000000;
     short accm_min=100000000;
     short accm_max_inc=0;
//   short accm_min_inc=0;

     short gyrm_max=-100000000;
//   short gyrm_max_inc=0;
    // short baro_max=-100000000;
  //   short baro_min=100000000;

     short gyrm_max_inc_i=0;
     short accm_max_inc_i=0;
     short fall_velocity=0;
     short fall_angle=0;
     
     for (i = 0, subscript = fall_accm_queue->rear; i < (fall_queue_Length-Rear_Length); i++)
     {
	if (subscript == 0)
		subscript = fall_accm_queue->len;
	subscript--;
     }

      for(i=1;i<fall_queue_Length - Rear_Length;i++)
      {
         if(accm_max<fall_accm_queue->data_frame[subscript])
         {
           accm_max=fall_accm_queue->data_frame[subscript];
           accm_max_inc=subscript;
           accm_max_inc_i=i;
         }
         if(accm_min>fall_accm_queue->data_frame[subscript])
         {
             accm_min=fall_accm_queue->data_frame[subscript];
//           accm_min_inc=i;
         }

         if(gyrm_max<fall_gyrm_queue->data_frame[subscript])
         {
           gyrm_max=fall_gyrm_queue->data_frame[subscript];
//           gyrm_max_inc=subscript;
           gyrm_max_inc_i=i;
         }

          if (subscript == 0)
               subscript = fall_accm_queue->len;
          subscript--;
      }

      if(1)
      {
          subscript = accm_max_inc;
          for(i=0;i<20;i++)
          {
            if(fall_accm_queue->data_frame[subscript]<G_F)
            {
                 fall_velocity +=abs(fall_accm_queue->data_frame[subscript]-G_F);
            }
            if(fall_gyrm_queue->data_frame[subscript]>=Gyrm_Static_F)
            {
                 fall_angle+=abs(fall_gyrm_queue->data_frame[subscript]-Gyrm_Static_F);
            }

            // uint8_t Is_break=(subscript==(fall_accm_queue->front));
             if(subscript==(fall_accm_queue->front))
             {
                break;
             }
             if (subscript == 0)
             	subscript = fall_accm_queue->len;
             subscript--;
          }
      }
   
    fall_featrue[0]=accm_max;
    fall_featrue[1]=fall_velocity;
    fall_featrue[2]=gyrm_max;
    fall_featrue[3]=fall_angle;
    fall_featrue[4]=abs(gyrm_max_inc_i-accm_max_inc_i);
   // fall_featrue[5]=abs(baro_min-baro_max);
    fall_featrue[5]=abs(fall_accx_queue->data_frame[subscript]);

}



void get_fall_AxisRear_featrue(short *fall_featrue,void *accx_queue,void *accm_queue,void *gyrm_queue)
{     
     fall_queue_t *fall_accx_queue=(fall_queue_t *)accx_queue;
     fall_queue_t *fall_accm_queue=(fall_queue_t *)accm_queue;
     fall_queue_t *fall_gyrm_queue=(fall_queue_t *)gyrm_queue;
     
     int i=0;
     uint8_t subscript=fall_accm_queue->rear;
     short accm_average_sum=0;
     short accm_variance_sum=0;
     short accx_average_sum=0;
     short accm_average=0;

     for(i=1;i<Rear_Length;i++)
    {      
        accm_average_sum+=fall_accm_queue->data_frame[subscript];
        accx_average_sum+=fall_accx_queue->data_frame[subscript];

        if (subscript == 0)
             subscript = fall_accm_queue->len;
        subscript--;
    }
    accm_average=accm_average_sum/Rear_Length;
      for(i=1;i<Rear_Length;i++)
    {
       accm_variance_sum+=(fall_accm_queue->data_frame[subscript]-accm_average)*(fall_accm_queue->data_frame[subscript]-accm_average);
    }
   
    fall_featrue[0]=abs(fall_accm_queue->data_frame[fall_accm_queue->rear]-G_F);
    fall_featrue[1]=abs(accx_average_sum/Rear_Length);   
}

void fall_queue_init(void *pp_fall_queue, uint8_t len)
{
        fall_queue_t *p_fall_queue=(fall_queue_t *)pp_fall_queue;
        
	p_fall_queue->front = len - 1;
	p_fall_queue->rear = len - 1;
	p_fall_queue->count = 0;
	p_fall_queue->len = len;
        //Serial.println("p_fall_queue->len:"+String(p_fall_queue->len));
}

void fall_queue_insert(void *pp_fall_queue, short p_data)
{
        fall_queue_t *p_fall_queue=(fall_queue_t *)pp_fall_queue;
  	p_fall_queue->rear = (p_fall_queue->rear + 1) % (p_fall_queue->len);
	p_fall_queue->count++;
	p_fall_queue->data_frame[p_fall_queue->rear] = p_data;
}

void fall_queue_out(void *pp_fall_queue, short *p_data)
{
        fall_queue_t *p_fall_queue=(fall_queue_t *)pp_fall_queue;
  	p_fall_queue->front = (p_fall_queue->front + 1) % (p_fall_queue->len);
	p_fall_queue->count--;
	*p_data = p_fall_queue->data_frame[p_fall_queue->front];
}
void fill_fall_queue(void *pp_fall_queue,short p_data)
{
  int i=0;  
  fall_queue_t *p_fall_queue=(fall_queue_t *)pp_fall_queue;
  if (p_fall_queue->len != 1)
  {
    for (i = 0; i <=p_fall_queue->len - 1; i++)
    {
      fall_queue_insert(p_fall_queue, p_data);
    }
  }
  Serial.println("insert ok:");
}


