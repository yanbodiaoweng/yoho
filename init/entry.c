#include "types.h"


typedef
enum real_color {
    rc_black = 0,
    rc_blue = 1,
    rc_green = 2,
    rc_cyan = 3,
    rc_red = 4,
    rc_magenta = 5,
    rc_brown = 6,
    rc_light_grey = 7,
    rc_dark_grey = 8,
    rc_light_blue = 9,
    rc_light_green = 10,
    rc_light_cyan = 11,
    rc_light_red = 12,
    rc_light_magenta = 13,
    rc_light_brown  = 14,   // yellow
    rc_white = 15
} real_color_t;


// VGA 的显示缓冲的起点是 0xB8000
static uint16_t *video_memory = (uint16_t *)0xB8000;

// 屏幕"光标"的坐标
static uint8_t cursor_x = 0;
static uint8_t cursor_y = 0;

// 端口写一个字节
void outb(uint16_t port, uint8_t value);
// 端口写一个字节
inline void outb(uint16_t port, uint8_t value)
{
    asm volatile ("outb %1, %0" : : "dN" (port), "a" (value));
}

static void move_cursor()
{
    // 屏幕是 80 字节宽
    uint16_t cursorLocation = cursor_y * 80 + cursor_x;

    // VGA 内部的寄存器多达300多个，显然无法一一映射到I/O端口的地址空间。
    // 对此 VGA 控制器的解决方案是，将一个端口作为内部寄存器的索引：0x3D4，
    // 再通过 0x3D5 端口来设置相应寄存器的值。
    // 在这里用到的两个内部寄存器的编号为14与15，分别表示光标位置的高8位与低8位。

    outb(0x3D4, 14);                  	// 告诉 VGA 我们要设置光标的高字节
    outb(0x3D5, cursorLocation >> 8); 	// 发送高 8 位
    outb(0x3D4, 15);                  	// 告诉 VGA 我们要设置光标的低字节
    outb(0x3D5, cursorLocation);     	// 发送低 8 位
}

void console_clear()
{
    uint8_t attribute_byte = (0 << 4) | (15 & 0x0F);
    uint16_t blank = 0x20 | (attribute_byte << 8);

    int i;
    for (i = 0; i < 80 * 25; i++) {
        video_memory[i] = blank;
    }

    cursor_x = 0;
    cursor_y = 0;
    move_cursor();
}

int kern_entry()
{
    console_clear();
    uint8_t *input = (uint8_t *)0xB8000;
    uint8_t color = (0 << 4) | (15 & 0x0A);
    //1111 & 0000 1111

    *input++ = 'H'; *input++ = color;
    *input++ = 'e'; *input++ = color;
    *input++ = 'l'; *input++ = color;
    *input++ = 'l'; *input++ = color;
    *input++ = 'o'; *input++ = color;
    *input++ = ','; *input++ = color;
    *input++ = ' '; *input++ = color;
    *input++ = 'O'; *input++ = color;
    *input++ = 'S'; *input++ = color;
    *input++ = ' '; *input++ = color;
    *input++ = 'K'; *input++ = color;
    *input++ = 'e'; *input++ = color;
    *input++ = 'r'; *input++ = color;
    *input++ = 'n'; *input++ = color;
    *input++ = 'e'; *input++ = color;
    *input++ = 'l'; *input++ = color;
    *input++ = '!'; *input++ = color;


    return 0;
}
