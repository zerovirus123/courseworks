//
//  lab4.c
//  Created by Brian Sia on 11/8/16.
//
//  Description: Provides functionalities of an alarm clock, including time settings, alarm settings, 12hr/24hr mode settings, alarm volume control, snooze function, and a simple user interface through an LCD screen.

// NOTE: Music is set in the kellen_music file. As such, TIMER1 is initialized there. Do not initialize timer 1
// in this file!
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

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "LCDDriver.c"    //contains routine that writes to the LCD
#include "kellen_music.c" //music file

//#NOTE: In Makefile, do not include kellen_music.o as an source file to be compiled,
//or the compiler will think that its contents are redefined

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

//routine declarations so compiler doesnt puke out warning messages
int16_t chk_buttons(uint8_t i);
void segment_value();
void segment_display();
void spi_init(void);
void tcnt0_init(void);
void tcnt2_init(void);
void tcnt3_init(void);
void adc_init(void);
void encoders(uint8_t spi_out);
void clk_count(uint8_t timer1);
void snooze(uint8_t s7_timer);
void run_alarm();
void alarm_mode();
void mode_switch(uint8_t mode_change);
void stop_alarm();

//*********************************************************************************
//                            chk_buttons
//  Button debouncing method, based on the debouncing function found in Ganssle's guide.
//  Shifts in 1 during a false pass (bouncing). Every time a button is pushed, 0 will
//  be left shifted into the state variable. Once twelve true passes (0s) have been detected
//  the method will return a 1. Otherwise it returns a 0.
//*********************************************************************************
int16_t chk_buttons(uint8_t button) {  //check which button is pushed
    static uint16_t state[8] = {0x0000}; //holds present state
    if (!(PINA & 0x01)){
        state[button] = (state[button] << 1) | (!(PINA&0x01)) | 0xE000;
        if (state[0] == 0xF000) return 1;
        return 0;
    }
    else {
        state[button] = (state[button] << 1) | (!bit_is_clear(PINA,button)) | 0xE000;
        if (state[button] == 0xF000)
            return 1;
        return 0;
    }
}

//**********************************************************************************
//                                   segment_value
//  Determines the number value and colon_state blink/blank out of the digit_data segment_data,
//  which would later be displayed by the segment_display method.
//**********************************************************************************
void segment_value() {
    int index;
    
    for (index = 0; index < 5; index++) {
        switch (digit_data[index]){
            case 0: segment_data[index] = 0xC0; break;  //0
            case 1: segment_data[index] = 0xF9; break;  //1
            case 2: segment_data[index] = 0xA4; break;  //2
            case 3: segment_data[index] = 0xB0; break;  //3
            case 4: segment_data[index] = 0x99; break;  //4
            case 5: segment_data[index] = 0x92; break;  //5
            case 6: segment_data[index] = 0x82; break;  //6
            case 7: segment_data[index] = 0xF8; break;  //7
            case 8: segment_data[index] = 0x80; break;  //8
            case 9: segment_data[index] = 0x90; break;  //9
            case 10: segment_data[index] = 0xFF; break; //blank out
            case 11: segment_data[index] = 0x04; break; //colon
            default: segment_data[index] = 0xFF;
        }//switch
    }//for
}

//**********************************************************************************
//                                   segment_display
// The 4 digit number cannot be display at the same time, so microcontroller should
// display each digit at a high frequency to look like they are showed simultaneously.
// This is called persistence of vision.

// Also, a PWM duty cycle counter will be incrementing. Every time a case statement is called,
// If duty cycle is above the threshold voltage at the ADC, seven segment lights up.
// Otherwise seven segment is blanked out. Doing it this quickly gives the illusion
// of dimming the seven segment.

// All 7-segment LEDS are active low
//**********************************************************************************
void segment_display() {
    int digits; //for looping through the digits digit_data
    
    for (digits = 0; digits < 5; digits++) {
            switch (digits) {
                case 0: //ones digit
                        PORTB = 0x00;
                        PORTA = segment_data[0];
                        _delay_ms(2);
                    break;
                    
                case 1: //tens digit
                        PORTB = 0x10;
                        PORTA = segment_data[1];
                        _delay_ms(2);
                    break;
                    
                case 2: //colon_state
                   
                        PORTB = 0x20;
                        PORTA = segment_data[2];
                        _delay_ms(2);
                    break;
                    
                case 3: //hundreds digit
                    
                        PORTB = 0x30;
                        PORTA = segment_data[3];
                        _delay_ms(2);
                    break;
                    
                case 4: //thousands digit
                        PORTB = 0x40;
                        PORTA = segment_data[4];
                        _delay_ms(2);
                    break;
                    
            }//switch
    }//for
}

