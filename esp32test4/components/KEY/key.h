#ifndef __KEY_H
#define __KEY_H

#include "stdio.h"
#include "driver/gpio.h"

//初始化板载按键
void Init_key(void);
//扫描板载按键
void Scanf_Key(void);

#endif