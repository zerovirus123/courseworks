//
//  lab3.c
//  Created by Jing Guang Sia on 10/12/16.
//  Using seven segment, pushbutton board and rotary encoder, increment or decrement the counter displayed on the seven segment.
//  The pushbutton allows for different increment/decrement modes (in the form of +/- 1, +/-2 or +/-4
//  Pressing the mode button once selects the desired increment/decrement mode. Pressing it again resets it to +/1 mode.

// 7-seg board  mega128
// -----------  -----
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
//-----------   -----
// gnd          gnd
// vcc          vdd
// com_lvl      gnd

//buttonboard   7-seg
//-----------   -----
//com_en        dec7

//encoder       mega128
//--------      -----
// vdd          vdd
// gnd          gnd
// SH/LD        PE7 (INT7)
// SCK          PE6 (INT6)
// CLKINH       PB1 (SCK)
// SOUT         PB3 (MISO)

//bar-graph     mega128
//---------     --------
//vdd           vdd
//gnd           gnd
//oe_n          PB7 (oc2/oc1c)
//sdin          PB2 (MOSI)
//srclk         PB1 (SCK)
//regclk        PB0 (|S|S)

#define F_CPU 16000000 // cpu speed in hertz
#define TRUE 1
#define FALSE 0
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

//variable declarations
uint8_t segment_data[5]; //holds data to be sent to the segments. logic zero turns segment on
uint8_t display_mode = 0x00; //increment mode to be displayed onto bar graph
int8_t num_sign; //the sign of the displayed sum, either 1 or -1
int8_t inc_amount; //the amount to be incremented or decremented by
uint8_t old_encoder_data; //holds the encoder data from the previous ISR call, to be used in new ISR call
uint8_t multiple; //increment by 1, 2 or 4

/***********************************************************************/
//                            spi_init
// Initializes the SPI port on the mega128. Does not do any further
// external device specific initializations.  Sets up SPI to be:
// master mode, clock=clk/2, cycle half phase, low polarity, MSB first
// interrupts disabled, poll SPIF bit in SPSR to check xmit completion
/***********************************************************************/
void spi_init(void){
    SPCR  |=   (1<<SPE)|(1<<MSTR);  //set up SPI mode
    SPSR  |=   (1<<SPI2X);          // double speed operation
}//spi_init

/***********************************************************************/
//                              tcnt0_init
// Initializes timer/counter0 (TCNT0). TCNT0 runs in async mode
// with external 32khz crystal.  Runs in normal mode with no prescaling.
// Interrupt occurs at overflow 0xFF.
/***********************************************************************/
void tcnt0_init(void){
    ASSR   |=  (1<<AS0);  //ext osc TOSC
    TIMSK  |=  (1<<TOIE0);  //enable timer/counter0 overflow interrupt
    TCCR0  |=  (1<<CS00);  //normal mode, clk/128 from prescaler
}

/***********************************************************************/
//                              encoders
// Method to handle encoder ISR interrupt
// Switching two encoders will increment or decrement the sum. To determine
// the data sent from 74HC165 one can decide which encoder is switched. Turn-
// ing counterclockwise on any encoder decrements the number. Turning clockwise increments the number
// . If the user does not push any button, increment multiple is set to one.
/***********************************************************************/
void encoders(uint8_t encoder_data){
    
    //SPI values sent from 74HC165 when right encoder is changed
    uint8_t encoder_right[4] = {0xFB, 0xF3, 0xF7, 0xFF};
    
    //SPI values sent from 74HC165 when left encoder is changed
    uint8_t encoder_left[4] = {0xFE, 0xFC, 0xFD, 0xFF};
    uint8_t mode_encoder;
    
    PORTE = 0x00; //SH/LD = 0 CLK_INH = 0
    PORTE |= 0x80; //SH/LD = 1 CLK_INH = 0
    
    //loops through each encoder data array, and selects the encoder mode for the state machine
    uint8_t index;
    for (index = 0; index < 4; index++) {
        if (encoder_data == encoder_left[index]) {mode_encoder = 0;}  //left mode
        else if (encoder_data == encoder_right[index]) {mode_encoder = 1;}  //right mode
    }
    
    //state machine that determines the increment sign and amount, according to encoder data
    //from the previous state
    switch (mode_encoder){
            
        case 0: //left mode
            if (encoder_data == 0xFD) {
                //turn clockwise, number is incremented
                if ((encoder_data - old_encoder_data) == 1) {
                    inc_amount = 1;
                    num_sign = 1;
                }// if the previous data is equal to 0xFC
                
                //turn counterclockwise, number is decremented
                else if ((encoder_data - old_encoder_data) == -2){
                    inc_amount = 1;
                    num_sign = -1;
                }// if the previous data is equal to 0xFF
            }
            break;
            
        case 1: //right mode
            if (encoder_data == 0xF7) {
                if ((encoder_data - old_encoder_data) == 4) {
                    inc_amount = 1;
                    num_sign = 1;
                }// if the previous data is equal to 0xF3
                
                else if ((encoder_data - old_encoder_data) == -8){
                    inc_amount = 1;
                    num_sign = -1;
                }// if the previous data is equal to 0xFF
            }
            break;
            
    }
    //storing the current encoder data to be used for next interrupt call
    old_encoder_data = encoder_data;
}

