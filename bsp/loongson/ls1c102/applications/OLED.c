#include "OLED.h"

#include <rtthread.h>

#include "soc_i2c.h"


/************************************************************************
OLED 一行可以显示16个字符，可以显示8行。
************************************************************************/

//--------这里修改定义端口------//

#define XLevelL 0x00
#define XLevelH 0x10
#define XLevel ((XLevelH & 0x0F) * 16 + XLevelL)
#define Max_Column 128
#define Max_Row 64
#define Brightness 0xCF

#define OLED_IIC
/*
4线SPI使用说明：

CS  片选管脚
DC  命令数据选择管脚
D0（SCLK） ，时钟脚，由MCU控制
D1（MOSI） ，主输出从输入数据脚，由MCU控制

*/

#define X_WIDTH 128
#define Y_WIDTH 64
//======================================
const uint8_t F6x8[][6] =
    {
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // space
        {0x00, 0x00, 0x00, 0x2f, 0x00, 0x00}, // !
        {0x00, 0x00, 0x07, 0x00, 0x07, 0x00}, // "
        {0x00, 0x14, 0x7f, 0x14, 0x7f, 0x14}, // #
        {0x00, 0x24, 0x2a, 0x7f, 0x2a, 0x12}, // $
        {0x00, 0x62, 0x64, 0x08, 0x13, 0x23}, // %
        {0x00, 0x36, 0x49, 0x55, 0x22, 0x50}, // &
        {0x00, 0x00, 0x05, 0x03, 0x00, 0x00}, // '
        {0x00, 0x00, 0x1c, 0x22, 0x41, 0x00}, // (
        {0x00, 0x00, 0x41, 0x22, 0x1c, 0x00}, // )
        {0x00, 0x14, 0x08, 0x3E, 0x08, 0x14}, // *
        {0x00, 0x08, 0x08, 0x3E, 0x08, 0x08}, // +
        {0x00, 0x00, 0x00, 0xA0, 0x60, 0x00}, // ,
        {0x00, 0x08, 0x08, 0x08, 0x08, 0x08}, // -
        {0x00, 0x00, 0x60, 0x60, 0x00, 0x00}, // .
        {0x00, 0x20, 0x10, 0x08, 0x04, 0x02}, // /
        {0x00, 0x3E, 0x51, 0x49, 0x45, 0x3E}, // 0
        {0x00, 0x00, 0x42, 0x7F, 0x40, 0x00}, // 1
        {0x00, 0x42, 0x61, 0x51, 0x49, 0x46}, // 2
        {0x00, 0x21, 0x41, 0x45, 0x4B, 0x31}, // 3
        {0x00, 0x18, 0x14, 0x12, 0x7F, 0x10}, // 4
        {0x00, 0x27, 0x45, 0x45, 0x45, 0x39}, // 5
        {0x00, 0x3C, 0x4A, 0x49, 0x49, 0x30}, // 6
        {0x00, 0x01, 0x71, 0x09, 0x05, 0x03}, // 7
        {0x00, 0x36, 0x49, 0x49, 0x49, 0x36}, // 8
        {0x00, 0x06, 0x49, 0x49, 0x29, 0x1E}, // 9
        {0x00, 0x00, 0x36, 0x36, 0x00, 0x00}, // :
        {0x00, 0x00, 0x56, 0x36, 0x00, 0x00}, // ;
        {0x00, 0x08, 0x14, 0x22, 0x41, 0x00}, // <
        {0x00, 0x14, 0x14, 0x14, 0x14, 0x14}, // =
        {0x00, 0x00, 0x41, 0x22, 0x14, 0x08}, // >
        {0x00, 0x02, 0x01, 0x51, 0x09, 0x06}, // ?
        {0x00, 0x32, 0x49, 0x59, 0x51, 0x3E}, // @
        {0x00, 0x7C, 0x12, 0x11, 0x12, 0x7C}, // A
        {0x00, 0x7F, 0x49, 0x49, 0x49, 0x36}, // B
        {0x00, 0x3E, 0x41, 0x41, 0x41, 0x22}, // C
        {0x00, 0x7F, 0x41, 0x41, 0x22, 0x1C}, // D
        {0x00, 0x7F, 0x49, 0x49, 0x49, 0x41}, // E
        {0x00, 0x7F, 0x09, 0x09, 0x09, 0x01}, // F
        {0x00, 0x3E, 0x41, 0x49, 0x49, 0x7A}, // G
        {0x00, 0x7F, 0x08, 0x08, 0x08, 0x7F}, // H
        {0x00, 0x00, 0x41, 0x7F, 0x41, 0x00}, // I
        {0x00, 0x20, 0x40, 0x41, 0x3F, 0x01}, // J
        {0x00, 0x7F, 0x08, 0x14, 0x22, 0x41}, // K
        {0x00, 0x7F, 0x40, 0x40, 0x40, 0x40}, // L
        {0x00, 0x7F, 0x02, 0x0C, 0x02, 0x7F}, // M
        {0x00, 0x7F, 0x04, 0x08, 0x10, 0x7F}, // N
        {0x00, 0x3E, 0x41, 0x41, 0x41, 0x3E}, // O
        {0x00, 0x7F, 0x09, 0x09, 0x09, 0x06}, // P
        {0x00, 0x3E, 0x41, 0x51, 0x21, 0x5E}, // Q
        {0x00, 0x7F, 0x09, 0x19, 0x29, 0x46}, // R
        {0x00, 0x46, 0x49, 0x49, 0x49, 0x31}, // S
        {0x00, 0x01, 0x01, 0x7F, 0x01, 0x01}, // T
        {0x00, 0x3F, 0x40, 0x40, 0x40, 0x3F}, // U
        {0x00, 0x1F, 0x20, 0x40, 0x20, 0x1F}, // V
        {0x00, 0x3F, 0x40, 0x38, 0x40, 0x3F}, // W
        {0x00, 0x63, 0x14, 0x08, 0x14, 0x63}, // X
        {0x00, 0x07, 0x08, 0x70, 0x08, 0x07}, // Y
        {0x00, 0x61, 0x51, 0x49, 0x45, 0x43}, // Z
        {0x00, 0x00, 0x7F, 0x41, 0x41, 0x00}, // [
        {0x00, 0x55, 0x2A, 0x55, 0x2A, 0x55}, // 55
        {0x00, 0x00, 0x41, 0x41, 0x7F, 0x00}, // ]
        {0x00, 0x04, 0x02, 0x01, 0x02, 0x04}, // ^
        {0x00, 0x40, 0x40, 0x40, 0x40, 0x40}, // _
        {0x00, 0x00, 0x01, 0x02, 0x04, 0x00}, // '
        {0x00, 0x20, 0x54, 0x54, 0x54, 0x78}, // a
        {0x00, 0x7F, 0x48, 0x44, 0x44, 0x38}, // b
        {0x00, 0x38, 0x44, 0x44, 0x44, 0x20}, // c
        {0x00, 0x38, 0x44, 0x44, 0x48, 0x7F}, // d
        {0x00, 0x38, 0x54, 0x54, 0x54, 0x18}, // e
        {0x00, 0x08, 0x7E, 0x09, 0x01, 0x02}, // f
        {0x00, 0x18, 0xA4, 0xA4, 0xA4, 0x7C}, // g
        {0x00, 0x7F, 0x08, 0x04, 0x04, 0x78}, // h
        {0x00, 0x00, 0x44, 0x7D, 0x40, 0x00}, // i
        {0x00, 0x40, 0x80, 0x84, 0x7D, 0x00}, // j
        {0x00, 0x7F, 0x10, 0x28, 0x44, 0x00}, // k
        {0x00, 0x00, 0x41, 0x7F, 0x40, 0x00}, // l
        {0x00, 0x7C, 0x04, 0x18, 0x04, 0x78}, // m
        {0x00, 0x7C, 0x08, 0x04, 0x04, 0x78}, // n
        {0x00, 0x38, 0x44, 0x44, 0x44, 0x38}, // o
        {0x00, 0xFC, 0x24, 0x24, 0x24, 0x18}, // p
        {0x00, 0x18, 0x24, 0x24, 0x18, 0xFC}, // q
        {0x00, 0x7C, 0x08, 0x04, 0x04, 0x08}, // r
        {0x00, 0x48, 0x54, 0x54, 0x54, 0x20}, // s
        {0x00, 0x04, 0x3F, 0x44, 0x40, 0x20}, // t
        {0x00, 0x3C, 0x40, 0x40, 0x20, 0x7C}, // u
        {0x00, 0x1C, 0x20, 0x40, 0x20, 0x1C}, // v
        {0x00, 0x3C, 0x40, 0x30, 0x40, 0x3C}, // w
        {0x00, 0x44, 0x28, 0x10, 0x28, 0x44}, // x
        {0x00, 0x1C, 0xA0, 0xA0, 0xA0, 0x7C}, // y
        {0x00, 0x44, 0x64, 0x54, 0x4C, 0x44}, // z
        {0x14, 0x14, 0x14, 0x14, 0x14, 0x14}  // horiz lines
};

