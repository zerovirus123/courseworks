// lab2.c
// Sihe Yang
// 10.11.2015
//  HARDWARE SETUP:
//  PORTA is connected to the segments of the LED display. and to the pushbuttons.
//  PORTA.0 corresponds to segment a, PORTA.1 corresponds to segement b, etc.
//  PORTB bits 4-6 go to a,b,c inputs of the 74HC138.
//  PORTB bit 7 goes to the PWM transistor base.
//Push button to add count, button S1 to S8 mean to increment 1/2/4/8/16/32/64/128
// 7_segment board       Mega128 board
// --------------        ----------------------   ------------------
//       A               PORTA bit 0 (AD0)
//       B               PORTA bit 1 (AD1)
//       C               PORTA bit 2 (AD2)
//       D               PORTA bit 3 (AD3)
//       E               PORTA bit 4 (AD4)
//       F               PORTA bit 5 (AD5)
//       G               PORTA bit 6 (AD6)
//       DP              PORTA bit 7 (AD7)
//      SEL0             PORTB bit 4 (AD8)
//      SEL1             PORTB bit 5 (OC1A)
//      SEL2             PORTB bit 6 (OC2/OC1C)
//      EN_N                   GND
//       EN                    VDD
//       GND                   GND
//       VDD                   VDD



// 8_push_Button board   7_segment board
// -------------------   ------------------
//      A_2                     J8
//      B_2                     J6
//      C_2                     J7
//      D_2                     J1
//      E_2                     J2
//      F_2                     J3
//      G_2                     J4
//      DP_2                    J5
//     COM_EN                  DEC7



// 8_push_Button board   Mega128 board
// -------------------   ------------------
//      COM_LVL                GND
//       GND                   GND
//       VDD                   VDD



#define F_CPU 16000000 // cpu speed in hertz
#define TRUE 1
#define FALSE 0
#include <avr/io.h>
#include <util/delay.h>

uint8_t a[4] = {0};   // reset arrary a, hold data about which digit display
uint8_t SEG[9] = {0};   //reset array SEE, hold data about decimal to 7-sgement LED


//*********************************************************************************
//                            chk_buttons
//Checks the state of the button number passed to it. It shifts in ones till
//the button is pushed. Function returns a 1 only once per debounced button
//push so a debounce and toggle function can be implemented at the same time.
//Adapted to check all buttons from Ganssel's "Guide to Debouncing"
//Expects active low pushbuttons on PINA port.  Debounce time is determined by
//external loop delay times 3.
//*********************************************************************************
int16_t chk_buttons(uint8_t i) {  //check which button is pushed
    
    static uint16_t state[8] = {0x0000}; //holds present state
    //the way of debounce S2-8 is not useful to S1, use another way
    if (!(PINA&0x01)){
        state[i] = (state[i] << 1) | (!(PINA&0x01)) | 0xE000;
        if (state[0] == 0xF000)
            return 0;
        return 1;
    }
    else {
        // S2-8 debounce
        state[i] = (state[i] << 1) | (!bit_is_clear(PINA,i)) | 0xE000;
        
        if (state[i] == 0xF000)
            return 1;
        return 0;
    }
}


//**********************************************************************************
//                                   segment_value
//takes a 16-bit binary input value and judge the appropriate value of 7-segment for 1 digit
//BCD segment code in the array segment_data for display.
//**********************************************************************************

uint8_t segment_value() {
    
    int k;
    //choose the 7-segmaent LEDs to display based on its value of the corresponding array
    for (k = 0; k < 5; k++) {
        switch (a[k]){
            case 0: SEG[k] = 0xC0; break;  //display 0
            case 1: SEG[k] = 0xF9; break;  //display 1
            case 2: SEG[k] = 0xA4; break;  //display 2
            case 3: SEG[k] = 0xB0; break;  //display 3
            case 4: SEG[k] = 0x99; break;  //display 4
            case 5: SEG[k] = 0x92; break;  //display 5
            case 6: SEG[k] = 0x82; break;  //display 6
            case 7: SEG[k] = 0xF8; break;  //display 7
            case 8: SEG[k] = 0x80; break;  //display 8
            case 9: SEG[k] = 0x90; break;  //display 9
            default: SEG[k] = 0xFF;
        }//switch
    }//for
}//segment_value


