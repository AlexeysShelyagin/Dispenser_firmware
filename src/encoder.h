#ifndef ENCODER_H
#define ENCODER_H

#define MAX_BUFFER_SIZE 100

#include <Arduino.h>

class Encoder_data{
public:
    int turns = 0, clicks = 0;

    Encoder_data() = default;
    Encoder_data(int turns_, int clicks_);
};

class Encoder{
    bool buffer[MAX_BUFFER_SIZE][3];
    int buff_size;

    int sa_pin, sb_pin, sw_pin;

    bool ready_to_read = true;
    bool unpressed = true;

    uint64_t tick_time_filter = 0;

    int get_rotation(bool v1, bool v2);
    bool get_click(bool val);
public:

    Encoder(int sa_pin_, int sb_pin_, int sw_pin_);

    void tick();

    Encoder_data get_updates();
    Encoder_data get_updates_normalized();
};

#endif