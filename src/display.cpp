#include "display.h"

Display_SH1106::Display_SH1106(int width_, int height_){
    display = new Adafruit_SH1106 (-1);
    width = width_;
    height = height_;
}

void Display_SH1106::init(int adress){
    display -> begin(SH1106_SWITCHCAPVCC, adress);
}

void Display_SH1106::init_font(int size){
    display -> setTextSize(size);
    display -> setTextColor(WHITE);
    display -> cp437(true);
    display -> setTextWrap(false);
}

void Display_SH1106::clear(){
    display -> clearDisplay();
}

void Display_SH1106::show(){
    display -> display();
}

int Display_SH1106::get_text_width(String text){
    int16_t  x1, y1;
    uint16_t w, h;
    display -> getTextBounds(text, 0, 0, &x1, &y1, &w, &h);
    return w;
}

int Display_SH1106::get_text_height(String text){
    int16_t  x1, y1;
    uint16_t w, h;
    display -> getTextBounds(text, 0, 0, &x1, &y1, &w, &h);
    return h;
}

void Display_SH1106::print(String text, int cur_x, int cur_y){
    display -> setCursor(cur_x, cur_y);
    display -> print(text);
}

void Display_SH1106::draw_line(int x0, int y0, int x1, int y1, bool inverted){
    uint16_t color = (inverted) ? 0 : 1;
    display -> drawLine(x0, y0, x1, y1, color);
}

void Display_SH1106::draw_rect(int x, int y, int w, int h, bool inverted, bool filled){
    uint16_t color = (inverted) ? 0 : 1;
    if(filled)
        display -> fillRect(x, y, w, h, color);
    else
        display -> drawRect(x, y, w, h, color);
}

void Display_SH1106::draw_bitmap(int x, int y, const uint8_t bitmap[], int w, int h, bool inverted){
    display -> drawBitmap(x, y, bitmap, w, h, !inverted);
}