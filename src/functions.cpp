#include "functions.h"

template < class T >
Spin_value < T >::Spin_value(T &value_, T step_, int x_, int y_, String name_){
    x = x_;
    y = y_;
    name = name_;
    value = &value_;
    step = step_;
}

template < class T >
void Spin_value < T >::set_size(int w_, int h_){
    w = w_;
    h = h_;
}

template < class T >
void Spin_value < T >::set_limits(T min, T max){
    limit_min = min;
    limit_max = max;
    limited = true;
}

template < class T >
void Spin_value < T >::change(T n){
    if(!limited){
        *value += n * step;
        return;
    }

    if(n > 0) *value = min(static_cast < T > (*value + n * step), limit_max);
    else *value = max(static_cast < T > (*value + n * step), limit_min);
}

template < class T >
void Spin_value < T >::render(Window *window){
    window -> print(name + ":< " + String(*value) + " >", x, y);
}

template < class T >
bool Spin_value < T >::slider_setup(Window *window){
    if(!limited){
        Serial.println("Unable to draw slider <" + name + ">");
        return false;
    }
    if(w == -1 || h == -1){
        w = window -> w;
        h = window -> h;
    }

    return true;
}

template < class T >
void Function_container::single_spin_template(String name, T* variable, T min_lim, T max_lim, int st_x, int st_y, T fixed_step){
    uint64_t timeout = millis() - last_call;
    if(event -> moved != 0) last_call = millis();

    int step;
    if(fixed_step == 0) 
        step = ceil((max_lim - min_lim) * SPIN_SPEED_FACTOR / timeout);
    else 
        step = fixed_step;

    Spin_value < T > spin_val(*variable, step, st_x, st_y, name);
    spin_val.set_limits(min_lim, max_lim);

    if(event -> selected)
        quit = true;

    spin_val.change(event -> moved);
    
    spin_val.render(window);
}

void Function_container::list_spin_template(String list[], int options_n, String name, int x, int y){
    if(event -> selected)
        quit = true;

    if(event -> moved){
        selected += event -> moved;
        selected %= options_n;
    }

    window -> print(name + ":< " + list[selected] + " >", x, y);
}

void Function_container::execute(int index){
    switch (index){
    case 0:
        func0();
        break;
    case 1:
        func1();
        break;
    case 2:
        func2();
        break;
    default:
        Serial.println("Function with index " + String(index) + " not found");
        quit = true;
        break;
    }
}

void Function_container::func0(){
    Serial.println(0);
    if(event -> selected)
        quit = true;
}

void Function_container::func1(){
    Serial.println(1);
    if(event -> selected)
        quit = true;
}

void Function_container::func2(){
    Serial.println(2);
    if(event -> selected)
        quit = true;
}