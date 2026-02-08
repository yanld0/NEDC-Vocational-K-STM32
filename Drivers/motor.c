#include "motor.h"

/********************* 本模块底层驱动实现 *********************/
// PWM初始化（底层硬件配置）
static void Motor_PWM_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;

    // 使能时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);

    // PWM引脚配置（复用推挽输出）
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // 定时器时基配置
    TIM_TimeBaseStructure.TIM_Prescaler = PWM_PRESCALER;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Period = PWM_PERIOD;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInit(PWM_TIM, &TIM_TimeBaseStructure);

    // PWM输出配置
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0; // 初始占空比0
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

    TIM_OC3Init(PWM_TIM, &TIM_OCInitStructure); // 左电机PWM
    TIM_OC4Init(PWM_TIM, &TIM_OCInitStructure); // 右电机PWM
    TIM_OC3PreloadConfig(PWM_TIM, TIM_OCPreload_Enable);
    TIM_OC4PreloadConfig(PWM_TIM, TIM_OCPreload_Enable);
    TIM_ARRPreloadConfig(PWM_TIM, ENABLE);
    TIM_Cmd(PWM_TIM, ENABLE);
}

// GPIO初始化（电机方向控制）
static void Motor_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitStructure.GPIO_Pin = MOTOR_LEFT_IN1_PIN | MOTOR_LEFT_IN2_PIN |
                                  MOTOR_RIGHT_IN1_PIN | MOTOR_RIGHT_IN2_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(MOTOR_PORT, &GPIO_InitStructure);

    // 初始停止
    GPIO_SetBits(MOTOR_PORT, MOTOR_LEFT_IN1_PIN | MOTOR_LEFT_IN2_PIN |
                             MOTOR_RIGHT_IN1_PIN | MOTOR_RIGHT_IN2_PIN);
}

/********************* 本模块功能逻辑实现 *********************/
// 电机初始化（对外接口，整合底层驱动初始化）
void Motor_Init(void)
{
    Motor_GPIO_Init();   // 底层GPIO驱动初始化
    Motor_PWM_Init();    // 底层PWM驱动初始化
}

// 设置电机方向（功能层逻辑）
void Motor_SetDir(Motor_Dir_E dir)
{
    switch(dir)
    {
        case MOTOR_DIR_FORWARD: // 前进
            GPIO_ResetBits(MOTOR_PORT, MOTOR_LEFT_IN1_PIN);
            GPIO_SetBits(MOTOR_PORT, MOTOR_LEFT_IN2_PIN);
            GPIO_ResetBits(MOTOR_PORT, MOTOR_RIGHT_IN1_PIN);
            GPIO_SetBits(MOTOR_PORT, MOTOR_RIGHT_IN2_PIN);
            break;
        case MOTOR_DIR_BACKWARD: // 后退
            GPIO_SetBits(MOTOR_PORT, MOTOR_LEFT_IN1_PIN);
            GPIO_ResetBits(MOTOR_PORT, MOTOR_LEFT_IN2_PIN);
            GPIO_SetBits(MOTOR_PORT, MOTOR_RIGHT_IN1_PIN);
            GPIO_ResetBits(MOTOR_PORT, MOTOR_RIGHT_IN2_PIN);
            break;
        case MOTOR_DIR_LEFT: // 左转
            GPIO_SetBits(MOTOR_PORT, MOTOR_LEFT_IN1_PIN);
            GPIO_ResetBits(MOTOR_PORT, MOTOR_LEFT_IN2_PIN);
            GPIO_ResetBits(MOTOR_PORT, MOTOR_RIGHT_IN1_PIN);
            GPIO_SetBits(MOTOR_PORT, MOTOR_RIGHT_IN2_PIN);
            break;
        case MOTOR_DIR_RIGHT: // 右转
            GPIO_ResetBits(MOTOR_PORT, MOTOR_LEFT_IN1_PIN);
            GPIO_SetBits(MOTOR_PORT, MOTOR_LEFT_IN2_PIN);
            GPIO_SetBits(MOTOR_PORT, MOTOR_RIGHT_IN1_PIN);
            GPIO_ResetBits(MOTOR_PORT, MOTOR_RIGHT_IN2_PIN);
            break;
        case MOTOR_DIR_STOP: // 停止
            GPIO_SetBits(MOTOR_PORT, MOTOR_LEFT_IN1_PIN | MOTOR_LEFT_IN2_PIN);
            GPIO_SetBits(MOTOR_PORT, MOTOR_RIGHT_IN1_PIN | MOTOR_RIGHT_IN2_PIN);
            break;
    }
}

// 设置电机速度（功能层逻辑）
void Motor_SetSpeed(uint16_t left_speed, uint16_t right_speed)
{
    // 限制占空比范围
    left_speed = (left_speed > PWM_MAX_VAL) ? PWM_MAX_VAL : left_speed;
    right_speed = (right_speed > PWM_MAX_VAL) ? PWM_MAX_VAL : right_speed;
    
    // 底层PWM寄存器操作（驱动层）
    TIM_SetCompare3(PWM_TIM, left_speed);
    TIM_SetCompare4(PWM_TIM, right_speed);
}

// 电机停止（功能层快捷接口）
void Motor_Stop(void)
{
    Motor_SetDir(MOTOR_DIR_STOP);
    Motor_SetSpeed(0, 0);
}
