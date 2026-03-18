#ifndef __BUZZER_H
#define __BUZZER_H

#define Buzzer_ON GPIO_SetBits(GPIOB, GPIO_Pin_12)

#define Buzzer_OFF GPIO_ResetBits(GPIOB, GPIO_Pin_12)

void buzzer_init(void);

void buzzer_turn(void);


#endif
