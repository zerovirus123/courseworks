//  Quick example for the ATTiny85 which fades an LED on and off

//  Set clock speed to 8 MHz
#define F_CPU 8000000

#include <avr/io.h>
#include <util/delay.h>


void main(void)
{
    
    //  Setup PWM
    TCCR0A |= (1 << COM0A1) | (1 << WGM00) | (1 << WGM01);
    TCCR0B |= (1 << CS01);
    DDRB |= (1 << PB0);
    
    int i = 0;
    
    while(1)
    {
        //  Fade LED on
        for(i = 0; i < 255; i++)
        {
            //  Load new value onto OCR0A register to change duty cycle
            OCR0A = i;
            _delay_ms(1);
        }
        
        //  Fade LED off
        for(i = 255; i >= 0; i--)
        {
            OCR0A = i;
            _delay_ms(1);
        }
    }
}