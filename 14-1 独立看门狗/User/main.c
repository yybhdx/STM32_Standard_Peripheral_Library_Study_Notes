#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "LED.h"
#include "Key.h"
#include "OLED.h"

uint8_t KeyNum;		//定义用于接收按键键码的变量


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


	// LCD模块初始化
	OLED_Init(); // 初始化OLED显示屏
	
	// 按键初始化PB1 和 PB11
	Key_Init();
	
	// 指示一下这个是独立看门狗的测试程序
	OLED_ShowString(1, 1, "IWDG TEST");
	
	// 获取 STM32 RCC（复位和时钟控制）模块的各种状态标志 
	// 读取 RCC 模块的状态标志RCC_FLAG_IWDGRST 查看本次复位是否是独立看门狗导致的
	if(RCC_GetFlagStatus(RCC_FLAG_IWDGRST) == SET) // SET：标志位被设置（1），说明本次复位是独立看门狗导致的
	{
		OLED_ShowString(2, 1, "IWDGRST"); // 发生此现象说明本次复位是独立看门狗导致的
		
		Delay_ms(500);
		OLED_ShowString(2, 1, "       "); // 清除上次发生的现象
		Delay_ms(100);
		
		// 清除复位标志位
		// 清除RCC_FLAG_IWDGRST 独立看门狗复位标志
		// 一定要主动清除!!因为根据实测:RCC_FLAG_IWDGRST标志位即使按下复位键，也不会自动清0
		// 如果置1后不给它清0。那下次，即使是正常的复位键复位，他也会判断为看门狗复位
		RCC_ClearFlag();
	}
	else{ // 不满足，说明这只是一次普通的复位
		OLED_ShowString(3, 1, "RST"); // 发生此现象说明本次复位只是一次普通的复位
		
		Delay_ms(500);
		OLED_ShowString(3, 1, "   "); // 清除上次发生的现象		
		Delay_ms(100);
	}
	
	/*独立看门狗的配置流程*/
	// 1.开启独立看门狗的LSI时钟(不需要我们写代码)
	// 独立看门狗使用的是LSI时钟，当我们开启了独立看门狗，那么LSI就会跟随强制打开。
	// 等LSI稳定后，就可以自动为独立看门狗提供时钟了
	
	// 2. 写入键寄存器，解除写保护。
	// 启用或禁用对 IWDG 寄存器的写访问权限
	// 启用对 IWDG 寄存器的写访问权限 IWDG_WriteAccess_Enable：启用写访问（通常值为 0x5555）
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	
	// 3. 写入预分频器(IWDG_PG)配置预分频值，写入重装寄存器(IWDG_RLR)，配置重装值
	
	// 设置超时时间为1000ms(根据项目要求改变S)此时预分频为16分频
	IWDG_SetPrescaler(IWDG_Prescaler_16);
	
	// 设置 IWDG 的重载值（Reload Value），用于确定看门狗的超时时间
	// 根据公式的计算 得到重载值(公式中的RL) 为2499 这样超时时间就变为1000ms了
	IWDG_SetReload(2499);
	
	// 4.启动独立看门狗
	
	// 启动之前先喂一次狗
	// 重载 IWDG 计数器（俗称"喂狗"），防止看门狗超时复位系统
	IWDG_ReloadCounter();
	
	// 启用并启动独立看门狗计数器
	// 同时，喂狗(IWDG_SetPrescaler)或使能(IWDG_Enable)的时候，会在键寄存器写入5555之外的值
	// 这时又顺便给寄存器写保护了
	// 所以在写完寄存器之后 我们就不需要再手动执行写保护了
	IWDG_Enable();
	// 5.在主循环中不断喂狗
	
	

	while (1)
	{
		Key_GetNum(); // 按下按键不放 会使程序阻塞无法喂狗 独立看门狗实现复位
		
		// 在主循环中不断喂狗
		IWDG_ReloadCounter();
		
		// feed闪烁
		OLED_ShowString(4, 1, "Feed"); // 显示Feed 说明喂狗
		Delay_ms(500);
		OLED_ShowString(4, 1, "    "); // 清除Feed
		Delay_ms(200);
		
		
//		// 我的程序中:若延时>993 则会触发独立看门狗复位
//		Delay_ms(993);
	}
}
