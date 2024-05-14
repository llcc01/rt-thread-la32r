//QlVQVC1LNjYgbWFkZGV2aWwgNzkzNTU4NzU4QlVQVC1LNjYgbWFkZGV2aWwgNzkzNTU4NzU4
/*!                    北京邮电大学 K66 学习例程
 *  文件名称：       OLED.h
 *      作者：       maddevil
 *      说明：       仅做内部学习使用，请勿外传
 *  参考资料：       历届学长代码、山外K60库、龙邱K66模板、北邮KEA模板
 *    版本号：       V1.0.0
 *  最后更新：       2018-12-21 13:41
 */
//QlVQVC1LNjYgbWFkZGV2aWwgNzkzNTU4NzU4QlVQVC1LNjYgbWFkZGV2aWwgNzkzNTU4NzU4
#ifndef _OELD_H
#define _OELD_H

#include <stdint.h>

#include "soc_i2c.h"

void OLED_Init(void);
void OLED_CLS(void);
void OLED_P6x8Str(uint8_t x, uint8_t y, uint8_t ch[]);
void OLED_P8x16Str(uint8_t x, uint8_t y, uint8_t ch[]);
void OLED_P14x16Str(uint8_t x, uint8_t y, uint8_t ch[]);
void OLED_NUMACSII(uint16_t hex, uint8_t *Print);
void OLED_Print_Num(uint8_t x, uint8_t y, uint16_t num);
void OLED_Print_Num2(uint8_t x, uint8_t y, uint16_t num);
void OLED_Print_HexNum(uint8_t x, uint8_t y, int16_t n);
void OLED_Print_Str(uint8_t x, uint8_t y, uint8_t ch[]);
void OLED_PutPixel(uint8_t x, uint8_t y);
void OLED_Rectangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t gif);
void OLED_Set_Pos(uint8_t x, uint8_t y);
void OLED_WrDat(uint8_t data);
void Draw_BMP(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t bmp[]);
void OLED_Fill(uint8_t dat);
void Dly_ms(uint16_t ms);
void OLED_Wr6Dat(uint8_t data);

void OLEDxianshi(int line_use);

// extern uint8 Image_Data[IMAGEH][IMAGEW]; //图像原始数据存放
// extern uint8 Cmp;
// extern uint16 Pick_table[IMAGEH];

#endif
