#include "key.h"
#include "led.h"

#define SW2 0

static uint8_t sw2Num0 = 0;
/*
    函数内容：初始化板载按键
    函数参数：无
    返回值：无
*/
void Init_key(void)
{
    gpio_reset_pin(SW2);
    gpio_set_direction(SW2, GPIO_MODE_INPUT);
    gpio_set_pull_mode(SW2, GPIO_PULLUP_ONLY);
}

/*
    函数内容：扫描板载按键
    函数参数：无
    返回值：无
*/
void Scanf_Key(void)
{
    if (gpio_get_level(SW2) == 0)
    {
        while (gpio_get_level(SW2) == 0)
            ;
        sw2Num0++;
        switch (sw2Num0)
        {
        case 1:
            Open_Led(0, Green);
            Close_Led(0, Red);
            Close_Led(0, Blue);

            Open_Led(1, Green);
            Close_Led(1, Red);
            Close_Led(1, Blue);
            break;
        case 2:
            Open_Led(0, Red);
            Close_Led(0, Blue);
            Close_Led(0, Green);

            Open_Led(1, Red);
            Close_Led(1, Blue);
            Close_Led(1, Green);
            break;
        case 3:
            Open_Led(0, Blue);
            Close_Led(0, Red);
            Close_Led(0, Green);

            Open_Led(1, Blue);
            Close_Led(1, Red);
            Close_Led(1, Green);
            sw2Num0 = 0;
            break;
        default:
            break;
        }
    }
}