/***********************************************************************/
//                              ISR
// This TIMER0 vector will run when the interrupt occurs, it executes
// the encoders() method and send the data of bar graph display by using the SPI databus
//***********************************************************************/
ISR(TIMER0_OVF_vect){
    
    encoders(SPDR); //contains a state machine that determines increment amount and sign
    SPDR = display_mode; //send increment mode to display
    
    while (bit_is_clear(SPSR, SPIF)) {}   //wait till data is sent out
    PORTB |= (1 << PORTB0); //strobe output data reg in HC595 - rising edge   0x02
    PORTB &= ~(1 << PORTB0);  //falling edge (clears PORTB)
}

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
    
    static uint16_t state[8] = {0x0000}; //holds present state
    if ( !(PINA&0x01) ){ //if PINA is pressed
        state[button] = (state[button] << 1) | (!(PINA&0x01)) | 0xE000;
        if (state[0] == 0xF000){ //twelve consecutives 0
            return 0;
        }
        return 1;
    }
    
    else {
        state[button] = (state[button] << 1) | ( !bit_is_clear(PINA,button) ) | 0xE000;
        
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
//takes in a BCD number, and returns the binary number that turns on the correct LED
//on the seven segment board
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
//                                   main()
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
//          bit7 pwm (PWM is grounded for this lab)

// in each loop, the following operations are carried out
// make portA input for buttons
// check button presses, and increments accordingly
// break up the number into seven-segment presentable form (decimal to BCD)
// make portA output for the seven segment
// update the seven segment display
uint8_t main(){
    
    int number = 0; //number to be displayed
    uint8_t mode; //1 - 1 increment, 2 - 2 increment, 4 - 4 increment
    uint8_t prev_button = 0;
    
    //set PORTB bits 4-7 as 74hC138 output, set PORTB bits 3-0 to turn the following bits
    //MISO   input
    //SS     output
    //MOSI   output
    //SCLK   output
    //these bits are built into PORTB
    DDRB = 0xF7;
    
    DDRE |= 0xC0; // set CLK_INH and SH/LD as output
    DDRA = 0x00;  // set PORTA as all inputs from seven-segment
    
    tcnt0_init(); //initializes counter timer 0
    spi_init(); //initializes the SPI databus
    sei(); //enable global interrupts
    
    while(1){
        
        PORTA = 0xFF; //PORTA Pullups
        
        //enable tristate buffer for pushbutton switches
        //this gives Y7 a LOW output, which enables the pushbutton switches
        PORTB = 0x70;
        
        //loop through each button to check if mode change occurs
        //button 6 and 7 are used to set increment mode
        //if both buttons are pressed simultaneously, reset
        //if S6 is pressed once, increment by 2. if pressed once, increment by 1
        //if S7 is pressed once, increment by 4. if pressed once, increment by 1
        uint8_t index;
        for(index=6; index<8; index++){ //polls only S6 and S7
            
            if(chk_buttons(index)){
                int button = index;
                if(button == 6){
                    if(prev_button == 6) mode = 1; //if pressed twice, increment by 1
                    else if (prev_button == 7) mode = 0;//if S6 and S7 pressed together, reset
                    else{
                        mode = 2;
                        prev_button = button; //store current button state
                        //for next loop
                    }
                }//if
            
                else if (button == 7){
                    if( prev_button == 7 ) mode = 1; //if pressed twice, increment by 1
                    else if ( prev_button == 6 ) mode = 0;//if S6 or S7 pressed together, reset
                    else{
                        mode = 4;
                        prev_button = button; //save current button state for next iteration
                    }
                }//else if
                
                else{
                    mode = 1; //nothing pressed, so default increment of 1
                }//else
            }//if
            
        }//for
        
        //depending on mode, different LED lights up on the bar graph
        //a reset to 0 is also supported
        switch(mode){
                
            case 0:
                multiple = 0;
                break;
            case 1:
                multiple = 1;
                prev_button = 0;
                display_mode &= 0x00;
                break;
            case 2:
                multiple = 2;
                display_mode &= 0x00; //reset...
                display_mode |= 0x03; //...and the set
                break;
            case 4:
                multiple = 4;
                display_mode &= 0x00; //reset...
                display_mode |= 0x0F; //...and then set
                break;
        }
        
        //display number in the range of 0-1023, supporting both positive and negative overflow
        if( (number < 1024) && (number >= 0) ){
            number += inc_amount * num_sign * multiple; //normal increment/decrement/reset
        }
        if (number >= 1024) number -= 1023; //positive overflow handling
        if (number < 0) number += 1024; //negative overflow handling
        
        //break up the disp_value to 4 BCD digits in the array: call (segsum)
        segsum(number);
        
        //determines the binary values for displaying
        int ones_bin = getBin(segment_data[0]);
        int tens_bin = getBin(segment_data[1]);
        int hundreds_bin = getBin(segment_data[3]);
        int thousands_bin = getBin(segment_data[4]);
        
        inc_amount = 0;//reset the incremented amount, which is set in ISR. very important, or else
        //the number increments/decrements without stop
        
        //disable tristate buffer for pushbutton switches
        PORTB = 0x5F;
        
        //make PORTA an output (seven segments and the decimal)
        DDRA = 0xFF;
        
        //reset PORTA
        PORTA = 0x00;
        
        //displays the digits on the seven segment
        displayDigit(number, ones_bin, tens_bin, hundreds_bin, thousands_bin);
        
    }//while
    
    return 0;
    
}//main