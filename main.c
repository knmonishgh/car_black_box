/*
 * File:   main.c
 * Author: monis
 *
 * Created on 6 February, 2024, 6:17 PM
 */

#include <xc.h>
#include "dashboard.h"
#include "matrix_keypad.h"
#include "password.h"
#include "CLCD.h"
#include "adc.h"
#include "main.h"
#include "menu.h"
#include "ds1307.h"
#include "ext_eeprom.h"
#include "i2c.h"
#include <stdio.h>

void init_config()
{
    init_clcd();
    init_matrix_keypad();
    init_adc();
    TRISB0 = 0;
    RB0 = 0;
    init_i2c();
    init_ds1307();
}

void main(void)
{
    init_config();

    while (1)
    {
        if (board_mode == DASH)
        {
            clear_flag = 0;
            display_dashboard();
        }
        if (board_mode == PASS)
        {
            if (clear_flag == 0)
            {
                clear_flag = 1;
                CLEAR_DISP_SCREEN;
                clcd_print("ENTER PASSWORD", LINE1(1));
            }
            password_screen();
        }
        if (board_mode == MENU)
        {
            menu_display();
        }
        if (board_mode == LOG)
        {
            view_log();
        }
    }

    return;
}