//======================================================
// 128X64I液晶底层驱动[8X16]字体库
// 描  述: [8X16]西文字符的字模数据 (纵向取模,字节倒序)
// !"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~
//======================================================
const uint8_t F8X16[] =
    {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //  0
        0x00, 0x00, 0x00, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x33, 0x30, 0x00, 0x00, 0x00, //! 1
        0x00, 0x10, 0x0C, 0x06, 0x10, 0x0C, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //" 2
        0x40, 0xC0, 0x78, 0x40, 0xC0, 0x78, 0x40, 0x00, 0x04, 0x3F, 0x04, 0x04, 0x3F, 0x04, 0x04, 0x00, //# 3
        0x00, 0x70, 0x88, 0xFC, 0x08, 0x30, 0x00, 0x00, 0x00, 0x18, 0x20, 0xFF, 0x21, 0x1E, 0x00, 0x00, //$ 4
        0xF0, 0x08, 0xF0, 0x00, 0xE0, 0x18, 0x00, 0x00, 0x00, 0x21, 0x1C, 0x03, 0x1E, 0x21, 0x1E, 0x00, //% 5
        0x00, 0xF0, 0x08, 0x88, 0x70, 0x00, 0x00, 0x00, 0x1E, 0x21, 0x23, 0x24, 0x19, 0x27, 0x21, 0x10, //& 6
        0x10, 0x16, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //' 7
        0x00, 0x00, 0x00, 0xE0, 0x18, 0x04, 0x02, 0x00, 0x00, 0x00, 0x00, 0x07, 0x18, 0x20, 0x40, 0x00, //( 8
        0x00, 0x02, 0x04, 0x18, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x40, 0x20, 0x18, 0x07, 0x00, 0x00, 0x00, //) 9
        0x40, 0x40, 0x80, 0xF0, 0x80, 0x40, 0x40, 0x00, 0x02, 0x02, 0x01, 0x0F, 0x01, 0x02, 0x02, 0x00, //* 10
        0x00, 0x00, 0x00, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x1F, 0x01, 0x01, 0x01, 0x00, //+ 11
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xB0, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, //, 12
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, //- 13
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, //. 14
        0x00, 0x00, 0x00, 0x00, 0x80, 0x60, 0x18, 0x04, 0x00, 0x60, 0x18, 0x06, 0x01, 0x00, 0x00, 0x00, /// 15
        0x00, 0xE0, 0x10, 0x08, 0x08, 0x10, 0xE0, 0x00, 0x00, 0x0F, 0x10, 0x20, 0x20, 0x10, 0x0F, 0x00, // 0 16
        0x00, 0x10, 0x10, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x20, 0x3F, 0x20, 0x20, 0x00, 0x00, // 1 17
        0x00, 0x70, 0x08, 0x08, 0x08, 0x88, 0x70, 0x00, 0x00, 0x30, 0x28, 0x24, 0x22, 0x21, 0x30, 0x00, // 2 18
        0x00, 0x30, 0x08, 0x88, 0x88, 0x48, 0x30, 0x00, 0x00, 0x18, 0x20, 0x20, 0x20, 0x11, 0x0E, 0x00, // 3 19
        0x00, 0x00, 0xC0, 0x20, 0x10, 0xF8, 0x00, 0x00, 0x00, 0x07, 0x04, 0x24, 0x24, 0x3F, 0x24, 0x00, // 4 20
        0x00, 0xF8, 0x08, 0x88, 0x88, 0x08, 0x08, 0x00, 0x00, 0x19, 0x21, 0x20, 0x20, 0x11, 0x0E, 0x00, // 5 21
        0x00, 0xE0, 0x10, 0x88, 0x88, 0x18, 0x00, 0x00, 0x00, 0x0F, 0x11, 0x20, 0x20, 0x11, 0x0E, 0x00, // 6 22
        0x00, 0x38, 0x08, 0x08, 0xC8, 0x38, 0x08, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x00, 0x00, 0x00, 0x00, // 7 23
        0x00, 0x70, 0x88, 0x08, 0x08, 0x88, 0x70, 0x00, 0x00, 0x1C, 0x22, 0x21, 0x21, 0x22, 0x1C, 0x00, // 8 24
        0x00, 0xE0, 0x10, 0x08, 0x08, 0x10, 0xE0, 0x00, 0x00, 0x00, 0x31, 0x22, 0x22, 0x11, 0x0F, 0x00, // 9 25
        0x00, 0x00, 0x00, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x30, 0x00, 0x00, 0x00, //: 26
        0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x60, 0x00, 0x00, 0x00, 0x00, //; 27
        0x00, 0x00, 0x80, 0x40, 0x20, 0x10, 0x08, 0x00, 0x00, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x00, //< 28
        0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x00, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x00, //= 29
        0x00, 0x08, 0x10, 0x20, 0x40, 0x80, 0x00, 0x00, 0x00, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01, 0x00, //> 30
        0x00, 0x70, 0x48, 0x08, 0x08, 0x08, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x30, 0x36, 0x01, 0x00, 0x00, //? 31
        0xC0, 0x30, 0xC8, 0x28, 0xE8, 0x10, 0xE0, 0x00, 0x07, 0x18, 0x27, 0x24, 0x23, 0x14, 0x0B, 0x00, //@ 32
        0x00, 0x00, 0xC0, 0x38, 0xE0, 0x00, 0x00, 0x00, 0x20, 0x3C, 0x23, 0x02, 0x02, 0x27, 0x38, 0x20, // A 33
        0x08, 0xF8, 0x88, 0x88, 0x88, 0x70, 0x00, 0x00, 0x20, 0x3F, 0x20, 0x20, 0x20, 0x11, 0x0E, 0x00, // B 34
        0xC0, 0x30, 0x08, 0x08, 0x08, 0x08, 0x38, 0x00, 0x07, 0x18, 0x20, 0x20, 0x20, 0x10, 0x08, 0x00, // C 35
        0x08, 0xF8, 0x08, 0x08, 0x08, 0x10, 0xE0, 0x00, 0x20, 0x3F, 0x20, 0x20, 0x20, 0x10, 0x0F, 0x00, // D 36
        0x08, 0xF8, 0x88, 0x88, 0xE8, 0x08, 0x10, 0x00, 0x20, 0x3F, 0x20, 0x20, 0x23, 0x20, 0x18, 0x00, // E 37
        0x08, 0xF8, 0x88, 0x88, 0xE8, 0x08, 0x10, 0x00, 0x20, 0x3F, 0x20, 0x00, 0x03, 0x00, 0x00, 0x00, // F 38
        0xC0, 0x30, 0x08, 0x08, 0x08, 0x38, 0x00, 0x00, 0x07, 0x18, 0x20, 0x20, 0x22, 0x1E, 0x02, 0x00, // G 39
        0x08, 0xF8, 0x08, 0x00, 0x00, 0x08, 0xF8, 0x08, 0x20, 0x3F, 0x21, 0x01, 0x01, 0x21, 0x3F, 0x20, // H 40
        0x00, 0x08, 0x08, 0xF8, 0x08, 0x08, 0x00, 0x00, 0x00, 0x20, 0x20, 0x3F, 0x20, 0x20, 0x00, 0x00, // I 41
        0x00, 0x00, 0x08, 0x08, 0xF8, 0x08, 0x08, 0x00, 0xC0, 0x80, 0x80, 0x80, 0x7F, 0x00, 0x00, 0x00, // J 42
        0x08, 0xF8, 0x88, 0xC0, 0x28, 0x18, 0x08, 0x00, 0x20, 0x3F, 0x20, 0x01, 0x26, 0x38, 0x20, 0x00, // K 43
        0x08, 0xF8, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x3F, 0x20, 0x20, 0x20, 0x20, 0x30, 0x00, // L 44
        0x08, 0xF8, 0xF8, 0x00, 0xF8, 0xF8, 0x08, 0x00, 0x20, 0x3F, 0x00, 0x3F, 0x00, 0x3F, 0x20, 0x00, // M 45
        0x08, 0xF8, 0x30, 0xC0, 0x00, 0x08, 0xF8, 0x08, 0x20, 0x3F, 0x20, 0x00, 0x07, 0x18, 0x3F, 0x00, // N 46
        0xE0, 0x10, 0x08, 0x08, 0x08, 0x10, 0xE0, 0x00, 0x0F, 0x10, 0x20, 0x20, 0x20, 0x10, 0x0F, 0x00, // O 47
        0x08, 0xF8, 0x08, 0x08, 0x08, 0x08, 0xF0, 0x00, 0x20, 0x3F, 0x21, 0x01, 0x01, 0x01, 0x00, 0x00, // P 48
        0xE0, 0x10, 0x08, 0x08, 0x08, 0x10, 0xE0, 0x00, 0x0F, 0x18, 0x24, 0x24, 0x38, 0x50, 0x4F, 0x00, // Q 49
        0x08, 0xF8, 0x88, 0x88, 0x88, 0x88, 0x70, 0x00, 0x20, 0x3F, 0x20, 0x00, 0x03, 0x0C, 0x30, 0x20, // R 50
        0x00, 0x70, 0x88, 0x08, 0x08, 0x08, 0x38, 0x00, 0x00, 0x38, 0x20, 0x21, 0x21, 0x22, 0x1C, 0x00, // S 51
        0x18, 0x08, 0x08, 0xF8, 0x08, 0x08, 0x18, 0x00, 0x00, 0x00, 0x20, 0x3F, 0x20, 0x00, 0x00, 0x00, // T 52
        0x08, 0xF8, 0x08, 0x00, 0x00, 0x08, 0xF8, 0x08, 0x00, 0x1F, 0x20, 0x20, 0x20, 0x20, 0x1F, 0x00, // U 53
        0x08, 0x78, 0x88, 0x00, 0x00, 0xC8, 0x38, 0x08, 0x00, 0x00, 0x07, 0x38, 0x0E, 0x01, 0x00, 0x00, // V 54
        0xF8, 0x08, 0x00, 0xF8, 0x00, 0x08, 0xF8, 0x00, 0x03, 0x3C, 0x07, 0x00, 0x07, 0x3C, 0x03, 0x00, // W 55
        0x08, 0x18, 0x68, 0x80, 0x80, 0x68, 0x18, 0x08, 0x20, 0x30, 0x2C, 0x03, 0x03, 0x2C, 0x30, 0x20, // X 56
        0x08, 0x38, 0xC8, 0x00, 0xC8, 0x38, 0x08, 0x00, 0x00, 0x00, 0x20, 0x3F, 0x20, 0x00, 0x00, 0x00, // Y 57
        0x10, 0x08, 0x08, 0x08, 0xC8, 0x38, 0x08, 0x00, 0x20, 0x38, 0x26, 0x21, 0x20, 0x20, 0x18, 0x00, // Z 58
        0x00, 0x00, 0x00, 0xFE, 0x02, 0x02, 0x02, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x40, 0x40, 0x40, 0x00, //[ 59
        0x00, 0x0C, 0x30, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x06, 0x38, 0xC0, 0x00, //\ 60
        0x00, 0x02, 0x02, 0x02, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x40, 0x40, 0x40, 0x7F, 0x00, 0x00, 0x00, //] 61
        0x00, 0x00, 0x04, 0x02, 0x02, 0x02, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //^ 62
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, //_ 63
        0x00, 0x02, 0x02, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //` 64
        0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x19, 0x24, 0x22, 0x22, 0x22, 0x3F, 0x20, // a 65
        0x08, 0xF8, 0x00, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x11, 0x20, 0x20, 0x11, 0x0E, 0x00, // b 66
        0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x0E, 0x11, 0x20, 0x20, 0x20, 0x11, 0x00, // c 67
        0x00, 0x00, 0x00, 0x80, 0x80, 0x88, 0xF8, 0x00, 0x00, 0x0E, 0x11, 0x20, 0x20, 0x10, 0x3F, 0x20, // d 68
        0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x1F, 0x22, 0x22, 0x22, 0x22, 0x13, 0x00, // e 69
        0x00, 0x80, 0x80, 0xF0, 0x88, 0x88, 0x88, 0x18, 0x00, 0x20, 0x20, 0x3F, 0x20, 0x20, 0x00, 0x00, // f 70
        0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x6B, 0x94, 0x94, 0x94, 0x93, 0x60, 0x00, // g 71
        0x08, 0xF8, 0x00, 0x80, 0x80, 0x80, 0x00, 0x00, 0x20, 0x3F, 0x21, 0x00, 0x00, 0x20, 0x3F, 0x20, // h 72
        0x00, 0x80, 0x98, 0x98, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x20, 0x3F, 0x20, 0x20, 0x00, 0x00, // i 73
        0x00, 0x00, 0x00, 0x80, 0x98, 0x98, 0x00, 0x00, 0x00, 0xC0, 0x80, 0x80, 0x80, 0x7F, 0x00, 0x00, // j 74
        0x08, 0xF8, 0x00, 0x00, 0x80, 0x80, 0x80, 0x00, 0x20, 0x3F, 0x24, 0x02, 0x2D, 0x30, 0x20, 0x00, // k 75
        0x00, 0x08, 0x08, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x20, 0x3F, 0x20, 0x20, 0x00, 0x00, // l 76
        0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00, 0x20, 0x3F, 0x20, 0x00, 0x3F, 0x20, 0x00, 0x3F, // m 77
        0x80, 0x80, 0x00, 0x80, 0x80, 0x80, 0x00, 0x00, 0x20, 0x3F, 0x21, 0x00, 0x00, 0x20, 0x3F, 0x20, // n 78
        0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x1F, 0x20, 0x20, 0x20, 0x20, 0x1F, 0x00, // o 79
        0x80, 0x80, 0x00, 0x80, 0x80, 0x00, 0x00, 0x00, 0x80, 0xFF, 0xA1, 0x20, 0x20, 0x11, 0x0E, 0x00, // p 80
        0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x0E, 0x11, 0x20, 0x20, 0xA0, 0xFF, 0x80, // q 81
        0x80, 0x80, 0x80, 0x00, 0x80, 0x80, 0x80, 0x00, 0x20, 0x20, 0x3F, 0x21, 0x20, 0x00, 0x01, 0x00, // r 82
        0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x33, 0x24, 0x24, 0x24, 0x24, 0x19, 0x00, // s 83
        0x00, 0x80, 0x80, 0xE0, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x20, 0x20, 0x00, 0x00, // t 84
        0x80, 0x80, 0x00, 0x00, 0x00, 0x80, 0x80, 0x00, 0x00, 0x1F, 0x20, 0x20, 0x20, 0x10, 0x3F, 0x20, // u 85
        0x80, 0x80, 0x80, 0x00, 0x00, 0x80, 0x80, 0x80, 0x00, 0x01, 0x0E, 0x30, 0x08, 0x06, 0x01, 0x00, // v 86
        0x80, 0x80, 0x00, 0x80, 0x00, 0x80, 0x80, 0x80, 0x0F, 0x30, 0x0C, 0x03, 0x0C, 0x30, 0x0F, 0x00, // w 87
        0x00, 0x80, 0x80, 0x00, 0x80, 0x80, 0x80, 0x00, 0x00, 0x20, 0x31, 0x2E, 0x0E, 0x31, 0x20, 0x00, // x 88
        0x80, 0x80, 0x80, 0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x81, 0x8E, 0x70, 0x18, 0x06, 0x01, 0x00, // y 89
        0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x21, 0x30, 0x2C, 0x22, 0x21, 0x30, 0x00, // z 90
        0x00, 0x00, 0x00, 0x00, 0x80, 0x7C, 0x02, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x40, 0x40, //{ 91
        0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, //| 92
        0x00, 0x02, 0x02, 0x7C, 0x80, 0x00, 0x00, 0x00, 0x00, 0x40, 0x40, 0x3F, 0x00, 0x00, 0x00, 0x00, //} 93
        0x00, 0x06, 0x01, 0x01, 0x02, 0x02, 0x04, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //~ 94

};