/***********************************************************************/
//                            spi_init
// Initialiazes the SPI port on the mega128. The following
// parameters are set up: master mode, clock=clk/2, half phase cycle,
// low polarity, MSB first interrupts disabled, poll SPIF bit in SPSR to
// check transmit completion.
/***********************************************************************/
void spi_init(void){
    SPCR |= (1<<SPE) | (1<<MSTR);  //set up SPI mode
    SPSR |= (1<<SPI2X);   // double speed operation
}

/***********************************************************************/
//                              tcnt0_init
// Initializes timer/counter0. TCNT0 allows running
// in async mode with an external 32khz crystal. If running in normal mode,
// there is no prescaling. Overflow interrupt occurs at 0xFF.
/***********************************************************************/
void tcnt0_init(void){
    ASSR  |= (1<<AS0);  //ext osc TOSC
    TIMSK |= (1<<TOIE0);  //enable timer/counter0 overflow interrupt
    TCCR0 |= (1<<CS00);  //normal mode, no prescale
    
    //TCCR0  |=  (0<<WGM01) | (0<<WGM00) | (0<<COM00) | (0<<COM01) | (0<<CS02) | (0<<CS01) | (1<<CS00);
}

//***********************************************************************/
//                          tcnt2_init
// Initialize timer counter 2 to allow LED dimming through PWM
//***********************************************************************/
void tcnt2_init(void){
    DDRB  |= (1<<PB7);
    TCCR2 &= (0<<FOC2); //no forced compare. fast pwm mode. Set OC2 on compare match
    TCCR2 |= (1<<WGM21) | (1<<WGM20) | (1<<COM21) | (1<<COM20);
    TCCR2 |= (1<<CS20); //normal mode, no prescaler
    OCR2  = 0x00; //initialize OCR register for PWM
}

//***********************************************************************/
//                          tcnt3_init
// Initializes Timer/Counter3 to control the output compare
// pin OCnA behavior. In fast mode, OCnA is set on compare match, and is
// cleared at BOTTOM. Function is for automatically changing volume of
// alarm on the audio amplifier board.
//
//***********************************************************************/
void tcnt3_init(void){
    DDRE  |= (1<<PE3);
    PORTE |= (1<<PE3); //enable pullups
    TCCR3A |= ( 1<<COM3A1 ) | ( 1<<COM3A0 ) | ( 1<<WGM31 ); // fast pwm, set on match.
    TCCR3B |= ( 1<<WGM33 ) | ( 1<<WGM32 ) | ( 1<<CS30 );  // ICR3 for TOP, clk/1, no prescale
    TCCR3C  = 0x00; //no forced compare
    ICR3 = 0x0FFF; //Input capture register
}

//***********************************************************************/
//                           adc_init
// Initializes the ADC. In free running mode, ADC is
// constantly sampled and its dataupdated to the 2 byte ADC Data Register.
// The ADC module contains a prescaler that generates a acceptable ADC clock
// frequency from CPU. This prescaler is decided by the ADPSn registers.
//***********************************************************************/
void adc_init(void)
{
    DDRF &= 0xFE;
    PORTF &= 0xFE;
    ADMUX |= (1<<REFS0) | (1<<ADLAR); //AVCC with external capacitor at AREF pin.
    //left adjusted, just read ADCH, otherwise,
    //need to read ADCL at first, then read ADCH
    //set ADC enable, start conversion, free running mode, enable adc interrupt
    ADCSRA |= (1<<ADEN) | (1<<ADSC) | (1<<ADFR) | (1<<ADIE);
    ADCSRA |= (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0); // set clk/128 prescaler
}

