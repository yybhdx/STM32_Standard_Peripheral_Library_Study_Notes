#ifndef __LED_H
#define __LED_H

#define LED1_ON 	GPIO_ResetBits(GPIOA, GPIO_Pin_1);
#define LED1_OFF 	GPIO_SetBits(GPIOA, GPIO_Pin_1);
#define LED1_Toggle	GPIO_ToggleBits(GPIOA, GPIO_Pin_1);

#define LED2_ON	  GPIO_ResetBits(GPIOA, GPIO_Pin_2);
#define LED2_OFF	GPIO_SetBits(GPIOA, GPIO_Pin_2);
#define LED2_Toggle	GPIO_ToggleBits(GPIOA, GPIO_Pin_2);

void led_init(void);

#endif
