#ifndef __LV_UI_H
#define __LV_UI_H

#include <stdint.h>

typedef struct {
    uint8_t person;
    uint8_t vol_input;
    uint8_t vol_output;
    uint8_t index;
}lv_ui_info_t;


void main_menu(void);


#endif

