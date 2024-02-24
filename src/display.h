#ifndef DISPLAY_H
#define DISPLAY_H

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH1106.h>

#include <Fonts/FreeSans9pt7b.h>
#ifndef FONT_H_OFFSET
    #define FONT_H_OFFSET 0
#endif

#include "config.h"

class Display_SH1106{
public:
    Adafruit_SH1106 *display;
    int width, height;

    Display_SH1106(int width_, int height_);

    void init(int adress);
    void init_font(int size = 1);

    void clear();
    void show();

    int get_text_width(String text);
    int get_text_height(String text);

    void print(String text, int cur_x = 0, int cur_y = 0);
    void write_char(char chr, int cur_x = 0, int cur_y = 0);
    void draw_line(int x0, int y0, int x1, int y1, bool inverted = false);
    void draw_rect(int x, int y, int w, int h, bool inverted = false, bool filled = false);
    void draw_bitmap(int x, int y, const uint8_t bitmap[], int w, int h, bool inverted = false);
};

#endif