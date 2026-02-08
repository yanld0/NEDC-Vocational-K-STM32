#include "sensor.h"
#include "timer.h"

/********************* 本模块底层驱动实现 *********************/
// GPIO初始化（红外传感器引脚）
static void Sensor_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitStructure.GPIO_Pin = IR_LEFT_PIN | IR_FRONT_PIN | IR_RIGHT_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // 上拉输入
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(IR_PORT, &GPIO_InitStructure);
}

// 获取单个传感器引脚状态
uint8_t Sensor_GetSingleState(uint16_t pin)
{
    // 红外传感器输出低电平表示检测到障碍物
    return GPIO_ReadInputDataBit(IR_PORT, pin) == 0 ? 1 : 0;
}

/********************* 本模块功能逻辑实现 *********************/
void Sensor_Init(void)
{
    Sensor_GPIO_Init(); // 底层GPIO驱动初始化
}

Sensor_ObstacleState_E Sensor_CheckObstacle(void)
{
    uint8_t left = Sensor_GetSingleState(IR_LEFT_PIN);
    uint8_t front = Sensor_GetSingleState(IR_FRONT_PIN);
    uint8_t right = Sensor_GetSingleState(IR_RIGHT_PIN);

    // 组合判断障碍物位置
    if (front && left && right)
        return SENSOR_OBSTACLE_ALL;
    else if (front)
        return SENSOR_OBSTACLE_FRONT;
    else if (left)
        return SENSOR_OBSTACLE_LEFT;
    else if (right)
        return SENSOR_OBSTACLE_RIGHT;
    else
        return SENSOR_NO_OBSTACLE;
}