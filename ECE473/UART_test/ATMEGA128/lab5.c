//
//  lab5.c
//  Created by Brian Sia on 11/8/16 (updated on 30th of November)
//
//  Description: Provides functionalities of an alarm clock, including time settings, alarm settings, 12hr/24hr mode settings, alarm volume control, snooze function, and a simple user interface through an LCD screen.

//  Lab 5 update: Added temperature sensing features. Temperature can be sensed locally by the ATmega128 board,
//  or remotely by the LM73 sensor.

//  NOTE: Music is set in the kellen_music file. As such, TIMER1 is initialized there. Do not initialize timer 1
//  in this file!
// ***************** List of the physical wirings on board *****************
// 7-seg board  mega128
// -----------  ------
// a2           PA0
// b2           PA1
// c2           PA2
// d2           PA3
// e2           PA4
// f2           PA5
// g2           PA6
// dp2          PA7
// sel0         PB4
// sel1         PB5
// sel2         PB6
// en           vdd
// en_n         gnd
// vdd          vdd
// gnd          gnd

//buttonboard   mega128
//-----------   ------
// gnd          gnd
// vcc          vdd
// com_lvl      gnd

//buttonboard  7-seg
//-----------  -----
// a            J5
// b            J4
// c            J3
// d            J2
// e            J1
// f            J7
// g            J6
// dp           J8
//com_en        dec7

//encoder       mega128
//-------       ------
// vdd          vdd
// gnd          gnd
// SH/LD        PE7 (INT7)
// SCK          PE6 (INT6)
// CLKINH       PB1 (SCK)
// SOUT         PB3 (MISO)

//bar-graph     mega128
//---------     -------
//vdd           vdd
//gnd           gnd
//oe_n          PB7 (oc2/oc1c)
//sdin          PB2 (MOSI)
//srclk         PB1 (SCK)
//regclk        PB0 (|S|S)s

//opamp         mega128
//-----         -------
//Input         PD7

//audio amp     mega128
//---------     -------
//Volume        PE3

//radio         mega128
//---------     -------

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "LCDDriver.c"    //contains routine that writes to the LCD
//#include "twi_master.c"   //two wire interface file
#include "usart.c"        //usart file

//variable declarations, all are relevant to operation
uint8_t digit_data[5];   //holds data about which digit to display
uint8_t segment_data[11] = {};   //hold data about decimal to 7-segment LED
uint8_t displayBits = 0x00; //holds bits to be displayed on bar graph. After resetting, it shows 1
int8_t count = 1;    // value to be incremented/decremented by rotating the encoder
int8_t count_sign;   // +1/-1, used to control the volume
uint8_t inc_multiple;   // For increment/ decrement 2 or 4, is changed by pushing button.
uint8_t leftState; //state of left encoder, it initial state is 3
uint8_t prevLeftState;  //It duplicates the value of encoder_data received by ISP
uint8_t rightState; //state of encoder, it initial state is 3(0xFF & 0x03).
uint8_t prevRightState;  //It is for duplicating the value of encoder_data received by ISP
int16_t sum = 0;   //the number to be displayed
uint8_t timer = 1; //executes every 500ms for tcnt0
uint8_t colon_state;     //determines if colon_state blinks or blanks out
uint8_t hours = 0; //holds the hours data
uint8_t minutes = 0; //holds the minutes data
uint8_t seconds = 0; //holds the seconds data
uint8_t adc_count = 0;
uint8_t clock_mode = 0; //a flag that stores different modes based on what button user push
uint8_t s2_timer;     //a timer that increments when the user sets the clock time
uint8_t s2_switch = 0; //clock time setting button
uint8_t s4_switch = 0; //12/24 hr mode toggling button
uint8_t changeTimeDisplay; //flag to determine if time change setting is requested
uint8_t changeAlarmDisplay; //flag to determine if alarm change setting is requested
uint8_t alarm_s = 0; //alarm seconds
uint8_t alarm_m = 0; //alarm minutes
uint8_t alarm_h = 0; //alarm hours
uint8_t time_notation; //determines if clock is a 12hr clock or 24hr clock
uint8_t time_notation_flag = 0; //determines if clock is in 12hr or 24hr mode
uint8_t am_pm_flag = 0; //
uint8_t am_or_pm = 2; // 0 - am, 1 - pm
uint8_t alarm = 0;  //alarm flag
uint8_t stop = 0;   //flag that indicates that alarm should stop. 1 for stop
uint8_t snooze_flag = 0; //1 for snooze, 0 for regular
uint16_t volume;     //variable that determines volume level
uint8_t alarm_state; //state that determines if alarm is mute or active (status will be sent to LCD)
uint8_t ms; //millisecond counter that determines note to be played from kellen_music

uint16_t lm73_temp; //a variable to assemble the temperature from the lm73

//for the temperature sensor
char mega128_temp_str[16];    //holds a string to refresh the LCD (for local temp)
char mega48_temp_str[16]; //for remote temp
volatile uint8_t rcv_rdy;

char  rx_char; //to receive data from the mega48
uint8_t i;                 //general purpose index

void readRemoteTemp(){
    LCD_PutStr("Remote: ");
    if(rcv_rdy == 1){
        LCD_PutStr(mega48_temp_str);
        rcv_rdy = 0;
    }
    LCD_PutStr("C");
}

ISR(USART0_RX_vect){
    
    rx_char = USART_receive();
        mega48_temp_str[i++] = rx_char;
    if(rx_char == '\0'){
        rcv_rdy = 1;
        i = 0;
    }
    
}

int main(){
    
    USART0_init(9600);   //initialize uart for temperature readings from mega48
    LCD_Init();   //initialize LCD for status displays
    sei();

    while(1){
    readRemoteTemp();
    }
}