#include "control.h" 

float Pitch,Roll,Yaw;						        //�Ƕ�
short aacx,aacy,aacz;		                        //�Ǽ��ٶ�
short gyrox,gyroy,gyroz;	                        //���ٶ�

int Vertical_out,Velocity_out;                      //ֱ����&�ٶȻ����������
int PWM1;                                           //�������
int Encoder_Motor;	                                //����������������������ݣ��ٶȣ�

float filter_angle =0.0;
float alpha = 0.98;
float Med_Angle=2.0;	                                //��е��ֵ---�������޸���Ļ�е��ֵ���ɡ�
float Vertical_Kp=-5,Vertical_Kd=-0.1;                  //ֱ����KP��KD
float previous_error = 0.0;
float integral = 0.0;
float Velocity_Kp=-58,Velocity_Ki=-0.25;                  //�ٶȻ�KP��KI


void EXTI9_5_IRQHandler(void)                       //MPU6050�ⲿ�жϷ�������10ms����һ��
{
	if(EXTI_GetITStatus(EXTI_Line5)!=0)                 //һ���ж�
	{
		if(PBin(5)==0)                                  //�����ж�
		{           
			EXTI_ClearITPendingBit(EXTI_Line5);         //����жϱ�־λ
            
			Encoder_Motor=Read_Encoder_TIM4();          //1.�ɼ�����������&MPU6050�Ƕ���Ϣ��
			
			mpu_dmp_get_data(&Pitch,&Roll,&Yaw);		//�Ƕ�
			MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//�����ǽ��ٶ�
			MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//�Ǽ��ٶ�
      
			
      filter_angle= Roll*alpha + gyrox *(1-alpha);
      Vertical_out=Vertical(Med_Angle,filter_angle,gyrox);//ֱ����
      //Velocity_out=Velocity(Encoder_Motor);	    //�ٶȻ�
            
			//2.�ѿ�����������ص�����ϣ�������յĵĿ��ơ�
            //PWM1=Vertical_out+Velocity_out;            //�������
						PWM1 = 790 + Vertical_out;
			//Limit(&PWM1);	                           //PWM�޷�			
			if(PWM1 > 1250)
			{
				PWM1 = 1250;
			};
			if(PWM1 < 250)
			{
				PWM1 = 250;
			}
			TIM_SetCompare1(TIM2, PWM1);
			//Load(PWM1);		                           //���ص�����ϡ�
			//Stop(&Med_Angle,&Roll);		            
		}
	}
}

float PID_Compute(float setpoint, float measurement, float rate) {
    float error = setpoint - measurement;
    float output = Vertical_Kp * error + Vertical_Kd * rate;

    return output;
}

/*********************
ֱ����PD��������Kp*Ek+Kd*Ek_D
��ڣ������Ƕȡ���ʵ�Ƕȡ���ʵ���ٶ�
���ڣ�ֱ�������
*********************/
int Vertical(float Med,float Angle,float gyro_x)
{
	int PWM_out1;
	PWM_out1=Vertical_Kp*(Med-Angle)-Vertical_Kd*(gyro_x);
	return PWM_out1;
}


/*********************
�ٶȻ�PI��Kp*Ek+Ki*Ek_S
*********************/
/*
int Velocity (int Encoder_motor)
{
	static int Encoder_S,EnC_Err_Lowout_last,PWM_out2,Encoder_Err,EnC_Err_Lowout;
	float a=0.7;
	
	Encoder_Err=Encoder_motor;	                             //1.�����ٶ�ƫ��
    
	//2.���ٶ�ƫ����е�ͨ�˲�:low_out=(1-a)*Ek+a*low_out_last
	EnC_Err_Lowout=(1-a)*Encoder_Err+a*EnC_Err_Lowout_last; //ʹ�ò��θ���ƽ�����˳���Ƶ���ţ���ֹ�ٶ�ͻ�䡣
	EnC_Err_Lowout_last=EnC_Err_Lowout;                     //��ֹ�ٶȹ����Ӱ��ֱ����������������
    
	Encoder_S+=EnC_Err_Lowout;                                               //3.���ٶ�ƫ����֣����ֳ�λ��
	Encoder_S=Encoder_S>10000?10000:(Encoder_S<(-10000)?(-10000):Encoder_S);//4.�����޷�
    
	PWM_out2=Velocity_Kp*EnC_Err_Lowout+Velocity_Ki*Encoder_S;	            //5.�ٶȻ������������
	return PWM_out2;
}

*/