/*************************************************************************
 *  函数名称：Dly_ms
 *  功能说明：延时函数
 *  参数说明： ms
 *  函数返回：无
 *  备    注：
 *************************************************************************/
void Dly_ms(uint16_t ms)
{
  uint16_t ii, jj;
  if (ms < 1)
    ms = 1;
  ms = ms * 12;
  for (ii = 0; ii < ms; ii++)
    for (jj = 0; jj < 1335; jj++)
      ; // 16MHz--1ms
  //   for(jj=0;jj<4006;jj++);  //48MHz--1ms
  // for(jj=0;jj<5341;jj++);    //64MHz--1ms
}

/*************************************************************************
 *  函数名称：OLED_DLY_ms
 *  功能说明：延时函数
 *  参数说明：ms
 *  函数返回：无
 *  备    注：
 *************************************************************************/
void OLED_DLY_ms(uint16_t ms)
{
  return;
  uint16_t a;
  while (ms)
  {
    a = 16000;
    while (a--)
      ;
    ms--;
  }
  return;
}

#ifdef OLED_IIC
/**********************************************
//IIC Start
**********************************************/
#define OLED_IIC_Start() soc_I2C_GenerateSTART(ENABLE)
// void OLED_IIC_Start()
// {
//   soc_I2C_GenerateSTART(ENABLE);// 起始信号
// }

