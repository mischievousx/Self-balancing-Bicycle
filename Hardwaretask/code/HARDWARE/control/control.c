#include "control.h" 

float Pitch,Roll,Yaw;						        //角度
short aacx,aacy,aacz;		                        //角加速度
short gyrox,gyroy,gyroz;	                        //角速度

int Vertical_out,Velocity_out;                      //直立环&速度环的输出变量
int PWM1;                                           //最终输出
int Encoder_Motor;	                                //动量轮驱动电机编码器数据（速度）

float filter_angle =0.0;
float alpha = 0.98;
float Med_Angle=2.0;	                                //机械中值---在这里修改你的机械中值即可。
float Vertical_Kp=-5,Vertical_Kd=-0.1;                  //直立环KP、KD
float previous_error = 0.0;
float integral = 0.0;
float Velocity_Kp=-58,Velocity_Ki=-0.25;                  //速度环KP、KI


void EXTI9_5_IRQHandler(void)                       //MPU6050外部中断服务函数，10ms触发一次
{
	if(EXTI_GetITStatus(EXTI_Line5)!=0)                 //一级判定
	{
		if(PBin(5)==0)                                  //二级判定
		{           
			EXTI_ClearITPendingBit(EXTI_Line5);         //清除中断标志位
            
			Encoder_Motor=Read_Encoder_TIM4();          //1.采集编码器数据&MPU6050角度信息。
			
			mpu_dmp_get_data(&Pitch,&Roll,&Yaw);		//角度
			MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//陀螺仪角速度
			MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//角加速度
      
			
      filter_angle= Roll*alpha + gyrox *(1-alpha);
      Vertical_out=Vertical(Med_Angle,filter_angle,gyrox);//直立环
      //Velocity_out=Velocity(Encoder_Motor);	    //速度环
            
			//2.把控制输出量加载到电机上，完成最终的的控制。
            //PWM1=Vertical_out+Velocity_out;            //最终输出
						PWM1 = 790 + Vertical_out;
			//Limit(&PWM1);	                           //PWM限幅			
			if(PWM1 > 1250)
			{
				PWM1 = 1250;
			};
			if(PWM1 < 250)
			{
				PWM1 = 250;
			}
			TIM_SetCompare1(TIM2, PWM1);
			//Load(PWM1);		                           //加载到电机上。
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
直立环PD控制器：Kp*Ek+Kd*Ek_D
入口：期望角度、真实角度、真实角速度
出口：直立环输出
*********************/
int Vertical(float Med,float Angle,float gyro_x)
{
	int PWM_out1;
	PWM_out1=Vertical_Kp*(Med-Angle)-Vertical_Kd*(gyro_x);
	return PWM_out1;
}


/*********************
速度环PI：Kp*Ek+Ki*Ek_S
*********************/
/*
int Velocity (int Encoder_motor)
{
	static int Encoder_S,EnC_Err_Lowout_last,PWM_out2,Encoder_Err,EnC_Err_Lowout;
	float a=0.7;
	
	Encoder_Err=Encoder_motor;	                             //1.计算速度偏差
    
	//2.对速度偏差进行低通滤波:low_out=(1-a)*Ek+a*low_out_last
	EnC_Err_Lowout=(1-a)*Encoder_Err+a*EnC_Err_Lowout_last; //使得波形更加平滑，滤除高频干扰，防止速度突变。
	EnC_Err_Lowout_last=EnC_Err_Lowout;                     //防止速度过大的影响直立环的正常工作。
    
	Encoder_S+=EnC_Err_Lowout;                                               //3.对速度偏差积分，积分出位移
	Encoder_S=Encoder_S>10000?10000:(Encoder_S<(-10000)?(-10000):Encoder_S);//4.积分限幅
    
	PWM_out2=Velocity_Kp*EnC_Err_Lowout+Velocity_Ki*Encoder_S;	            //5.速度环控制输出计算
	return PWM_out2;
}

*/

