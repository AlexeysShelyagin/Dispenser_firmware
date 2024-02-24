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
        "Tare",
        "Calibrate",
        "Settings",
        "Custom feed",
        "Clean",
        "Update",

        "Ammounts",
        "Feed speed",
        "Mixer speed",
        "Encoder dir",
        "Restore",
        "About"
    };

    static uint8_t menu_sizes[] = {7, 6};

    static uint8_t menu_linking[] = {
        0, 1, 2, 1, 3, 11, 9,
        4, 5, 6, 7, 8, 10
    };

    static uint8_t menu_types[] = {
        FUNC, FUNC, FUNC, MENU, FUNC, FUNC, FUNC,
        FUNC, FUNC, FUNC, FUNC, FUNC, FUNC
    };

}

/*
home:                                   FUNC_??
    
root:                               MENU_0
    Start:                              FUNC_0
    Tare:                               FUNC_1
    Calibrate:                          FUNC_2
    Settings:                       MENU_1
        Ammounts:                       FUNC_4
        Feed speed:                     FUNC_5
        Mixer speed:                    FUNC_6
        Endcoder dir:                   FUNC_7
        Restore:                        FUNC_8
        About:                          FUNC_10
    Custom feed:                        FUNC_11
    Clean:                              FUNC_3
    Update:                             FUNC_9

    
*/

#endif