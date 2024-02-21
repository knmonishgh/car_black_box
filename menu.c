#include "matrix_keypad.h"
#include "clcd.h"
#include "ext_eeprom.h"
#include <xc.h>
#include "menu.h"
#include "main.h"

void menu_display()
{
    unsigned static char mkey, menu_count = 0, temp_key = ALL_RELEASED;
    static char ch_flag = 0;
    static char reset = 0;
    static short longpress = 0;

    mkey = read_switches(LEVEL_CHANGE);
    if (mkey == MK_SW11 || mkey == MK_SW12 && reset != 2)
    {
        reset = 1;
        longpress++;
        temp_key = mkey;
        if (longpress > 300)
        {
            ch_flag = 2;
            reset = 2;
        }
    }
    else if (mkey == ALL_RELEASED && reset == 1)
    {
        longpress = 0;
        reset = 0;
        ch_flag = 1;
    }
    else if (mkey == ALL_RELEASED)
    {
        reset = 0;
        longpress = 0;
    }
    if (temp_key == MK_SW11 && ch_flag == 1 && menu_count > 0)
    {
        ch_flag = 0;
        temp_key = ALL_RELEASED;
        CLEAR_DISP_SCREEN;
        menu_count--;
    }
    if (temp_key == MK_SW12 && ch_flag == 1 && menu_count < 4)
    {
        ch_flag = 0;
        temp_key = ALL_RELEASED;
        CLEAR_DISP_SCREEN;
        menu_count++;
    }
    if (temp_key == MK_SW11 && ch_flag == 2)
    {
        if (menu_count == 0)
        {
            CLEAR_DISP_SCREEN;
            board_mode = LOG;
            return;
        }
        else
        {
            CLEAR_DISP_SCREEN;
            board_mode = DASH;
            return;
        }
    }

    if (menu_count % 2 == 0)
    {
        clcd_print("->", LINE1(0));
        clcd_print(menu_card[menu_count], LINE1(2));
        if (menu_count != 4)
        {
            clcd_print(menu_card[menu_count + 1], LINE2(2));
        }
    }
    else if (menu_count % 2 == 1)
    {
        clcd_print(menu_card[menu_count - 1], LINE1(2));
        clcd_print(menu_card[menu_count], LINE2(2));
        clcd_print("->", LINE2(0));
    }
}

void view_log()
{
    static char read_buff[16];
    static char view_index = 1;
    static char view_flag = 0;
    unsigned static char eprom_read_addr = 0x00;
    clcd_print("# TIME     E  SP", LINE1(0));
    unsigned static char view_key;
    view_key = read_switches(STATE_CHANGE);
    if (view_key == MK_SW11 && view_index < 11)
    {
        view_index++;
        view_flag = 0;
    }
    if (view_key == MK_SW12 && view_index > 1)
    {
        view_index--;
        view_flag = 0;
        eprom_read_addr = eprom_read_addr - 24;
    }
    if (view_flag != 1)
    {
        read_buff[0] = view_index + 48;
        read_buff[1] = ' ';
        for (char i = 2; i < 16; i++)
        {
            if (i == 10 || i == 13)
            {
                read_buff[i] = ' ';
            }
            else
            {
                read_buff[i] = read_external_eeprom(eprom_read_addr++);
            }
        }
        read_buff[16] = '\0';
        clcd_print(read_buff, LINE2(0));
        view_flag = 1;
    }
}