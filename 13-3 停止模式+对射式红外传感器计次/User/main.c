#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "CountSensor.h"

int main(void)
{
	/*模块初始化*/
	OLED_Init();			//OLED初始化
	CountSensor_Init();		//计数传感器初始化
	
	// 开启PWR的时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	
	/*显示静态字符串*/
	OLED_ShowString(1, 1, "Count:");	//1行1列显示字符串Count:
	
	while (1)
	{
		OLED_ShowNum(1, 7, CountSensor_Get(), 5);		//OLED不断刷新显示CountSensor_Get的返回值
		
		OLED_ShowString(2, 1, "Running");
		Delay_ms(100);
		OLED_ShowString(2, 1, "        ");
		Delay_ms(100);
		
		// PWR_EnterSTOPMode()让微控制器进入停止模式
		// PWR_Regulator: 设置停止模式下电压调节器的工作状态 // o	PWR_Regulator_ON：电压调节器保持开启。唤醒速度更快，但功耗相对较高。
		// PWR_STOPEntry: 选择进入停止模式的具体方式（WFI 或 WFE）// o	使用__WFI()指令进入。可被任意一个使能的EXTI中断唤醒。
		PWR_EnterSTOPMode(PWR_Regulator_ON, PWR_STOPEntry_WFI);
		
		SystemInit();
	}
}
