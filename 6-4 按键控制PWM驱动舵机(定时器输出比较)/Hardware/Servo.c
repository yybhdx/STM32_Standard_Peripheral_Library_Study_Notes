#include "stm32f10x.h"                  // Device header
#include "PWM.h"

/**
 * @brief   PWM初始化函数
 * @param   无
 * @return  无
 */
void Servo_Init(void)
{
	
	// PWM初始化
	pwm_init();
}

/**
 * @brief   舵机设置角度
 * @param   Angele
 * @return  无
 */

// 0 - 180°
//500(对应0.5ms 0转动)
//1000(对应1.0ms 45°转动)
//1500(对应1.5ms 90°转动)
//2000(对应2.0ms 135°转动)
//2500(对应2.5ms 180°转动)
void Servo_SetAngel(float Angele)
{
	pwm_Tim_SetCompare2(Angele / 180 * 2000 + 500);
}
