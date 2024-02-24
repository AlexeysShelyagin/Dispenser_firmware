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

void Function_container::list_template(String list[], int options_n){
    if(event -> moved){
        if(event -> moved < 0)                                                   
            selected = max(selected + event -> moved, 0);
        else
            selected = min(selected + event -> moved, options_n - 1);

        if(selected >= scroll + window -> max_rows)
            scroll = selected - window -> max_rows + 1;
        else
            scroll = min(selected, scroll);
    }

    for(uint8_t i = 0; i < min((int) options_n - scroll, window -> max_rows); i++){
        uint8_t i_absolute = i + scroll;
        String line;

        if(i_absolute == selected)
             line += ">";
        line += list[i_absolute];

        window -> print(line, 0, window -> row_h * (i + 1) - window -> font_h);
    }

    window -> draw_scroll_bar(options_n, scroll);
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
    if(first_call){
        scroll = selected = 1;
        first_call = false;
    }

    String options[DISPENSE_SLOTS_N + 1];
    options[0] = "back";
    for(uint8_t i = 1; i <= DISPENSE_SLOTS_N; i++){
        options[i] = "slot " + String(i) + ":    ";
    }
    list_template(options, DISPENSE_SLOTS_N + 1);

    if(event -> selected){
        if(selected == 0)
            quit = true;
        else{
            
        }
    }
}

void Function_container::func1(){
    if(event -> selected)
        quit = true;
}

void Function_container::func2(){
    if(event -> selected)
        quit = true;
}