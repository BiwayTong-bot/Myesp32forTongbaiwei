#ifndef OLED_H
#define OLED_H

#include "u8g2.h" // 确保包含u8g2库的头文件

extern u8g2_t u8g2; // 声明外部u8g2对象

void OLED_Init(void);
void Show_Info(const char *class_id, const char *name, const char *student_id, const char *temp_hum);

#endif // OLED_H