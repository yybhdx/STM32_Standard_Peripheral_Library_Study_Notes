/**
  ******************************************************************************
  * @文件    LED.h
  * @作者    Jiangxie Technology
  * @版本    V1.0
  * @日期    2026-07-07
  * @摘要    LED驱动模块头文件
  *          提供LED1和LED2的开启、关闭、翻转操作的函数声明。
  ******************************************************************************
  */

#ifndef __LED_H
#define __LED_H

void LED_Init(void);        /* LED初始化 */
void LED1_ON(void);         /* LED1开启 */
void LED1_OFF(void);        /* LED1关闭 */
void LED1_Turn(void);       /* LED1状态翻转 */
void LED2_ON(void);         /* LED2开启 */
void LED2_OFF(void);        /* LED2关闭 */
void LED2_Turn(void);       /* LED2状态翻转 */

#endif
