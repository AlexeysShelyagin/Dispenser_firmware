#ifndef UI_H
#define UI_H

#define MAX_MENU_ITEMS 30

#include <Arduino.h>

#include "display.h"
#include "functions.h"

#define SELECT_X_OFFSET 10
#define MAIN_PAGE_NAME "Home"
#define BACK_NAME "Back"
#define LIMIT_SCROLL_TO_1 true      // filter multiple triggering
#define FILTER_CLICKS true          // filter multiple triggering
#define MIN_CLICK_INTERVAL 200

#define DEFAULT_ROW_H 10

#define MENU 0
#define BACK 1
#define FUNC 2
#define PARAM 3

class Menu;
class Menu_item;
class Menu_func;
class Function_container;

//---------------------------FUNCTION EVENT TYPE----------------------------------

class function_event_t{
public:
    int moved = 0, selected = 0;

    function_event_t() = default;
};

//--------------------------------------------------------------------------------



//----------------------------------WINDOW----------------------------------------

class Window{
public:
    int x, y;
    int w, h;
    int row_h, max_rows;
    Display_SH1106 *display;

    Window() = default;
    Window(Display_SH1106 *display_, int x_, int y_, int w_, int h_, int row_h_ = DEFAULT_ROW_H);
    Window(Display_SH1106 *display_, int x_, int y_, int row_h_ = DEFAULT_ROW_H);
    Window(Display_SH1106 *display_, int row_h_ = DEFAULT_ROW_H);

    void clear();

    int get_text_width(String text);

    void print(String text, int cur_x = 0, int cur_y = 0);
    void write_char(char chr, int cur_x = 0, int cur_y = 0);
    void print_right(String text, int cur_x = 0, int cur_y = 0);
    void print_fit(String text, int cur_x = 0, int cur_y = 0);
    void print_centered(String text, bool center_h = false, int cur_y = 0);

    void draw_line(int x0, int y0, int x1, int y1, bool inverted = false);
    
    void draw_rect(int x_, int y_, int w_, int h_, bool inverted = false, bool filled = false);
};

//--------------------------------------------------------------------------------



//-------------------------------------UI-----------------------------------------

class UI{
    Display_SH1106 *display;
    uint8_t current_item = 1, scroll = 1;
    int font_h;

    Window menu_window, function_window;

    uint64_t last_render = 0;

    function_event_t func_event;
    Function_container* functions;
    Menu_func* current_func = nullptr;

    bool filter_scrolling = LIMIT_SCROLL_TO_1;
    bool filter_clicks = FILTER_CLICKS;
    uint64_t last_click_time = 0;

    void set_function_containter();

    void render_menu(Menu* menu);
    void render_scroll_bar(uint8_t menu_item_n, bool is_home_menu = false);
    void render_function();

    void print_tree(Menu* menu, uint8_t deep);
    
public:
    Menu* current_menu = nullptr;
    Menu* default_menu;

    bool event_done = true;                 //true to initially render screen
    bool function_is_running = false;

    UI() = default;
    UI(Menu* default_menu_, Display_SH1106 *display_, int font_h_, int row_h_);
    void init(Menu* default_menu_, Display_SH1106 *display_, int font_h_, int row_h_);

    void render();

    void move(int steps);
    void select();

    void tree();
};

//--------------------------------------------------------------------------------



//------------------------------------MENU----------------------------------------

class Menu_func{
    int moved = 0, selected = 0;
public:
    String name;
    uint8_t index;

    Menu_item* root;

    Menu_func() = default;
    Menu_func(String name_, uint8_t index_);
};

class Menu_item{
public:
    String name = "";
    
    Menu* link_menu = nullptr;
    Menu_func* link_func = nullptr;

    uint8_t type;

    Menu_item() = default;
    Menu_item(String name_, Menu* link, bool back = false);
    Menu_item(String name_, Menu_func* link);
};

class Menu{
public:
    Menu_item items[MAX_MENU_ITEMS];
    uint8_t size = 0;

    Menu() = default;
    Menu(String items_[], uint8_t size_);
};

Menu* load_menu(String menu_list[], uint8_t menu_sizes[], uint8_t menu_linking[], uint8_t menu_types[], uint8_t menu_n);

//--------------------------------------------------------------------------------

#endif