#include "matrix_keypad.h"
#include "clcd.h"
#include "ext_eeprom.h"
#include "password.h"
#include "dashboard.h"
#include "ds1307.h"
#include <xc.h>
#include "menu.h"
#include "main.h"
#include "uart.h"
#include "stdio.h"

unsigned static char mkey, menu_count = 0, temp_key = ALL_RELEASED;
static char ch_flag = 0;
static char reset = 0;
static int longpress = 0;

void readkey()
{
    mkey = read_switches(LEVEL_CHANGE);
    if (mkey == MK_SW11 || mkey == MK_SW12 && reset != 2)
    {
        reset = 1;
        longpress++;
        temp_key = mkey;
        if (longpress > 500)
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
}

void menu_display()
{
    readkey();
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
        ch_flag = 0;
        if (menu_count == 0)
        {
            CLEAR_DISP_SCREEN;
            clcd_print("   View log  ", LINE1(0));
            __delay_ms(2100);
            board_mode = LOG;
            return;
        }
        else if (menu_count == 1)
        {
            CLEAR_DISP_SCREEN;
            board_mode = DOWN_LOG;
            return;
        }
        else if (menu_count == 2)
        {
            CLEAR_DISP_SCREEN;
            board_mode = CLEAR_LOG;
            return;
        }
        else if (menu_count == 4)
        {
            CLEAR_DISP_SCREEN;
            board_mode = CH_PASS;
            return;
        }
        else
        {
            CLEAR_DISP_SCREEN;
            board_mode = CH_TIME;
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
    if (eprom_clear_flag == 0)
    {
        clcd_print("EMPTY LOGS", LINE1(2));
        __delay_ms(2100);
        board_mode = MENU;
        return;
    }
    readkey();
    unsigned static char read_buff[16];
    unsigned static char view_index = 1, view_flag = 0;
    unsigned static char eprom_read_addr = 0x00;
    clcd_print("# TIME     E  SP", LINE1(0));

    if (temp_key == MK_SW11 && view_index < 10 && ch_flag == 1)
    {
        ch_flag = 0;
        temp_key = ALL_RELEASED;
        view_index++;
        view_flag = 0;
    }
    if (temp_key == MK_SW12 && view_index > 1 && ch_flag == 1)
    {
        ch_flag = 0;
        temp_key = ALL_RELEASED;
        view_index--;
        eprom_read_addr = eprom_read_addr - 24;
        view_flag = 0;
    }
    if (temp_key == MK_SW12 && ch_flag == 2)
    {
        CLEAR_DISP_SCREEN;
        clcd_print("exiting...", LINE1(2));
        __delay_ms(2100);
        board_mode = MENU;
        return;
    }

    if (view_flag == 0)
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

void down_log()
{
    if (eprom_clear_flag == 0)
    {
        clcd_print("EMPTY LOGS", LINE1(2));
        __delay_ms(2100);
        board_mode = MENU;
        return;
    }
    readkey();
    unsigned static char down_buff[16];
    unsigned static char down_index = 0;
    unsigned static char uart_eprom_addr = 0x00;
    clcd_print("Downloading..", LINE1(0));
    clcd_print("Log", LINE2(1));
    if (down_index < 10)
    {
        down_buff[0] = down_index + 48;
        down_buff[1] = ' ';
        for (char i = 2; i < 16; i++)
        {
            if (i == 10 || i == 13)
            {
                down_buff[i] = ' ';
            }
            else
            {
                down_buff[i] = read_external_eeprom(uart_eprom_addr++);
            }

            down_buff[16] = '\0';
        }
        put_st
        put_string(down_buff);
        put_char('\n');
        put_char('\r');
        down_index++;
    }
    if (temp_key == MK_SW12 && ch_flag == 2)
    {
        CLEAR_DISP_SCREEN;
        clcd_print("exiting...", LINE1(2));
        __delay_ms(2100);
        board_mode = MENU;
        return;
    }
}

void change_password()
{
    unsigned static char change_flag = 0;
    unsigned static new_pass_buff[4];
    unsigned static char chn_pass_cnt = 0;
    readkey();
    if (change_flag == 0)
    {
        clcd_print("  NEW PASSWORD  ", LINE1(0));
        if (temp_key == MK_SW11 && ch_flag == 1)
        {
            ch_flag = 0;
            clcd_print("*", LINE2(chn_pass_cnt + 2));
            new_pass_buff[chn_pass_cnt++] = 1;
        }
        else if (temp_key == MK_SW12 && ch_flag == 1)
        {
            ch_flag = 0;
            clcd_print("*", LINE2(chn_pass_cnt + 2));
            new_pass_buff[chn_pass_cnt++] = 0;
        }
        if (chn_pass_cnt == 4)
        {
            CLEAR_DISP_SCREEN;
            change_flag = 1;
            chn_pass_cnt = 0;
        }
    }
    else if (change_flag == 1)
    {
        clcd_print("  CNF PASSWORD  ", LINE1(0));
        if (temp_key == MK_SW11 && ch_flag == 1)
        {
            ch_flag = 0;
            if (new_pass_buff[chn_pass_cnt++] != 1)
            {
                CLEAR_DISP_SCREEN;
                clcd_print("PASS NO MATCH", LINE1(1));
                __delay_ms(2100);
                board_mode = MENU;
                return;
            }
            else
            {
                clcd_print("*", LINE2(chn_pass_cnt + 2));
            }
        }
        else if (temp_key == MK_SW12 && ch_flag == 1)
        {
            ch_flag = 0;
            if (new_pass_buff[chn_pass_cnt++] != 0)
            {
                CLEAR_DISP_SCREEN;
                clcd_print("PASS NO MATCH", LINE1(1));
                __delay_ms(2100);
                board_mode = MENU;
                return;
            }
            else
            {
                clcd_print("*", LINE2(chn_pass_cnt + 2));
            }
        }
        if (chn_pass_cnt == 4)
        {
            CLEAR_DISP_SCREEN;
            change_flag = 2;
            chn_pass_cnt = 0;
        }
    }
    else if (change_flag == 2)
    {
        for (char chn = 0; chn < 4; chn++)
        {
            write_external_eeprom(0x82 + chn, new_pass_buff[chn]);
        }
        for (char chn = 0; chn < 4; chn++)
        {
            pass[chn] = read_external_eeprom(0x82 + chn);
        }

        CLEAR_DISP_SCREEN;
        clcd_print("PASS CHN SUCSS", LINE1(1));
        __delay_ms(2100);
        CLEAR_DISP_SCREEN;
        board_mode = DASH;
        change_flag = 0;
    }
}

void clear_log()
{
    eprom_clear_flag = 0;
    for (char eprom_clear_addr = 0; eprom_clear_addr <= 120; eprom_clear_addr++)
    {
        write_external_eeprom(eprom_clear_addr, 0xFF);
    }
    CLEAR_DISP_SCREEN;
    clcd_print("EEPROM CLEARED", LINE1(0));
    __delay_ms(2100);
    CLEAR_DISP_SCREEN;
    board_mode = MENU;
    return;
}

void set_rtc()
{
    unsigned static char set_time_buff[8];
    unsigned char static set_time_flag = 0;
    unsigned static char hours, minutes, seconds;
    unsigned static char set_wait_delay = 0;
    unsigned static char set_dummy = 0;
    clcd_print("SET TIME", LINE1(3));
    if(set_time_flag != 0 )
    {
        readkey();
    }
    if (set_time_flag == 0)
    {
        set_dummy = read_ds1307(SEC_A   DDR);
        write_ds1307(SEC_ADDR, set_dummy | 0x80);
        hours = (((time[0] - 48) * 10) + time[1] - 48);
        minutes = (((time[3] - 48) * 10) + time[4] - 48);
        seconds = (((time[6] - 48) * 10) + time[7] - 48);
        set_time_flag = 1;
        sprintf(set_time_buff, "%d:%d:%d", hours, minutes, seconds);
        __delay_ms(2100);
    }
    if (temp_key == MK_SW12 && ch_flag == 1 && set_time_flag < 4)
    {
        ch_flag = 0;
        set_time_flag++;
    }
    if (set_time_flag == 1)
    {
        if (temp_key == MK_SW11 && ch_flag == 1)
        {
             ch_flag = 0;
            seconds++;
            if (seconds > 60)
            {
                seconds = 0;
            }
           
        }
        if (set_wait_delay++ < 200)
        {

            sprintf(set_time_buff, " %d:%d:  ", hours, minutes);
        }
        else if (set_wait_delay < 400)
        {
            sprintf(set_time_buff, " %d:%d:%d", hours, minutes, seconds);
        }
        else
        {
            set_wait_delay = 0;
        }
        if (temp_key == MK_SW11 && ch_flag == 2)
        {
            ch_flag == 0;
            set_time_flag = 4;
        }
    }
    if (set_time_flag == 2)
    {
        if (temp_key == MK_SW11 && ch_flag == 1)
        {
            ch_flag = 0;
            minutes++;
            if (minutes > 60)
            {
                minutes = 0;
            }
            
        }
        if (set_wait_delay++ < 200)
        {

            sprintf(set_time_buff, " %d:  :%d", hours, seconds);
        }
        else if (set_wait_delay < 400)
        {
            sprintf(set_time_buff, " %d:%.2d:%d", hours, minutes, seconds);
        }
        else
        {
            set_wait_delay = 0;
        }
        if (temp_key == MK_SW11 && ch_flag == 2)
        {
            ch_flag == 0;
            set_time_flag = 4;
        }
    }
    if (set_time_flag == 3)
    {
        if (temp_key == MK_SW11 && ch_flag == 1)
        {
            ch_flag = 0;
            hours++;
            if (hours > 23)
            {
                hours = 0;
            } 
        }
        if (set_wait_delay++ < 200)
        {
            sprintf(set_time_buff, "   :%d:%d", minutes, seconds);
        }
        else if (set_wait_delay < 400)
        {
            sprintf(set_time_buff, " %.2d:%d:%d", hours, minutes, seconds);
        }
        else
        {
            set_wait_delay = 0;
        }
        if (temp_key == MK_SW11 && ch_flag == 2)
        {
            ch_flag == 0;
            set_time_flag = 4;
        }
    }
    if (set_time_flag == 4)
    {

        write_ds1307(SEC_ADDR, (seconds % 10) | ((seconds / 10) << 4));
        write_ds1307(MIN_ADDR, (minutes % 10) | ((minutes / 10) << 4));
        write_ds1307(HOUR_ADDR, (hours % 10) | ((hours / 10) << 4));
        set_dummy = read_ds1307(SEC_ADDR);
        set_time_flag = 0;
        write_ds1307(SEC_ADDR, set_dummy & 0x7F);
        CLEAR_DISP_SCREEN;
        clcd_print("TIME SET SUCC", LINE1(0));
        __delay_ms(2100);
        CLEAR_DISP_SCREEN;
        board_mode = MENU;
        return;
    }
    clcd_print(set_time_buff, LINE2(2));
}
