 /* 
 * File:   main.h
 * Author: monis
 *
 * Created on 6 February, 2024, 6:12 PM
 */

#ifndef MAIN_H
#define	MAIN_H

#ifdef	__cplusplus 
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* MAIN_H */


char gear[8][3] = {"ON","GN","GR","G1","G2","G3","G4","C "};

typedef enum
{
    DASH,
    PASS,
    MENU,
    LOG,
    DOWN_LOG,
    CH_PASS,
    CLEAR_LOG,
    CH_TIME,      
}Mode;


unsigned char key;
unsigned char speed[2];
unsigned char disp_clear_flag=0;
unsigned char eprom_clear_flag=0;

unsigned char time[9];

Mode board_mode = DASH;































































































































