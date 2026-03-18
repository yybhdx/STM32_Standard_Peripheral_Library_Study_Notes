#include "stm32f10x.h" // Device header
#include "Delay.h"
#include "OLED.h"
#include "Motor.h"
#include "Key.h"

uint8_t KeyNum; // 瀹氫箟for寰?鐜?鐨勫彉閲?

int8_t Speed;

int main(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); // 寮�鍚疓PIOB鐨勬椂閽?

	/*GPIO鍒濆?嬪寲*/
	GPIO_InitTypeDef GPIOB_InitStructure;
	GPIOB_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIOB_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIOB_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIOB_InitStructure);
	// PB6榛樿?や綆鐢靛钩 PB7榛樿?ら珮鐢靛钩缁橭LDE渚涚數
	GPIO_ResetBits(GPIOB, GPIO_Pin_6);
	GPIO_SetBits(GPIOB, GPIO_Pin_7);

	/*妯″潡鍒濆?嬪寲*/
	OLED_Init(); // OLED鍒濆?嬪寲

	/*初始化按键*/
	Key_Init();

	/*初始化电机驱动*/
	Motor_Init();

	// /*设置速度为正转50*/
	// Motor_SetSpeed(-20);

	/*OLED 1行1列显示速度字符*/
	OLED_ShowString(1, 1 ,"Speed");

	while (1)
	{
		/*读取按键状态*/
		KeyNum = Key_GetNum();

		/*如果按键PB1按下*/
		if (KeyNum == 1)
		{
			/*按键速度+20*/
			Speed += 20;

			/*如果Speed速度>100*/
			if (Speed > 100)
			{
				/*电机反转*/
				Speed = -100;
			}
		}
		Motor_SetSpeed(Speed);

		/*OLED1行7列显示实时速度
		长度为3*/
		OLED_ShowSignedNum(1, 7, Speed, 3);
	}
}