//**********************************************************************************
//                                   segment_sum
//takes a 16-bit binary input value and places the appropriate equivalent 4 digit
//BCD segment code in the array segment_data for display.
//array is loaded at exit as:  |digit3|digit2|colon|digit1|digit0|
//**********************************************************************************

void  segment_sum() {   //display 4 digits of final sum
    
    int n;
    for (n = 0; n < 5; n++) {
        switch (n) {
            case 0: PORTB = 0x00; PORTA = SEG[0]; _delay_ms(2); break;  //display units
            case 1: PORTB = 0x10; PORTA = SEG[1]; _delay_ms(2); break;  //display tens
            case 3: PORTB = 0x30; PORTA = SEG[3]; _delay_ms(2); break;  //display hundreds
            case 4: PORTB = 0x40; PORTA = SEG[4]; _delay_ms(2); break;  //display thousands
            default : PORTB = 0x20; PORTA = 0xFF; _delay_ms(2);
        }//switch 4 digits | PORTB: digit | PORTA: value
    }//for
}//segment_sum


//***********************************************************************************
// main function for execuating the counter based on the different values of S1 - S8

//***********************************************************************************
int main()

{
    
    DDRB = 0xF0;     //set port bits 4-7 B as output for 74HC138 decoder
    //PORTB = 0x00;    //reset at beginning
    uint8_t count;   // for returning the counting number when push the button
    uint16_t sum = 0;   //the value of sum
    int8_t rawkeypressed = 0;
    
    while(1){
        //_delay_ms(3);    //4 digital display can light equally if don't use delay 3 ms
        DDRA = 0x00;     //set port A all input for 7-segment
        PORTA = 0xFF;    //set port A to be with pullups
        PORTB = 0x70;    //set the decoder
        
        uint8_t i;
        //use i to check out which button is pushed
        for (i = 0; i < 10; i++){
            //push the button
            if (chk_buttons(i)) {
                //release the button, specially for S1
                if (rawkeypressed == 1){
                    rawkeypressed = 0;  //reset button
                    switch (i){
                        case 0: count = 1; break;     // S1 button is pushed
                        case 1: count = 2; break;     // S2 button is pushed
                        case 2:	count = 4; break;     // S3 button is pushed
                        case 3: count = 8; break;     // S4 button is pushed
                        case 4: count = 16; break;    // S5 button is pushed
                        case 5: count = 32; break;    // S6 button is pushed
                        case 6: count = 64; break;    // S7 button is pushed
                        case 7: count = 128; break;   // S8 button is pushed
                        default: count = 1;
                    }//switch
                }//if
            }//if
            else rawkeypressed = 1;
        }//for
        
        if (sum < 1024) {     //only showing 0-1023
            sum += count;
            a[0] = sum%10; //get the number of units
            a[1] = sum/10%10; //get the number of tens
            a[3] = sum/100%10;  //get the number of hundreds
            a[4] = sum/1000; //get the number of thousands
        }
        else {
            //if the sum is larger than the max value 1024, subtract 1023 and execute a new loop
            sum -= 1023;
            a[0] = sum%10; //get the number of units 
            a[1] = sum/10%10; //get the number of tens
            a[3] = sum/100%10;  //get the number of hundreds
            a[4] = 0;	
        }		
        //send sum to display 7-segement
        segment_value(a); 
        //reset counter
        count = 0;  
        //use buffer to forbid from assigning value of button	
        PORTB = 0x5F;
        //set port A all output for 7-segment
        DDRA = 0xFF; 
        //reset PORTA
        PORTA = 0x00; 
        //reset button state for releasing
        rawkeypressed = 0; 
        //send value of 4 digit segments to display 
        segment_sum(SEG); 
        
    }//while
}//main