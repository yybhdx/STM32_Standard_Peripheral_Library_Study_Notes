#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "MyRTC.h"

int main(void)
{
	/*模块初始化*/
	OLED_Init();		//OLED初始化
	MyRTC_Init();		//RTC初始化
	
	// 使用待机模式，开启PWR的时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);		//开启PWR的时钟

	/*显示静态字符串*/
	OLED_ShowString(1, 1, "CNT:"); // 显示CNT秒计数器
	OLED_ShowString(2, 1, "ALR:"); // 显示ALR闹钟值
	OLED_ShowString(3, 1, "ALRF:"); // 显示ALRF闹钟标志位
	
	// 设置闹钟为10秒后
	// RTC_SetAlarm();设置RTC（实时时钟）的闹钟寄存器值
	// 参数为Alarm：一个32位无符号整数，代表闹钟触发的时间点，通常是以秒为单位的Unix时间戳。
	uint32_t Alarm = RTC_GetCounter();
	RTC_SetAlarm(Alarm + 10);
	OLED_ShowNum(2, 6, Alarm, 10);
	
	while (1)
	{
		// RTC_GetCounter()，读取RTC（实时时钟）中32位计数器的当前值
		OLED_ShowNum(1, 6, RTC_GetCounter(), 10);	//显示32位的秒计数器
		
		// RTC_GetFlagStatus()检查指定的RTC（实时时钟）标志位是否被设置
		// RTC_FLAG_ALR:闹钟标志，当RTC计数器的值达到预设的闹钟值（RTC_ALR）时，此标志被置位。
		OLED_ShowNum(3, 6, RTC_GetFlagStatus(RTC_FLAG_ALR), 1);
		
		OLED_ShowString(4, 1, "Running");
		Delay_ms(100);
		OLED_ShowString(4, 1, "       ");
		Delay_ms(100);
		
		// PWR_WakeUpPinCmd()管理唤醒引脚（Wake-up Pin）
		PWR_WakeUpPinCmd(ENABLE);
		
		OLED_ShowString(4, 9, "STANDBY");
		Delay_ms(1000);
		OLED_ShowString(4, 9, "       ");
		Delay_ms(100);
		
		// 清屏
		OLED_Clear();

		// PWR_EnterSTANDBYMode()让微控制器进入待机模式
		PWR_EnterSTANDBYMode();
	}
}
