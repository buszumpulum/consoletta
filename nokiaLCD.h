#ifndef _NOKIALCD_H
#define _NOKIALCD_H
#include <Arduino.h>


void lcdFunTime();
void setPixel(int x, int y);
void clearPixel(int x, int y);
void setPixel(int x, int y, boolean bw);
void setLine(int x0, int y0, int x1, int y1, boolean bw);
void setRect(int x0, int y0, int x1, int y1, boolean fill, boolean bw);
void setCircle (int x0, int y0, int radius, boolean bw, int lineThickness);
void setChar(char character, int x, int y, boolean bw);
void setStr(char * dString, int x, int y, boolean bw);
void setBitmap(const char * bitArray);
void clearDisplay(boolean bw);
void gotoXY(int x, int y);
void updateDisplay();
void setContrast(byte contrast);
void invertDisplay();
void lcdBegin(void);
void backlightAnimate(byte bgn, byte stp,int time);
void setBacklight(byte brightness);

#endif
