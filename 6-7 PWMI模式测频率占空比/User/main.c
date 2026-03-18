#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "PWM.h"
#include "IC.h"

uint8_t i;			//定义for循环的变量

int main(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);		//开启GPIOB的时钟
	
	/*GPIO初始化*/
	GPIO_InitTypeDef GPIOB_InitStructure;
	GPIOB_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIOB_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIOB_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIOB_InitStructure);		
	// PB6默认低电平 PB7默认高电平给OLDE供电
	GPIO_ResetBits(GPIOB, GPIO_Pin_6);
	GPIO_SetBits(GPIOB, GPIO_Pin_7);
	
	/*模块初始化*/
	OLED_Init();		//OLED初始化
	
	OLED_Clear();
	
	PWM_Init();			//PWM初始化
	
	/*输入捕获初始化*/
	IC_Init();
	
	/*OLED显示频率*/
	OLED_ShowString(1,1, "Freq:00000Hz");
	
	OLED_ShowString(2,1, "Duty:00%");
	
	// 通过设置预分频器值与占空比，PWM模块已经将待测信号输出到PA0了
	// PA0又通过导线，输入到PA6
	// PA6是TIM3的通道1，通道1通过输入捕获模块，测量得到频率
	// 然后在主循环里，我们不断刷新显示频率
	/*设置预分频器值:720-1(其实是720)*/ // Freq(频率) = 72M / (PSC + 1) / ARR
	PWM_SetPrescaler(720-1);
	
	/*设置占空比*/
	PWM_SetCompare1(80); // Duty = CCR / ARR(我们设置的是100)
	while (1)
	{
		/*OLED显示频率*/
		OLED_ShowNum(1,6, IC_GetFreq(), 5); // 01001Hz
		
		OLED_ShowNum(2,6, IC_GetDuty(), 2); // 显示的值与我PWM_SetCompare1(80);函数设置的占空比一样
	}
}
