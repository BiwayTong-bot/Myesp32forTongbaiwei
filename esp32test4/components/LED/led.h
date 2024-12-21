#ifndef __LED_H
#define __LED_H

#include "stdio.h"
#include "driver/gpio.h"

enum
{
    Green = 1,
    Red,
    Blue,
};

//初始化三色灯引脚
void Init_LED(uint8_t ledNum);
//根据指定的颜色和三色灯位号，打开三色灯
void Open_Led(uint8_t LedNum, uint8_t ColorBit);
//根据指定的颜色和三色灯位号，关闭三色灯
void Close_Led(uint8_t LedNum, uint8_t ColorBit);
#endif