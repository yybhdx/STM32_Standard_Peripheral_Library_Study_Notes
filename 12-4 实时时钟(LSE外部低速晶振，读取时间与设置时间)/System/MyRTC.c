#include "stm32f10x.h"                  // Device header
#include <time.h>

// 定义数组存放年月日时分秒
uint16_t MyRTC_Time[] = {2023, 1, 1, 23, 59, 55}; // 因为2023超过了uint8_t的范围，所以我们直接整个数组都定义成16位的


void MyRTC_SetTime(void);

// 实时时钟RTC初始化
void MyRTC_Init(void)
{
	// 1.开启PWR和BKP的时钟，使能BKP和RTC的访问
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP, ENABLE);
	
	// PWR_BackupAccessCmd()使能或禁止对备份域（Backup Domain） 的访问权限
	// 使能对RTC和备份寄存器访问权限
	PWR_BackupAccessCmd(ENABLE);
	
	// 防止重复初始化和时间重置
	// 第一次上电时，或者系统完全断电之后，BKP_DR1默认是0，if成立，执行初始化,所以我们随便比较一个值即可
	if(BKP_ReadBackupRegister(BKP_DR1) != 0XA5A5)
	{
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

	//	// 6.配置CNT的值，给这个RTC一个初始时间
	//	
	//	// RTC_SetCounter() 设置RTC（实时时钟）内部32位计数器的值，从而直接设定当前的时间
	//	// 参数CounterValue：要设置的32位无符号整型时间值，通常是以秒为单位的Unix时间戳（自1970年1月1日以来的秒数）。
	//	// 时间戳为1672588795，若不调用此函数预设时间，那么CNT默认为0，time.h里的函数，解析出来就是1970年
	//	RTC_SetCounter(1672588795);
	//	
	//	// 调用等待函数，这个等待函数，我们可以在每次写入操作之后调用一次。也可以在每次写入操作之前调用一次。或者写入之前之后都调用一次，这样更保险
	//	// 等待上一次操作完成
	//	// RTC_WaitForLastTask()等待RTC（实时时钟）模块完成上一次对寄存器的写操作
	//	RTC_WaitForLastTask();
		
		
		MyRTC_SetTime();	
		
		// 初始化之后，向BKP_DR1里写入数据0XA5A5。下次读到是这个数，就说明初始化过了，并且中途备用电池没断电，这样就不用再重复初始化了，避免时间重置
		// 写入BKP_DR1置标志位
		// BKP_WriteBackupRegister()向STM32微控制器的BKP（备份）寄存器写入数据
		BKP_WriteBackupRegister(BKP_DR1, 0XA5A5);
		
	}
	// 如果已经初始化过了
	else
	{
		RTC_WaitForSynchro();
	  RTC_WaitForLastTask();
	}
	
}

// 设置时间函数
void MyRTC_SetTime(void)
{
	// 定义秒计数器
	time_t time_cnt;
	
	// 定义日期时间
	struct tm time_date;
	
	// 将数组的数据填充到结构体中
	time_date.tm_year = MyRTC_Time[0] - 1900; // 年减去1900
	time_date.tm_mon = MyRTC_Time[1] - 1; // 月减去1
	time_date.tm_mday = MyRTC_Time[2];
	time_date.tm_hour = MyRTC_Time[3];
	time_date.tm_min = MyRTC_Time[4];
	time_date.tm_sec = MyRTC_Time[5];
	
	// 将本地时间转换为时间戳并赋值给time_cnt
	// mktime():将一个 struct tm 类型的本地时间结构体，转换成对应的 time_t 类型的时间戳	
	time_cnt = mktime(&time_date) - 8 * 60 * 60;
	
	
	// RTC_SetCounter()设置RTC（实时时钟）内部32位计数器的值，从而直接设定当前的时间
	// CounterValue：要设置的32位无符号整型时间值，通常是以秒为单位的Unix时间戳（自1970年1月1日以来的秒数）
	RTC_SetCounter(time_cnt);

	// 调用等待函数，这个等待函数，我们可以在每次写入操作之后调用一次。也可以在每次写入操作之前调用一次。或者写入之前之后都调用一次，这样更保险
	// 等待上一次操作完成
	// RTC_WaitForLastTask()等待RTC（实时时钟）模块完成上一次对寄存器的写操作
	RTC_WaitForLastTask();
}

// 读取时间函数
void MyRTC_ReadTime(void)
{
	// 定义秒计数器
	time_t time_cnt;
	
	// 定义日期时间
	struct tm time_date;	
	
	// 伦敦时间
//	// 读取CNT的秒数，存储到time_cnt中
//	// RTC_GetCounter()读取RTC（实时时钟）中32位计数器的当前值
//	time_cnt = RTC_GetCounter();
	
	// 北京时间
	// 读取CNT的秒数，存储到time_cnt中
	// RTC_GetCounter()读取RTC（实时时钟）中32位计数器的当前值
	time_cnt = RTC_GetCounter() + 8 * 60 * 60;
	
	// localtime()把时间戳（time_t）转换成“本地时间结构体”
	// 返回值 struct tm *	返回一个结构体指针，包含拆分好的本地时间
	time_date = *localtime(&time_cnt);
	
	// 将time_date的日期时间，转移到数组里
	MyRTC_Time[0] = time_date.tm_year + 1900; 
	MyRTC_Time[1] = time_date.tm_mon + 1;
	MyRTC_Time[2] = time_date.tm_mday;
	MyRTC_Time[3] = time_date.tm_hour;
	MyRTC_Time[4] = time_date.tm_min;
	MyRTC_Time[5] = time_date.tm_sec;
}
