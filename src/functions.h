#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <Arduino.h>
#include "ui.h"
#include "values.h"

#define SPIN_SPEED_FACTOR 1.5

class Window;
class function_event_t;

template < class T >
class Spin_value{
    String name;
    int x, y;

    int w = -1, h = -1;

    T *value;
    T step;
    T limit_min, limit_max;
    bool limited = false;

    bool slider_setup(Window *window);
public:

    Spin_value() = default;
    Spin_value(T &value_, T step_, int x_ = 0, int y_ = 0, String name_ = "");

    void set_size(int w_, int h_);
    void set_limits(T min, T max);
    void change(T n);

    void render(Window *window);
};

class Function_container{
    uint64_t last_call = 0;
    uint8_t selected = 0;
    uint8_t scroll = 0;
    uint8_t selected_to_change = 0;
    bool first_call = true;

    template < class T >
    void single_spin_template(String name, T* variable, T min_lim, T max_lim, int st_x, int st_y, T fixed_step = 0);

    void list_spin_template(String list[], int options_n, String name = "", int x = 0, int y = 0);

    void list_template(String list[], int options_n, bool handle_event = true);

    void func0();
    void func2();
    void func4();
    void func8();
    
public:
    Window* window;
    function_event_t* event;

    bool quit = false;

    Function_container() = default;

    void execute(int index);
};

#endif