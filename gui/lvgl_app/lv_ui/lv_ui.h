#ifndef __LV_UI_H
#define __LV_UI_H

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    uint8_t person;
    uint8_t vol_input;
    uint8_t vol_output;
    uint8_t index;
    uint8_t exit_status;
    bool selected;
}lv_ui_info_t;

typedef enum
{
    LV_UI_MODE_EXIT,
    LV_UI_MODE_ENTER,
    LV_UI_MODE_SELECT,
} lv_ui_mode_e; 

void main_menu(void);
void scan_menu(void);
void menu_status_handler(void);
void read_data_from_mem(void);

#endif