/**********************************************
//IIC Stop
**********************************************/
#define OLED_IIC_Stop() soc_I2C_GenerateSTOP(ENABLE)
// void OLED_IIC_Stop()
// {
//   soc_I2C_GenerateSTOP(ENABLE);// 停止信号
// }
/**********************************************
// 通过I2C总线写一个字节
**********************************************/
#define OLED_Write_IIC_Byte(IIC_Byte) do{soc_I2C_SendData(IIC_Byte);soc_I2C_wait();}while(0)
// void OLED_Write_IIC_Byte(unsigned char IIC_Byte)
// {
//   soc_I2C_SendData(IIC_Byte);
// 	soc_I2C_wait();
// }
#endif

/*************************************************************************
 *  函数名称：OLED_WrDat
 *  功能说明：写数据函数
 *  参数说明： data
 *  函数返回：无
 *  备    注：
 *************************************************************************/
void OLED_WrDat(uint8_t data)
{

#ifdef OLED_IIC
  OLED_IIC_Start();
  OLED_Write_IIC_Byte(0x78);
  OLED_Write_IIC_Byte(0x40); // write data
  OLED_Write_IIC_Byte(data);
  OLED_IIC_Stop();
  soc_I2C_wait_available();
#else
  uint8_t i = 8;

  OLED_DC = 1;
  OLED_SCL = 0;
  while (i--)
  {
    if (data & 0x80)
    {
      OLED_SDA = 1;
    }
    else
    {
      OLED_SDA = 0;
    }
    OLED_SCL = 1;
    asm("nop");
    OLED_SCL = 0;
    data <<= 1;
  }

#endif
}