/***********************************************************************/
//                              encoders
// Contains a state machine that is used by the encoders
// Turning left encoder will increment or decrement the audio amplifier's volume.
// Turning right encoder will increment or decrement the number shown
// on 7-segment LCD board.
/***********************************************************************/
void encoders(uint8_t spi_out){
    
    //variables to hold temporary and previous state of the encoders
    uint8_t tempRight, prevRight;
    uint8_t tempLeft, prevLeft;
    
    uint8_t encoder_mode; //determines left or right rotation
    uint8_t left_encoder;
    PORTE = 0x00;         // SH/LD = 0 CLK_INH = 0
    PORTE |= 0x80;        // SH/LD = 1 CLK_INH = 0
    
    //make a copy of the left and right encoder data
    tempLeft = spi_out & 0x03;
    tempRight = spi_out & 0x0C;
    
    if (tempLeft == prevLeft) encoder_mode = 1;       //keep left state and switch right state
    if (tempRight == prevRight) encoder_mode = 0;     //keep right state and switch left state
    
    if(encoder_mode == 0) {  //switch left
        spi_out &= 0x03;
        switch (leftState){
            case 2:
                if (prevLeftState == 3) {
                    count = 1;
                    count_sign = 1;
                }//turn clockwise, the sum is incremented
                if (prevLeftState == 0) {
                    count = 1;
                    count_sign = -1;
                }//turn counterclockwise, the sum is decremented
                
                prevLeftState = leftState; //store current left state as previous state for next case
                if (spi_out == 0) leftState = 0;
                if (spi_out == 1) leftState = 1;
                if (spi_out == 3) leftState = 3;
                break;
                
            case 0:
                if (prevLeftState == 2) {
                    count = 1;
                    count_sign = 1;
                }//clockwise
                
                if (prevLeftState == 1) {
                    count = 1;
                    count_sign = -1;
                }//counterclockwise
                
                prevLeftState = leftState;
                if (spi_out == 1) leftState = 1;
                if (spi_out == 2) leftState = 2;
                if (spi_out == 3) leftState = 3;
                break;
                
            case 1:
                if (prevLeftState == 0) {
                    count = 1;
                    count_sign = 1;
                }//clockwise
                
                if (prevLeftState == 3) {
                    count = 1;
                    count_sign = -1;
                }//counterclockwise
                
                prevLeftState = leftState;
                if (spi_out == 0) leftState = 0;
                if (spi_out == 2) leftState = 2;
                if (spi_out == 3) leftState = 3;
                break;
                
            case 3:
                if (prevLeftState == 1) {
                    count = 1;
                    count_sign = 1;
                }//clockwise
                
                if (prevLeftState == 2) {
                    count = 1;
                    count_sign = -1;
                }//counterclockwise
                
                prevLeftState = leftState;
                if (spi_out == 0) leftState = 0;
                if (spi_out == 1) leftState = 1;
                if (spi_out == 2) leftState = 2;
                break;
                
                default: leftState = 3;
        }//switch
        
        left_encoder &= 0xC0;
        left_encoder |= (spi_out << 6);
        if ((left_encoder == 0xC0) & (clock_mode == 0)) displayBits |= 0x80;
        //show that left encoder can work underlying this state of right one
    }
    
    if(encoder_mode == 1) {  //switch right
        spi_out = (spi_out & 0x0C) >> 2;
        switch (rightState){
            case 2:
                if (prevRightState == 3) {
                    count = 1;
                    count_sign = 1;
                }//turn clockwise, the sum is incremented
                
                if (prevRightState == 0) {
                    count = 1;
                    count_sign = -1;
                }//turns counterclockwise, the sum is decremented
                
                prevRightState = rightState; //store current state for next case call
                if (spi_out == 0) rightState = 0;
                if (spi_out == 1) rightState = 1;
                if (spi_out == 3) rightState = 3;
                break;
                
            case 0:
                if (prevRightState == 2) {
                    count = 1;
                    count_sign = 1;
                }//clockwise
                
                if (prevRightState == 1) {
                    count = 1;
                    count_sign = -1;
                }//counterclockwise
                
                prevRightState = rightState;
                if (spi_out == 1) rightState = 1;
                if (spi_out == 2) rightState = 2;
                if (spi_out == 3) rightState = 3;
                break;
                
            case 1:
                if (prevRightState == 0) {
                    count = 1;
                    count_sign = 1;
                }//counterclockwise
                
                if (prevRightState == 3) {
                    count = 1;
                    count_sign = -1;
                }//clockwise
                
                prevRightState = rightState;
                if (spi_out == 0) rightState = 0;
                if (spi_out == 2) rightState = 2;
                if (spi_out == 3) rightState = 3;
                break;
                
            case 3:
                if (prevRightState == 1) {
                    count = 1;
                    count_sign = 1;
                }//clockwise
                
                if (prevRightState == 2) {
                    count = 1;
                    count_sign = -1;
                }//counterclockwise
                
                prevRightState = rightState;
                
                if (spi_out == 0) rightState = 0;
                if (spi_out == 1) rightState = 1;
                if (spi_out == 2) rightState = 2;
                break;
                
            default: rightState = 3;
                
        }//switch
    }
    prevLeft = spi_out;
    prevRight = spi_out & 0x0C;
}

