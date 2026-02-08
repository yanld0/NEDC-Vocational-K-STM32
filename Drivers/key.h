#ifndef __KEY_H
#define __KEY_H

#include "stm32f10x.h"

/********************* 本模块底层硬件定义（驱动层） *********************/
#define KEY_MODE_PIN    GPIO_Pin_0
#define KEY_PORT        GPIOC
#define KEY_DELAY_MS    20  // 消抖延时

/********************* 本模块功能接口（功能层） *********************/
#define MODE_MAX        3   // 3种运行模式
extern uint8_t g_current_mode; // 当前模式：0-避障 1-蛇行 2-绕圈

// 初始化（底层驱动）
void Key_Init(void);
// 获取当前模式（功能层）
uint8_t Key_GetCurrentMode(void);
// 扫描按键（底层+功能：消抖+模式切换）
void Key_Scan(void);

#endif
