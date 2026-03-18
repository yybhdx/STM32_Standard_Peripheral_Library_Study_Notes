#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"

int main(void)
{
	/*模块初始化*/
	OLED_Init();		//OLED初始化
	
	// 读取F_SIZE
	OLED_ShowString(1, 1, "F_SIZE");
	
	OLED_ShowHexNum(1, 8,  *((__IO uint16_t *)(0X1FFFF7E0)), 4); // 0080
	
	// 读取UID
	OLED_ShowString(2, 1, "U_ID");
	
	OLED_ShowHexNum(2, 6,  *((__IO uint16_t *)(0X1FFFF7E8)), 4); // 0240
	OLED_ShowHexNum(2, 11, *((__IO uint16_t *)(0X1FFFF7E8 + 0X02)), 4); // 0253
	OLED_ShowHexNum(3, 1, *((__IO uint32_t *)(0X1FFFF7E8 + 0X04)), 8); // 52310023
	OLED_ShowHexNum(4, 1, *((__IO uint32_t *)(0X1FFFF7E8 + 0X08)), 8); // 3638414B
	while (1)
	{
		
	}
}