/*************************************************************************
 *  函数名称：OLED_Wr6Dat
 *  功能说明：写数6位据函数
 *  参数说明： data
 *  函数返回：无
 *  备    注：
 *************************************************************************/
// void OLED_Wr6Dat(uint8_t data)
// {
//   uint8_t i = 6;

//   OLED_DC = 1;
//   OLED_SCL = 0;
//   while (i--)
//   {
//     if (data & 0x80)
//     {
//       OLED_SDA = 1;
//     }
//     else
//     {
//       OLED_SDA = 0;
//     }
//     OLED_SCL = 1;
//     asm("nop");
//     OLED_SCL = 0;
//     data <<= 1;
//   }
// }

/*************************************************************************
 *  函数名称：OLED_WrCmd
 *  功能说明：写数命令函数
 *  参数说明： cmd
 *  函数返回：无
 *  备    注：
 *************************************************************************/
void OLED_WrCmd(uint8_t cmd)
{

#ifdef OLED_IIC
  OLED_IIC_Start();
  OLED_Write_IIC_Byte(0x78); // Slave address,SA0=0
  OLED_Write_IIC_Byte(0x00); // write command
  OLED_Write_IIC_Byte(cmd);
  OLED_IIC_Stop();
  soc_I2C_wait_available();
#else
  uint8_t i = 8;

  OLED_DC = 0;
  ;
  OLED_SCL = 0;
  ;

  while (i--)
  {
    if (cmd & 0x80)
    {
      OLED_SDA = 1;
    }
    else
    {
      OLED_SDA = 0;
    }
    OLED_SCL = 1;
    asm("nop");

    OLED_SCL = 0;
    cmd <<= 1;
  }
#endif
}

