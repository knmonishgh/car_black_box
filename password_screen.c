#include "password.h"
#include "clcd.h"
#include "matrix_keypad.h"
#include "main.h"
#include <xc.h>

void password_screen()
{
    unsigned static char pcount = 0;
    unsigned static char pcount_pass = 0;
    unsigned static char attempt = 3;
    unsigned char att_left[2];
    key = read_switches(STATE_CHANGE);
    if (key == MK_SW11)
    {
        clcd_print("*", LINE2(pcount + 1));
        if (pass[pcount] == 1)
        {
            pcount_pass++;
        }
        if (pcount_pass == 3)
        {
            pcount = 0;
            pcount_pass = 0;
            CLEAR_DISP_SCREEN;
            clcd_print("Welcome", LINE1(2));
            clcd_print(" Main Menu ", LINE2(2));
            __delay_ms(2500);
            CLEAR_DISP_SCREEN;
            board_mode = MENU;
            return;
        }
        pcount++;
    }
    if (key == MK_SW12)
    {
        clcd_print("*", LINE2(pcount + 1));
        if (pass[pcount] == 0)
        {
            pcount_pass++;
        }
        if (pcount_pass == 3)
        {
            pcount = 0;
            pcount_pass = 0;
            CLEAR_DISP_SCREEN;
            clcd_print("welcome", LINE1(1));
            clcd_print(" Main Menu ", LINE2(2));
            __delay_ms(2500);
            CLEAR_DISP_SCREEN;
            board_mode = MENU;
            return;
        }
        pcount++;
    }
    else if (pcount == 4)
    {
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
    if (attempt == 0)
    {
        CLEAR_DISP_SCREEN;
        

    }
}