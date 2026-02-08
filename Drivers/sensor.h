#ifndef __SENSOR_H
#define __SENSOR_H

#include "stm32f10x.h"

/********************* 本模块底层硬件定义（驱动层） *********************/
// 红外传感器引脚定义
#define IR_LEFT_PIN     GPIO_Pin_4
#define IR_FRONT_PIN    GPIO_Pin_5
#define IR_RIGHT_PIN    GPIO_Pin_6
#define IR_PORT         GPIOB

/********************* 本模块功能接口（功能层） *********************/
// 障碍物检测结果
typedef enum {
    SENSOR_NO_OBSTACLE,     // 无障碍物
    SENSOR_OBSTACLE_FRONT,  // 前方有障碍
    SENSOR_OBSTACLE_LEFT,   // 左侧有障碍
    SENSOR_OBSTACLE_RIGHT,  // 右侧有障碍
    SENSOR_OBSTACLE_ALL     // 全方向有障碍
} Sensor_ObstacleState_E;

// 初始化（底层驱动初始化）
void Sensor_Init(void);
// 检测障碍物（功能层逻辑）
Sensor_ObstacleState_E Sensor_CheckObstacle(void);
// 获取单个传感器状态（底层驱动接口）
uint8_t Sensor_GetSingleState(uint16_t pin);

#endif