/***********************************************************************/
//                             TIMER0_OVF_vect
// TIMER0 vector will run when the interrupt occurs, executing
// the encoders function and send the data to bar graph using SPI.
//***********************************************************************/
ISR(TIMER0_OVF_vect){
    
    mode_switch(1); //for changing to different alarm mode
    
    encoders(SPDR); //runs the state machine that drives the encoders
    SPDR = displayBits; //send to display_test function
    
    //strobe register - trigger an action each time it is written to (or sometimes, read from)
    
    while (bit_is_clear(SPSR, SPIF)) {}   //wait till data is sent out
    PORTB |= (1 << PORTB0);  //strobe output data reg in HC595 - rising edge 0x02
    PORTB &=  ~(1 << PORTB0);  //falling edge

}

/*********************************************************************
//                              TIMER3_COMP_vect
// TIMER3 vector which runs whenever a volume change service routine is requested.
//
//********************************************************************/
ISR(TIMER3_COMP_vect){
    volume = count_sign * 20;
    OCR3A = 0x0F00 - volume;          //output volume PWM

    if (OCR3A < 300) alarm_state = 0; //mute mode
    else alarm_state = 1;             //active mode
}

//***********************************************************************/
//                             ADC_vect
// Everytime the interrupt is called, the ADC reads voltage from the photoresistor
// voltage divider. Then, the ADC integer value is fed into a lookup table, and
// OCR2 will have its value set accordingly. The dimming occurs through comparison
// between OCR2 and TCNT2.
//***********************************************************************/
ISR(ADC_vect){
    
    uint8_t adc_out;
    adc_out = ADCH; //ADCL and ADCH, but only 8 bits needed
    
    //look up table
    /*
    if(adc_out >= 0 && adc_out <= 15) {OCR2 = 255;}
    if(adc_out >= 16 && adc_out <= 31) {OCR2 = 240;}
    if(adc_out >= 31 && adc_out <= 46) {OCR2 = 225;}
    if(adc_out >= 47 && adc_out <= 62) {OCR2 = 210;}
    if(adc_out >= 63 && adc_out <= 78) {OCR2 = 195;}
    if(adc_out >= 79 && adc_out <= 94) {OCR2 = 180;}
    if(adc_out >= 95 && adc_out <= 110) {OCR2 = 165;}
    if(adc_out >= 110 && adc_out <= 125) {OCR2 = 150;}
    if(adc_out >= 126 && adc_out <= 141) {OCR2 = 135;}
    if(adc_out >= 142 && adc_out <= 157) {OCR2 = 120;}
    if(adc_out >= 158 && adc_out <= 173) {OCR2 = 105;}
    if(adc_out >= 174 && adc_out <= 189) {OCR2 = 90;}
    if(adc_out >= 190 && adc_out <= 205) {OCR2 = 75;}
    if(adc_out >= 206 && adc_out <= 221) {OCR2 = 60;}
    if(adc_out >= 222 && adc_out <= 237) {OCR2 = 45;}
    if(adc_out >= 238 && adc_out <= 253) {OCR2 = 30;}
     */
    
    
    if(adc_out >= 0 && adc_out <= 15) {OCR2 = 30;}
    if(adc_out >= 16 && adc_out <= 31) {OCR2 = 40;}
    if(adc_out >= 31 && adc_out <= 46) {OCR2 = 50;}
    if(adc_out >= 47 && adc_out <= 62) {OCR2 = 60;}
    if(adc_out >= 63 && adc_out <= 78) {OCR2 = 70;}
    if(adc_out >= 79 && adc_out <= 94) {OCR2 = 80;}
    if(adc_out >= 95 && adc_out <= 110) {OCR2 = 90;}
    if(adc_out >= 110 && adc_out <= 125) {OCR2 = 100;}
    if(adc_out >= 126 && adc_out <= 141) {OCR2 = 110;}
    if(adc_out >= 142 && adc_out <= 157) {OCR2 = 120;}
    if(adc_out >= 158 && adc_out <= 173) {OCR2 = 130;}
    if(adc_out >= 174 && adc_out <= 189) {OCR2 = 140;}
    if(adc_out >= 190 && adc_out <= 205) {OCR2 = 150;}
    if(adc_out >= 206 && adc_out <= 221) {OCR2 = 160;}
    if(adc_out >= 222 && adc_out <= 237) {OCR2 = 170;}
    if(adc_out >= 238 && adc_out <= 253) {OCR2 = 180;}
    
}


