#include "ui.h"

//-------------------------------------UI-----------------------------------------

UI::UI(Menu* default_menu_, Display_SH1106 *display_, int font_h_, int row_h_){
    init(default_menu_, display_, font_h_, row_h_);
}

void UI::init(Menu* default_menu_, Display_SH1106 *display_, int font_h_, int row_h_){
    default_menu = default_menu_;
    display = display_;

    menu_window = Window(display, row_h_);
    function_window = Window(display, row_h_);
    
    set_function_containter();
}

void UI::set_function_containter(){
    functions = new Function_container();
    functions -> event = &func_event;
}

void UI::render(){
    //uint64_t timeout = millis() - last_render;
    last_render = millis();

    if(current_menu == nullptr)             // TODO: avoid empty pointer in menu
        {}
    else{
        render_menu(current_menu);

        if(function_is_running)
            render_function();
    }
    
    if(functions -> quit){
        function_is_running = false;
        delete functions;
        set_function_containter();
        render();
    }

    event_done = false;
    display -> show();
}

void UI::render_menu(Menu* menu){
    menu_window.clear();

    if(menu -> items[0].link_menu == nullptr)                   //disabling "home" which is nullptr in menu
        scroll = max(scroll, (uint8_t) 1);

    for(uint8_t i = 0; i < min((int) menu -> size - scroll, menu_window.max_rows); i++){
        uint8_t i_absolute = i + scroll;
        String name = menu -> items[i_absolute].name;

        int x_offset = 0;
        if(i_absolute == current_item){
             x_offset = SELECT_X_OFFSET;
             // TODO: add some arrow or selecting area with rectangle to highlight selected item
        }

        menu_window.print(name, x_offset, menu_window.row_h * i);
    }
}

void UI::render_function(){
    function_window.clear();

    functions -> execute(current_func -> index);

    func_event.moved = 0;
    func_event.selected = 0;
}

void UI::move(int steps){
    event_done = true;

    // filter of multiple scroll
    if(filter_scrolling)                                            
        steps = steps / abs(steps);

    // send user move to function
    if(function_is_running){                                        
        func_event.moved += steps;
        return;
    }
    if(current_menu == nullptr) return;

    // scroll through the menu
    if(steps < 0)                                                   
        current_item = max(current_item + steps, 0);
    else
        current_item = min(current_item + steps, current_menu -> size - 1);

    if(current_item >= scroll + menu_window.max_rows)
        scroll = current_item - menu_window.max_rows + 1;
    else
        scroll = min(current_item, scroll);

    //disabling "home" which is nullptr in menu
    if(current_menu -> items[0].link_menu == nullptr)               
        current_item = max(current_item, (uint8_t) 1);
}

void UI::select(){
    // filter multiple (false) clicks
    if(filter_clicks && millis() - last_click_time < MIN_CLICK_INTERVAL)
        return;
    last_click_time = millis();

    event_done = true;

    // send user tap to function
    if(function_is_running){
        func_event.selected++;
        return;
    }
    if(current_menu == nullptr)             // TODO: remove this
        return;
    

    uint8_t type = current_menu -> items[current_item].type;                // check menu or function selected
    if(type == MENU || type == BACK){                                       // change folder
        current_menu = current_menu -> items[current_item].link_menu;

        current_item = 1;
        scroll = 1;
    }
    if(type == FUNC || type == PARAM){                                      //send to function
        function_is_running = true;
        current_func = current_menu -> items[current_item].link_func;
        functions -> window = &function_window; 
    }
}

//prints full menu tree to serial port
void UI::tree(){
    Serial.println("root:");
    print_tree(default_menu, 1);
}

//recursive menu bypass
void UI::print_tree(Menu* menu, uint8_t deep){
    for(uint8_t i = 1; i < menu -> size; i++){
        for(uint8_t j = 0; j < deep; j++) Serial.print("    ");
            Serial.print(menu -> items[i].name + ":");

        if(menu -> items[i].type == FUNC || menu -> items[i].type == PARAM){
            Serial.print(menu -> items[i].link_func -> index);
        }
        Serial.println();
        
        if(menu -> items[i].type == MENU || menu -> items[i].type == BACK){
            print_tree(menu -> items[i].link_menu, deep + 1);
        }
        
    }
}

//--------------------------------------------------------------------------------



//------------------------------------MENU----------------------------------------