/*************************************************************************
 *  函数名称：OLED_Set_Pos
 *  功能说明：设置位置函数
 *  参数说明： x
 *             y
 *  函数返回：无
 *  备    注：
 *************************************************************************/
void OLED_Set_Pos(uint8_t x, uint8_t y)
{
  OLED_WrCmd(0xb0 + y);
  OLED_WrCmd(((x & 0xf0) >> 4) | 0x10);
  OLED_WrCmd((x & 0x0f) | 0x01);
}

/*************************************************************************
 *  函数名称：OLED_Fill
 *  功能说明：填充函数
 *  参数说明： bmp_data
 *  函数返回：无
 *  备    注：
 *************************************************************************/
void OLED_Fill(uint8_t bmp_data)
{
  uint8_t y, x;

  for (y = 0; y < 8; y++)
  {
    OLED_WrCmd(0xb0 + y);
    OLED_WrCmd(0x01);
    OLED_WrCmd(0x10);
    for (x = 0; x < X_WIDTH; x++)
      OLED_WrDat(bmp_data);
  }
}

/*************************************************************************
 *  函数名称：OLED_CLS
 *  功能说明：清除函数
 *  参数说明： 无
 *  函数返回：无
 *  备    注：
 *************************************************************************/
void OLED_CLS(void)
{
  uint8_t y, x;
  for (y = 0; y < 8; y++)
  {
    OLED_WrCmd(0xb0 + y);
    OLED_WrCmd(0x01);
    OLED_WrCmd(0x10);
    for (x = 0; x < X_WIDTH; x++)
      OLED_WrDat(0);
  }
}

/*************************************************************************
 *  函数名称：OLED_Init
 *  功能说明：初始化函数
 *  参数说明：无
 *  函数返回：无
 *  备    注：
 *************************************************************************/