//***********************************************************************/
//                          mode_switch
// This function switches the LEDs on the bar graph display and set time counter
// in different modes.
// In mode 0, 3 and 4, incrementing timer means that clock runs in normal mode.
// In mode 1, 2 and snooze, incrementing s2_timer could blink some digit numbers
//***********************************************************************/
void mode_switch(uint8_t mode_change){
    
    if (mode_change) {
        if (clock_mode == 0) {   //push S0
            timer++;             //increment count every 7.8125 ms
            inc_multiple = 0;
            displayBits &= 0x70;
            displayBits |= 0x01;
        }
        
        if ((clock_mode == 1) || (clock_mode == 2)) {   //push S1
            s2_timer++;
            displayBits &= 0x70;
            displayBits |= 0x02;
        }
        
        if ((clock_mode == 3) || (clock_mode == 4)) {   //push S2
            timer++;
            displayBits &= 0x70;
            displayBits |= 0x04;
        }
        
        if (snooze_flag == 1) { //push S6
            s2_timer++;
            snooze(s2_timer);
        }
    }
    clk_count(timer); //allows user to switch time mode displayed
    
    switch (clock_mode) {
        case 0:
            digit_data[0] = minutes % 10; //get the number of units
            digit_data[1] = minutes / 10 % 10; //get the number of tens
            digit_data[3] = hours % 10;  //get the number of hundreds
            digit_data[4] = hours / 10 % 10; //get the number of thousands
            break;
            
        case 1:
            if ((s2_timer % 64) == 0) {    //change mode, set time minutes
                changeTimeDisplay = !changeTimeDisplay; //blink/blank minute display
                digit_data[0] = 10;
                digit_data[1] = 10;
                if (changeTimeDisplay == 0) { //setting ones and tens digit
                    digit_data[0] = minutes % 10;
                    digit_data[1] = minutes / 10 % 10;
                }
            }
            digit_data[3] = hours % 10;
            digit_data[4] = hours / 10 % 10;
            break;
            
        case 2:
            if ((s2_timer % 64) == 0) {    //change mode, set time hours
                changeTimeDisplay = !changeTimeDisplay; //blink/blank hour display
                digit_data[3] = 10;
                digit_data[4] = 10;
                if (changeTimeDisplay == 0) { //setting hundreds and thousands digit
                    digit_data[3] = hours % 10;
                    digit_data[4] = hours / 10 % 10;
                }
            }
            digit_data[0] = minutes % 10;
            digit_data[1] = minutes / 10 % 10;
            break;
            
        case 4:
            if ((timer % 64) == 0) {    //change mode, set alarm hours
                changeAlarmDisplay = !changeAlarmDisplay; //blink/blank minute display
                digit_data[0] = 10;          //switch off
                digit_data[1] = 10;
                if (changeAlarmDisplay == 0) { //setting ones and tens digit
                    digit_data[0] = alarm_m % 10;
                    digit_data[1] = alarm_m / 10 % 10;
                }
            }
            digit_data[3] = alarm_h % 10;
            digit_data[4] = alarm_h / 10 % 10;
            break;
            
        case 3:
            if ((timer % 64) == 0) {    //change mode, set alarm minutes
                changeAlarmDisplay = !changeAlarmDisplay; //blink/blank hour display
                digit_data[3] = 10;          //switch off
                digit_data[4] = 10;
                if (changeAlarmDisplay == 0) { //setting hundreds and thousands digit
                    digit_data[3] = alarm_h % 10;
                    digit_data[4] = alarm_h / 10 % 10;
                }
            }
            digit_data[0] = alarm_m % 10;
            digit_data[1] = alarm_m / 10 % 10;
            break;
    }
}
//***********************************************************************************

