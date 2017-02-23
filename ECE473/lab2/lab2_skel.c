// lab2_skel.c
// R. Traylor
// 9.12.08

//  HARDWARE SETUP:
//  PORTA is connected to the segments of the LED display. and to the pushbuttons.
//  PORTA.0 corresponds to segment a, PORTA.1 corresponds to segement b, etc.
//  PORTB bits 4-6 go to a,b,c inputs of the 74HC138.
//  PORTB bit 7 goes to the PWM transistor base.

#define F_CPU 16000000 // cpu speed in hertz
#define TRUE 1
#define FALSE 0
#include <avr/io.h>
#include <util/delay.h>

//holds data to be sent to the segments. logic zero turns segment on
uint8_t segment_data[5];

//decimal to 7-segment LED display encodings, logic "0" turns on segment
uint8_t dec_to_7seg[12];

//******************************************************************************
//                            chk_buttons
//Checks the state of the button number passed to it. It shifts in ones till
//the button is pushed. Function returns a 1 only once per debounced button
//push so a debounce and toggle function can be implemented at the same time.
//Adapted to check all buttons from Ganssel's "Guide to Debouncing"
//Expects active low pushbuttons on PINA port.  Debounce time is determined by
//external loop delay times 12.
//
uint8_t chk_buttons(uint8_t button) {
    //S1 has some debounce problem, and hence will have separate code to handle them
    static uint16_t state[8] = {0x0000}; //holds present state
    if (!(PINA&0x01)){ //if PINA is pressed
        state[button] = (state[button] << 1) | (!(PINA&0x01)) | 0xE000;
        if (state[0] == 0xF000){
            return 0;
        }
        return 1;
    }
    
    else {
        // S2-8 debounce
        state[button] = (state[button] << 1) | (!bit_is_clear(PINA,button)) | 0xE000;
        
        if (state[button] == 0xF000)
            return 1;
        return 0;
    }
    

}

//******************************************************************************

//***********************************************************************************
//                                   segment_sum
//takes a 16-bit binary input value and places the appropriate equivalent 4 digit
//BCD segment code in the array segment_data for display.
//array is loaded at exit as:  |digit3|digit2|colon|digit1|digit0|
void segsum(uint16_t sum) {
    
    segment_data[4]=sum / 1000; //thousand
    segment_data[3]=(sum / 100) % 10; //hundred
    segment_data[1]=(sum / 10) % 10; //ten
    segment_data[0]=sum % 10; //one
    
}//segment_sum
//***********************************************************************************

//***********************************************************************************


//***********************************************************************************
//                                   displayDigit
//displays the number on each of the digits
//if there are leading zeroes (such as 0020), then the leading zeroes are truncated
//and only 20 is displayed on the seven segment
void displayDigit(int number, int one, int ten, int hundred, int thousand){

    int counter = 0;
    for ( counter = 0; counter < 5; counter++ ){
        
        //selects which digit to multiplex to
        //since the 3-8 decoder ony gives mutually exclusive output
        //this is done quickly so all segments look lit up simultaneously
        //because the transistor supplying current to each seven seg is a PNP
        //active LOW switches on the transistor
        //refer to decoder datasheet and LED schematic for more detail
        switch ( counter ){
                
            //turns Q1 on (Y0 LOW)
            case 0:
                PORTB = 0x00;
                PORTA = one;
                _delay_ms(2); //_delay_ms() added to make sure that LED are equally bright when multiplexed
                //too high of a delay and the digit will not light up simultaneously
                //too low of a delay and the one of the digits will be dimmer
                break;
                
            //turns Q2 on (Y1 LOW)
            case 1:
                if(number > 9){
                    PORTB = 0x10;
                    PORTA = ten;
                    _delay_ms(2);
                }
                break;
                
            //turns Q4 on (Y3 LOW)
            case 3:
                if(number > 99){
                    PORTB = 0x30;
                    PORTA = hundred;
                    _delay_ms(2);
                }
                break;
            
            //turns Q5 on (Y4 LOW)
            case 4:
                if(number > 999 && number < 1024) {
                    PORTB = 0x40;
                    PORTA = thousand;
                    _delay_ms(2);
                }
                break;
                
            default:
                break;
                
        }//switch
        
    }//for
    return;
}
//***********************************************************************************

