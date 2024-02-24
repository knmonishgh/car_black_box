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
#include "uart.h"
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
    init_uart();
    
    clcd_print("ON",LINE1(10));
    //  write_external_eeprom(0x82, 1);
    //  write_external_eeprom(0x83, 1);
    //  write_external_eeprom(0x84, 1);
    //  write_external_eeprom(0x85, 1);
    pass[0] = read_external_eeprom(0x82);
    pass[1] = read_external_eeprom(0x83);
    pass[2] = read_external_eeprom(0x84);
    pass[3] = read_external_eeprom(0x85);
}

unsigned char clock_reg[3];

static void get_time(void)
{
    clock_reg[0] = read_ds1307(HOUR_ADDR);
    clock_reg[1] = read_ds1307(MIN_ADDR);
    clock_reg[2] = read_ds1307(SEC_ADDR);

    if (clock_reg[0] & 0x40)
    {
        time[0] = '0' + ((clock_reg[0] >> 4) & 0x01);
        time[1] = '0' + (clock_reg[0] & 0x0F);
    }
    else
    {
        time[0] = '0' + ((clock_reg[0] >> 4) & 0x03);
        time[1] = '0' + (clock_reg[0] & 0x0F);
    }
    time[2] = ':';
    time[3] = '0' + ((clock_reg[1] >> 4) & 0x0F);
    time[4] = '0' + (clock_reg[1] & 0x0F);
    time[5] = ':';
    time[6] = '0' + ((clock_reg[2] >> 4) & 0x0F);
    time[7] = '0' + (clock_reg[2] & 0x0F);
    time[8] = '\0';
}

void main(void)
{
    init_config();
    
    while (1)
    {
        get_time();
        if (board_mode == DASH)
        {
            disp_clear_flag = 0;
            display_dashboard();
        }
        else if (board_mode == CH_TIME)
        {
            set_rtc();
        }
        else if (board_mode == PASS)
        {
            if (disp_clear_flag == 0)
            {
                disp_clear_flag = 1;
                CLEAR_DISP_SCREEN;
                clcd_print("ENTER PASSWORD", LINE1(1));
            }
            password_screen();
        }
        else if (board_mode == MENU)
        {
            menu_display();
        }
        else if (board_mode == LOG)
        {
            view_log();
        }
        else if (board_mode == DOWN_LOG)
        {
            down_log();
        }
        else if(board_mode == CH_PASS )
        {
           change_password();
        }
        else if (board_mode == CLEAR_LOG)
        {
            clear_log();
        }
    }

    return;
}