//***********************************************************************/
//                           clk_count
// Allows user to toggle from 12hr to 24hr mode
//    time_notation_flag = 0  ---- Switches from 12hr to 24hr
//    time_notation_flag = 1  ---- Switches from 24hr to 12hr
// When operating in the 12hr mode:
//    am_pm_flag = 0 ---- the time changed from 11:59 a.m. to 12:00 p.m.
//    am_pm_flag = 1 ---- the time changed from 12:59 p.m. to 1:00 p.m.
//***********************************************************************/
void clk_count(uint8_t timer1) {
    
    if ((timer1 % 128) == 0) {
        colon_state = !colon_state; //blinks or blanks the colon_state every second
        seconds += 1;
        if (colon_state == 0) digit_data[2] = 10; //lights up colon_state
        else digit_data[2] = 11;  //blanks out colon_state
    }
    
    //handles time ticking increments and rollovers
    if (inc_multiple == 0) {
        if (seconds == 60)
        {
            minutes += 1;
            seconds = 0;
            if (minutes == 60) {
                hours++;
                minutes = 0;
                if (hours == 24) hours = 0;
            }
        }
        
        alarm_mode();  //compare clock time with alarm time
        
        if (s4_switch == 1) {       //changes from 24hr to 12hr
            if (hours >= 12) {      // 12:00/12:00 p.m. ~ 23:59/11:59 p.m.
                if (hours > 12) {
                    
                    if (hours == 24) {  //midnight
                        hours = 0;
                        am_or_pm = 0;    //am
                    }
                    else {
                        hours -= 12; //  noon
                        am_or_pm = 1;    //pm
                    }
                }
                else if (time_notation_flag != 1) {
                    hours = 12;
                    am_or_pm = 1;        //pm
                }
            }
            else if (hours == 0) {     //00:00 to 12:00 am
                hours += 12;
                time_notation_flag = 1;
                am_or_pm = 0;          //am
            }
            else if ((hours > 0) && (hours <= 11) && ((displayBits & 0x10) != 0x10)) am_or_pm = 0; //morning
                time_notation = 1;
        }
        
        if (s4_switch == 0) {       //changes from 12hr to 24hr
            if (time_notation == 1) {
                if (hours == 12) {   // midnight
                    if (time_notation_flag == 1) {
                        if (hours == 12) hours = 0;
                        time_notation_flag = 0;
                    }
                }
                else if ((hours != 12) && ((displayBits & 0x10) == 0x10))  hours += 12;
                time_notation = 0;
                am_or_pm = 2;           //pm
            }
        }
    }
    
    if ((inc_multiple == 1) && (clock_mode == 1)) {  // set minutes
        if ((minutes >= 0) && (minutes < 60)) minutes += count * count_sign * inc_multiple;
        else if (minutes == 60) minutes -= 60; //minute rollover
        else minutes -= 196;
    }
    
    if ((inc_multiple == 1) && (clock_mode == 2)) {   // set hours
        if (time_notation == 1) {                     // 12hr mode
            if ((displayBits & 0x20) == 0x20) {    //at am
                if ((hours >= 1) && (hours <= 12)) {
                    if (hours == 12) {
                        if (am_pm_flag == 0) am_or_pm = 1;  //noon
                        else {
                            hours += count * count_sign * inc_multiple;
                            if (hours == 13) hours = 1;    // from 12am to 1am
                            else if (hours == 11) am_or_pm = 1;//from 12am to 11pm
                        }
                    }
                    else {
                        hours += count * count_sign * inc_multiple;
                        if (hours == 12) am_pm_flag = 0; //11am to 12pm
                        else am_pm_flag = 1;  //am
                    }
                }
                else if (hours == 0) hours = 12;  //hour decrement
            }
            else if  ((hours >= 1) && (hours <= 12)) { //pm
                if (hours == 12) {
                    if (am_pm_flag == 1) am_or_pm = 0;  //12am
                    else {
                        hours += count * count_sign * inc_multiple;
                        if (hours == 13) hours = 1;  //noon to 1pm
                        else if (hours == 11) am_or_pm = 0;//noon to 11am
                    }
                }
                else {
                    hours += count * count_sign * inc_multiple;
                    if (hours == 12) am_pm_flag = 1;//noon to 1pm
                    else am_pm_flag = 0;
                }
            }
            else if (hours == 0) hours = 12;   //1pm to noon
        }
        else if ((hours >= 0) && (hours < 24)) hours += count * count_sign * inc_multiple;
        else if (hours == 24) hours -= 24;
        else hours = 23;
    }
    
    // set alarm minutes
    if ((inc_multiple == 1) && (clock_mode == 3)) {
        if ((alarm_m >= 0) && (alarm_m < 60)) alarm_m +=  count * count_sign * inc_multiple;
        else if (alarm_m == 60) alarm_m -= 60;
        else alarm_m -= 196;
    }
    
    // set alarm hours
    if ((inc_multiple == 1) && (clock_mode == 4)) {
        if (time_notation == 1) {              // 12hr mode
            if ((displayBits & 0x20) == 0x20) {  //am
                if ((alarm_h >= 1) && (alarm_h <= 12)) {
                    if (alarm_h == 12) {
                        if (am_pm_flag == 0) am_or_pm = 1;      //noon
                        else {
                            alarm_h += count * count_sign * inc_multiple;
                            if (alarm_h == 13) alarm_h = 1;    // midnight to 1am
                            else if (alarm_h == 11) am_or_pm = 1; //midnight to 11pm
                        }
                    }
                    else {
                        alarm_h += count * count_sign * inc_multiple;
                        if (alarm_h == 12) am_pm_flag = 0; //11am to noon(pm)
                        else am_pm_flag = 1;  // am
                    }
                }
                else if (alarm_h == 0) alarm_h = 12;    // 1 am to 12 am; 1pm to 12pm
            }
            else if  ((alarm_h >= 1) && (alarm_h <= 12)) { //pm
                if (alarm_h == 12) {
                    if (am_pm_flag == 1) am_or_pm = 0;  //am
                    else {
                        alarm_h += count * count_sign * inc_multiple;
                        if (alarm_h == 13) alarm_h = 1;  //noon to 1pm
                        else if (alarm_h == 11) am_or_pm = 0;  // noon to 11am
                    }
                }
                else {
                    alarm_h += count * count_sign * inc_multiple;
                    if (alarm_h == 12) am_pm_flag = 1;     //noon to 1pm
                    else am_pm_flag = 0;
                }
            }
            else if (alarm_h == 0) alarm_h = 12;   //1pm to noon
        }
        else if ((alarm_h >= 0) && (alarm_h < 24)) alarm_h += count * count_sign * inc_multiple;
        else if (alarm_h == 24) alarm_h -= 24;
        else alarm_h = 23;
    }
    
    //bar graph showing am or pm
    switch (am_or_pm) {
        case 0: displayBits &= 0x8F; displayBits |= 0x20; break; // am
        case 1: displayBits &= 0x8F; displayBits |= 0x10; break; // pm
        case 2: displayBits &= 0x8F; break;
    }
    
}
//***********************************************************************************


