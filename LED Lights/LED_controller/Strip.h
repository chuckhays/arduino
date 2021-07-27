#ifndef _STRIP_H
#define _STRIP_H

#include <Arduino.h>

uint16_t numPixels();
void show();
uint32_t Color(uint8_t r, uint8_t g, uint8_t b, uint8_t w);
void setupStrip();
void setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b, uint8_t w);
void setPixelColor(uint16_t n, uint32_t c);

#endif //_STRIP_H
