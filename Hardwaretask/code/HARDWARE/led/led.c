#include "led.h" 
#include "stm32f10x_gpio.h" 

//LEDӲ����ʼ����������
void Led_Init(void)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;                  //����һ�����ų�ʼ���Ľṹ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //ʹ��GPIOAʱ�ӣ�GPIOA������APB2ʱ���£���STM32��ʹ��IO��ǰ��Ҫʹ�ܶ�Ӧʱ��
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8|GPIO_Pin_12; 
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;        //�����������ģʽΪ�������ģʽ
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;       //��������ٶ�Ϊ50MHZ
	GPIO_Init(GPIOA, &GPIO_InitStructure); 
	
	GPIO_SetBits(GPIOA, GPIO_Pin_12);                     //��ʼ����������GPIOA12Ϊ�ߵ�ƽ
  GPIO_SetBits(GPIOA, GPIO_Pin_8);                      //��ʼ����������GPIOA8Ϊ�ߵ�ƽ
}