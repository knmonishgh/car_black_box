/*
 * File:   main.c
 * Author: monis
 *
 * Created on 6 February, 2024, 6:17 PM
 */


#include <xc.h>
#include"dashboard.h"
#include"matrix_keypad.h"
#include"password.h"
#include"CLCD.h"
#include"adc.h"
#include"main.h"
#include"menu.h"
#include"ds1307.h"
#include"i2c.h"
#include<stdio.h>

void init_config() {
    init_clcd();
    init_matrix_keypad();
    init_adc();
    TRISB0 = 0;
    RB0 = 0;
    init_i2c();
    init_ds1307();

}

void main(void) {
    init_config();
    gear_select = 0;
    unsigned short temp_speed;
    while (1) {
        if (board_mode == DASH) {
            key = read_switches(STATE_CHANGE);
            temp_speed = read_adc(CHANNEL4) / 10.33;
            sprintf(speed, "%d", temp_speed);
            if (key == MK_SW1 && gear_select < 6) {
                gear_select++;
            } else if (key == MK_SW2 && gear_select > 1 && gear_select != 7) {
                gear_select--;
            } else if (key == MK_SW3) {
                gear_select = 7;
            } else if ((key == MK_SW1 || key == MK_SW2) && gear_select == 7) {
                gear_select = 1;
            } else if (key == MK_SW11) {
                board_mode = PASS;
            }
            display_dashboard(gear[gear_select], speed);
        }
        if (board_mode == PASS) {
            if (clear_flag == 0) {
                clear_flag = 1;
                CLEAR_DISP_SCREEN;
                clcd_print("ENTER PASSWORD", LINE1(1));
            }
            password_screen();
        }
        if (board_mode == MENU) {
            menu_display();
        }
    }


    return;
}
