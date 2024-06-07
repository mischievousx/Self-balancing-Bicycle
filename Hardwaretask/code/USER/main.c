#include "sys.h"
                                          
int main(void)
{  
		NVIC_Config();                         //�����ж����ȼ�            
   	delay_init();                 
    //Led_Init();                            //��ʼ��LED 
    //Beep_Init();                           //��ʼ�������� 
    //Wave_Init();                           //��ʼ�������� 
   	//uart3_init(9600);                      //��ʼ������3��������
		OLED_Init();			               //��ʼ��OLED  
    OLED_Clear();                          //OLED����
		adc_Init();                            //��ʼ��ADC 
    //MOTOR_1_Init();                        //��ʼ�������ֵ������������������ת������
    //MOTOR_2_Init();                        //��ʼ���˶��������������ת������
    //PWM_Init_TIM3(7199,0);                 //��ʱ��3��ʼ��PWM 10KHZ���������������ֵ�����˶���� 
    PWM_Init_TIM2(9999, 143);              //��ʱ��2��ʼ��PWM 50HZ�������������
    TIM_SetCompare1(TIM2, 780);            //�����λ�������ô�װ�벶��ȽϼĴ���������ֵ��
    //Init_TIM1(9999,7199);                  //��ʱ��1��ʼ�������ڳ�������࣬ÿ��һ����Ϊ100us������T=1s  
    //Encoder_Init_TIM4(65535,0);
    OLED_ShowString(25,4,"MPU6050...",16);
    MPU_Init();                            //MPU6050��ʼ��
    while(mpu_dmp_init())
		{
			OLED_ShowString(25,4,"MPU6050 Error",16);
		}  
			OLED_ShowString(25,4,"MPU6050 OK!",16); 
    //Beep=1;
    delay_ms(400);
    //Beep=0;
    MPU6050_EXTI_Init();                          //MPU6050�ⲿ�жϳ�ʼ��
    OLED_Clear();
		OLED_ShowString(0,0,"Roll :         C",16);
    OLED_ShowString(0,3,"Speed:         R ",16);
    OLED_ShowString(0,6,"Power:        V ",16);    
    
    while(1)
    {	        
        //Wave_Strat();                             //��������������
        //ADCֵ��ΧΪ��0-2^12=4095��111111111111��һ������¶�Ӧ��ѹΪ0-3.3V
        AdcValue=11.09*(3.3*Get_adc_Average(ADC_Channel_4,10)/0x0fff); 
        
        OLED_Showdecimal(55,0,Roll,9,16);                 //��ʾ��̬�Ƕ�Roll
        //OLED_Showdecimal(55,3,Encoder_Motor*0.25,9,16);   //��ʾ������ת��
        OLED_Showdecimal(50,6,AdcValue,9,16);             //��ʾ��صĵ�ǰ��ѹֵ
	}
}
