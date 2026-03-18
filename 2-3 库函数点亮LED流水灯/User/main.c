#include "stm32f10x.h"                  // Device header


void delay_ms(u16 time)
{    
   u16 i=0;  
   while(time--)
   {
      i=12000;  //自己定义
      while(i--) ;    
   }
 }


int main()
{
	// PA0低电平点亮外置LED
	
	// 开启GPIOA对应的APB2外设总线的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	// 熄灭LED 高电平熄灭
	GPIO_SetBits(GPIOA, GPIO_Pin_0);
	
	// 点亮LED 低电平点亮
//	GPIO_ResetBits(GPIOA, GPIO_Pin_0);
	
//	GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_SET); // Bit_SET:1 高电平
	
//	GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_RESET); // Bit_RESET:0 低电平
	
	GPIO_Write(GPIOA, 0xFF);
	
	while(1)
	{
//		for(int i = 0; i < 4; i++)
//		{
//			GPIO_Write(GPIOA, 0xFE << i);
//			delay_ms(500);
//			GPIO_Write(GPIOA, 0xFF);
//			delay_ms(500);
//		}
		
	}
	
}



