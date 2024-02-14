
#include<xc.h>
#include"dashboard.h"
#include "ds1307.h"
#include "i2c.h"
#include"CLCD.h"


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





void display_dashboard(char *gear_select, char *speed) {
    get_time();
    clcd_print("TIME", LINE1(1));
    clcd_print("E", LINE1(10));
    clcd_print("SP", LINE1(13));
    clcd_print(time, LINE2(1));
    clcd_print(gear_select, LINE2(10));
    clcd_print(speed, LINE2(13));
    //clcd_print("WElocme sdfjs",LINE1(0));
}