//***********************************************************************************
//                                    alarm_mode
// Sets alarm mode to 24hr or 12hr mode. This
// The routine includes 5 time sections. Normal 24hr mode / 1:00 p.m. ~ 11:59 p.m./
// 12 a.m. = 00:00/  12 p.m. = 12:00 / 1:00 a.m. ~ 11:59 a.m.
// Runs the alarm again after the snooze method has been called.
//***********************************************************************************
void alarm_mode() {
    
    //24hr mode (24hr mode alarm / 12hr mode alarm at p.m.)
    if ((time_notation_flag == 0) && (am_pm_flag == 0)) {
        if ((displayBits & 0x10) != 0x10) {    //24hr mode
            if ((minutes == alarm_m) && (hours == (alarm_h))) {
                if (stop == 0)  run_alarm();  // run alarm
                else stop_alarm();  // stop alarm after running for 1 minute
            }
            else stop_alarm();  // stop alarm after running 1 min
        }
        else if ((displayBits & 0x10) == 0x10) {  //from 1:00 pm to 11:59 pm
            if ((minutes == alarm_m) && (hours == (alarm_h - 12)))
                run_alarm();  // run alarm
            else stop_alarm();
        }
    }
    else if ((time_notation_flag == 1) && (am_pm_flag == 0)) { //12hr mode clock and 24hr mode alarm
        if ((hours == 12) && ((displayBits & 0x20) == 0x20)) { // at midnights
            if ((minutes == alarm_m) && (hours == (alarm_h + 12))) run_alarm();
            else stop_alarm();
        }
        else if ((displayBits & 0x20) == 0x20) {  // from 1:00 am to 11:59 am
            if ((minutes == alarm_m) && (hours == (alarm_h + 12))) run_alarm();
            else stop_alarm();
        }
        else if ((hours == 12) && ((displayBits & 0x10) == 0x10)) {// at noon
            if ((minutes == alarm_m) && (hours == alarm_h)) run_alarm();
            else stop_alarm();
        }
    }
    else if (snooze_flag == 1) { //if S6 is pressed
        song++;      //switch to next song
        run_alarm(); //run alarm again
    }
}


//***********************************************************************************

//***********************************************************************************
//                                      run_alarm
// when called, runs the alarm, using case statements from kellen_music.c as the audio
//***********************************************************************************
void run_alarm() {
    if (alarm == 0) {
        music_on();
        alarm = 1;
        stop = 0;
    }
    else {  // run alarm
        ms++;
        if(ms % 8 == 0) beat++;   //play notes (64th notes)
    }
}

//***********************************************************************************

//***********************************************************************************
//                                      stop_alarm
// when called, stops the alarm by killing the music
//***********************************************************************************
void stop_alarm() {
    music_off();
    alarm = 0;     // reset alarm
}
//***********************************************************************************

