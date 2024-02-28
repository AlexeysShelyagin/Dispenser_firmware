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
        "Calibrate",
        "Custom feed",
        "Settings",
        "Clean",
        "Update",

        "Ammounts",
        "Feed speed",
        "Ref. mass",
        "Restore",
        "About"
    };

    static uint8_t menu_sizes[] = {6, 5};

    static uint8_t menu_linking[] = {
        0, 2, 6, 1, 3, 9,
        4, 5, 1, 8, 7
    };

    static uint8_t menu_types[] = {
        FUNC, FUNC, FUNC, MENU, FUNC, FUNC,
        FUNC, FUNC, FUNC, FUNC, FUNC
    };

}

/*
home:                                   FUNC_??
    
root:                               MENU_0
    Start:                              FUNC_0
    Calibrate:                          FUNC_2
    Custom feed:                        FUNC_6
    Settings:                       MENU_1
        Ammounts:                       FUNC_4
        Feed speed:                     FUNC_5
        Reference mass                  FUNC_1
        Restore:                        FUNC_8
        About:                          FUNC_7
    Clean:                              FUNC_3
    Update:                             FUNC_9

    
*/

#endif