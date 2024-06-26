#include "beep.h"
void Beep_Init(void)
{	
	GPIO_InitTypeDef GPIO_InitStructure;                  //定义一个引脚初始化的结构体
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //使能GPIOA时钟，GPIOA挂载在APB2时钟下，在STM32中使用IO口前都要使能对应时钟
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11;              //引脚11
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;        //引脚输入输出模式为推挽输出模式
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;       //引脚输出速度为50MHZ
	GPIO_Init(GPIOA, &GPIO_InitStructure);                //根据上面设置好的GPIO_InitStructure参数，初始化引脚GPIOA_PIN11		
}