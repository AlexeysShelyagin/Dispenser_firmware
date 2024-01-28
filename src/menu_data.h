#ifndef MENU_DATA_H
#define MENU_DATA_H

#include <Arduino.h>

#define MENU 0
#define BACK 1
#define FUNC 2
#define PARAM 3

namespace menu{
    static uint8_t menu_n = 2;

    static String menu_list[] = {
        "Start",
        "Stat",
        "Calibrate",
        "Settings",

        "Mass",
        "Speed"
    };

    static uint8_t menu_sizes[] = {4, 2};

    static uint8_t menu_linking[] = {
        0, 1, 2, 1,
        3, 4
    };

    static uint8_t menu_types[] = {
        FUNC, FUNC, FUNC, MENU,
        FUNC, FUNC
    };

}

/*
home:                                   FUNC_??
    
root:                               MENU_0
    Start:                              FUNC_0
    Stat:                               FUNC_1
    Calibrate:                          FUNC_2
    Settings:                           FUNC_3
    
*/

#endif