//                  getBin()
//takes in a BCD number, and returns the binary number that turns on the correct LED on the seven segment board
int getBin(number){

    switch( number ){
            
        case 0:
            return 0xC0; //1100 0000
            break;
            
        case 1:
            return 0xF9; //1111 1001
            break;
            
        case 2:
            return 0xA4; //1010 0100
            break;
            
        case 3:
            return 0xB0; //1011 0000
            break;
            
        case 4:
            return 0x99; //1001 1001
            break;
            
        case 5:
            return 0x92; //1001 0010
            break;
            
        case 6:
            return 0x82; //1000 0010
            break;
            
        case 7:
            return 0xF8; //1111 1000
            break;
            
        case 8:
            return 0x80; //1000 0000
            break;
            
        case 9:
            return 0x90; //1001 1000
            break;
            
        default:
            return 0xFF;
            break;
            
    }//switch
    
    return 0;
}
//***********************************************************************************

//***********************************************************************************
// port/pin initializations occur here, as as well as the main logic driver
// Port A pins are all outputs to each LCD segment of a digit
// Port A:  bit0 segment A
//          bit1 segment B
//          bit2 segment C
//          bit3 segment D
//          bit4 segment E
//          bit5 segment F
//          bit6 segment G
//          bit7 decimal point
//               Vdd
//               Vss

// use the tri-state buffer to toggle between sending output to seven segment, and reading
// the button states
// Port A: bit0 Button0
//         bit1 Button1
//         bit2 Button2
//         bit3 Button3
//         bit4 Button4
//         bit5 Button5
//         bit6 Button6
//         bit7 Button7

// Port B   bits 4-6 will determine select the digit to display via a 74HC138 3-to-8 decoder.
// Port B:  bit4 A0
//          bit5 A1
//          bit6 A2
//          bit7 pwm

// in each loop, the following operations are carried out
// make portA input for buttons
// check button presses, and increments accordingly
// break up the number into seven-segment presentable form (decimal to BCD)
// make portA output for the seven segment
// update the seven segment display
uint8_t main(){
    //counter to store the number to be displayed
    int number = 0;
    
    //set PORTB bits 4-7 as outputs, controlling the PWM of the 7-seg, and A2, A1, and A0 of the 3to8 decoder
    DDRB = 0x70;
    while(1){

        //make PORTA an input port with pullups (for buttons)
        //PORTA handles output to the seven segments, as well as input from the pushbuttons
        DDRA = 0x00;
        PORTA = 0xFF;
        
        //enable tristate buffer for pushbutton switches
        //this gives Y7 a LOW output, which enables the pushbutton switches
        PORTB = 0x70;
        
        int cnt = 0;
        //now check each button and increment the count as needed
        if( chk_buttons(0) )  {number += 1;}
        else if( chk_buttons(1) ) {number += 2;}
        else if( chk_buttons(2) ) {number += 4;}
        else if( chk_buttons(3) ) {number += 8;}
        else if( chk_buttons(4) ) {number += 16;}
        else if( chk_buttons(5) ) {number += 32;}
        else if( chk_buttons(6) ) {number += 64;}
        else if( chk_buttons(7) ) {number += 128;}
        
        //bound the count to 0 - 1023
        if(number > 1023){
            number = 1;
        }//if
 
        //disable tristate buffer for pushbutton switches
        PORTB = 0x00;
        
        //make PORTA an output (seven segments and the decimal)
        DDRA = 0xFF;
        
        //break up the disp_value to 4 BCD digits in the array: call (segsum)
        segsum(number);
        
        //determines the binary values for displaying
        int ones_bin = getBin(segment_data[0]);
        int tens_bin = getBin(segment_data[1]);
        int hundreds_bin = getBin(segment_data[3]);
        int thousands_bin = getBin(segment_data[4]);
        
        //displays the digits on the seven segment
        displayDigit(number, ones_bin, tens_bin, hundreds_bin, thousands_bin);

    }//while
    
    return 0;
    
}//main