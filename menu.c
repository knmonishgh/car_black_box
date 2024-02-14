#include"matrix_keypad.h"
#include"clcd.h"
#include<xc.h>
#include"menu.h"

void menu_display() {
    unsigned static char mkey, menu_count = 0;
    mkey = read_switches(STATE_CHANGE);
    if (mkey == MK_SW11 && menu_count > 0) {
        CLEAR_DISP_SCREEN;
        menu_count--;
    } else if (mkey == MK_SW12 && menu_count < 4) {
        CLEAR_DISP_SCREEN;
        menu_count++;
    }
    if (menu_count % 2 == 0) {
        clcd_print("->", LINE1(0));
        clcd_print(menu_card[menu_count], LINE1(2));
        if (menu_count != 4) {
            clcd_print(menu_card[menu_count + 1], LINE2(2));
        }
    } else if (menu_count % 2 == 1) {
        clcd_print(menu_card[menu_count - 1], LINE1(2));
        clcd_print(menu_card[menu_count], LINE2(2));
        clcd_print("->", LINE2(0));
    }


}