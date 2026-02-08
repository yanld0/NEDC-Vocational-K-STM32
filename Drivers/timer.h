#ifndef __TIMER_H
#define __TIMER_H

#include "stm32f10x.h"

/********************* 本模块底层硬件定义（驱动层） *********************/
#define TIM2_PRESCALER    7199  // 72MHz→10kHz
#define TIM2_PERIOD       9     // 10kHz→1ms中断

/********************* 本模块功能接口（功能层） *********************/
extern volatile uint32_t g_sys_time_ms; // 系统累计时间(ms)

// 初始化（底层驱动）
void Timer_Init(void);
// 获取系统时间（功能层）
uint32_t Timer_GetMs(void);
// 超时判断（功能层）
uint8_t Timer_CheckTimeout(uint32_t start_ms, uint32_t timeout_ms);
// 延时函数（功能层）
void Delay_ms(uint32_t ms);

#endif
