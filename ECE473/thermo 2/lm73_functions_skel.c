LCD_Init();
LCD_PutStr("Set clock alarm");// lm73_functions.c
// Roger Traylor 11.28.10

#include <util/twi.h>
#include "lm73_functions_skel.h"
#include <util/delay.h>

//TODO: remove volatile type modifier?  I think so.
//TODO: initialize with more resolution and disable the smb bus timeout
//TODO: write functions to change resolution, alarm etc.

//volatile: tells the tells the compiler that the value of the variable may change at any time

uint8_t lm73_wr_buf[2];
uint8_t lm73_rd_buf[2];

//********************************************************************************

//******************************************************************************
uint8_t lm73_temp_convert(char temp_digits[], uint16_t lm73_temp, uint8_t f_not_c){
//given a temperature reading from an LM73, the address of a buffer
//array, and a format (deg F or C) it formats the temperature into ascii in 
//the buffer pointed to by the argument.
//TODO:Returns what???(uint8_t)??? Probably a BUG?

//Yeah, this is for you to do! ;^)
    
    
    
}//lm73_temp_convert
//******************************************************************************
