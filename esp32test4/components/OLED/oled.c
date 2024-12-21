#include "oled.h"
#include "driver/i2c.h"
#include <stdio.h>

#define I2C_SCL_IO 22
#define I2C_SDA_IO 21
#define I2C_MASTER_NUM 0
#define I2C_MASTER_FREQ_HZ 400000
#define I2C_MASTER_TX_BUF_DISABLE 0
#define I2C_MASTER_RX_BUF_DISABLE 0

u8g2_t u8g2; // 定义u8g2对象

static __inline void delay_clock(int ts)
{
uint32_t start, curr;

__asm__ __volatile__("rsr %0, ccount"
: "=r"(start));
do
__asm__ __volatile__("rsr %0, ccount"
: "=r"(curr));
while (curr - start <= ts);
}

#define delay_us(val) delay_clock(240 * val)
#define delay_100ns(val) delay_clock(24 * val)

// 初始化I2C
static esp_err_t i2c_master_init(void)
{
int i2c_master_port = I2C_MASTER_NUM;

i2c_config_t conf = {
.mode = I2C_MODE_MASTER,
.sda_io_num = I2C_SDA_IO,
.scl_io_num = I2C_SCL_IO,
.sda_pullup_en = GPIO_PULLUP_ENABLE,
.scl_pullup_en = GPIO_PULLUP_ENABLE,
.master.clk_speed = I2C_MASTER_FREQ_HZ,
};

i2c_param_config(i2c_master_port, &conf);

return i2c_driver_install(i2c_master_port, conf.mode, I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0);
}

// I2C写数据
static void esp32_i2c_write(uint8_t addr, uint32_t idx, uint8_t *data)
{
i2c_cmd_handle_t handler = i2c_cmd_link_create();
i2c_master_start(handler);
i2c_master_write_byte(handler, addr | I2C_MASTER_WRITE, true);
i2c_master_write(handler, data, idx, true);
i2c_master_stop(handler);
i2c_master_cmd_begin(I2C_MASTER_NUM, handler, 100 / portTICK_PERIOD_MS);
i2c_cmd_link_delete(handler);
}

// GPIO和延时函数
uint8_t u8x8_gpio_and_delay(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
switch (msg)
{
case U8X8_MSG_DELAY_100NANO: // 延时 arg_int * 100 纳秒
delay_100ns(arg_int);
break;
case U8X8_MSG_DELAY_10MICRO: // 延时 arg_int * 10 微秒
delay_us(arg_int * 10);
break;
case U8X8_MSG_DELAY_MILLI: // 延时 arg_int * 1 毫秒
vTaskDelay(arg_int / portTICK_PERIOD_MS);
break;
case U8X8_MSG_DELAY_I2C: // 延时 arg_int 是 I2C 速度，例：4 = 400 KHz
delay_us(10 / arg_int);
break; // arg_int=1: delay by 5us, arg_int = 4: delay by 1.25us
case U8X8_MSG_GPIO_I2C_CLOCK: // arg_int=0: I2C时钟引脚输出低电平
if (arg_int == 1)
{
gpio_set_level(I2C_SCL_IO, 1);
}
else if (arg_int == 0)
{
gpio_set_level(I2C_SCL_IO, 0);
}
break; // arg_int=1: Input dir with pullup high for I2C clock pin
case U8X8_MSG_GPIO_I2C_DATA: // arg_int=0: I2C数据引脚输出低电平
if (arg_int == 1)
{
gpio_set_level(I2C_SDA_IO, 1);
}
else if (arg_int == 0)
{
gpio_set_level(I2C_SDA_IO, 0);
}
break; // arg_int=1: Input dir with pullup high for I2C data pin
case U8X8_MSG_GPIO_MENU_SELECT:
u8x8_SetGPIOResult(u8x8, 0);
break;
case U8X8_MSG_GPIO_MENU_NEXT:
u8x8_SetGPIOResult(u8x8, 0);
break;
case U8X8_MSG_GPIO_MENU_PREV:
u8x8_SetGPIOResult(u8x8, 0);
break;
case U8X8_MSG_GPIO_MENU_HOME:
u8x8_SetGPIOResult(u8x8, 0);
break;
default:
u8x8_SetGPIOResult(u8x8, 1); // 默认返回值
break;
}
return 1;
}

// I2C字节处理函数
uint8_t u8x8_byte_i2c(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
static uint8_t buffer[32]; // u8g2/u8x8 不会发送超过32字节的数据
static uint8_t buf_idx;
uint8_t *data;

switch (msg)
{
case U8X8_MSG_BYTE_SEND:
data = (uint8_t *)arg_ptr;
while (arg_int > 0)
{
buffer[buf_idx++] = *data;
data++;
arg_int--;
}
break;
case U8X8_MSG_BYTE_INIT:
// 自定义I2C初始化代码
break;
case U8X8_MSG_BYTE_SET_DC:
// 对于I2C来说忽略
break;
case U8X8_MSG_BYTE_START_TRANSFER:
buf_idx = 0;
break;
case U8X8_MSG_BYTE_END_TRANSFER:
esp32_i2c_write(u8x8_GetI2CAddress(u8x8), buf_idx, buffer);
break;
default:
return 0;
}
return 1;
}

// u8g2初始化
void u8g2Init(u8g2_t *u8g2)
{
u8g2_Setup_ssd1306_i2c_128x64_noname_f(u8g2, U8G2_R0, u8x8_byte_i2c, u8x8_gpio_and_delay);
u8g2_InitDisplay(u8g2);
u8g2_SetPowerSave(u8g2, 0);
u8g2_ClearBuffer(u8g2);
printf("u8g2 init done\n");
}

// OLED初始化
void OLED_Init(void)
{
i2c_master_init();
u8g2Init(&u8g2);
u8g2_SendBuffer(&u8g2); // 清屏
u8g2_SetFontDirection(&u8g2, 0);
u8g2_SetFont(&u8g2, u8g2_font_ncenB14_tr); // 设置较小的字体
}

// 显示信息
void Show_Info(const char *class_id, const char *name, const char *student_id, const char *temp_hum)
{
u8g2_DrawUTF8(&u8g2, 0, 15, class_id); // 班别
u8g2_DrawUTF8(&u8g2, 0, 30, name); // 姓名
u8g2_DrawUTF8(&u8g2, 0, 45, student_id); // 学号
u8g2_DrawUTF8(&u8g2, 0, 60, temp_hum); // 温度/湿度
// 注意：不要在这里发送或清除缓冲
}