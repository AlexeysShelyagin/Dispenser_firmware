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
void Spin_value < T >::change(double n){
    if(!limited){
        *value += n * step;
        return;
    }

    if(n > 0) *value = min(static_cast < T > (*value + n * step), limit_max);
    else if(-(n * step) > *value) *value = 0;
    else *value = max(static_cast < T > (*value + n * step), limit_min);
}

template < class T >
void Spin_value < T >::render(Window *window, bool selected){
    String bracket_open = ":< ", bracket_close = " >";
    if(selected && (millis() / (BLINK_TIME * 2)) % 2){
        bracket_open = ":   ";
        bracket_close = "";
    }
    window -> print(name + bracket_open + String(*value) + bracket_close, x, y);
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

    T step;
    if(fixed_step == 0) 
        step = ceil((max_lim - min_lim) * SPIN_SPEED_FACTOR / timeout);
    else 
        step = fixed_step;

    Spin_value < T > spin_val(*variable, step, st_x, st_y, name);
    spin_val.set_limits(min_lim, max_lim);

    if(event -> selected)
        quit = true;

    spin_val.change(event -> moved);
    
    spin_val.render(window, true);
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

void Function_container::list_template(String list[], int options_n, bool handle_event){
    if(handle_event && event -> moved){
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
    case 3:
        func3();
        break;
    case 4:
        func4();
        break;
    case 5:
        func5();
        break;
    case 6:
        func6();
        break;
    case 7:
        func7();
        break;
    case 8:
        func8();
        break;
    case 9:
        func9();
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
        options[i] = "slot " + String(i) + ":    " + String(values -> ammounts[i - 1]);
    }
    list_template(options, DISPENSE_SLOTS_N + 1);

    if(event -> selected){
        if(selected == 0)
            quit = true;
        else{
            values -> dispenser_mode = Dispenser_modes::DISPENSE;
            values -> ammount = values -> ammounts[selected - 1];
        }
    }
}

void Function_container::func1(){
    single_spin_template < float > ("mass", &(values -> reference_mass), 0, MAX_REFERENCE_MASS, 0, 0, 5);
    if(event -> selected)
        values -> save();
}

void Function_container::func2(){
    if(event -> selected && !first_call){
        values -> dispenser_mode = Dispenser_modes::CALIBRATE;
        quit = true;
    }
    
    if(first_call){
        values -> dispenser_mode = Dispenser_modes::TARE;
        first_call = false;
    }
    else{
        window -> print_centered("Put " + String(values -> reference_mass, 1) + "g");
        window -> print_centered("click to", false, window -> row_h);
        window -> print_centered("calibrate", false, window -> row_h * 2);
    }
}

void Function_container::func3(){
    values -> dispenser_mode = Dispenser_modes::CLEAN;
    quit = true;
}

void Function_container::func4(){
    if(first_call){
        scroll = selected = 1;
        first_call = false;
    }

    String options[DISPENSE_SLOTS_N + 1];
    options[0] = "back";
    for(uint8_t i = 1; i <= DISPENSE_SLOTS_N; i++){
        uint16_t ammount = values -> ammounts[i - 1];

        bool blink_state = (millis() / (BLINK_TIME * 2)) % 2;
        if(selected_to_change == i && blink_state)
            options[i] = "slot " + String(i) + ":   " + String(ammount) + " ";
        else
            options[i] = "slot " + String(i) + ": <" + String(ammount) + ">";
    }

    if(selected_to_change == 0){
        list_template(options, DISPENSE_SLOTS_N + 1);
        if(event -> selected){
            if(selected == 0){
                quit = true;
                values -> save();
            }
            else
                selected_to_change = selected;
        }
    }
    else{
        list_template(options, DISPENSE_SLOTS_N + 1, false);
        if(event -> selected)
            selected_to_change = 0;
        if(event -> moved){
            int16_t new_ammount = values -> ammounts[selected_to_change - 1] + event -> moved;
            
            if(new_ammount < 0)
                new_ammount = 0;
            if(new_ammount > MAX_DISPENSE_AMMOUNT)
                new_ammount = MAX_DISPENSE_AMMOUNT;

            values -> ammounts[selected_to_change - 1] = new_ammount;
        }
    }
}

void Function_container::func5(){
    single_spin_template < float > ("spd", &(values -> feed_speed), 0.2, MAX_FEED_SPEED, 0, 0, 0.1);
    if(event -> selected)
        values -> save();
}

void Function_container::func6(){
    single_spin_template < uint16_t > ("weight", &(values -> ammount), 0, MAX_DISPENSE_AMMOUNT, 0, 0, 1);

    bool blink = (millis() / (BLINK_TIME * 2)) % 2;
    if(values -> ammount == 0){
        if(blink)
            window -> print_centered("back", false, window -> h - window -> row_h);
    }
    else{
        if(quit)
            values -> dispenser_mode = Dispenser_modes::DISPENSE;
    }
}

void Function_container::func7(){
    String lines[] = {
        "Dispenser by:",
        "DevelTeam",
        "",
        "Firmware ver.:",
        FIRMWARE_VERSION,
        "",
        "MAC:",
        (values -> mac_addr).substring(0, 9),
        (values -> mac_addr).substring(8)
    };
    list_template(lines, 9);

    if(event -> selected)
        quit = true;
}

void Function_container::func8(){
    window -> print_centered("Restore to");
    window -> print_centered("defaults?", false, window -> row_h);

    String brackets = "<        >";
    if((millis() / (BLINK_TIME * 2)) % 2)
        brackets = "";

    int check_h = window -> h - window -> row_h;
    String options[] = {"NO", "YES"};
    window -> print_centered(options[selected], false, check_h);
    window -> print_centered(brackets, false, check_h);

    if(event -> selected){
        if(selected != 0){
            values -> clear();
            values -> dispenser_mode = Dispenser_modes::RESTORE;
        }
        quit = true;
    }
    
    if(event -> moved)
        selected = !selected;
}

void Function_container::func9(){
    window -> print_centered("Update");
    window -> print_centered("firmware?", false, window -> row_h);

    String brackets = "<        >";
    if((millis() / (BLINK_TIME * 2)) % 2)
        brackets = "";

    int check_h = window -> h - window -> row_h;
    String options[] = {"NO", "YES"};
    window -> print_centered(options[selected], false, check_h);
    window -> print_centered(brackets, false, check_h);

    if(event -> selected){
        if(selected != 0)
            values -> dispenser_mode = Dispenser_modes::UPDATE;
        quit = true;
    }

    if(event -> moved)
        selected = !selected;
}