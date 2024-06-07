#ifndef __CONTROL_H 
#define __CONTROL_H 
#include "sys.h" 

typedef struct PID_stc
{
s16 target;
s16 actual;
float Kp;
float Ki;
float Kd;
float dt;
s32 previous_error;
s32 integral;
} PID_stc;

void PID_init(PID_stc *, float, float, float, float);

int Vertical(float Med,float Angle,float gyro_x);
int Velocity(int encoder_motor);
void EXTI9_5_IRQHandler(void);
#endif