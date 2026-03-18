#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "LED.h"
#include "Key.h"

uint8_t KeyNum;
int main(void)
{
	led_init();
	key_init();
	
	LED1_OFF;
	LED2_OFF;
	while(1)
	{
		// 不断读取键码值
		KeyNum = Key_GetNum();
		
		// PB1按键抬起后
		if(KeyNum == 1)
		{
			LED1_Turn();
		}
		// PB11按键抬起后
		if(KeyNum == 2)
		{
			LED2_Turn();
		}
//		LED1_ON;
//		Delay_ms(500);
//		LED1_OFF;
//		Delay_ms(500);
//		
//		LED2_ON;
//		Delay_ms(500);
//		LED2_OFF;
//		Delay_ms(500);
		
	}
	
}
//int main(void)
//{
//	/*开启时钟*/
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//开启GPIOB的时钟
//															//使用各个外设前必须开启时钟，否则对外设的操作无效
//	
//	/*GPIO初始化*/
//	GPIO_InitTypeDef GPIO_InitStructure;					//定义结构体变量
//	
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		//GPIO模式，赋值为推挽输出模式
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;				//GPIO引脚，赋值为第12号引脚
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//GPIO速度，赋值为50MHz
//	
//	GPIO_Init(GPIOB, &GPIO_InitStructure);					//将赋值后的构体变量传递给GPIO_Init函数
//															//函数内部会自动根据结构体的参数配置相应寄存器
//															//实现GPIOB的初始化
//	
//	/*主循环，循环体内的代码会一直循环执行*/
//	while (1)
//	{
//		GPIO_ResetBits(GPIOB, GPIO_Pin_12);		//将PB12引脚设置为低电平，蜂鸣器鸣叫
//		Delay_ms(100);							//延时100ms
//		GPIO_SetBits(GPIOB, GPIO_Pin_12);		//将PB12引脚设置为高电平，蜂鸣器停止
//		Delay_ms(100);							//延时100ms
//		GPIO_ResetBits(GPIOB, GPIO_Pin_12);		//将PB12引脚设置为低电平，蜂鸣器鸣叫
//		Delay_ms(100);							//延时100ms
//		GPIO_SetBits(GPIOB, GPIO_Pin_12);		//将PB12引脚设置为高电平，蜂鸣器停止
//		Delay_ms(700);							//延时700ms
//	}
//}
