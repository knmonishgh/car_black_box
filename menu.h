/* 
 * File:   menu.h
 * Author: monis
 *
 * Created on 13 February, 2024, 2:09 PM
 */

#ifndef MENU_H
#define	MENU_H

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* MENU_H */

void menu_display();

char menu_card[5][16] = {"View log  ","Download log  ","Clear log  ","Set time","Change Password"};


void view_log();
void down_log();
void clear_log();
void change_password();
void set_rtc();
