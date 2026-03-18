#include "stm32f10x.h"                  // Device header


// 实时时钟RTC初始化
void MyRTC_Init(void)
{
	// 1.开启PWR和BKP的时钟，使能BKP和RTC的访问
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP, ENABLE);
	
	// PWR_BackupAccessCmd()使能或禁止对备份域（Backup Domain） 的访问权限
	// 使能对RTC和备份寄存器访问权限
	PWR_BackupAccessCmd(ENABLE);
	
	// 2.启动RTC的时钟(计划使用LSE),并等待LSE时钟启动完成
	
	// 为实时时钟（RTC）开启外部低速晶振（LSE）
	// RCC_LSEConfig():配置外部低速晶振（LSE）
  // RCC_LSE_ON：开启LSE振荡器
	RCC_LSEConfig(RCC_LSE_ON);
	
	// 等待LSE时钟启动完成
	// RCC_GetFlagStatus() 检查复位与时钟控制单元（RCC）中各种状态标志位的当前状态
	// RCC_FLAG_LSERDY:外部低速时钟（LSE）就绪
	while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) != SET); //如果返回标志位状态为RESET，则未置为完成，继续等待
	
	// 3.配置RTCCLK这个数据选择器，为RTCCLK选择时钟源并使能时钟
	// RCC_RTCCLKConfig()选择实时时钟 (RTC) 的时钟源
	// RCC_RTCCLKSource_LSE:选择外部低速晶振 (LSE，通常为 32.768kHz) 作为 RTC 时钟
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
	
	
	// RCC_RTCCLKCmd():使能或失能RTC（实时时钟）的时钟
	// ENABLE:使能RTC时钟
	RCC_RTCCLKCmd(ENABLE);
	
	// 4.调用两个等待函数 第一个:等待同步 第二个:等待上一次操作完成
	
	// RTC_WaitForSynchro()和RTC_WaitForSynchro();函数里面自带了while死循环等待的功能，所以我们直接调用函数就可以实现等待的效果
	// 这两行代码是一个安全保障措施，可以防止因为时钟不同步，造成bug
	
	// 等待同步
	// RTC_WaitForSynchro():等待RTC寄存器与APB1总线时钟同步
	RTC_WaitForSynchro();

	// 等待上一次操作完成
	// RTC_WaitForLastTask()等待RTC（实时时钟）模块完成上一次对寄存器的写操作
	RTC_WaitForLastTask();
	
	// 5.配置预分频器，给PRL重装寄存器一个合适的分频值，以确保输出给计数器的频率是1HZ
	// RTC_SetPrescaler():设置RTC（实时时钟）的预分频器值 此库函数和SetCounter，SetAlarm一样，内置进入配置模式的代码，之后也自带了退出配置模式的代码
	// HSE的频率为32.768KHZ 进行32768分频，就是1HZ了。所以分频系数我们给32768-1
	RTC_SetPrescaler(32768-1);
	
	// 调用等待函数，这个等待函数，我们可以在每次写入操作之后调用一次。也可以在每次写入操作之前调用一次。或者写入之前之后都调用一次，这样更保险
	// 等待上一次操作完成
	// RTC_WaitForLastTask()等待RTC（实时时钟）模块完成上一次对寄存器的写操作
	RTC_WaitForLastTask();

	// 6.配置CNT的值，给这个RTC一个初始时间
	
	// RTC_SetCounter() 设置RTC（实时时钟）内部32位计数器的值，从而直接设定当前的时间
	// 参数CounterValue：要设置的32位无符号整型时间值，通常是以秒为单位的Unix时间戳（自1970年1月1日以来的秒数）。
	// 时间戳为1672588795，若不调用此函数预设时间，那么CNT默认为0，time.h里的函数，解析出来就是1970年
	RTC_SetCounter(1672588795);
	
	// 调用等待函数，这个等待函数，我们可以在每次写入操作之后调用一次。也可以在每次写入操作之前调用一次。或者写入之前之后都调用一次，这样更保险
	// 等待上一次操作完成
	// RTC_WaitForLastTask()等待RTC（实时时钟）模块完成上一次对寄存器的写操作
	RTC_WaitForLastTask();
	
}
