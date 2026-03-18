#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Timer.h"

uint16_t Num;			//定义在定时器中断里自增的变量

int main(void)
{
	// OLED供电
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
	Timer_Init();		//定时中断初始化
	
	/*显示静态字符串*/
	OLED_ShowString(1, 1, "Num:");			//1行1列显示字符串Num:
	
	// 在第二行 第一列显示计数器的变化情况(我们设的计数最大值为9999 到10000时置0,num++)
	OLED_ShowString(2, 1, "CNT:"); // 在指定位置显示字符串	
	
	while (1)
	{
		OLED_ShowNum(1, 5, Num, 5);			//不断刷新显示Num变量
		OLED_ShowNum(2, 5, TIM_GetCounter(TIM2), 5); // 二行五列选择TIM2计数器的值
	}
}

///**
//  * 函    数：TIM2中断函数
//  * 参    数：无
//  * 返 回 值：无
//  * 注意事项：此函数为中断函数，无需调用，中断触发后自动执行
//  *           函数名为预留的指定名称，可以从启动文件复制
//  *           请确保函数名正确，不能有任何差异，否则中断函数将不能进入
//  */
//void TIM2_IRQHandler(void)
//{
//	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)		//判断是否是TIM2的更新事件触发的中断
//	{
//		Num ++;												//Num变量自增，用于测试定时中断
//		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);			//清除TIM2更新事件的中断标志位
//															//中断标志位必须清除
//															//否则中断将连续不断地触发，导致主程序卡死
//	}
//}