//***********************************************************************************
//                                      snooze
// when called, alarm clock enters snooze modes
// alarm music is turned off automatically. No intervention from user needed
//***********************************************************************************
void snooze(uint8_t s7_timer) {
    
    if ((s7_timer % 128) == 0) { //if snooze button is pushed
        alarm_s += 1; //increment snooze count
        if (alarm_s > 10) {    //wait 10s for alarm to buzz again
            if (alarm_s == 11) {
                run_alarm();
                LCD_Init();
                LCD_PutStr("Wake up!");
                LCD_MovCursorLn2();
                LCD_PutStr("Press S6 to snooze.");
            }
            else run_alarm();
        }
    }
}
//***********************************************************************************s

//***********************************************************************************
//  Main function. Initializes all the timers, spi, adc, as well as ports. Always checks for
//  button state, to determine the clock mode/settings the user desire.
//  S1 - normal mode
//  S2 - set clock time
//  S3 - set alarm mode
//  S4 - switches to 12hr or 24 hr format
//  S5 - stops alarm clock
//  S6 - runs alarm snooze
//***********************************************************************************
int main() {
    
    DDRB = 0xFF;     //set PORTB as all outputs
    DDRE |= 0xFF;    // Set CLK_INH, SH/LD as output(PE7,PE6) and OCR3A (PE3)
    DDRA = 0x00;     //set port A to all inputs (for the 7 segment)
    
    music_init(); //runs the necessary init methods from kellen_music.c
    spi_init(); //initialize spi to send data to bar graph
    adc_init(); //initialize adc for photoresistor PWM
    tcnt0_init();  //initalize counter/timer0 (for 32kHz clock timebase)
    tcnt2_init();  //initalize counter/timer2 (for LED dimming through PWM)
    tcnt3_init();  //initalize counter/timer3 (for volume control pwm)
    sei();         //enable global interrupts
    
    LCD_Init();
    LCD_PutStr("Alarm Clock");
    LCD_MovCursorLn2();
    if (alarm_state == 0) LCD_PutStr("Mute mode");
    if (alarm_state == 1) LCD_PutStr("Active mode");
    
    while(1){
        _delay_ms(3);    //delay 3 ms, for proper function (timing is important in embedded)
        PORTA = 0xFF;    //set port A to pullups
        PORTB = 0x70;    //enable the tristate buffer
        
        //button press handling cases
        if (chk_buttons(1)) {
            clock_mode = 0; //normal mode
            LCD_Init();     //initialize the LCD
            LCD_PutStr("Alarm Clock");
            LCD_MovCursorLn2();
            if (alarm_state == 0) LCD_PutStr("Mute mode");
            if (alarm_state == 1) LCD_PutStr("Active mode");
        }
        
        if (chk_buttons(2)) {        //change time mode
            stop_alarm();
            clock_mode = 1; // set minutes
            timer = 1;               //stop clock
            inc_multiple = 1;        //+/- 1
            s2_switch = !s2_switch;
            if (s2_switch)  clock_mode = 2;       // set hours
            LCD_Init();
            LCD_PutStr("Set clock time");
        }
        
        if (chk_buttons(3)) {           //change alarm mode
            stop_alarm();
            clock_mode = 3;                       // set alarm minutes
            stop = 0;                       //reset alarm
            inc_multiple = 1;               //+/1
            s2_switch = !s2_switch;
            if (s2_switch)  clock_mode = 4;       // set alarm hours
            LCD_Init();
            LCD_PutStr("Set clock alarm");
        }
        
        if (chk_buttons(4)) {
            s4_switch = !s4_switch; // 12/24hr switching mode
            LCD_Init();
            LCD_PutStr("12/24 hour ");
            LCD_MovCursorLn2();
            LCD_PutStr("Mode change");
        }
        
        if (chk_buttons(5)) {               // stop alarm
            stop_alarm();
            stop = 1;
            snooze_flag = 0;                //reset snooze
            alarm_s = 0;                    //reset snooze count
            LCD_Init();
            LCD_PutStr("Stop alarm");
        }
        
        if  (chk_buttons(6)){              //snooze function
            stop_alarm();
            stop = 1;
            snooze_flag = 1;
            LCD_Init();
            LCD_PutStr("Snooze for 10s...");
        }
        
        //send sum to display 7-segment
        segment_value(digit_data);
        
        //reset encoder counter (important! you don't want to break the state machine)
        count = 0;
        
        //disable tristate buffer
        PORTB = 0x5C;
        
        //set port A to all output for 7-segment
        DDRA = 0xFF;
        
        //reset PORTA
        PORTA = 0x00;
        
        //send value of 4 digit segments to display 
        segment_display(segment_data);
        
    }//while
}//main


 

