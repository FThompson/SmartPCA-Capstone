/*
 * Header for Adafruit's code to draw BMPs from the TFT's SD card.
 */

#ifndef DrawBMP_h
#define DrawBMP_h

void drawBMP(Adafruit_HX8357 &tft, const char *filename, uint8_t x, uint16_t y);

#endif
