#include "key.h"
#include "timer.h"  // 仅依赖定时器的延时功能

// 模块内全局变量（仅本模块可见）
static uint8_t s_key_state = 0; // 按键状态
// 对外暴露的模式变量
uint8_t g_current_mode = 0;

/********************* 本模块底层驱动实现 *********************/
// 按键GPIO初始化
static void Key_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    GPIO_InitStructure.GPIO_Pin = KEY_MODE_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // 上拉输入
    GPIO_Init(KEY_PORT, &GPIO_InitStructure);
}

// 按键消抖（底层驱动逻辑）
static uint8_t Key_Debounce(void)
{
    uint8_t key_val = GPIO_ReadInputDataBit(KEY_PORT, KEY_MODE_PIN);
    if(key_val == 0) // 按键按下
    {
        Delay_ms(KEY_DELAY_MS); // 调用定时器模块的延时
        if(GPIO_ReadInputDataBit(KEY_PORT, KEY_MODE_PIN) == 0)
        {
            return 1; // 确认按下
        }
    }
    return 0;
}

/********************* 本模块功能逻辑实现 *********************/
void Key_Init(void)
{
    Key_GPIO_Init(); // 底层GPIO驱动初始化
}

uint8_t Key_GetCurrentMode(void)
{
    return g_current_mode; // 功能层：返回当前模式
}

void Key_Scan(void)
{
    if(Key_Debounce() && s_key_state == 0) // 底层消抖 + 功能层状态判断
    {
        s_key_state = 1; // 标记按键按下
        g_current_mode = (g_current_mode + 1) % MODE_MAX; // 模式切换逻辑
    }
    else if(GPIO_ReadInputDataBit(KEY_PORT, KEY_MODE_PIN) == 1)
    {
        s_key_state = 0; // 标记按键释放
    }
}
