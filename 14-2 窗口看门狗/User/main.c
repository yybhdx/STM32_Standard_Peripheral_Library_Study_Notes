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
	
	// 指示一下这个是窗口看门狗的测试程序
	OLED_ShowString(1, 1, "WWDG TEST");
	
	// 获取 STM32 RCC（复位和时钟控制）模块的各种状态标志 
	// 读取 RCC 模块的状态标志RCC_FLAG_WWDGRST 查看本次复位是否是窗口看门狗导致的
	if(RCC_GetFlagStatus(RCC_FLAG_WWDGRST) == SET) // SET：标志位被设置（1），说明本次复位是窗口看门狗导致的
	{
		OLED_ShowString(2, 1, "WWDGRST"); // 发生此现象说明本次复位是窗口看门狗导致的
		
		Delay_ms(500);
		OLED_ShowString(2, 1, "       "); // 清除上次发生的现象
		Delay_ms(100);
		
		// 清除复位标志位
		// 清除RCC_FLAG_WWDGRST 窗口看门狗复位标志
		// 一定要主动清除!!因为根据实测:RCC_FLAG_WWDGRST标志位即使按下复位键，也不会自动清0
		// 如果置1后不给它清0。那下次，即使是正常的复位键复位，他也会判断为看门狗复位
		RCC_ClearFlag();
	}
	else{ // 不满足，说明这只是一次普通的复位
		OLED_ShowString(3, 1, "RST"); // 发生此现象说明本次复位只是一次普通的复位
		
		Delay_ms(500);
		OLED_ShowString(3, 1, "   "); // 清除上次发生的现象		
		Delay_ms(100);
	}
	
	/*窗口看门狗的配置流程*/
	// 1.开启窗口看门狗的APB1时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);
	
	// 2.配置预分频寄存器和窗口值
	// 窗口看门狗没有写保护 所以不需要再像独立看门狗那样解除写保护了
	
	// 设置窗口看门狗（WWDG）的预分频器
	// 预分频系数选择8
	WWDG_SetPrescaler(WWDG_Prescaler_8);
	
	// 设置窗口看门狗（WWDG）的窗口值 
	// 设置窗口时间为30ms
	// 运算得出得出窗口值W[5:0] = 21 W6这一位也需要置1 所以需要21 | 0x40
	WWDG_SetWindowValue(21 | 0x40);
	
	// 3.写入控制寄存器CR(包含看门狗使能位，计数器溢出标志位和计数器有效位。这些需要一起设置)
	
	// 启用并启动窗口看门狗（WWDG） 
	// 设置超时时间为50ms
	// 因此我们主循环的喂狗周期要在30ms到50ms之间
	// 运算得出T[5:0]=54 T6位又要设置为1 所以需要54 | 0x40 或者 54 + 0x40
	WWDG_Enable(54 | 0x40);
//	WWDG_Enable(54 + 0x40);
	
	// 4.在运行过程中 我们不断向计数器写入想要的重装值。这样就可以进行喂狗了。
	
	
	
	

	while (1)
	{
		Key_GetNum(); // 按下按键不放 会使程序阻塞无法喂狗 独立看门狗实现复位
		
		
		// feed闪烁
		OLED_ShowString(4, 1, "Feed"); // 显示Feed 说明喂狗
		Delay_ms(20);
		OLED_ShowString(4, 1, "    "); // 清除Feed
		Delay_ms(20);
		
		// 对窗口看门狗进行喂狗
		// 运算得出T[5:0]=54 T6位又要设置为1 所以需要54 | 0x40 或者 54 + 0x40
		WWDG_SetCounter(54 | 0x40);
		
	}
}
