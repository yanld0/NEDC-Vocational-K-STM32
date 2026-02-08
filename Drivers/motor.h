#ifndef __MOTOR_H
#define __MOTOR_H

#include "stm32f10x.h"

/********************* 本模块底层硬件定义（驱动层） *********************/
// 电机GPIO引脚
#define MOTOR_LEFT_IN1_PIN    GPIO_Pin_0
#define MOTOR_LEFT_IN2_PIN    GPIO_Pin_1
#define MOTOR_RIGHT_IN1_PIN   GPIO_Pin_2
#define MOTOR_RIGHT_IN2_PIN   GPIO_Pin_3
#define MOTOR_PORT            GPIOB

// PWM参数（TIM3 CH3/CH4）
#define PWM_LEFT_CH           TIM_Channel_3
#define PWM_RIGHT_CH          TIM_Channel_4
#define PWM_TIM               TIM3
#define PWM_PRESCALER         71    // 72MHz→1MHz
#define PWM_PERIOD            999   // 1MHz→1kHz PWM
#define PWM_MAX_VAL           999   // 最大占空比

/********************* 本模块功能接口（功能层） *********************/
// 电机运行方向
typedef enum {
    MOTOR_DIR_FORWARD,  // 前进
    MOTOR_DIR_BACKWARD, // 后退
    MOTOR_DIR_LEFT,     // 左转
    MOTOR_DIR_RIGHT,    // 右转
    MOTOR_DIR_STOP      // 停止
} Motor_Dir_E;

// 初始化（底层驱动初始化）
void Motor_Init(void);
// 运动控制（功能层接口）
void Motor_SetDir(Motor_Dir_E dir);
void Motor_SetSpeed(uint16_t left_speed, uint16_t right_speed);
void Motor_Stop(void);

#endif
