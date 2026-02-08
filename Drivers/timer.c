#include "timer.h"

// 模块全局变量（驱动层+功能层共用）
volatile uint32_t g_sys_time_ms = 0;

/********************* 本模块底层驱动实现 *********************/
// TIM2初始化（系统计时）
static void Timer_TIM2_Init(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    TIM_TimeBaseStructure.TIM_Prescaler = TIM2_PRESCALER;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Period = TIM2_PERIOD;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    TIM_Cmd(TIM2, ENABLE);
}

// TIM2中断服务函数（底层驱动）
void TIM2_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
        g_sys_time_ms++; // 驱动层：1ms累加
    }
}

/********************* 本模块功能逻辑实现 *********************/
void Timer_Init(void)
{
    Timer_TIM2_Init(); // 底层定时器驱动初始化
}

uint32_t Timer_GetMs(void)
{
    return g_sys_time_ms; // 功能层：获取系统时间
}

uint8_t Timer_CheckTimeout(uint32_t start_ms, uint32_t timeout_ms)
{
    // 功能层：超时判断逻辑
    return (Timer_GetMs() - start_ms) >= timeout_ms ? 1 : 0;
}

void Delay_ms(uint32_t ms)
{
    // 功能层：ms级延时逻辑
    uint32_t start = Timer_GetMs();
    while(!Timer_CheckTimeout(start, ms));
}