void OLED_Init(void)
{
#ifdef OLED_IIC
  OLED_DLY_ms(100);
#else
  gpio_init(OLED_RST_PIN, GPO, 0);
  gpio_init(OLED_DC_PIN, GPO, 1);
  OLED_SCL = 1;
  OLED_RST = 0;
  OLED_DLY_ms(50);
  OLED_RST = 1;
#endif
  OLED_WrCmd(0xae);       //--turn off oled panel
  OLED_WrCmd(0x00);       //---set low column address
  OLED_WrCmd(0x10);       //---set high column address
  OLED_WrCmd(0x40);       //--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
  OLED_WrCmd(0x81);       //--set contrast control register
  OLED_WrCmd(Brightness); // Set SEG Output Current Brightness
  OLED_WrCmd(0xa1);       //--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
  OLED_WrCmd(0xc8);       // Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
  OLED_WrCmd(0xa6);       //--set normal display
  OLED_WrCmd(0xa8);       //--set multiplex ratio(1 to 64)
  OLED_WrCmd(0x3f);       //--1/64 duty
  OLED_WrCmd(0xd3);       //-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
  OLED_WrCmd(0x00);       //-not offset
  OLED_WrCmd(0xd5);       //--set display clock divide ratio/oscillator frequency
  OLED_WrCmd(0x80);       //--set divide ratio, Set Clock as 100 Frames/Sec
  OLED_WrCmd(0xd9);       //--set pre-charge period
  OLED_WrCmd(0xf1);       // Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
  OLED_WrCmd(0xda);       //--set com pins hardware configuration
  OLED_WrCmd(0x12);
  OLED_WrCmd(0xdb); //--set vcomh
  OLED_WrCmd(0x40); // Set VCOM Deselect Level
  OLED_WrCmd(0x20); //-Set Page Addressing Mode (0x00/0x01/0x02)
  OLED_WrCmd(0x02); //
  OLED_WrCmd(0x8d); //--set Charge Pump enable/disable
  OLED_WrCmd(0x14); //--set(0x10) disable
  OLED_WrCmd(0xa4); // Disable Entire Display On (0xa4/0xa5)
  OLED_WrCmd(0xa6); // Disable Inverse Display On (0xa6/a7)
  OLED_WrCmd(0xaf); //--turn on oled panel
  OLED_Fill(0x00);  //初始清屏
  OLED_Set_Pos(0, 0);
}

/*************************************************************************
 * 函数名： void OLED_PutPixel(uint8_t x,uint8_t y)
 * 功能描述：绘制一个点（x,y）
 * 参数：真实坐标值(x,y),x的范围0～127，y的范围0～64
 * 返回：无
 *************************************************************************/
void OLED_PutPixel(uint8_t x, uint8_t y)
{
  uint8_t data1; // data1当前点的数据

  OLED_Set_Pos(x, y);
  data1 = 0x01 << (y % 8);
  OLED_WrCmd(0xb0 + (y >> 3));
  OLED_WrCmd(((x & 0xf0) >> 4) | 0x10);
  OLED_WrCmd((x & 0x0f) | 0x00);
  OLED_WrDat(data1);
}
/*************************************************************************
* 函数名： void OLED_Rectangle(uint8_t x1,uint8_t y1,
                   uint8_t x2,uint8_t y2,uint8_t color,uint8_t gif)
* 功能描述：绘制一个实心矩形
* 参数：左上角坐标（x1,y1）,右下角坐标（x2，y2）
      其中x1、x2的范围0～127，y1，y2的范围0～63，即真实坐标值
* 返回：无
*************************************************************************/
void OLED_Rectangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t gif)
{
  uint8_t n;

  OLED_Set_Pos(x1, y1 >> 3);
  for (n = x1; n <= x2; n++)
  {
    OLED_WrDat(0x01 << (y1 % 8));
    if (gif == 1)
      OLED_DLY_ms(50);
  }
  OLED_Set_Pos(x1, y2 >> 3);
  for (n = x1; n <= x2; n++)
  {
    OLED_WrDat(0x01 << (y2 % 8));
    if (gif == 1)
      OLED_DLY_ms(5);
  }
}
/*************************************************************************
//函数名：LCD_P6x8Str(uint8_t x,uint8_t y,uint8_t *p)
//功能描述：写入一组标准ASCII字符串
//参数：显示的位置（x,y），y为页范围0～7，要显示的字符串
//返回：无
*************************************************************************/
void OLED_P6x8Str(uint8_t x, uint8_t y, uint8_t ch[])
{
  uint8_t c = 0, i = 0, j = 0;
  while (ch[j] != '\0')
  {
    c = ch[j] - 32;
    if (x > 126)
    {
      x = 0;
      y++;
    }
    OLED_Set_Pos(x, y);
    for (i = 0; i < 6; i++)
      OLED_WrDat(F6x8[c][i]);
    x += 6;
    j++;
  }
}
/*************************************************************************
 * 函数名：LCD_P8x16Str(uint8_t x,uint8_t y,uint8_t *p)
 * 功能描述：写入一组标准ASCII字符串
 * 参数：显示的位置（x,y），y为页范围0～7，要显示的字符串
 * 返回：无
 *************************************************************************/
void OLED_P8x16Str(uint8_t x, uint8_t y, uint8_t ch[])
{
  uint8_t c = 0, i = 0, j = 0;

  while (ch[j] != '\0')
  {
    c = ch[j] - 32;
    if (x > 120)
    {
      x = 0;
      y++;
    }
    OLED_Set_Pos(x, y);
    for (i = 0; i < 8; i++)
      OLED_WrDat(F8X16[c * 16 + i]);
    OLED_Set_Pos(x, y + 1);
    for (i = 0; i < 8; i++)
      OLED_WrDat(F8X16[c * 16 + i + 8]);
    x += 8;
    j++;
  }
}

/***********************************************************
 * 函数功能：数字转换成ACSII
 * 入口参数：num :   需要显示的数
 *           Print : 转换后的数组指针
 * 出口参数：无
 ***********************************************************/

void OLED_NUMACSII(uint16_t num, uint8_t *Print)
{
  uint8_t numcheck;
  uint8_t TEMP;
  TEMP = 6;
  Print[TEMP] = '\0';
  while (TEMP)
  {
    TEMP--;
    numcheck = num % 10;
    num /= 10;
    Print[TEMP] = numcheck + 0x30;
  }
}

