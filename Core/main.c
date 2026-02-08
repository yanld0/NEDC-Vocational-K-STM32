#include "motor.h"
#include "sensor.h"
#include "key.h"
#include "timer.h"

// 模式处理函数声明
void Mode_AvoidObstacle(void);  // 模式1：避障
void Mode_SnakeMove(void);      // 模式2：蛇行
void Mode_CircleObstacle(void); // 模式3：绕圈

int main(void)
{
    // 各模块初始化（调用模块内的驱动初始化）
    Timer_Init();    // 定时器模块初始化
    Motor_Init();    // 电机模块初始化
    Sensor_Init();   // 传感器模块初始化
    Key_Init();      // 按键模块初始化

    while(1)
    {
        Key_Scan(); // 按键扫描（模块内功能逻辑）
        
        // 根据当前模式执行对应逻辑
        switch(Key_GetCurrentMode())
        {
            case 0:
                Mode_AvoidObstacle();
                break;
            case 1:
                Mode_SnakeMove();
                break;
            case 2:
                Mode_CircleObstacle();
                break;
            default:
                Motor_Stop();
                break;
        }
    }
}

// 模式1：避障逻辑（调用传感器+电机模块功能接口）
void Mode_AvoidObstacle(void)
{
    Sensor_ObstacleState_E state = Sensor_CheckObstacle();
    if(state == SENSOR_OBSTACLE_FRONT)
    {
        Motor_SetDir(MOTOR_DIR_LEFT);
        Motor_SetSpeed(500, 500);
        Delay_ms(500);
    }
    else if(state == SENSOR_OBSTACLE_LEFT)
    {
        Motor_SetDir(MOTOR_DIR_RIGHT);
        Motor_SetSpeed(500, 500);
        Delay_ms(500);
    }
    else if(state == SENSOR_OBSTACLE_RIGHT)
    {
        Motor_SetDir(MOTOR_DIR_LEFT);
        Motor_SetSpeed(500, 500);
        Delay_ms(500);
    }
    else
    {
        Motor_SetDir(MOTOR_DIR_FORWARD);
        Motor_SetSpeed(600, 600);
    }
}

// 模式2：蛇行逻辑（示例）
void Mode_SnakeMove(void)
{
    static uint32_t start = 0;
    if(start == 0) start = Timer_GetMs();
    
    if(Timer_CheckTimeout(start, 1000)) // 1秒左转
    {
        Motor_SetDir(MOTOR_DIR_LEFT);
        Motor_SetSpeed(400, 400);
    }
    else if(Timer_CheckTimeout(start, 2000)) // 2秒右转
    {
        Motor_SetDir(MOTOR_DIR_RIGHT);
        Motor_SetSpeed(400, 400);
        start = 0;
    }
    else
    {
        Motor_SetDir(MOTOR_DIR_FORWARD);
        Motor_SetSpeed(500, 500);
    }
}

// 模式3：绕圈逻辑（示例）
void Mode_CircleObstacle(void)
{
    Motor_SetDir(MOTOR_DIR_LEFT);
    Motor_SetSpeed(300, 600); // 差速绕圈
}

