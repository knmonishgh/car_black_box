
#include <xc.h>
#include "dashboard.h"
#include "ds1307.h"
#include "i2c.h"
#include "ext_eeprom.h"
#include "CLCD.h"
#include "matrix_keypad.h"
#include "main.h"
#include "adc.h"
#include<stdio.h>

unsigned char clock_reg[3];
unsigned char time[9];

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

void display_dashboard()
{
    get_time();
    unsigned short temp_speed;
    key = read_switches(STATE_CHANGE);
    temp_speed = read_adc(CHANNEL4) / 10.33;
    unsigned static char eprom_addr = 0x00;
    unsigned static char prev_gear = 0;
    char static temp_buff[12];
    sprintf(speed, "%d", temp_speed);
    if (key == MK_SW1 && gear_select < 6)
    {
        gear_select++;
    }
    else if (key == MK_SW2 && gear_select > 1 && gear_select != 7)
    {
        gear_select--;
    }
    else if (key == MK_SW3)
    {
        gear_select = 7;
    }
    else if ((key == MK_SW1 || key == MK_SW2) && gear_select == 7)
    {
        gear_select = 1;
    }
    else if (key == MK_SW11)
    {
        board_mode = PASS;
    }
    if(prev_gear != gear_select)
    {
        prev_gear = gear_select;
        sprintf(temp_buff, "%s%s%d" ,time,gear[gear_select],temp_speed);
        for (char i = 0; i < 12; i++)
        {
            write_external_eeprom(eprom_addr++,temp_buff[i]);
            if(eprom_addr==120)
                eprom_addr = 0;
        }
        
    }
    clcd_print("TIME", LINE1(1));
    clcd_print("E", LINE1(10));
    clcd_print("SP", LINE1(13));
    clcd_print(time, LINE2(1));
    clcd_print(gear[gear_select], LINE2(10));  
    clcd_print(speed, LINE2(13));
    // clcd_print("WElocme sdfjs",LINE1(0));
}
