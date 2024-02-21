#include "password.h"
#include "clcd.h"
#include "matrix_keypad.h"
#include "main.h"
#include <xc.h>
#include <stdio.h>

void password_screen() {

    unsigned static char pcount = 0;
    unsigned static char pcount_pass = 0;
    unsigned static char attempt = 3;
    unsigned char att_left[2];
    unsigned static int wait = 0;  
    unsigned static long int t_wait = 0;
    unsigned static char wait_count = 120;
    unsigned char temp_wait_count[3];
    if (attempt == 0) {
            clcd_print("USER BLOCKED   ", LINE1(0));
            clcd_print("Wait for ", LINE2(0));
            sprintf(temp_wait_count, "%u", wait_count);
            clcd_print(temp_wait_count,LINE2(11));
        if (wait++ == 500) {
            wait = 0;
            wait_count--;
            CLEAR_DISP_SCREEN;
            if(wait_count==0)
            {
                attempt = 3;
                wait_count = 120;
                board_mode = DASH;
                CLEAR_DISP_SCREEN;
            }
        } 
    } else if (attempt > 0) {
        for(int k = 100;k--;);
        key = read_switches(STATE_CHANGE);
        if (key == MK_SW11) {
            t_wait = 0;
            clcd_print("*", LINE2(pcount + 1));
            if (pass[pcount] == 1) {
                pcount_pass++;
            }
            if (pcount_pass == 4) {
                pcount = 0;
                pcount_pass = 0;
                CLEAR_DISP_SCREEN;
                clcd_print("Welcome", LINE1(2));
                clcd_print("Main Menu ", LINE2(2));
                __delay_ms(2500);
                CLEAR_DISP_SCREEN;
                board_mode = MENU;
                return; 
            }
            pcount++;
        }
        if (key == MK_SW12) {
            t_wait = 0;
            clcd_print("*", LINE2(pcount + 1));
            if (pass[pcount] == 0) {
                pcount_pass++;
            }
            if (pcount_pass == 4) {
                pcount = 0;
                pcount_pass = 0;
                CLEAR_DISP_SCREEN;
                clcd_print("Welcome", LINE1(1));
                clcd_print("Main Menu ", LINE2(2));
                __delay_ms(2500);
                CLEAR_DISP_SCREEN;
                board_mode = MENU;
                return;
            }
            pcount++;
        } else if (pcount == 4) {
            pcount = 0;
            attempt--;
            pcount_pass = 0;
            CLEAR_DISP_SCREEN;
            clcd_print("Wrong pass", LINE1(1));
            __delay_ms(2500);
            CLEAR_DISP_SCREEN;
            clcd_print("Attempt left", LINE1(0));
            sprintf(att_left, "%d", attempt);
            clcd_print(att_left, LINE1(14));
            __delay_ms(2500);
            CLEAR_DISP_SCREEN;
            clcd_print("ENTER PASSWORD", LINE1(1));
        } 
         if(t_wait++ == 100000)
         {
             CLEAR_DISP_SCREEN;
             clear_flag = 0;
             t_wait = 0;
             pcount = 0;
             pcount_pass = 0;
             board_mode = DASH;
         }
    }
}