Menu* load_menu(String menu_list[], uint8_t menu_sizes[], uint8_t menu_linking[], uint8_t menu_types[], uint8_t menu_n){
    uint8_t items_n = 0;
    uint8_t menu_list_i[menu_n];
    for(uint8_t i = 0; i < menu_n; i++){
        menu_list_i[i] = items_n;
        items_n += menu_sizes[i];
    }

    Menu* menus[menu_n];
    for(uint8_t i = 0; i < menu_n; i++){
        String items[menu_sizes[i] + 1];

        for(uint8_t j = 0; j < menu_sizes[i]; j++){
            items[j + 1] = menu_list[menu_list_i[i] + j];
        }

        menus[i] = new Menu(items, menu_sizes[i] + 1);
    }

    for(uint8_t i = 0; i < menu_n; i++){
        for(uint8_t j = 0; j < menu_sizes[i]; j++){
            uint8_t cur_i = menu_list_i[i] + j;

            menus[i] -> items[j + 1].type = menu_types[cur_i];

            if(menu_types[cur_i] == MENU){
                menus[i] -> items[j + 1].link_menu = menus[menu_linking[cur_i]];
                menus[menu_linking[cur_i]] -> items[0] = Menu_item(BACK_NAME, menus[i], true);
            }
            if(menu_types[cur_i] == FUNC || menu_types[cur_i] == PARAM){
                menus[i] -> items[j + 1].link_func = new Menu_func(menu_list[cur_i], menu_linking[cur_i]);
                menus[i] -> items[j + 1].link_func -> root = new Menu_item(BACK_NAME, menus[i], true);
            }
        }
    }

    menus[0] -> items[0].type = BACK;
    menus[0] -> items[0].name = MAIN_PAGE_NAME;

    return menus[0];
}

Menu::Menu(String items_[], uint8_t size_){
    size = size_;
    for(uint8_t i = 0; i < size; i++){
        items[i].name = items_[i];
    }
}

Menu_func::Menu_func(String name_, uint8_t index_){
    name = name_;
    index = index_;
}

Menu_item::Menu_item(String name_, Menu* link, bool back){
    name = name_;
    link_menu = link;
    type = (back) ? BACK : MENU;
}

Menu_item::Menu_item(String name_, Menu_func* link){
    name = name_;
    link_func = link;
    type = FUNC;
}

//--------------------------------------------------------------------------------



//----------------------------------WINDOW----------------------------------------

Window::Window(Display_SH1106 *display_, int x_, int y_, int w_, int h_, int row_h_){
    display = display_;
    x = x_;
    y = y_;
    w = w_;
    h = h_;

    row_h = row_h_;
    max_rows = h / row_h;
}

Window::Window(Display_SH1106 *display_, int x_, int y_, int row_h_){
    display = display_;
    x = x_;
    y = y_;
    w = display -> width;
    h = display -> height;

    row_h = row_h_;
    max_rows = h / row_h;
}

Window::Window(Display_SH1106 *display_, int row_h_){
    display = display_;
    x = 0;
    y = 0;
    w = display -> width;
    h = display -> height;

    row_h = row_h_;
    max_rows = h / row_h;
}

void Window::clear(){
    display -> draw_rect(x, y, w, h, true, true);
}

int Window::get_text_width(String text){
    return display -> get_text_width(text);
}

void Window::print(String text, int cur_x, int cur_y){
    display -> print(text, cur_x + x, cur_y + y);
}

void Window::print_right(String text, int cur_x, int cur_y){
    display -> print(text, cur_x + x - display -> get_text_width(text), cur_y + y);
}

void Window::print_fit(String text, int cur_x, int cur_y){
    int text_w = display -> get_text_width(text);
    if(cur_x + text_w > w) cur_x = w - text_w;
    if(cur_y + row_h > h) cur_y = h - row_h;
    display -> print(text, cur_x + x, cur_y + y);
}

void Window::print_centered(String text, bool center_h, int cur_y){
    int text_x = w / 2 - display -> get_text_width(text) / 2;
    int text_y = cur_y;
    if(center_h)
        text_y = h / 2 - display -> get_text_height(text) / 2;
    
    display -> print(text, text_x + x, text_y + y);
}

void Window::draw_line(int x0, int y0, int x1, int y1, bool inverted){
    display -> draw_line(x + x0, y + y0, x + x1, y + y1, inverted);
}

void Window::draw_rect(int x_, int y_, int w_, int h_, bool inverted, bool filled){
    display -> draw_rect(x + x_, y + y_, w_, h_, inverted, filled);
}

//--------------------------------------------------------------------------------