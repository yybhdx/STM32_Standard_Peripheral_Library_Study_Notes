#ifndef __MyRTC_H
#define __MyRTC_H

// 实时时钟RTC初始化
void MyRTC_Init(void);

void MyRTC_SetTime(void);

void MyRTC_ReadTime(void);

extern uint16_t MyRTC_Time[];

#endif
