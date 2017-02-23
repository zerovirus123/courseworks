// lab1_code.c
// Brian
// 26th Sept 2016

//This program increments a binary display of the number of button pushes on switch
//S0 on the mega128 board.
#include <avr/io.h>
#include <util/delay.h>

//*******************************************************************************
//                            debounce_switch
// Adapted from Ganssel's "Guide to Debouncing"
// Checks the state of pushbutton S0 It shifts in ones till the button is pushed.
// Function returns a 1 only once per debounced button push so a debounce and toggle
// function can be implemented at the same time.  Expects active low pushbutton on
// Port D bit zero.  Debounce time is determined by external loop delay times 12.
//*******************************************************************************
int8_t debounce_switch() {
    //this is a count based debouncing
    static uint16_t state = 0; //holds present state
    state = (state << 1) | (! bit_is_clear(PIND, 0)) | 0xE000; //bit is clear means that the button is pressed. According to Gansles, it is !RawKeyPressed()
    //0xE000 ==> 1110 0000 0000 0000
    
    if (state == 0xF000) return 1; //if there are twelve sequential closers ==> 1111 0000 0000 0000
    return 0;
}

//*******************************************************************************
// Check switch S0.  When found low for 12 passes of "debounce_switch(), increment
// PORTB.  This will make an incrementing count on the port B LEDS.
//*******************************************************************************
int main()
{
    DDRB = 0xFF;  //set port B to all outputs (for lab 1 this would be the LEDS)
    
    //the number to be incremented
    uint8_t number = 0;
    
    //stores the lower and upper bytes of the incremented number
    uint8_t lower = 0;
    uint8_t upper = 0;
    
    while(1){
        if(debounce_switch())
            {
                //increment and reset
                number++;
                if(number > 99){
                    number = 0;
                }
                
               //binary to bcd conversion
               upper = number / 10;
               lower = number % 10;
                
               //first clear PORTB of all bits
               PORTB &= ~(PORTB);
                
               //then set the upper and lower bytes of PORTB to the corresponding bits
               //to do this, extract a specific bit from a either upper or lower byte, the syntax is ((number >> position) & 0x01) <<PORTx(position)
               //if number at position is 1, one is set, otherwise 0 is set
               //this is done by right bit shifting the number to the desired position, and then performing an or to get the specific bit
                PORTB = ( ((upper >> 3)  & 0x01) << PORTB7) | ( ((upper >> 2)  & 0x01) << PORTB6) | ( ((upper >> 1)  & 0x01) << PORTB5) | ( ((upper >> 0)  & 0x01) << PORTB4)
                | (((lower >> 3)  & 0x01) << PORTB3) | (((lower >> 2)  & 0x01) << PORTB2) | (((lower >> 1)  & 0x01) << PORTB1) | (((lower >> 0)  & 0x01) << PORTB0);
                
            }
        _delay_ms(2); //keep in loop to debounce 24ms
    }
}
