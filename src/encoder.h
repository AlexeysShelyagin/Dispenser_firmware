#ifndef ENCODER_H
#define ENCODER_H

#ifndef ENCODER_BUFFER_SIZE
    #define ENCODER_BUFFER_SIZE 20
#endif

#include <Arduino.h>

class Encoder_data{
public:
    int turns = 0, clicks = 0;

    Encoder_data() = default;
    Encoder_data(int turns_, int clicks_);
};

class Encoder{
    bool buffer[ENCODER_BUFFER_SIZE][2];
    bool click = false, last_click_state = false;
    int buff_size;

    int sa_pin, sb_pin, sw_pin;

    bool ready_to_read = true;

    uint64_t tick_time_filter = 0, click_time_filter = 0;

    void handle_click();

    int get_rotation(bool v1, bool v2);
    bool get_click(bool val);
public:

    Encoder(int sa_pin_, int sb_pin_, int sw_pin_);

    void tick();

    Encoder_data get_updates();
    Encoder_data get_updates_normalized();
};

#endif