/***********************************************************
 * 函数名称：OLED_Print_Num
 * 函数功能：数字显示
 * 入口参数：x : 位置
 *           y : 位置
 *          num: 显示的数字
 * 出口参数：无
 ***********************************************************/
void OLED_Print_Num(uint8_t x, uint8_t y, uint16_t num)
{
  uint8_t ch[7];
  uint8_t i = 0;

  OLED_NUMACSII(num, ch);

  // while (ch[ii] == 0x30)
  // {
  //   ii++;
  // }

  while (ch[i] != '\0')
  {
    OLED_P8x16Str(x, y, ch + i); //显示数字
    x += 8;
    i++;
  }
}

/***********************************************************
 * 函数名称：OLED_Print_Num
 * 函数功能：数字显示
 * 入口参数：x : 位置
 *           y : 位置
 *          num: 显示的数字
 * 出口参数：无
 ***********************************************************/
void OLED_Print_Num2(uint8_t x, uint8_t y, uint16_t num)
{
  uint8_t ch[7] = {0};
  uint8_t i = 6;

  OLED_NUMACSII(num, ch);

  while (num)
  {
    i--;
    ch[i] = num % 10 + 0x30;
    num /= 10;
  }

  OLED_P8x16Str(x, y, ch+i); //显示数字
}

/***********************************************************
 * 函数名称：OLED_Print_HexNum
 * 函数功能：显示4位16进制数字
 * 入口参数：x : 位置
 *           y : 位置
 *          num: 显示的数字
 * 出口参数：无
 ***********************************************************/
void OLED_Print_HexNum(uint8_t x, uint8_t y, int16_t n)
{
  uint8_t ch[] = "     ";
  int8_t i = 4;
  uint16_t num;
  const uint8_t hexTab[] = "0123456789ABCDEF";
  if (n < 0)
  {
    num = -n;
    ch[0] = '-';
  }
  else
  {
    num = n;
  }
  while (num)
  {
    ch[i] = hexTab[num & 0x000f];
    num >>= 4;
    i--;
  }
  OLED_P8x16Str(x, y, ch); //显示数字
}

/***********************************************************
 * 函数名称：OLED_Print_Str
 * 函数功能：字符显示
 * 入口参数：x : 位置
 *           y : 位置
 *          ch: 显示的字符串数组
 * 出口参数：无
 ***********************************************************/
void OLED_Print_Str(uint8_t x, uint8_t y, uint8_t ch[])
{
  uint8_t ch2[3];
  uint8_t ii = 0;
  while (ch[ii] != '\0')
  {
    ch2[0] = ch[ii];
    ch2[1] = '\0';            //字母占一个字节
    OLED_P8x16Str(x, y, ch2); //显示字母
    x += 8;
    ii += 1;
  }
}

/***********************************************************
 * 函数名： void Draw_BMP(uint8_t x,uint8_t y)
 * 功能描述：显示BMP图片128×64
 * 参数：起始点坐标(x,y),x的范围0～127，y为页的范围0～7
 * 返回：无
 ***********************************************************/
void Draw_BMP(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t bmp[])
{
  uint16_t ii = 0;
  uint8_t x, y;

  for (y = y0; y < y1; y++)
  {
    OLED_Set_Pos(x0, y);
    for (x = x0; x < x1; x++)
    {
      OLED_WrDat(bmp[ii++]);
    }
  }
}

// void OLEDxianshi(int line_use)
// {

//   int bmp[8][128];
//   int line = 0, lie = 0;
//   int image[64][128];
//   for (int j = 0; j < 64; j++)
//   {
//     for (int i = 0; i < 128; i++)
//       image[j][i] = 0;
//   }
//   for (int j = 0; j < 8; j++)
//   {
//     for (int i = 0; i < 128; i++)
//       bmp[j][i] = 0;
//   }
//   for (int j = 0; j < 120; j += 2)
//   {
//     for (int i = 0; i < 188; i += 2)
//     {
//       if (Image_Data[j][i] > Cmp && Pick_table[j] != i)
//       {
//         image[line][lie] = 1;
//       }
//       lie++;
//     }
//     lie = 0;
//     line++;
//   }
//   //划定有效行
//   /*for(int i=0;i<128;i++)
//   {
//       image[(128-line_use)/2][i]=0;
//   }*/

//   for (int i = 0; i < 128; i++)
//   {
//     image[10][i] = 0;
//     image[20][i] = 0;
//     image[30][i] = 0;
//     image[40][i] = 0;
//     image[50][i] = 0;
//     image[60][i] = 0;
//   }
//   //划定中线
//   for (int i = 0; i < 64; i++)
//   {
//     image[i][45] = 0;
//     image[i][93] = 1;
//   }

//   for (int j = 0; j < 8; j++)
//   {
//     for (int i = 0; i < 128; i++)
//     {
//       for (int k = 0; k < 8; k++)
//         bmp[j][i] = bmp[j][i] + ((image[j * 8 + k][i]) << (k));
//     }
//   }

//   for (int y = 0; y < 8; y++)
//   {
//     OLED_WrCmd(0xb0 + y);
//     OLED_WrCmd(0x01);
//     OLED_WrCmd(0x10);
//     for (int x = 0; x < 128; x++)
//     {
//       OLED_WrDat(bmp[y][x]);
//     }
//   }
// }
