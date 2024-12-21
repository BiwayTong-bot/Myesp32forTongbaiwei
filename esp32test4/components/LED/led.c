#include "led.h"

#define LED1_Green 12
#define LED1_Red 13
#define LED1_Blue 14

#define LED2_Green 26
#define LED2_Red 27
#define LED2_Blue 25

/*
    函数内容：初始化三色灯引脚
    函数参数：uint8_t ledNum---三色灯位号,0--三色灯1，1--三色灯2
    返回值：  无
*/
void Init_LED(uint8_t ledNum)
{
    if (ledNum == 0)
    {
        gpio_reset_pin(LED1_Green);
        gpio_reset_pin(LED1_Red);
        gpio_reset_pin(LED1_Blue);
        gpio_set_direction(LED1_Green, GPIO_MODE_OUTPUT);
        gpio_set_direction(LED1_Red, GPIO_MODE_OUTPUT);
        gpio_set_direction(LED1_Blue, GPIO_MODE_OUTPUT);
        Close_Led(0, Green);
        Close_Led(0, Red);
        Close_Led(0, Blue);
    }
    else
    {
        gpio_reset_pin(LED2_Green);
        gpio_reset_pin(LED2_Red);
        gpio_reset_pin(LED2_Blue);
        gpio_set_direction(LED2_Green, GPIO_MODE_OUTPUT);
        gpio_set_direction(LED2_Red, GPIO_MODE_OUTPUT);
        gpio_set_direction(LED2_Blue, GPIO_MODE_OUTPUT);
        Close_Led(1, Green);
        Close_Led(1, Red);
        Close_Led(1, Blue);
    }
}

/*
    函数内容：根据指定的颜色和三色灯位号，打开三色灯
    函数参数：uint8_t LedNum----三色灯位号，0--三色灯1，1--三色灯2
              uint8_t ColorBit--三色灯颜色
    返回值：无
*/
void Open_Led(uint8_t LedNum, uint8_t ColorBit)
{
    if (LedNum == 0)
    {
        switch (ColorBit)
        {
        case Green:
            gpio_set_level(LED1_Green, 0);
            break;
        case Red:
            gpio_set_level(LED1_Red, 0);
            break;
        case Blue:
            gpio_set_level(LED1_Blue, 0);
            break;
        default:
            break;
        }
    }
    else
    {
        switch (ColorBit)
        {
        case Green:
            gpio_set_level(LED2_Green, 0);
            break;
        case Red:
            gpio_set_level(LED2_Red, 0);
            break;
        case Blue:
            gpio_set_level(LED2_Blue, 0);
            break;
        default:
            break;
        }
    }
}

/*
    函数内容：根据指定的颜色和三色灯位号，关闭三色灯
    函数参数：uint8_t LedNum----三色灯位号，0--三色灯1，1--三色灯2
              uint8_t ColorBit--三色灯颜色
    返回值：无
*/
void Close_Led(uint8_t LedNum, uint8_t ColorBit)
{
    if (LedNum == 0)
    {
        switch (ColorBit)
        {
        case Green:
            gpio_set_level(LED1_Green, 1);
            break;
        case Red:
            gpio_set_level(LED1_Red, 1);
            break;
        case Blue:
            gpio_set_level(LED1_Blue, 1);
            break;
        default:
            break;
        }
    }
    else
    {
        switch (ColorBit)
        {
        case Green:
            gpio_set_level(LED2_Green, 1);
            break;
        case Red:
            gpio_set_level(LED2_Red, 1);
            break;
        case Blue:
            gpio_set_level(LED2_Blue, 1);
            break;
        default:
            break;
        }
    }
}